# 2.基本使用
在本章節我們將簡單介紹迴圈、判斷以及函數等基本使用。

## 迴圈
Lua可以透過while、repeat以及for兩種方式實現迴圈。

### while
```
while(條件式) do
    條件式成立時會重複執行裡面的內容
end
```

利用while迴圈從1印到4
```lua
i=1
while(i<=4) do
    print(i)
    i=i+1
end
```
結果如下
```
1
2
3
4
```

通常Lua下的無窮迴圈是用while實現：
```lua
while(true) do
    print("hello world")
end
```

### repeat
```
repeat
    條件尚未達成前會重複執行的內容
until(離開迴圈條件)
```

利用repeat迴圈印出1到4
```lua
i=1
repeat
    print(i)
    i=i+1
until(i>4)
```
結果為
```
1
2
3
4
```

### for
```
for 變數=初始值,停止條件,遞增/減 do
    條件尚未滿足時重複執行內容
end
```

同樣從1印到4
```lua
for i=1,4,1 do
    print(i)
end
```
結果為：
```
1
2
3
4
```
如果遞增值沒被指定的情況下，預設為1。
所以上面的範例也可以這樣寫
```lua
for i=1,4 do
    print(i)
end
```
遞增值為負數時會遞減，如下
```lua
for i=4,1,-1 do
    print(i)
end
```
結果如下：
```
4
3
2
1
```
for迴圈還可以用來走訪table，方法如下：
```lua
t={2,5,7}
for k,v in ipairs(t) do
    print(k,v)
end
```
詳細會在後面說明。

## 判斷
Lua提供的判斷只有if，常見於其他語言的switch case可以利用table來實現，詳情在table(表)再討論。
```
if 條件式 then
    條件成立時執行內容
end
```
如果要同時指定條件不成立時執行內容的話：
```
if 條件式 then
    條件成立時執行內容
else
    條件不成立時執行內容
end
```
如果要給定多項條件判斷的話：
```
if 條件式1 then
    條件1成立時執行內容
elseif 條件式2 then
    條件2成立時執行內容
end
```

以下為範例：
```lua
i=io.read()
if i==0 then
    print("zero")
elseif (i<=9 and i>0) then
    print("digit")
else
    print("other")
end
```
這裡會讓使用者輸入內容，
如果內容為0便會印出zero，
如果為小於9但大於0的數就會印出digit，
如果都不是就會印出other。

在迴圈中，我們也可以利用if以及break/continue來控制流程。
```
i=1
while(true) do
    print(i)
    i=i+1
    if i>4 then
        break
    end
end
```

## 函數
Lua的函數可以用Lua撰寫也可以用C語言撰寫，C語言的部份在之後C API再討論。

```
function 函數名稱(引數)
    函數內容
    return 返回值
end
```
函數的返回值不是必要的
下面是函數的範例：
```lua
function pyramid(num)
    local sum = 0
    for i=1,num do
        for j=1,i do
            io.write("*")
            sum = sum + 1
        end
        io.write("\n")
    end
    return sum
end
```
最後執行`print("Print "..pyramid(io.read()).." * totally.")`，
我們會得到這個函數總共印出了多少\*。
第一行函式的部份也可以寫成`pyramid=function(num)`
有趣的是，如果我們執行`print(pyramid)`，結果為`function: 0x10bdbd0`，此為function之記憶體位置。

還有，Lua的函數也支援多值返回的，如下：
```lua
function m_return()
    return 5,"Hello world"
end

num,str = m_return()
print(num,str)
```

### require
將上面範例存成`star.lua`，可以利用require來使用檔案中的函數
```lua
require("star")
require "star"
```
上面兩種寫法都是有效的。
後面就可以直接呼叫pyrimad()函數了。

大部分函數庫會寫成如範例檔`example.lua`，利用table來實現部份物件導向概念。使用方法如下：
```lua
example = require "example"
print(example.pyramid(5))
print(example.anti_tri(5))
```
table的部份會在後面再詳述。
*注意：require吃的檔案可以在當下目錄，也可能來自系統函數庫位置。且副檔名接受.lua以及.so。但檔名重複的話可能會出現錯誤。*

### dofile
dofile函數用來直接執行外部腳本，引數為完整檔名.lua腳本。
```lua
example = dofile("./example.lua")
print(example.pyramid(5))
print(example.anti_tri(5))
```
因為example.lua會返回table，所以變數example會成為底下為函數的table。

### loadstring
loadstring函數可以將字串轉換為腳本語言並執行。
```lua
code = [[
        local a = {foo = "bar"}
        return a
]]

echo = loadstring(code)
print(echo().foo)

b = {"Hello"}
source = string.dump("function() return unpack(b) end")

hi = loadstring(source)
print(hi())
```

### LuaRocks
[LuaRocks](https://luarocks.org/)為Lua模組的套件管理員，我們可以使用LuaRocks來安裝第三方提供的模組。
在Ubuntu/Debian環境下安裝LuaRocks:
```
$ sudo apt-get install luarocks
```

簡單的操作如下：
```
$ luarocks help
$ luarocks search 套件名稱
$ sudo luarocks install 套件名稱
$ sudo luarocks remove 套件名稱
```
