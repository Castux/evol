require "evol"

EC = evol.curses

EC.init(80,24,"DejaVuSansMono.ttf",12)

print(evol.nw, evol.nh, evol.cw, evol.ch)

quit = false

while not quit do	
	
	dt,fps = evolcore.timer()
	
	evol.input {
		function(key)
			if key == evol.key_escape then quit = true end
		end,
		
		function(key)
		end,
		
		function(x,y,b)
		end,
		
		function(x,y,b)
		end
	}
	
	evol.curses.refresh()
	evol.flip()
end

evol.quit()
