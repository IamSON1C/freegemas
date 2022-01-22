#ifndef _INTER_H_
#define _INTER_H_

#if defined(__vita__) || defined(WIN32) || defined(__amigaos4__)
 #define _(x) x
#else
 #include <libintl.h>
 #include <locale.h>
 #define _(x) gettext(x)
#endif

#endif /* _INTER_H_ */
