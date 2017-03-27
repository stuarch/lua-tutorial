# 1.Lua簡介
[Lua](https://www.lua.org/)是一個設計十分精簡，且非常適合嵌入在其他語言(特別是C和C++)中的腳本語言。
這樣的特性使的Lua常見於嵌入式系統操作腳本、應用程式配置手稿以及遊戲劇情事件腳本。

## 開始
這裡我們使用的是Lua5.1，完整文件可以參考[官方文件](https://www.lua.org/manual/5.1/)。
在Ubuntu/Debian作業系統下，安裝執行環境方式如下:
```
$ sudo apt-get install lua5.1
```

然後嘗試執行Lua prompt：
```
$ lua5.1
Lua 5.1.5  Copyright (C) 1994-2012 Lua.org, PUC-Rio
> print("Hello World!")
Hello World!
>
```
如此基本的執行環境已經建置完成了。

然後新增檔案`helloworld.lua`，使用文字編輯器開啟並加入：
```lua
print("Hello World!")
```
完成腳本撰寫後，執行該腳本檔案：
```
$ lua5.1 helloworld.lua
```
如此我們知道如何執行檔案了。

## 註解
單行註解
```lua
-- This is comment.
```

多行註解
```lua
--[[
This is comment.
And this is still the comment.
]]--
```

## 符號

| 特殊符號 | 敘述             | 範例                                                     |
|----------|------------------|----------------------------------------------------------|
| ^        | 次方             | `> print(10^2) --100`                                    |
| ~=       | 不等於           | `> print(10~=10) --false`                                |
| and      | "且"邏輯         | `> print(true and false) --false`                        |
| or       | "或"邏輯         | `> print(true and false) --true`                         |
| not      | "反"邏輯         | `> print(not true) --false`                              |
| ..       | 合併字串         | `> print("Hello".."World") --HelloWorld`                 |
| #        | 變數長度         | `> print(#"Hello") --5`                                  |


## 資料型態
Lua為弱型別語言，Lua提供的資料型態非常少。以下為Lua提供的資料型態：

| 資料型態 | 敘述                                                                                                 |
|----------|------------------------------------------------------------------------------------------------------|
| nil      | 空值，相當於C語言中的NULL。                                                                            |
| number   | (可運算)數字，預設為double，可以在執行環境中更變。                                                    |
| string   | 字串，和其他腳本語言一樣有[字串處理函數](https://www.tutorialspoint.com/lua/lua_strings.htm)可以使用。 |
| boolean  | 布林代數，為`true`或`false`。                                                                         |
| function | 函式，可以為Lua或是C所撰寫，可以直接執行也可作為指定類型變數的操作方法。                             |
| userdata | 從C語言傳遞來的數據，通常為組態(struct)。                                                                    |
| table    | 類似陣列，不過擁有key和value。是Lua中唯一比較複雜的資料型態，常用來取代class。                        |
| thread   | 執行緒，詳情參照 coroutune(協作)。                                                                    |

不指定變數種類時預設為global，而強調`local`時變數為區域變數。定義變數值方法如下：
```lua
a = 10
local b = 20
local c
d, e = 30, 40
local f, g = 50, 60
```
