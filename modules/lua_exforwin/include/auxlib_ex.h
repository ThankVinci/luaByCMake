#ifndef _AUXLIB_EX_H
#define _AUXLIB_EX_H

#include "efw.h"

typedef struct LoadF {
    int n;  /* number of pre-read characters */
    FILE* f;  /* file being read */
    char buff[BUFSIZ];  /* area for reading file */
} LoadF;

static const char* getF(lua_State* L, void* ud, size_t* size) {
    LoadF* lf = (LoadF*)ud;
    (void)L;  /* not used */
    if (lf->n > 0) {  /* are there pre-read characters to be read? */
        *size = lf->n;  /* return them (chars already in buffer) */
        lf->n = 0;  /* no more pre-read characters */
    }
    else {  /* read a block from file */
        /* 'fread' can return > 0 *and* set the EOF flag. If next call to
           'getF' called 'fread', it might still wait for user input.
           The next check avoids this problem. */
        if (feof(lf->f)) return NULL;
        *size = fread(lf->buff, 1, sizeof(lf->buff), lf->f);  /* read block */
    }
    return lf->buff;
}

static int skipBOM(FILE* f) {
    int c = getc(f);  /* read first character */
    if (c == 0xEF && getc(f) == 0xBB && getc(f) == 0xBF)  /* correct BOM? */
        return getc(f);  /* ignore BOM and return next char */
    else  /* no (valid) BOM */
        return c;  /* return first character */
}


static int skipcomment(FILE* f, int* cp) {
    int c = *cp = skipBOM(f);
    if (c == '#') {  /* first line is a comment (Unix exec. file)? */
        do {  /* skip first line */
            c = getc(f);
        } while (c != EOF && c != '\n');
        *cp = getc(f);  /* next character after comment, if present */
        return 1;  /* there was a comment */
    }
    else return 0;  /* no comment */
}

static int errfile(lua_State* L, const char* what, int fnameindex) {
    const char* serr = strerror(errno);
    const char* filename = lua_tostring(L, fnameindex) + 1;
    lua_pushfstring(L, "cannot %s %s: %s", what, filename, serr);
    lua_remove(L, fnameindex);
    return LUA_ERRFILE;
}

//#define luaL_loadfile(L,f)	luaL_loadfilex(L,f,NULL)

#endif // _AUXLIB_EX_H