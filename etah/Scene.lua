local class = require "etah.class"
local Color = require "etah.Color"
local Entity = require "etah.Entity"

local Scene = class(Entity)

function Scene:init()

	Scene.super.init(self)

	self.color = Color()
end

return Scene