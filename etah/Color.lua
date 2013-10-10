local class = require "etah.class"

local Color = class()

function Color:init(r,g,b,a)
	self:set(r,g,b,a)
end

function Color:set(r,g,b,a)
	self.r = r or 0
	self.g = g or 0
	self.b = b or 0
	self.a = a or 255
end

return Color