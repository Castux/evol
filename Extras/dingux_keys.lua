require "evol_keys"

dingux = {

	up = keys.up,
	down = keys.up,
	left = keys.left,
	right = keys.right,

	A = keys.lctrl,
	B = keys.lalt,
	X = keys.space,
	Y = keys.lshift,

	L = keys.tab,
	R = keys.backspace,

	start = keys.enter,
	select = keys.escape

}

-- reverse table

for k,v in pairs(dingux) do
	dingux[v] = k
end
