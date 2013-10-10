local class = require "etah.class"
local Color = require "etah.Color"

local Entity = class()

function Entity:init()
	
	-- transform
	self.x = 0
	self.y = 0

	-- color transform
	self.color = Color()

	-- tree structure
	self.children = {}

	-- flags
	self.listening = true
	self.active = true
	self.visible = true

end

function Entity:internal_input(event)

	if not self.listening then
		return
	end

	if self.input then
		self:input(event)
	end

	for i,v in ipairs(self.children) do
		v:internal_input(event)
	end

end

function Entity:internal_update()

	if not self.active then
		return
	end

	if self.update then
		self:update()
	end

	for i,v in ipairs(self.children) do
		v:internal_update(event)
	end
end

function Entity:internal_draw()

	if not self.visible then
		return
	end

	if self.draw then
		self:draw()
	end

	for i,v in ipairs(self.children) do
		v:internal_draw(event)
	end
end

return Entity