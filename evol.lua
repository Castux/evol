-- evol.lua -- evöL
-- Copyright (C) 2008 Noé Falzon

-- This program is  free software:  you can redistribute it and/or modify it
-- under  the terms  of the  GNU General Public License  as published by the
-- Free Software Foundation, version 3 of the License.
--
-- This program  is  distributed  in the hope  that it will  be useful,  but
-- WITHOUT  ANY WARRANTY;  without  even the implied  warranty of MERCHANTA-
-- BILITY  or  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
-- License for more details.
--
-- You should have received a copy of the  GNU General Public License  along
-- with this program. If not, see <http://www.gnu.org/licenses/>.

require "evolcore"

module(..., package.seeall)

require("evol_keys")

--- **General**

--- Initializes evöL
-- evol.init(w,h,[full,[gl]])
-- w,h: width and height in pixels of the desired window
-- full: if true, gets fullscreen mode
-- gl: if true, enables OpenGL
-- Sets the values evol.width and evol.height
init = function(w,h,full,gl)
	width = w
	height = h
	evolcore.init(w,h,full,gl)
	
	-- Init vinput system
	
	vhats = {}	-- indexed as padnum-hatnum, ie '0-0'
	vmap = nil	-- last map used
end

--- Timer utility function
-- dt,fps,t = evol.timer([fpsmax])
-- Must be called each frame.
-- dt: time since last frame
-- fps: average number of frame per seconds (updated every second)
-- t: ticks (miliseconds since the program started)
-- fpsmax: if provided, limits the fps to this number
timer = evolcore.timer

--- Change cursor visibility
-- evol.showCursor(show)
-- show: if true, makes cursor visible, if false, makes it invisible
showCursor = evolcore.showCursor

--- Frees the memory associated to a previously loaded/created image, font, sound or music
-- evol.free(stuff) or stuff:free()
-- stuff: an image, font, sound or music to be freed
free = function(stuff)
	
	if stuff.type == "Image" then
		evolcore.freeSurf(stuff.surf)
	elseif stuff.type == "Font" then
		evolcore.freeFont(stuff.font)
	elseif stuff.type == "Sound" then
		evolcore.freeSound(stuff.sound)
	elseif stuff.type == "Music" then
		evolcore.freeMusic(stuff.music)
	end

end

--- Shuts down evöL
-- evol.quit()
quit = evolcore.quit

--- **Video**

--- Updates screen
-- evol.flip()
flip = evolcore.flip

--- **Images** 

--- A container for a drawable surface
-- Image object
-- Fields:
-- 	cx,cy: center of the image, relatively to top-left corner. Used by draw() to place the image.
-- Shortcut methods:
-- 	image:draw([x,y,[dest]])
-- 	image:fill(color,[rect])
-- 	image:putpix(x,y,color)
-- 	image:free()


-- INTERNAL! -- Prepares an Image table
local Image = function()
	local tmp = {}
	tmp.blit = blit
	tmp.free = free
	tmp.fill = function(image,color,rect)
		fill(color,rect,image)
	end
	tmp.putpixel = function(image,x,y,color)
		putpix(x,y,color,image)
	end
	
	tmp.type = "Image"
	
	return tmp
end


--- Creates an image from a file (any format supported by SDL_image)
-- image = evol.loadImage(path)
-- path: string containing the path to the image to load (relative to current folder, or absolute)
-- image: the created image
loadImage = function(path)
	local tmp = Image()
	tmp.surf, tmp.w, tmp.h = evolcore.loadImage(path)
	
	tmp.cx = tmp.w / 2
	tmp.cy = tmp.h / 2
	
	return tmp
end

--- Creates an empty image
-- image = evol.makeImage(w,h)
-- w,h: width and height of the image to create
-- image: the created image
makeImage = function(w,h)
	local tmp = Image()
	tmp.surf, tmp.w, tmp.h = evolcore.makeSurf(w,h)
	
	tmp.cx = tmp.w / 2
	tmp.cy = tmp.h / 2
	
	return tmp
end

--- Fills a rectangle of an image with given color
-- evol.fill(color,[rect,[img]]) or image:fill(color,[rect])
-- color = {r,g,b,a} or {r,g,b}: color to fill with
-- rect = {x,y,w,h}: the rectangle to fill. Defaults to full surface.
-- img: image to fill. Defaults to screen.
fill = function(color, rect, img)
	
	local img = img or {}
	local rect = rect or {0,0,0,0}
	
	if not color[4] then color[4] = 255 end		-- RGB are opaque by default
	
	evolcore.fill(rect[1],rect[2],rect[3],rect[4],color[1],color[2],color[3],color[4], img.surf)
