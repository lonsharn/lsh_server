
#include "kc_string.h"

#ifndef _WIN32

#ifndef strnicmp
int strnicmp (
			  const char * first,
			  const char * last,
			  int count
			  )
{
	int f, l;

	do {

		if ( ((f = (unsigned char)(*(first++))) >= 'A') &&
			(f <= 'Z') )
			f -= 'A' - 'a';

		if ( ((l = (unsigned char)(*(last++))) >= 'A') &&
			(l <= 'Z') )
			l -= 'A' - 'a';

	} while ( --count && f && (f == l) );

	return ( f - l );
}
#endif

#ifndef stricmp
int stricmp (
			 const char * dst,
			 const char * src
			 )
{
	int f, l;

	do {
		if ( ((f = (unsigned char)(*(dst++))) >= 'A') &&
			(f <= 'Z') )
			f -= 'A' - 'a';
		if ( ((l = (unsigned char)(*(src++))) >= 'A') &&
			(l <= 'Z') )
			l -= 'A' - 'a';
	} while ( f && (f == l) );

	return(f - l);
}
#endif

#endif // _WIN32

char * strstri (
				const char * str1,
				const char * str2
				)
{
	char *cp = (char *) str1;
	char *s1, *s2;
	int	c1, c2;

	if ( !*str2 )
		return((char *)str1);

	while (*cp)
	{
		s1 = cp;
		s2 = (char *) str2;

		do
		{
			c1 = *s1;
			if (c1 >= 'A' && c1 <= 'Z')
				c1 -= 'A' - 'a';
			c2 = *s2;
			if (c2 >= 'A' && c2 <= 'Z')
				c2 -= 'A' - 'a';
			if (c1==c2)
			{
				s1++, s2++;
			}
		}
		while ( c1==c2 && *s1 && *s2 );

		if (!*s2)
			return(cp);

		cp++;
	}

	return	0;
}