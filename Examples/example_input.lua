-- This example describes the three levels at which you can deal with 
-- imput. Either raw hardware info, virtual events, or directly callbacks.

-- Uncomment one section at a time below to see how it works.

require "evol"
require "keymap"

evol.init(640,480)

quit = false

while not quit do	
	
	dt,fps = evol.timer()
	
	--[[ The most basic way: raw input
	
	local input = evol.input()
	
	if input then
		print(unpack(input))
	end
	
	--]]
	
	--[[ Same, but with virtual keys/axes defined in keymap.lua
	--		It returns a list of events, because some raw actions are mapped
	--		to several virtual ones (especially moving the mouse).
	
	
	local input = evol.vInput(map)
	
	if input then
		for i,v in ipairs(input) do
			print(unpack(v))
		end
	end
	
	--]]
	
	---[[ The complete way: give a keymap, some callbacks, and let it do it 
	--		all for you.
	
	evol.treatAllInput(map,cb)
	
	--]]
	
	evol.flip()
end

evol.quit()
