star = {
    pyramid = function (num)
        local sum = 0
        for i=1,num do
            for j=1,i do
                io.write("*")
                sum = sum + 1
            end
            io.write("\n")
        end
        return sum
    end,
    anti_tri = function(num)
        local sum = 0
        for i=num,1,-1 do
            for j=1,i do
                io.write("*")
                sum = sum + 1
            end
            io.write("\n")
        end
        return sum
    end
}
return star
