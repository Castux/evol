local class = require "etah.class"
local Color = require "etah.Color"

local Entity = class()

function Entity:init()
	
	-- public --

	-- transform
	self.x = 0
	self.y = 0

	-- color transform
	self.color = Color(1,1,1,1)

	-- tree structure
	self.children = {}
	self.parent = nil

	-- flags
	self.listening = true
	self.active = true
	self.visible = true

	-- internal --

	-- global transform
	self.gx = 0
	self.gy = 0
	self.gcolor = Color()

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

	self:update_global_transform()

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

function Entity:add_child(child, position)

	if position then
		table.insert(self.children, position, child)
	else
		table.insert(self.children, child)
	end

	child.parent = self
end

function Entity:remove_child(child)

	for i,v in ipairs(self.children) do
		if v == child then
			child.parent = nil
			table.remove(self.children, i)
			return
		end
	end
end

function Entity:update_global_transform()

	if not self.parent then
		return
	end

	self.gx = self.parent.gx + self.x
	self.gy = self.parent.gy + self.y

	self.gcolor:multiply(self.parent.gcolor)
end

return Entity