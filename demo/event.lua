-- vim: set noexpandtab:

module('event', package.seeall)

listeners = {}

function on(t, cb)
    if listeners[t] == nil then
        listeners[t] = {}
    end
    table.insert(listeners[t], cb)
end

function throw(t, data)
	print("--- " , t)
    if listeners[t] ~= nil then
        for i,v in pairs(listeners[t]) do
            if v(data) then
               --table.remove(listeners[t], i)
            end
        end
    end
end

GAME:setEventCallback(throw)
