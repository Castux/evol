require "evol"

require "luaglu"
require "luagl"


-- view: fourth argument is true to have OpenGL
evol.init(800,500, false, true)

-- init GL stuff

gl.ShadeModel('SMOOTH')            -- Enable Smooth Shading
gl.ClearColor(0, 0, 0, 0.5)        -- Black Background
gl.ClearDepth(1.0)                 -- Depth Buffer Setup
gl.Enable('DEPTH_TEST')            -- Enables Depth Testing
gl.DepthFunc('LEQUAL')             -- The Type Of Depth Testing To Do
gl.Enable('COLOR_MATERIAL')
gl.Hint('PERSPECTIVE_CORRECTION_HINT','NICEST')

gl.Viewport(0, 0, evol.width, evol.height)

gl.MatrixMode('PROJECTION')   -- Select The Projection Matrix
gl.LoadIdentity()             -- Reset The Projection Matrix

if evol.height == 0 then evol.height = 1 end       -- Avoid division by zero
glu.Perspective(80, evol.width / evol.height, 1, 5000) -- Calculate The Aspect Ratio And Set The Clipping Volume

gl.MatrixMode('MODELVIEW')    -- Select The Model View Matrix
gl.LoadIdentity()             -- Reset The Model View Matrix

-- main drawing routine

function Draw(dt)
	local function sin(a)
		return math.sin(math.rad(a))
	end
	
	gl.Clear('COLOR_BUFFER_BIT,DEPTH_BUFFER_BIT') -- Clear Screen And Depth Buffer

	gl.LoadIdentity()             -- Reset The Current Modelview Matrix
	gl.Translate(-1.5, 0, -6)     -- Move Left 1.5 Units And Into The Screen 6.0
	gl.Rotate(rtri, 1, 0.001, 0)      -- Rotate The Triangle On The Y axis ( NEW )

	gl.Begin('TRIANGLES')         -- Drawing Using Triangles
		gl.Color ( 1, 0, 0)         
		gl.Vertex( 0, 1, 0)
		gl.Color ( 0, 1, 0)         
		gl.Vertex(-1,-1, 1)         
		gl.Color ( 0, 0, 1)         
		gl.Vertex( 1,-1, 1)         
		
		gl.Color ( 1, 0, 0)         
		gl.Vertex( 0, 1, 0)    
		gl.Color ( 0, 1, 0)         
		gl.Vertex(-1,-1, -1)         
		gl.Color ( 0, 0, 1)         
		gl.Vertex( 1,-1, -1)
		
		gl.Color ( 1, 0, 0)         
		gl.Vertex( 0, 1, 0)    
		gl.Color ( 0, 1, 0)         
		gl.Vertex(-1,-1, -1)         
		gl.Color ( 0, 1, 0)         
		gl.Vertex(-1,-1, 1)
		
		gl.Color ( 1, 0, 0)         
		gl.Vertex( 0, 1, 0)    
		gl.Color ( 0, 0, 1)         
		gl.Vertex(1,-1, -1)         
		gl.Color ( 0, 0, 1)         
		gl.Vertex(1,-1, 1)
		
	gl.End()                      -- Done Drawing A Triangle

	gl.LoadIdentity()             -- Reset The Current Modelview Matrix
	gl.Translate(1.5, 0, -7)      -- Move Right And Into The Screen
	gl.Rotate(rquad, 1, 0, 0)     -- Rotate The Quad On The X axis ( NEW )

	gl.Color (0.5, 0.5, sin(rquad)/2+0.5)        -- Set The Color To Blue One Time Only
	gl.Begin('QUADS')             -- Draw A Quad
		gl.Vertex(-1, 1, 0)         -- Top Left
		gl.Vertex( 1, 1, 0)         -- Top Right
		gl.Vertex( 1,-1, 0)         -- Bottom Right
		gl.Vertex(-1,-1, 0)         -- Bottom Left
	gl.End()

	rtri = rtri + 180*dt             -- Increase The Rotation Variable For The Triangle ( NEW )
	rquad = rquad - 180*dt          -- Decrease The Rotation Variable For The Quad     ( NEW )

end

-- init our vars
rtri = 0
rquad = 0

while not quit do	
	
	dt,fps,t = evolcore.timer()
	
	evol.input {
		function(key)
			if key == evol.key_escape then quit = true
			end
		end,
		nil,
		function(x,y,b) print(b, "fps: " .. fps) end
	}
	
	Draw(dt)
	evol.flip()
	
	
end

evol.quit()