end

--- Blits an image onto another
-- evol.blit(image,[x,y,[dest]]) or image:blit([x,y,[dest]])
-- x,y: position in pixels where to put the center of the image to. Defaults to image center, so that it is put in the top left corner.
-- dest: destination image. Defaults to screen.
blit = function(image, x, y, dest)
	
	local dest = dest or {}
	local x = x or image.cx
	local y = y or image.cy 
		
	evolcore.blit(image.surf, x-image.cx, y-image.cy, dest.surf)
end

--- Changes the color of a pixel on an image
-- evol.putpix(x,y,color,[image]) or image:putpix(x,y,color)
-- x,y: coordinates of the pixel to change
-- color = {r,g,b,[a]}: color to set the pixel to
-- image: the image to modify. Defaults to screen.
putpix = function(x, y, color, img)
	
	local img = img or {}
	if not color[4] then color[4] = 255 end
	
	evolcore.putpix(x, y, color[1],color[2],color[3],color[4], img.surf)
end

--- **Input**


--- Polls one event and returns raw information
-- event = evol.input()
-- {'kdown', keyname}
-- {'kup', keyname}
-- {'mdown',x,y,button}
-- {'mup',x,y,button}
-- {'mmotion',x,y,xrel,yrel}
-- {'axis',padnumber,axisnumer,value}
-- {'hat',padnumber,hatnumer,up,right,down,left}
-- {'bdown',padnumber,buttonnumber}
-- {'bup',padnumber,buttonnumber}

local event_names = {'kdown','kup','mdown','mup','mmotion','axis','hat','bdown','bup'}

input = function()
	
	local arg = {evolcore.input()}
		
	if arg[1] == 0 then
		return
	
	-- convert key numbers into key names
	elseif arg[1] == 1 or arg[1] == 2 then
		arg[2] = keys[arg[2]]
	end
	
	-- convert event numbers into event names
	arg[1] = event_names[arg[1]]
	
	return arg
end

--- Polls one event, maps it to virtual buttons/axes, and returns information
-- vevents = evol.vInput(map,[event])
-- map: the mapping table to use (see example)
-- event: if present, used instead of polling new event
-- vevents is a list of virtual events. A virtual event is
-- Returns nil if no virtual event happened
-- {vkey,'up'/'down'}
-- {vaxis, newval, relval}


vInput = function(map, e)

	local e = e or input()
	local vkey,dir
	local list = {}
	
	-- save last map used
	vmap = map
	
	if not e then return end
	
	--
	-- deal with virtual buttons first
	--
	
	-- key
	if e[1] == 'kdown' or e[1] == 'kup' then
		vkey = map.keyboard and map.keyboard[e[2]]
		dir = e[1] == 'kdown' and 'down' or 'up'
		
	-- gamepad button
	elseif e[1] == 'bdown' or e[1] == 'bup' then
		local g = 'gamepad' .. e[2]

		vkey = map[g] and map[g].buttons and map[g].buttons[e[3]]
		dir = e[1] == 'bdown' and 'down' or 'up'
	
	-- mouse button
	elseif e[1] == 'mdown' or e[1] == 'mup' then
		vkey = map.mousebuttons and map.mousebuttons[e[4]]
		dir = e[1] == 'mdown' and 'down' or 'up'
	
	end
	
	if vkey then
		table.insert(list, {vkey,dir})
	end
	
	
	-- hat. tougher.
	if e[1] == 'hat' then
		local index = e[2] .. '-' .. e[3]
		local g = 'gamepad' .. e[2]
		local h = 'hat' .. e[3]
		
		
		local old = vhats[index] or {false,false,false,false}
		local new = {e[4],e[5],e[6],e[7]}
		
		for i = 1,4 do
			vkey,dir = nil,nil
			
			if not old[i] and new[i] then
				vkey = map[g] and map[g][h] and map[g][h][i]
				dir = 'down'
				
			elseif old[i] and not new[i] then
				vkey = map[g] and map[g][h] and map[g][h][i]
				dir = 'up'
				
			end
			
			if vkey then
				table.insert(list, {vkey,dir})
			end
		end
		
		-- save hat info
		vhats[index] = new
	end
	
		
	--
	-- virtual axes
	--
	
	local axis,val,relval
	
	if e[1] == 'mmotion' then
		
		if e[4] ~= 0 then
			axis = map.mousex
			val = e[2]
			relval = e[4]		
			
			if axis then
				table.insert(list, {axis,val,relval})
			end
		end
		
		if e[5] ~= 0 then
			axis = map.mousey
			val = e[3]
			relval = e[5]		
			
			if axis then
				table.insert(list, {axis,val,relval})
			end
		end
		
	-- gamepad axis
	elseif e[1] == 'axis' then
		local g = 'gamepad' .. e[2]
		local a = 'axis' .. e[3]
		
		axis = map[g] and map[g][a]
		
		local old = vmap.vaxes[axis] or 0
		val = e[4]
		relval = e[4] - old
		
		if axis and relval ~= 0 then
			table.insert(list, {axis,val,relval})
		end
		
	end
	
	
	-- save axis or button states
	
	map.vkeys = map.vkeys or {}		-- in case they don't exist
	map.vaxes = map.vaxes or {}
	
	for i,v in ipairs(list) do
		if #v == 2 then
			map.vkeys[v[1]] = v[2]
		
		else
			map.vaxes[v[1]] = v[2]
		end			
	end
	
	
	if #list > 0 then
		return list
	end
