-- key mapping: defines virtual keys/buttons

map = {}

-- if initialisation is needed
map.vkeys = {attack = 'up'}
map.vaxes = {stick1x = 54}

map.keyboard = {
	
	a = 'attack',
	p = 'print',
	q = 'print',	-- it too !
	up = 'up'
}

map.mousebuttons = {'fire','zoom','reload',[5] = 'zoomout'}

map.gamepad0 = {}

map.gamepad0.buttons = {
	[1] = 'blah',
	[2] = 'blij',
	[5] = 'buh'
}

map.gamepad0.hat0 = {'up','right','down','left'}

-- and analogs

map.mousex = 'mousex'
map.mousey = 'mousey'

map.gamepad0.axis0 = 'stick1x'
map.gamepad0.axis1 = 'stick1y'
map.gamepad0.axis2 = 'stick2x'
map.gamepad0.axis3 = 'stick2y'


-- event triggering (-->callbacks)
-- for the virtual buttons

-- event is a table indexed by virtual buttons, containing tables of the
-- form {down = fun, up = fun, isdown = , isup}

-- down and up are called when status changes.
-- isdown and isup are called every frame.

vkeys = {}

vkeys.attack = {
	down = function()
		-- do attack
	end
}
