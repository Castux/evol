Game = etah.class(etah.Scene)
local super = Game.super

function Game:init()

	super.init(self)

	print("game!")
end

function Game:draw()

	evol.clear_screen(100,100,100)

end