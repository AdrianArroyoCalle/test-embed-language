#include <boost/python.hpp>
#include <Python.h>
#include <iostream>

namespace py = boost::python;

int main()
{
	try{
		Py_Initialize();
	
		py::object main_module=py::import("__main__");
		
		py::object main_namespace = main_module.attr("__dict__");
		
		py::exec("print 'Hello, world'", main_namespace);
		
		py::exec("import testex", main_namespace);
		
		py::object author = py::eval("testex.getAuthor()", main_namespace);
		
		std::cout << py::extract<const char*>(author) << std::endl;
	}
	catch(py::error_already_set const&){
		PyErr_Print();
	}
	
	return 0;
}
