Game = etah.class(etah.Scene)
local super = Game.super

function Game:init()

	super.init(self)

	self.color:set(100,100,0)

	print("game!")
end

function Game:handle_input(event)

	for k,v in pairs(event) do
		io.write(k,':',v,', ')
	end
	io.write('\n')

end