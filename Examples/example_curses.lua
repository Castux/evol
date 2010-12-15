require "evol"

EC = evol.curses

EC.init(80,24,"DejaVuSansMono.ttf",12)

print(evol.nw, evol.nh, evol.cw, evol.ch)

quit = false

while not quit do	
	
	dt,fps = evolcore.timer()
	
	repeat
		local input = evol.input()
		
		if input and input[1] == 'kdown' and input[2] == 'escape' then
			quit = true
		end
	until not input
	
	evol.curses.addch(string.char(math.random(255)),math.random(evol.nw)-1,math.random(evol.nh)-1)
	
	evol.curses.refresh()
	evol.flip()
end

evol.quit()
