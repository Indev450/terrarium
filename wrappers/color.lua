color = {}

function color.new(r, g, b, a)
    return setmetatable({
        r = r,
        g = g,
        b = b,
        a = a or 255,
    }, {__index = color})
end


function color:invert()
    return color.new(255 - self.r, 255 - self.g, 255 - self.b, self.a)
end
