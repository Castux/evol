local class = require "etah.class"

local Color = class()

function Color:init(r,g,b,a)
	self:set(r,g,b,a)
end

function Color:set(r,g,b,a)
	self.r = r or 0
	self.g = g or 0
	self.b = b or 0
	self.a = a or 1
end

function Color:multiply(other)
	self.r = self.r * other.r
	self.g = self.g * other.g
	self.b = self.b * other.b
	self.a = self.a * other.a
end

return Color