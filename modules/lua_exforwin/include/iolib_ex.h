#ifndef _IOLIB_EX_H
#define _IOLIB_EX_H

#include "efw.h"

#if !defined(l_checkmode)

/* accepted extensions to 'mode' in 'fopen' */
#if !defined(L_MODEEXT)
#define L_MODEEXT	"b"
#endif

/* Check whether 'mode' matches '[rwa]%+?[L_MODEEXT]*' */
static int l_checkmode(const char* mode) {
	return (*mode != '\0' && strchr("rwa", *(mode++)) != NULL &&
		(*mode != '+' || ((void)(++mode), 1)) &&  /* skip if char is '+' */
		(strspn(mode, L_MODEEXT) == strlen(mode)));  /* check extensions */
}

#endif

#if !defined(l_checkmodep)
/* Windows accepts "[rw][bt]?" as valid modes */
#define l_checkmodep(m)	((m[0] == 'r' || m[0] == 'w') && \
  (m[1] == '\0' || ((m[1] == 'b' || m[1] == 't') && m[2] == '\0')))
#endif

int (luaload_io_ex)(lua_State* L);

#endif // _IOLIB_EX_H