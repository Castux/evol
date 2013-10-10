local class = require "etah.class"

local Director = class()

function Director:init(first_scene, fps_cap)

	if not first_scene then
		error("first_scene argument missing")
	end

	self.fps_cap = fps_cap

	self.quit = false
	self.scene = nil
	self.next_scene_class = nil

	self.dt = 0
	self.fps = 0
	self.t = 0

	self:switch_scene(first_scene)
end

function Director:switch_scene(scene)
	self.next_scene_class = scene
end

function Director:execute_scene_switch()

	-- cleanup old scene?

	self.scene = self.next_scene_class()
	self.next_scene_class = nil
end

function Director:run()

	self:execute_scene_switch()

	local event = {}

	while not self.quit do

		-- timer

		self.dt, self.fps, self.t = evol.update_timer(self.fps_cap)

		-- input

		while evol.input(event) do

			if event.type == "quit" then
				self.quit = true
			else
				self.scene:handle_input(event)
			end

			for k,v in pairs(event) do
				event[k] = nil
			end
		end

		-- update

		self.scene:update()

		-- drawing

		evol.clear_screen(self.scene.color.r, self.scene.color.g, self.scene.color.b)
		self.scene:draw()
		evol.update_screen()

		-- switching

		if self.next_scene_class then
			self:execute_scene_switch()
		end
	end
end

return Director