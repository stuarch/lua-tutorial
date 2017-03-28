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
