local winapi = require("winapi")

local Network = {}

function Network.OnRecv(buffer)
    print(buffer)
end 

winapi.lua_network_capture_start(Network.OnRecv);


--[[
往栈里面压入一个值
void lua_pushnil      (lua_State *L);
void lua_pushboolean  (lua_State *L, int bool);
void lua_pushnumber   (lua_State *L, lua_Number n);
void lua_pushinteger  (lua_State *L, lua_Integer n);
void lua_pushunsigned (lua_State *L, lua_Unsigned n);
void lua_pushlstring  (lua_State *L, const char *s, size_t len);
void lua_pushstring   (lua_State *L, const char *s);

查询栈里面的元素
int lua_is* (lua_State * L, int index);


获取栈内给定位置的元素值
xxx lua_toXXX(lua_State * L, int index);


//取得栈中元素个数
 int  lua_gettop    (lua_State *L);
//设置栈的大小为一个指定的值，而lua_settop(L,0)会把当前栈清空
//如果指定的index大于之前栈的大小，那么空余的空间会被nil填充
//如果index小于之前的栈中元素个数，则多余的元素会被丢弃
 void lua_settop    (lua_State *L, int index);
//把栈中index所在位置的元素压入栈
 void lua_pushvalue (lua_State *L, int index);
//移除栈中index所在位置的元素
void lua_remove(lua_State *L, int index);
//在栈的顶部的元素移动至index处
void lua_insert(lua_State *L, int index);
//从栈顶弹出一个值，并把它设置到给定的index处
void lua_replace(lua_State *L, int index);
//把fromidx处的元素copy一份插入到toidx，这操作不会修改fromidx处的元素
void lua_copy(lua_State *L, int fromidx, int toidx);


lua_getfield(L, -1, "age");
它可以取代：

 //压入另一个key:age
 lua_pushstring(L, "age");
 //取出-2位置的table,把table[age]的值压入栈
 lua_gettable(L, -2);
]]