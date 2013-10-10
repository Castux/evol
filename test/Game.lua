Game = etah.new_scene()

function Game:init()

	Game.super.init(self)

	self.color:set(100,100,0)

	print("game!")
end

function Game:input(event)

	for k,v in pairs(event) do
		io.write(k,':',v,', ')
	end
	io.write('\n')

end