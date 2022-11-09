timer = {}

function timer.new(time_to_wait, is_repeat)
    return setmetatable({
        timer = time_to_wait,
        is_repeat = is_repeat or false,
        time_to_wait = time_to_wait,
    }, {__index = timer})
end

function timer:tick(dtime)
    self.timer = self.timer - dtime

    if self.timer < 0 then
        if self.is_repeat then
            self.timer = self.time_to_wait
        end

        return true
    end

    return false
end

function timer:ready()
    return self.timer < 0
end

function timer:restart()
    self.timer = self.time_to_wait
end
