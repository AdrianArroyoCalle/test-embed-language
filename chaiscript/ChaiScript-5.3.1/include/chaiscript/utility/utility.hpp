// This file is distributed under the BSD License.
// See "license.txt" for details.
// Copyright 2009-2012, Jonathan Turner (jonathan@emptycrate.com)
// Copyright 2009-2014, Jason Turner (jason@emptycrate.com)
// http://www.chaiscript.com

#ifndef CHAISCRIPT_UTILITY_UTILITY_HPP_
#define CHAISCRIPT_UTILITY_UTILITY_HPP_

#include <string>
#include <utility>
#include <vector>

#include "../chaiscript.hpp"
#include "../dispatchkit/proxy_functions.hpp"
#include "../dispatchkit/type_info.hpp"


namespace chaiscript 
{
  namespace utility
  {

    /// \todo Use of this utility, and uniform initializer lists, is causing memory errors in MSVC
    
    template<typename Class, typename ModuleType>
      void add_class(ModuleType &t_module,
          const std::string &t_class_name,
          const std::vector<chaiscript::Proxy_Function> &t_constructors,
          const std::vector<std::pair<chaiscript::Proxy_Function, std::string>> &t_funcs)
      {
        t_module.add(chaiscript::user_type<Class>(), t_class_name); 

        for(const chaiscript::Proxy_Function &ctor: t_constructors)
        {
          t_module.add(ctor, t_class_name);
        }

        for(auto fun: t_funcs)
        {
          t_module.add(fun.first, fun.second);
        }
        
      }
      
  }
}

#endif

