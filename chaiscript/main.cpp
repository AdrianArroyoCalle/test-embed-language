#include "ChaiScript-5.3.1/include/chaiscript/chaiscript.hpp"
#include "ChaiScript-5.3.1/include/chaiscript/chaiscript_stdlib.hpp"

int sumar(int a, int b){
	return a+b;
}

int main(int argc, char** argv)
{
	chaiscript::ChaiScript chai(chaiscript::Std_Lib::library());
	chai.add(chaiscript::fun(&sumar),"sumar");
	chai.eval("puts(sumar(4,5));");
	
	return 0;
}
