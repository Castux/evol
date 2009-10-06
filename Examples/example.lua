require "evol"

evol.init(640,480)

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
	
	evol.flip()		
end

evol.quit()
