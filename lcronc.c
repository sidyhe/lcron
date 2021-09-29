#include <lua.h>
#include <lauxlib.h>
#include <string.h> // memset
#include "ccronexpr.h"

#define METANAME_LCRONC		"lcronc"

static int lcronc_expr(lua_State* ls) {
	cron_expr* expr = NULL;
	const char* expr_err = NULL;
	const char* str = luaL_checkstring(ls, 1);

	expr = (cron_expr*)lua_newuserdata(ls, sizeof(*expr));
	memset(expr, 0, sizeof(*expr));
	cron_parse_expr(str, expr, &expr_err);
	if (expr_err) {
		lua_pop(ls, 1); // userdata
		luaL_error(ls, "%s", expr_err);
	}

	luaL_setmetatable(ls, METANAME_LCRONC);
	return 1;
}

static int lcronc_meta_next(lua_State* ls) {
	cron_expr* expr = (cron_expr*)luaL_checkudata(ls, 1, METANAME_LCRONC);
	time_t chk_time = (time_t)luaL_checkinteger(ls, 2);
	time_t next_time = cron_next(expr, chk_time);

	lua_pushinteger(ls, next_time);
	return 1;
}

static const luaL_Reg funcs[] = {
  {"expr", lcronc_expr},
  {NULL, NULL}
};

static const luaL_Reg obj_funcs[] = {
  {"next", lcronc_meta_next},
  {NULL, NULL}
};

static void createmeta(lua_State* L) {
	luaL_newmetatable(L, METANAME_LCRONC);  /* create metatable */
	lua_pushvalue(L, -1);  /* push metatable */
	lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
	luaL_setfuncs(L, obj_funcs, 0);  /* add file methods to new metatable */
	lua_pop(L, 1);  /* pop new metatable */
}

LUA_API int luaopen_lcronc(lua_State* L) {
	luaL_newlib(L, funcs);
	createmeta(L);
	return 1;
}
