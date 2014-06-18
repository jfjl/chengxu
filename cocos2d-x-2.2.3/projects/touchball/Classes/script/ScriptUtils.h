#include "LuaScript.h"

#define ObjectMT "OMT";
#define ArrayMT  "AMT";

//#define __index  "__index";
//#define __newindex "__newindex";
//#define __equal  "__eq"; 

char* readFile(const char* fileName);
void loadGlobalUtilFunctions(lua_State* ls);
int global_alert(lua_State* ls);
int global_writeLog(lua_State* ls);
int global_loadScript(lua_State* ls);
int global__call(lua_State* ls);
int global__method(lua_State* ls);
void initRandomSeed(lua_State* ls);
int object__index(lua_State* ls);
int object__newindex(lua_State* ls);
int object__equal(lua_State* ls);