
-- Requires liblua5.1-socket2
require 'socket'

mode = 'lua'

function connect()
	local s = socket.tcp()
	local r = s:connect('127.0.0.1', 12345)
	if r == nil then
		print("Couldn't connect.")
		os.exit(1)
	end
	return s
end

function send(s, msg)
	local idx, err, succ_idx = s:send(msg .. '\n\0')
end

function receive(s)
	local data = ''
	local x
	while true do 
		x = s:receive(1)
		if x == '\0' or x == nil then
			break
		else
			data = data .. x
		end
	end
	return data
end

function command_loop(s)
	while true do
		io.write(mode .. '> ')
		local line = io.input():read('*l')
		send(s, line)
		local r = receive(s)
		local t = assert(loadstring('return ' .. r))()
		if t.data.error_code == 0 then
			print(t.data.return_value)
		else
			print("ERROR: " .. t.data.error_code .. ": " ..  t.data.error_message)
		end
	end
end

s = connect()

component = io.open('editor_component.lua'):read('*a')
send(s, component)
receive(s)
command_loop(s)

