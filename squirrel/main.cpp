#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "SQUIRREL3/include/squirrel.h"
#include "SQUIRREL3/include/sqstdio.h"
#include "SQUIRREL3/include/sqstdaux.h"
#define SQUIRREL_STACK_SIZE 1024

static void squirrel_print_function(HSQUIRRELVM sqvm, const SQChar *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}

SQInteger ShowVersion(HSQUIRRELVM vm)
{
	SQInteger argc=sq_gettop(vm);
	printf("Squirrel 3.0.6\n");
	return 0;
}
SQInteger Multiply(HSQUIRRELVM vm)
{
	SQInteger argc=sq_gettop(vm);
	if(argc==2+1)
	{
		if(sq_gettype(vm,2)==OT_INTEGER && sq_gettype(vm,3)==OT_INTEGER)
		{
			SQInteger a,b;
			sq_getinteger(vm,2,&a);
			sq_getinteger(vm,3,&b);
			int result=a*b;
			sq_pushinteger(vm,result);
			return 1;
		}else{
			printf("ERROR: Bad type of arguments");
			return 0;
		}
	}else{
		printf("ERROR: Bad number of arguments");
		return 0;
	}
}
SQInteger register_global_func(HSQUIRRELVM v,SQFUNCTION f,const char *fname)
 {
        sq_pushroottable(v);
        sq_pushstring(v,fname,-1);
        sq_newclosure(v,f,0); //create a new function
        sq_createslot(v,-3);
        sq_pop(v,1); //pops the root table
        return 0;
 }
int main(int argc, char** argv)
{
	HSQUIRRELVM vm=sq_open(SQUIRREL_STACK_SIZE);
	
	sq_setprintfunc(vm, squirrel_print_function, NULL);
	
	register_global_func(vm,ShowVersion,"ShowVersion");
	register_global_func(vm,Multiply,"Multiply");
	
	if(SQ_FAILED(sqstd_dofile(vm,"test.nut",SQFalse,SQFalse)))
		return 1;
	/*const SQChar *program = "::print(\"Hello World!\\n\");";
 
    if (SQ_FAILED(sq_compilebuffer(vm, program,
                                   sizeof(SQChar) * strlen(program),
                                   "program", SQFalse))) {
        return 1;
    }*/
 
    sq_pushroottable(vm);
    if (SQ_FAILED(sq_call(vm, 1, SQFalse, SQFalse))) {
        //squirrel_print_last_error(sqvm);
        return 1;
    }
 
    sq_close(vm);
    return 0;
}
