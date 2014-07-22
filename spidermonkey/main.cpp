#include <jsapi.h>
#include <stdio.h>

namespace Runtime{
	static JSBool version(JSContext* cx, unsigned int argc, JS::Value* vp){
		printf("Tested with SpiderMonkey 24");
	}
	static JSBool echo(JSContext* cx, unsigned int argc, JS::Value* vp){
		jsval* argv=JS_ARGV(cx,vp);
		JSString* message=JS_ValueToString(cx,argv[0]);
		char* str=JS_EncodeString(cx,message);
		printf("\n%s\n",str);
	}
}

static JSClass global_class = {"global",
                                JSCLASS_NEW_RESOLVE | JSCLASS_GLOBAL_FLAGS,
                                JS_PropertyStub,
                                JS_DeletePropertyStub,
                                JS_PropertyStub,
                                JS_StrictPropertyStub,
                                JS_EnumerateStub,
                                JS_ResolveStub,
                                JS_ConvertStub,
                                nullptr,
                                JSCLASS_NO_OPTIONAL_MEMBERS
};
static JSFunctionSpec global_functions[]={
	JS_FS("echo", Runtime::echo,1,0),
	JS_FS("logVersion", Runtime::version,0,0),
	JS_FS_END
};

void reportError(JSContext *cx, const char *message, JSErrorReport *report) {
     fprintf(stderr, "%s:%u:%s\n",
             report->filename ? report->filename : "[no filename]",
             (unsigned int) report->lineno,
             message);
}
int run(JSContext *cx) {
    /* Enter a request before running anything in the context */
    JSAutoRequest ar(cx);

    /* Create the global object in a new compartment. */
    JSObject *global = JS_NewGlobalObject(cx, &global_class, nullptr);
    if (!global)
        return 1;

    /* Set the context's global */
    JSAutoCompartment ac(cx, global);
    JS_SetGlobalObject(cx, global);

    /* Populate the global object with the standard globals, like Object and Array. */
    if (!JS_InitStandardClasses(cx, global))
        return 1;

    /* Your application code here. This may include JSAPI calls to create your own custom JS objects and run scripts. */
    
    if(!JS_DefineFunctions(cx,global,global_functions))
		return 1;
		
	const char* script="var text='Hello People';"
				 "logVersion();"
				 "echo(text)";
	JSBool ok;
	jsval rval;
	ok=JS_EvaluateScript(cx,global,script,strlen(script),"INLINE",0,&rval);

    return 0;
}

int main(int argc, const char *argv[]) {
    /* Initialize the JS engine -- new/required as of SpiderMonkey 31. */
    /*if (!JS_Init())
       return 1;*/

    /* Create a JS runtime. */
    JSRuntime *rt = JS_NewRuntime(8L * 1024L * 1024L, JS_NO_HELPER_THREADS);
    if (!rt)
       return 1;

    /* Create a context. */
    JSContext *cx = JS_NewContext(rt, 8192);
    if (!cx)
       return 1;
    JS_SetOptions(cx, JSOPTION_VAROBJFIX);
    JS_SetErrorReporter(cx, reportError);

    int status = run(cx);

    JS_DestroyContext(cx);
    JS_DestroyRuntime(rt);

    /* Shut down the JS engine. */
    JS_ShutDown();

    return status;
}