end


--- Returns the state of a virtual key/axis in the last mapping used
-- state = evol.vState(v)
-- v: a virtual key or axis
-- state: 'down' or 'up' for a vkey, a number for an axis
-- Note that if not initialised manually in map.vaxes and map.vkeys,
-- axes and keys have a nil value until their first related event
vState = function(v)
	if not vmap or not vmap.vkeys or not vmap.vaxes then return end
	
	return vmap.vkeys[v] or vmap.vaxes[v]
end

--- Call callback associated to the virtual event
-- evol.treatVEvent(vevent,callbacks)
-- vevent: a virtual event, as returned by evol.vInput()
-- callbacks: the table of callbacks (see example)

treatVEvent = function(vevent,callbacks)
	
	-- vkeys
	if #vevent == 2 then
		
		local cb
		
		-- if there is a callback 'key', call it on down
		if vevent[2] == 'down' then
			cb = callbacks[vevent[1]]
		
		-- if there is a callback 'key_up', call it on up
		else
			cb = callbacks[vevent[1] .. '_up']
		end
		
		if cb then
			cb()
		end
	
	
	-- vaxes
	elseif #vevent == 3 and callbacks[vevent[1]] then
		
		-- call the callback, pass the newval and relval
		callbacks[vevent[1]](vevent[2],vevent[3])
	end
	
end


--- Polls all virtual events and calls the associated callbacks
-- evol.treatAllInput(map,callbacks)
-- map: the mapping table to use (see example)
-- callbacks: the table of callbacks (see example)

treatAllInput = function(map,cbs)
	
	local vevents = vInput(map)
	
	while vevents do
	
		for i,v in ipairs(vevents) do
			treatVEvent(v,cbs)
		end
		
		vevents = vInput(map)
	end	
end

--- Queries for mouse position and optionally the status of a button
-- x,y,[down] = evol.mouse([button])
-- button: the button to check status of
-- x,y: position of mouse
-- down: boolean stating if given button is down
mouse = evolcore.mouse

--- States if given mouse button is down
-- down = evol.isButtonDown(button)
-- button: the button to check status of
-- down: boolean stating if given button is down
isButtonDown = function(button)
	local _,_,down = mouse(button)
	return down == 1
end

--- States if given key is down
-- down = evol.isKeyDown(key)
-- key: the key to check status of
-- down: boolean stating if given key is down
isKeyDown = evolcore.isKeyDown

--- **Fonts**

--- A container for fonts
-- Font object
-- Fields:
-- 	font.width: glyph width if the font is monospace, or else 0
-- Shortcut methods:
-- 	font:write(text,x,y,[color,[image]])
-- 	font:free()

-- INTERNAL! -- Prepares a Font table
local Font = function()
	local tmp = {}
	
	tmp.write = write
	tmp.free = free
	
	tmp.type = "Font"
	
	return tmp
end

--- Loads a font
-- font = evol.loadFont(path,size)
-- path: string containing the path to the font to load (relative to current folder, or absolute)
-- size: number of pixels high the font should render in later
loadFont = function(path, size)
	local tmp = Font()
	tmp.font,tmp.width = evolcore.loadFont(path, size)

	return tmp
end

