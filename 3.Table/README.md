# 3.表(Table)
表是Lua語言中唯一一種複雜的資料型態，不過表的使用非常靈活，本章節將會介紹表的使用。

## 基本使用
```lua
t = {}
simpletable = {"hello", "world"}
complextable = {1, "foo",
    function()
        return "bar"
    end,
    0x6892}
```

如上所示，table內容可以為空。table有點像是陣列，不過可以多種資料型態混雜。
我們想要呼叫table裡面的值時如下：
```lua
t = {"foo","bar"}
print(t[1])
```

上面的情況我們都沒有給table中的值指定索引，預設情況下會表現的像是陣列一樣。
如果想要指定索引值的話，如下：
```lua
t = { 1="foo", 2="bar", 6="test", ping = "pong"}
print(t[1])
print(t["ping"])
print(t.ping)
```
索引值可以為整數，也可以為字串。
如果索引值為字串，呼叫時可以直接用`.`連接，像是在呼叫物件中的屬性值一樣。

走訪table的方式如下：
```lua
t = {"hello", 3=4, foo="bar"}
for k,v in ipairs(t) do
    print(k,v)
end
```
在上面程式中，`k`為table中的索引，v為table存放的值。結果如下：
```
1   hello
3   4
foo bar
```

Lua中還有內建函數可以處理Table，包括插入、排序等，可以參考[這裡](https://www.tutorialspoint.com/lua/lua_tables.htm)。

## Metatable
Lua語言另外為table提供了改變table行為的方法，稱作為meta method。
不過在幫table套用meta method之前，必須先將table設為metatable。
```
表名稱=setmetatable(表內容,表行為)
```
以下為示範：
```lua
mytable = setmetatble({foo = "bar"},{
    __index = function(mytable, key)
        return new
    end
})

print(mytable.foo, mytable.new)
```
結果為：
```
bar     new
```
在這個範例中，mytable為一metatable。
當我們呼叫當中的foo，會返回我們指定的內容bar。
而當我們呼叫mytable裡面沒有的索引時，則會執行下面的`__index`函式，然後獲得字串"new"。
metatable提供的meta method眾多，就不在這裡一一介紹了，詳細可以參考[這裡](https://www.tutorialspoint.com/lua/lua_metatables.htm)。


## 模組
在上一章我們提到`example.lua`使用table輸出模組，在本節會簡單介紹。
```lua
modules = {
    a = function(x)
            print(x)
            return x
        end,
}
return modules
```
上面為簡單的模組，我們可以require這個檔案然後使用他。
在上面我們利用table來包裝模組，使的我們可以用類似物件導向的方法使用裡面的函式。
接著我們可以更進階的把物件導向的概念帶進來：
```lua
module = {
    foo = "bar",
    hello = "world",
    method = function(self, x)
        if self[x] then
            return self[x]
        end
    end,
}
return {new = function() return module end}
```
把上面存入example.lua中，然後這樣使用
```lua
example = require("example")
a = example.new()
print(a:method("foo"))
```
導入example後example會得到module那張表。
下一行我們用a去接module中的表new。此時a={foo="bar",hello="world"}
再來我們執行`a:method("foo")`。
這裡會呼叫函式method，變數self會接套用此方法的變數a，而x會接到引數"foo"。

## Table實現switch case
因為索引值可以為數字和字串，我們可以利用這一點用Table來實現判斷式：
```lua
action = {
    [1] = function() return "test" end,
    ["foo"] = function() return "bar" end
}

i=io.read()
action[i]()
```
這是比較簡單的實作方法。

我們也可以用更進階的方式實現更具體的實現switch case，概念基本上和上面一樣：
```lua
function switch(t)
    t.case = function(self,x)
        local f=self[x] or self.default
        if f then
            if type(f) == "function" then
                f(x,self)
            else
                error("case "..tostring(x).."not a function")
            end
        end
    end
    return t
end
```

我們先來看這個函式怎麼使用：
```lua
a=switch {
    [1] = function (x) print(x,10) end,
    [2] = function (x) print(x,20) end,
    default = function (x) print(x,0) end,
}

a:case(2) --2   20
a:case(9) --9   0
```

我們呼叫switch函數時，傳入了一張table由t接收。
除了原來我們傳進去的數值以外，t中還會再加入case函數然後把table回傳給a。
然後我們呼叫`a:case(2)`時他會去執行t.case(self,x)這個函數。
self為使用這個方法的變數本身，也就是變數a。而x為我們傳入的索引，這裡為2。

當索引x存在時我們會執行a[x]，不存在時執行a["default"]
當索引指向的值唯一函數時，程式便會直接執行該函數。
如此我們利用table實現了switch case功能。
