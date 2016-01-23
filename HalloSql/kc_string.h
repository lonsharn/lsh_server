
#ifndef __KC_STRING_H__
#define __KC_STRING_H__


#ifndef _WIN32

#ifndef strnicmp
int strnicmp (
        const char * first,
        const char * last,
        int count
        );
#endif

#ifndef stricmp
int stricmp (
        const char * dst,
        const char * src
        );
#endif


#endif // _WIN32

char * strstri (
        const char * str1,
        const char * str2
        );

#endif //__KC_STRING_H__
