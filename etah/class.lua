local isa = function(obj,cla)

	-- Is is even an object?
	if type(obj) ~= "table" or not obj.class then
		return false
	end

	-- Is it a descendant of class?
	if obj.super then
		return obj.class == cla or obj.super:isa(cla)
	else
		return obj.class == cla
	end
end

local class = function(base)
	local c = {}
	c.__index = c
	c.class = c

	-- constructor (sets metatable and calls init)
	c.new = function(...)
		local obj = {}
		setmetatable(obj,c)

		-- automatically call init method with same arguments
		local res = obj:init(...)
		return res or obj
	end

	-- isa() method
	c.isa = isa

	-- inheritance
	if base then
		setmetatable(c, {__index = base, __call = function(t, ...) return c.new(...) end })
		c.super = base
	else
		-- default init, needed by base class only.
		-- the children always inherit one, or redefine one
		function c:init()
			return self
		end

		setmetatable(c, {__call = function(t, ...) return c.new(...) end })
	end

	return c
end

return class