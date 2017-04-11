# C中的相關資料型態
前面提到Lua語言中可用的資料型態少，特別是對較複雜的資料型態更是只有Table。
本章要介紹的便是如何利用相關的資料型態來解決遇到的問題。

## Userdata
userdata是一個與C並用才會出現的資料型態。當我們宣告了一變數為Userdata時，系統會依照我們給定的大小安排一段空白的記憶體供使用。
大部分的時候Userdata都會在C語言底下才做處理，Lua只是傳遞這段記憶體區塊。

底下為一段新增userdata之範例：

```c
typedef struct {
    char *a;
    int b;
} ctx_t;

static int l_new(lua_State *L){
    ctx_t *ctx;
    ctx = (ctx_t *)lua_newuserdata(L, sizeof(ctx_t));
    ctx->a = "Hello World";
    ctx->b = 3;

    return 1;
}
```

上面範例我們建立了一個新的userdata，內容的資料型態為一個由字串和整數組成之組態`ctx_t`。
在函式中我們還對userdata的兩個變數給了值，函式執行完時userdata會在stack最上方且兩個變數已經為我們給定的值了。

現在我們得到了userdata，那麼該如何調用呢？以下示範：
```c
static int l_get(lua_State *L){
    ctx_t *ctx = (ctx_t *)lua_touserdata(L, 1);
    lua_pushstring(L, ctx->a);
    lua_pushinteger(L, ctx->b);
    return 2;
}
```
```lua
a = example.new()
print(example.get(a))
```
當我們印出此函式返回之結果，能得到userdata中兩個變數之值。

## Metatable
我們必須要將userdata作為參數之一來調用userdata。而用來調用userdata的方法卻是在userdata外部的函數。
如果今天要實現物件導向的概念的話，我們也可以利用metatable將調用userdata之方法註冊在userdata的表下。

以下為完整的程式範例：
```c
typedef struct{
    char *path;
    int dep;
} ctx_t;

static int l_new(lua_State *L){
    ctx_t *ctx;
    ctx = (ctx_t *)lua_newuserdata(L, sizeof(ctx_t));

    luaL_getmetatable(L, "example");
    lua_setmetatable(L, -2);

    ctx->a = lua_tostring(L, 2);
    ctx->b = lua_tointeger(L, 3);

    return 1;
}

static int l_get(lua_State *L){
    ctx_t *ctx; 
    ctx = (ctx_t *)luaL_checkudata(L, 1, "example");

    lua_pushstring(L, ctx->a);
    lua_pushinteger(L, ctx->b);
    return 2;
}

static const struct luaL_Reg obj[] = {
    {"new", l_new},
    {NULL, NULL},
};

static const struct luaL_Reg method[] = {
    {"get", l_get},
    {NULL, NULL},
};

int luaopen_example(lua_State *L){
    luaL_newmetatable(L, "example");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, obj);
    luaL_regisger(L, "example", method);
    return 1;
}
```
在這裡，我們註冊了兩張表。一張註冊在global中，另一張則註冊在metatable中。
在`new()`函式中，我們會把userdata註冊在metatable中，而調用userdata的方法也同在裡面。
於是我們就可以這樣調用userdata：
```lua
a = example.new()
print(a:get())
```

*注意：在版本5.2以上的Lua中，在 luaopen_example() 中的兩行註冊函式已經無效，需要如下更變：*
```c
int luaopen_mttest(lua_State *L){
    luaL_newmetatable(L, "example");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, method, 0);
    luaL_newlib(L,  obj);
    return 1;
}
```
