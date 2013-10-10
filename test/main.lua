package.path = package.path .. ";../?.lua;../?/init.lua"

etah = require "etah"
require "Game"

function main()
	print("etah version:", etah.version)

	evol.init()
	evol.set_mode("windowed", 400, 300)

	etah.run(Game)

	evol.quit()
end

local success, message = pcall(main)
if not success then
	print(debug.traceback(message))
end