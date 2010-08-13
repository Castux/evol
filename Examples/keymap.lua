-- Key mapping: defines virtual keys/buttons from actual hardware input devices

-- Every section is facultative, just fill what you need.
-- In the "keyboard" section, indices are the key name strings (found in evol_keys.lua).
-- Every section that ends in a number (gamepad*, hat*, axis*) can have
-- any number needed. The third hat of the 5th gamepad is gamepad5.hat3

-- The hat table doesn't have to be full. If you only need the right arrow,
-- you can do hat0[2] = 'rightaction'

-- Normally, vkeys and vaxes states are nil until the first action related to
-- them occurs. If you need an initial value, put it in the tables vkeys and
-- vaxes, each indexed by the vkey/axis name string.

-- This table is usually in a separate file, to allow the player to adapt it
-- (especially for gamepads which have very variable button numbering systems)

map = {

	-- if initialisation is needed
	vkeys = {actionA = 'up'},
	vaxes = {axisA = 0},

	keyboard = {
		
		a = 'actionA',
		b = 'actionB',
		space = 'actionC',
		escape = 'actionD',
		
		['1'] = 'actionC'
	},

	mousex = 'roll',
	mousey = 'pitch',

	mousebuttons = {
		[1] = 'actionE',
		[2] = 'actionF',
		[3] = 'actionG'
	},

	gamepad0 = {

		buttons = {
			[1] = 'actionH',
			[2] = 'actionI',
			[5] = 'actionJ'
		},

		hat0 = {'up','right','down','left'},
		
		axis0 = 'roll',
		axis1 = 'pitch'
	}
}

-- Callbacks for the vevents

-- When a vkey is pressed, the function with index 'vkey' is called.
-- When a vkey is released, the entry 'vkey_up' is called.
-- When a vaxis changes, the entry 'vaxis' is called with parameters newval and relval.

-- This table is usually in your main loop, directly passed to treatAllInput(),
-- since the callbacks usually need references to game objects.

cb = {

	actionA = function()
	
		print("actionA pressed")
	end,
	
	actionA_up = function()
	
		print("actionA released")
	end,
	
	roll = function(...)
	
		print("axis 'roll' changed: ", ...)
	end
	
}
