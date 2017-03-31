# 4.C API
Lua能夠輕易的與C語言並用是其受歡迎重要的原因之一。此章我們會討論如何使用Lua的C語言API。
先安裝好需要的環境：
```shell
$ sudo apt-get install liblua5.1-dev pkg-config
```

## 狀態機
Lua語言的實現為一儲存現在狀態之狀態機(State)。
Lua和C語言故無法直接對彼此的變數做存取，但可以透過狀態機的查詢及新增狀態來實現兩者互通。

那什麼是狀態機呢？
狀態機為一個儲存各階段狀態之堆疊，最初始的狀態疊在第一層，後面狀態改變時會把狀態一層一層疊上去。
最上層的狀態為現在狀態，而整個堆疊會成為狀態歷史紀錄。
當函式執行完時，會將現在的狀態返回，然後由垃圾回收機制將堆疊回收。

下面舉個例子，假設今天我們設計了一個函式`total(i,j)`，函式的功能是返回變數i和j之加總。
那在狀態機中可以如何實現呢？以下我們用`total(1, 2)`為範例演示狀態機各步驟之狀態：

* 首先Lua會先新增一狀態機，將Lua的基本函式庫載入進來，然後將兩個引數載入進狀態機堆疊。
```
.---.
| 2 | <- 現在狀態
|---|
| 1 |
|___|
```
* 然後我們將現在狀態之值設置為變數j並pop掉。
```
.---.
| 1 | <- 現在狀態
|___|
```
* 我們接著把現在狀態之值設置為變數i並pop掉。此時狀態機已經空了。
* 接著我們要將i和j之值相加並返回，於是我們將i和j之總和被放上狀態機堆疊中，然後函式結束返回了現在狀態
```
.---.
| 3 | <- 現在狀態
|___|
```
於是執行`total(1,2)`我們得到了輸出3。

## 在C語言中操作Lua狀態機
在上一節中我們嘗試敘述狀態機在一函式中的運作流程。
在下面我們會先接續上面的範例來演示如何在C底下操作狀態機>

首先在C語言的程式下要使用Lua之狀態機，要先導入Lua的函式庫：
```c
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
```

接著我們就可以開始實現上面的範例了：
```c
int main(){
    lua_State *L = lua_newstate();
    luaL_openlibs(L);
    
    int i = 1;
    int j = 2;

    luaL_loadstring(L, "total = function(i,j) return i+j end");
    lua_pcall(L, 0, 0, 0);

    lua_getfield(L, LUA_GLOBALSINDEX, "total");
    lua_pushnumber(L ,i);
    lua_pushnumber(L ,j);
    lua_pcall(2, 1, 0);
    
    printf("i+j= %lf\n", lua_tonumber(L, 1));
    lua_close(L);
    return 1;
}
```
一開始先開啟新的狀態機叫L，第二行載入Lua的函式庫。
接著在第三段我們在狀態機中載入了函數`total()`，第四段我們呼叫了前面載入的函式`total`。

在這裡，程式把狀態機裡的運行視為一張表，表中的key為底下定義的變數名稱(包括函式)，所以我們用查詢全域索引中的total欄位來呼叫`total()`。
然後把i和j兩個變數放進狀態機堆疊中，接著我們執行了`lua_pcall()`。
`lua_pcall()`的第一個引數為從狀態機堆疊中擷取的層數，且擷取後會將用了的堆疊層給pop掉。
第二個引數代表會返回的結果數，這些返回的結果會放到狀態機堆疊中。

`lua_tonumber(L, 1)`會將堆疊層從底下數來的第1層之值返回給C語言程式，於是我們收到了Lua狀態機的返回值。
最後我們把Lua狀態機關掉，完成了這個範例。

## 用C語言撰寫Lua函式庫
前面我們簡單介紹了要怎麼在C語言下調用Lua狀態機。
同理，我們只需要利用狀態機的傳遞Lua就能夠調用C語言函式庫。
所以我們需要把想要使用的C語言程式加入狀態機的存取，然後把函式輸出成一張表輸入到Lua裡面。
先來看看怎麼實現把函式做成一張表：
```c
static const struct luaL_Reg example[] = {
    {"hello", l_hello},
    {"total", l_total},
    {NULL, NULL},
};
```
在這裡我們定義了一個`luaL_Reg`類型的陣列。
這個`luaL_Reg`本身由兩個部份組成，一個是前面的字串，為Lua語言那一邊註冊此函式的名稱，後面為此C語言程式中函式的名稱。
也就是說，舉`{"hello",l_hello)`為例，這個程式裡面必須有一個`l_hello()`函式，當Lua那邊呼叫hello()這邊就會執行`l_hello()`函式。
而陣列的最後必須是`{NULL, NULL}`，讓Lua走訪表格時得知表格結束位置。
接著，我們要讓Lua語言在require了這個函式庫時，把上表載入進去：
```c
int luaopen_example(lua_State *L){
    lua_newtable(L);
    luaL_register(L, NULL, example);
    return 1;
}
```
在Lua語言require了一個`.so`檔後會找到`luaopen_檔名()`作為進入點執行。
在這裡我們的檔案輸出之後叫做`example.so`，所以進入點叫做`luaopen_example()`。
在進入點中最重要的就是把上面函式表註冊進去，於是我們用`lua_newtable()`新增了一張表。
作為後面`luaL_register(L, NULL, example)`所需的空間。
接著`luaL_register(L, NULL, example)`會把表example註冊到Lua裡，中間的參數為另外一個表，函式會把表註冊到那個表中。
留NULL代表註冊到global中，未來要做類似物件導向的處理方法時，就會需要把表註冊到另一個表中，這個將再下一章討論。

我們完成了表的註冊，接著只要在上面把兩個要註冊進去的函式完成就行了。
```c
static int l_hello(lua_State *L){
    const char *name = lua_tostring(L, 1);
    lua_pop(L, 1);
    printf("Hello %s\n", name);
    return 1;
}

static int l_total(lua_State *L){
    double i = lua_tonumber(L, 1);
    double j = lua_tonumber(L, 2);
    lua_pop(L, 2);
    lua_pushnumber(L, i+j);
    return 1;
}
```
第一個函式的引數為一字串，執行此函數會直接印出"Hello 引數"，沒有返回值。
第二個函數的引數為兩個數字，此函數會返回兩個引數加總值。
使用方法如下：
```lua
ex = require "example"
ex.hello("Stuarch")
print(ex.total(1,2))
```
這裡我們有附上範例檔案，可以clone下來嘗試在這個文件夾執行`make`輸出後試試看調用此函數庫。
