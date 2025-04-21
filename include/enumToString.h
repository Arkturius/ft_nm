/**
 * enumToString | enumerate to string generic function
 *
 * @brief	Returns a pointer to a string representing an enumerate value
 *			passed as parameter.
 *
 * @param	ENUM_TYPE e		Value to convert.
 *
 * @example
 *
 *		-	Generate an X-macro list for your enum
 *
 *		#define		e_exampleList(EXPAND)	\
 *			EXPAND(example0, 0)				\
 *			EXPAND(example1, 1)				\
 *			EXPAND(example2, 2)				\
 *			EXPAND(example3, 3)				\
 *
 *		-	define ENUM_TYPE and ENUM_LIST then include enumToString.h
 *
 *		#define		ENUM_TYPE	e_example
 *		#define		ENUM_LIST	e_exampleList
 *		#include	"path/to/enumToString.h"
 *
 *		-	You can override some of the results with special conditions.
 *			Using ENUM_VAR as placeholder and specifying a conditionnal,
 *			followed by the string that should be returned
 *
 *		#define		e_exampleSpec(EXPAND)					\
 *			EXPAND((ENUM_VAR > 2), "exampleMoreThanTwo")	\
 *			EXPAND((ENUM_VAR < 0), "exampleLessThanTwo")	\
 *
 *		#define		ENUM_SPEC	e_exampleSpec
 *
 *      -	It will generate this function, returning the LITTERAL string
 *			for ENUM_TYPE ENUM_VAR. handling ENUM_SPEC as list of exceptions
 *
 *		const char	*ENUM_TYPEToString(ENUM_TYPE e);
 */

#if !defined (ENUM_TYPE) || !defined(ENUM_LIST)
# error "ENUM_TYPE and ENUM_LIST must be defined."
#endif

#if defined(ETOS_CONCAT_INNER) || defined(ETOS_CONCAT) || defined(ETOS_FUNC_NAME)
# error "enumToString macros already defined"
#endif

#define	ETOS_CONCAT_INNER(a, b)		a##b
#define	ETOS_CONCAT(a, b)			ETOS_CONCAT_INNER(a, b)
#define	ETOS_FUNC_NAME(type)		ETOS_CONCAT(type, ToString)

typedef enum
{
	#define	ENUM_VALS(k, v)	k = v,
	ENUM_LIST(ENUM_VALS)
	#undef ENUM_VALS
}	ENUM_TYPE;

const char	*ETOS_FUNC_NAME(ENUM_TYPE)(ENUM_TYPE ENUM_VAR)
{
	#ifdef ENUM_SPEC
	# define ENUM_IF(cond, v)	if (cond) { return ((v)); }
	ENUM_SPEC(ENUM_IF)
	# undef ENUM_IF
	#endif

	static const struct	{ ENUM_TYPE k; const char *v; } map[] = 
	{
		#define ENUM_STRINGS(k, v)	{(k), (#k)},
		ENUM_LIST(ENUM_STRINGS)
		#undef	ENUM_STRINGS
	};
	
	for (unsigned int i = 0; i < sizeof(map) / sizeof(map[0]); ++i)
		if (map[i].k == ENUM_VAR) { return (map[i].v); }
	return ("UNKNOWN");
}

#undef	ETOS_CONCAT_INNER
#undef	ETOS_CONCAT
#undef	ETOS_FUNC_NAME

#undef	ENUM_TYPE
#undef	ENUM_LIST
#undef	ENUM_SPEC
