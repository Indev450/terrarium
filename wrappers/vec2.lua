vec2 = {}

function vec2.new(x, y)
    return setmetatable({
        x = x or 0,
        y = y or 0
    }, {
        __index = vec2,

        __add = function(self, other)
            return vec2.new(self.x + other.x, self.y + other.y)
        end,

        __sub = function(self, other)
            return vec2.new(self.x - other.x, self.y - other.y)
        end,

        __mul = function(self, num)
            return vec2.new(self.x * num, self.y * num)
        end,

        __div = function(self, num)
            return vec2.new(self.x / num, self.y / num)
        end,

        __unm = function(self)
            return vec2.new(-self.x, -self.y)
        end,

        __eq = function(self, other)
            return self.x == other.x and self.y == other.y
        end,

        __tostring = function(self)
            return string.format("vec2(x=%f; y=%f)", self.x, self.y)
        end,
    })
end

function vec2.new_from_direction(length, angle)
    return vec2.new(math.cos(angle)*length, math.sin(angle)*length)
end

function vec2.new_from_table(t)
    return vec2.new(t.x, t.y)
end

function vec2:dot(other)
    return self.x*other.x + self.y*other.y
end

function vec2:length_squared()
    return self.x*self.x + self.y*self.y
end

function vec2:length()
    return math.sqrt(self:length_squared())
end

function vec2:resize_ip(new_length)
    local length = self:length()

    self.x = self.x / length * new_length
    self.y = self.y / length * new_length
end

function vec2:resize(new_length)
    local vec = vec2.new(self.x, self.y)

    vec:resize_ip(new_length)

    return vec
end

function vec2:rotate_ip(angle)
    local x = self.x
    local y = self.y

    self.x = math.cos(angle)*x - math.sin(angle)*y
    self.y = math.sin(angle)*x + math.cos(angle)*y
end

function vec2:rotate(angle)
    local new_vec = vec2.new(self.x, self.y)

    new_vec:rotate_ip(angle)

    return new_vec
end

function vec2:normal()
    local len = self:length()

    return vec2.new(self.x/len, self.y/len)
end

function vec2:direction()
    local alpha = math.atan(math.abs(self.y/self.x))

    if self.y >= 0 then
        if self.x >= 0 then
            return alpha
        else
            return math.pi - alpha
        end
    else
        if self.x >= 0 then
            return 2*math.pi - alpha
        else
            return math.pi + alpha
        end
    end
end

function vec2:angle_to(other)
    return other:direction() - self:direction()
end
