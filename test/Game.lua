Game = etah.new_scene()

function Game:init()

	Game.super.init(self)

	self.background:set(100,100,0)

	print("game!")

	local ent = etah.Entity()
	ent.x = 20
	ent.y = 30
	ent.update = function()
		print("lol from ent")
	end
	self:add_child(ent)

	local ent2 = etah.Entity()
	ent2.x = 100
	ent2.y = 100
	ent2.update = function()
		print("lol from ent2", ent2.gx, ent2.gy)
	end
	ent:add_child(ent2)

end

function Game:input(event)

	for k,v in pairs(event) do
		io.write(k,':',v,', ')
	end
	io.write('\n')

end