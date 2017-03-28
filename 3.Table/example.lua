mytable = {
    foo = "bar",
    hello = "world",
}
module = {
    new = mytable,
    method = function(self, x)
        if self[x] then
            return self[x]
        end
    end,
}
return module
