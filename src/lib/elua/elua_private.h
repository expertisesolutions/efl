#ifndef _ELUA_PRIVATE_H
#define _ELUA_PRIVATE_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <locale.h>

#ifdef ENABLE_NLS
# include <libintl.h>
# define _(x) dgettext(PACKAGE, x)
#else
# define _(x) (x)
#endif

#include <stdio.h>
#include <stdlib.h>

#include <Eina.h>
#include <Ecore.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "Elua.h"

#ifdef _WIN32
# include <evil_private.h> // popen, pclose
#endif

struct _Elua_State
{
   lua_State *luastate;
   Eina_Stringshare *progname;
   Eina_Stringshare *coredir;
   Eina_Stringshare *moddir;
   Eina_Stringshare *appsdir;
   Eina_List *lmods;
   Eina_List *cmods;
   Eina_List *lincs;
   int requireref, apploadref;
};

extern int _elua_log_dom;

#define DBG(...) EINA_LOG_DOM_DBG(_elua_log_dom, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(_elua_log_dom, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_elua_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(_elua_log_dom, __VA_ARGS__)
#define CRT(...) EINA_LOG_DOM_CRITICAL(_elua_log_dom, __VA_ARGS__)

int _elua_io_popen(lua_State *L);
Eina_Bool _elua_state_io_setup(const Elua_State *es);

#if LUA_VERSION_NUM < 502

#define elua_register(L, lib) luaL_register(L, NULL, 0)
#define elua_strlen(L, i) lua_strlen(L, i)

#else

#define elua_register(L, lib) luaL_setfuncs(L, lib, 0)
#define elua_strlen(L, i) lua_rawlen(L, i)

#endif

#endif
