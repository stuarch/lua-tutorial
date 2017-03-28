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
```
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