#include <boost/python.hpp>

BOOST_PYTHON_MODULE(testex)
{
	using namespace boost::python;
	def("getAuthor",getAuthor);
}