--- Renders text to an image
-- evol.write(font,text,x,y,[color,[image]]) or font:write(text,x,y,[color,[image]])
-- font: the font to use for render
-- text: UTF-8 string to render
-- x,y: position in pixels where the top-left corner of render text should appear
-- color = {r,g,b}: color to render the text in. Defaults to black.
-- image: destination image. Defaults to screen. Put 'new' to get a new surface.
write = function(font, text, x, y, color, surf)
	
	local surf = surf or {}
	local color = color or {0,0,0}
	local new = false
	
	if surf == 'new' then
		new = true
		surf = {}
	end
	
	local img,w,h = evolcore.printFont(font.font, text, color[1], color[2], color[3], x, y, surf.surf, new)
	
	-- If we requested a new image, we have to make it look like one
	if new then
		local tmp = Image()
		tmp.surf, tmp.w, tmp.h = img,w,h
		
		tmp.cx = 0
		tmp.cy = tmp.h
		
		return tmp
	end
end

--- **Graphic primitives**

--- Draws a line
-- evol.line(x,y,u,v,[color,[aa,[image]]])
-- (x,y),(u,v): coordinates in pixel of the start and end of the line
-- color = {r,g,b,[a]}: color of the line. Defaults to black.
-- aa: set to true to draw an anti-aliased line
-- image: the image to draw to. Defaults to screen.
line = function(x, y, u, v, color, aa, surf)
	
	local surf = surf or {}
	local color = color or {0,0,0}
	local aa = aa and 1 or 0
	
	if not color[4] then color[4] = 255 end		-- RGB are opaque by default
	
	evolcore.draw(x,y,u,v, color[1], color[2], color[3], color[4], 0, aa, surf.surf)
end

--- Draws a rectangle
-- evol.box(x,y,u,v,color,[fill,[image]])
-- (x,y),(u,v): coordinates in pixel of one corner and the opposite corner of the rectangle
-- color = {r,g,b,[a]}: color of the line. If nil, no line.
-- fill: = {r,g,b,[a]}: color of the filling. If nil, no filling.
-- image: the image to draw to. Defaults to screen.
box = function(x, y, u, v, color, fill, surf)
	
	local surf = surf or {}
	
	if color and not color[4] then color[4] = 255 end	-- RGB are opaque by default
	if fill and not fill[4] then fill[4] = 255 end		-- RGB are opaque by default
	
	if fill then
		evolcore.draw(x,y,u,v, fill[1], fill[2], fill[3], fill[4], 1, 2, surf.surf)
	end
	
	if color then
		evolcore.draw(x,y,u,v, color[1], color[2], color[3], color[4], 1, 0, surf.surf)
	end
end

--- Draws a circle
-- evol.circle(x,y,r,color,[aa,[fill,[image]]])
-- (x,y),r: coordinates in pixel of center of circle, and radius
-- color = {r,g,b,[a]}: color of the line. If nil, no line.
-- aa: if true, circle is antialiased
-- fill: = {r,g,b,[a]}: color of the filling. If nil, no filling.
-- image: the image to draw to. Defaults to screen.
circle = function(x, y, r, color, aa, fill, surf)

	local surf = surf or {}

	if color and not color[4] then color[4] = 255 end	-- RGB are opaque by default
	if fill and not fill[4] then fill[4] = 255 end		-- RGB are opaque by default

	if fill then	
		evolcore.draw(x,y,r,nil, fill[1], fill[2], fill[3], fill[4], 2, 2, surf.surf)
		if aa and not color then color = fill end
	end

	if color then
		if aa then aa = 1 else aa = 0 end
		evolcore.draw(x,y,r,nil, color[1], color[2], color[3], color[4], 2, aa, surf.surf)
	end	
end

--- Draws a triangle
-- evol.tri(x,y,u,v,m,n,color,[aa,[fill,[image]]])
-- (x,y),(u,v),(m,n): coordinates in pixel of the three vertices of the triangle
-- color = {r,g,b,[a]}: color of the line. If nil, no line.
-- aa: if true, triangle is antialiased
-- fill: = {r,g,b,[a]}: color of the filling. If nil, no filling.
-- image: the image to draw to. Defaults to screen.
tri = function(x, y, u, v, m, n, color, aa, fill, surf)

	local surf = surf or {}

	if color and not color[4] then color[4] = 255 end	-- RGB are opaque by default
	if fill and not fill[4] then fill[4] = 255 end		-- RGB are opaque by default

	if fill then
		evolcore.drawTri(x,y,u,v,m,n, fill[1], fill[2], fill[3], fill[4], 2, surf.surf)
		if aa and not color then color = fill end
	end

	if color then
		if aa then aa = 1 else aa = 0 end
		evolcore.drawTri(x,y,u,v,m,n, color[1], color[2], color[3], color[4], aa, surf.surf)
	end	
