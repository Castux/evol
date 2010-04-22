require "evol"
require "keymap"

evol.init(640,480)

quit = false

while not quit do	
	
	dt,fps = evol.timer()
	
	local input = evol.vinput(map)
	
	---[[
	if input then
		for i,v in ipairs(input) do
			print(unpack(v))
		end
	end
	--]]
	
	--print(evol.vstate('stick1x'))
	
	evol.flip()		
end

evol.quit()
