#include "emacs-modules.h"
#include "stdafx.h"
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
#pragma comment(linker, "/export:emacs_module_init")	    
	/* Declare mandatory GPL symbol.  */
	__declspec(dllexport) int plugin_is_GPL_compatible;
	const char *clang_complete(char *filename,int line_number, int column_number);
	/* New emacs lisp function. All function exposed to Emacs must have this prototype. */

	static emacs_value
		Fmymod_test(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
	{
		ptrdiff_t l = 0;
		int lineno = env->extract_integer(env, args[1]);
		int colnum = env->extract_integer(env, args[2]);
		int required = env->copy_string_contents(env, args[0], NULL, &l);

		char* filename = (char*)malloc(l);
		if (env->copy_string_contents(env, args[0], filename, &l) == true)
		{

			const char* v = clang_complete(filename, lineno, colnum);
			return  env->make_string(env, v, strlen(v));
		}
		else {
			return  env->make_string(env, "error on filename", 17);
		}

	}
	}

	/* Bind NAME to FUN.  */
	static void
		bind_function(emacs_env *env, const char *name, emacs_value Sfun)
	{
		/* Set the function cell of the symbol named NAME to SFUN using
		the 'fset' function.  */

		/* Convert the strings to symbols by interning them */
		emacs_value Qfset = env->intern(env, "fset");
		emacs_value Qsym = env->intern(env, name);

		/* Prepare the arguments array */
		emacs_value args[] = { Qsym, Sfun };

		/* Make the call (2 == nb of arguments) */
		env->funcall(env, Qfset, 2, args);
	}

	/* Provide FEATURE to Emacs.  */
	static void
		provide(emacs_env *env, const char *feature)
	{
		/* call 'provide' with FEATURE converted to a symbol */

		emacs_value Qfeat = env->intern(env, feature);
		emacs_value Qprovide = env->intern(env, "provide");
		emacs_value args[] = { Qfeat };

		env->funcall(env, Qprovide, 1, args);
	}

	int
		emacs_module_init(struct emacs_runtime *ert)
	{


		emacs_env *env = ert->get_environment(ert);

		/* create a lambda (returns an emacs_value) */
		emacs_value fun = env->make_function(env,
			2,            /* min. number of arguments */
			2,            /* max. number of arguments */
			Fmymod_test,  /* actual function pointer */
			"doc",        /* docstring */
			NULL          /* user pointer of your choice (data param in Fmymod_test) */
		);

		bind_function(env, "clang-complete", fun);
		provide(env, "emacs-clangcomplete");

		/* loaded successfully */
		return 0;
	}


#ifdef __cplusplus
}
#endif