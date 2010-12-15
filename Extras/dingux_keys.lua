-- mapping between Dingoo's buttons and evol's key values

require "evol_keys"

dingux = {

	up = 'up',
	down ='down',
	left = 'left',
	right = 'right',

	A = 'lctrl',
	B = 'lalt',
	X = 'space',
	Y = 'lshift',

	L = 'tab',
	R = 'backspace',

	start = 'enter',
	select = 'escape'

}

-- reverse table

for k,v in pairs(dingux) do
	dingux[v] = k
end