end

--- **Sound**

-- INTERNAL! -- Prepares a Sound table
local Sound = function()
	local tmp = {}
	
	tmp.play = play
	tmp.free = free
	
	tmp.type = "Sound"
	
	return tmp
end

-- INTERNAL! -- Prepares a Music table
local Music = function()
	local tmp = {}
	
	tmp.play = play
	tmp.free = free
	
	tmp.type = "Music"
	
	return tmp
end

--- Loads a sound
-- sound = evol.loadSound(path)
-- path: string containing the path to the sound to load (relative to current folder, or absolute)
loadSound = function(path)
	local tmp = Sound()
	tmp.sound = evolcore.loadSound(path)
	
	return tmp
end

--- Loads a music
-- music = evol.loadMusic(path)
-- path: string containing the path to the music to load (relative to current folder, or absolute)
loadMusic = function(path)
	local tmp = Music()
	tmp.music = evolcore.loadMusic(path)
	
	return tmp
end

--- Plays a sound or a music
-- evol.play(stuff,n)
-- stuff: a sound or music
-- n: the number of times to play. Defaults to 1 for sounds, and infinite (-1) for music
play = function(sound, loop)

	if sound.type == "Sound" then
		if not loop then loop = 1 end
		evolcore.playSound(sound.sound, loop-1)
	elseif sound.type == "Music" then
		if not loop then loop = -1 end
		evolcore.playMusic(sound.music, loop)
	end
end

--- Sets the music volume
-- old = evol.musicVolume(new)
-- new: if from 0 to 128, sets the volume this value. Else, does nothing.
-- old: volume before changing, or current volume if new isn't between 0 and 128.
musicVolume = evolcore.musicVolume


--- **Curses**

curses = {}

--- Inits evöL in curses mode
-- evol.curses.init(w,h,font,size)
-- w,h: width and height of the terminal, in characters.
-- font: string containing the path to the font to use
-- size: the desired font height in pixels
-- Sets the values evol.width, evol.height (screen size in pixels)
-- Sets the values evol.nw, evol.nh (terminal size in characters)
curses.init = function(w,h,font,size)
	nw = w
	nh = h
	cw, ch = evolcore.curses_init(w,h,font,size)
	width = nw * cw
	height = nh * ch
end

--- Prints the terminal to the screen
-- evol.curses.refresh()
-- (evol.flip() is still needed)
curses.refresh = evolcore.curses_refresh

--- Add a character at given position
-- evol.curses.addch(char,x,y)
-- char: a string containing the character to be printed
-- x,y: the position where to print the character, in the range (0,evol.nw), (0,evol.nh)
curses.addch = evolcore.curses_addch

--- Set the color at given position
-- evol.curses.setcol(x,y,r,g,b,br,bg,bb)
-- x,y: position where to change the color
-- r,g,b: the text color
-- br,bg,bb: the background color
-- Each of the 6 color values is unmodified if negative or nil
curses.setcol = evolcore.curses_setcol

--- Prints a string
-- evol.curses.print(str,x,y,[transpkey])
-- str: the string to print
-- x,y: the starting position (first character)
-- transpkey: a one character long string. If present, indicates a transparent character that is not printed
curses.print = function(str,x,y,transpkey)
	
	for i = 1,#str do
		local char = str:sub(i,i)
		if char ~= transpkey then
			evolcore.curses_addch(char,x+i-1,y)
		end
	end
end


--- Fills a rectangle character/color
-- evol.curses.fill(rect, [char, [fg, [bg]]])
-- rect = {x,y,w,h}: portion of screen to fill. Defaults to entire screen.
-- char: string containing the one character to fill the rectangle with. May be ommited to leave the text unchanged.
-- fg = {r,g,b}: text color to fill with. May be ommited.
-- bg = {r,g,b}: background color to fill with. May be ommited.
curses.fill = function(rect,char,fg,bg)
	local fg = fg or {}
	local bg = bg or {}
	local rect = rect or {0,0,nw,nh}
	
	evolcore.curses_fill(
		rect[1],rect[2],rect[3],rect[4],
		char,
		fg[1],fg[2],fg[3],
		bg[1],bg[2],bg[3]
	)
end

