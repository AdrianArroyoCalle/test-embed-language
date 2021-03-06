// This file is distributed under the BSD License.
// See "license.txt" for details.
// Copyright 2009-2012, Jonathan Turner (jonathan@emptycrate.com)
// and Jason Turner (jason@emptycrate.com)
// http://www.chaiscript.com

#ifndef CHAISCRIPT_ANY_HPP_
#define CHAISCRIPT_ANY_HPP_

#include <utility>

namespace chaiscript {
  namespace detail {
    namespace exception
    {
      /// \brief Thrown in the event that an Any cannot be cast to the desired type
      ///
      /// It is used internally during function dispatch.
      ///
      /// \sa chaiscript::detail::Any
      class bad_any_cast : public std::bad_cast
      {
        public:
          bad_any_cast() CHAISCRIPT_NOEXCEPT
            : m_what("bad any cast")
          {
          }

          virtual ~bad_any_cast() CHAISCRIPT_NOEXCEPT {}

          /// \brief Description of what error occured
          virtual const char * what() const CHAISCRIPT_NOEXCEPT CHAISCRIPT_OVERRIDE
          {
            return m_what.c_str();
          }

        private:
          std::string m_what;
      };
    }
  

    class Any {
      private:
        struct Data
        {
          virtual ~Data() {}
          virtual void *data() = 0;
          virtual const std::type_info &type() const = 0;
          virtual std::shared_ptr<Data> clone() const = 0;
        };

        template<typename T>
          struct Data_Impl : Data
          {
            Data_Impl(T t_type)
              : m_type(typeid(T)),
                m_data(std::move(t_type))
            {
            }

            virtual ~Data_Impl() {}

            virtual void *data() CHAISCRIPT_OVERRIDE
            {
              return &m_data;
            }

            const std::type_info &type() const CHAISCRIPT_OVERRIDE
            {
              return m_type;
            }

            std::shared_ptr<Data> clone() const CHAISCRIPT_OVERRIDE
            {
              return std::shared_ptr<Data>(new Data_Impl<T>(m_data));
            }

            Data_Impl &operator=(const Data_Impl&) = delete;
              
            const std::type_info &m_type;
            T m_data;
          };

        std::shared_ptr<Data> m_data;

      public:
        // construct/copy/destruct
        Any() = default;

        Any(const Any &t_any) 
        { 
          if (!t_any.empty())
          {
            m_data = t_any.m_data->clone(); 
          } else {
            m_data.reset();
          }
        }

        template<typename ValueType> 
        Any(const ValueType &t_value)
          : m_data(std::shared_ptr<Data>(new Data_Impl<ValueType>(t_value)))
        {
        }

        Any & operator=(const Any &t_any)
        {
          Any copy(t_any);
          swap(copy);
          return *this; 
        }

        template<typename ValueType> 
          Any & operator=(const ValueType &t_value)
        {
          m_data = std::shared_ptr<Data>(new Data_Impl<ValueType>(t_value));
          return *this;
        }

        template<typename ToType>
          ToType &cast() const
          {
            if (m_data && typeid(ToType) == m_data->type())
            {
              return *static_cast<ToType *>(m_data->data());
            } else {
              throw chaiscript::detail::exception::bad_any_cast();
            }
 
          }


        ~Any()
        {
        }

        // modifiers
        Any & swap(Any &t_other)
        {
          std::shared_ptr<Data> data = t_other.m_data;
          t_other.m_data = m_data;
          m_data = data;
          return *this;
        }

        // queries
        bool empty() const
        {
          return !bool(m_data);
        }

        const std::type_info & type() const
        {
          if (m_data)
          {
            return m_data->type();
          } else {
            return typeid(void);
          }
        }
    };

  }
}

#endif


