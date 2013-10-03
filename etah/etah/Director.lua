Director = class()

function Director:init(first_scene)
	self:switch_scene(first_scene)
end

function Director:switch_scene(scene)
	self.scene = scene()
end

function Director:run()

	-- main loop

end