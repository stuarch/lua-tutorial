# 6.協同
我們也可以在同一個Lua腳本中同時執行多個執行序。以下介紹：

## Thread
Thread為Lua中的資料型態之一。印出Thread會得到記憶體位置。

下面我們先新增一個Thread：
```lua
co = coroutine.create(function(a,b)
    return a+b
end
)
```
上面co為一Thread，我們可以如以下讓它執行：
```lua
coroutine.resume(co, 1, 2) -- 3
```
這裡a得到了1，b得到了2，而co開始執行。

我們可以讓Thread執行中斷，且中斷也可以返回目前狀態。
我們改變一下Thread的內容：
```lua
co = coroutine.create(function(a,b)
    sum = 1
    for i = 1,b do
        sum = sum * a
    end
    c = coroutine.yield(sum)
    return sum+c
)

coroutine.resume(co, 2, 5) -- 32
coroutine.resume(co, 16) -- 48
```

執行到`coroutine.yield()`時會進行一次中斷，並且返回其參數。
重新進入一次進入Thread時會把`coroutine.resume()`的參數傳入本來yield的位置，並接下去執行。

`coroutine.running()`則會返回當前執行Thread，如果當前在主程式線程則會返回nil。
```lua
co = coroutine.create(function()
    coroutine.yield(coroutine.running())
end
)

print(coroutine.resume(co)) -- thread
```

我們可以用`status`查詢Thread的狀態：
```lua
co = coroutine.create(function()
    sum = 0
    for i = 1,100000 do
        sum = sum + i
    end
    coroutine.yeild(sum)
    return "sum="..sum
)

coroutine.resume(co)
print(coroutine.status(co)) -- run
while true do
    if coroutine.status(co) == "suspend" then
    break
end
coroutine.resume(co)
print(coroutine.resume(co)) -- dead
```
Thread的狀態有三種，分別是run、suspend、dead。
run為正在執行中，還未收到返回值或是中斷狀態之前。
當Thread收到中斷訊息時，會進入suspend狀態。
在suspend狀態下可以再執行resume讓Thread繼續執行。
在Thread已經執行return或是end時，會進入dead狀態。此時程式已經無法再resume了。
