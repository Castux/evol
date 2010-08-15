--[[
	Copyright © 2010 Noé Falzon

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
]]

local cos = math.cos
local sin = math.sin
local max = math.max
local sqrt = math.sqrt
local acos = math.acos
local atan2 = math.atan2

require "class"

v2 = class()

function v2:init(x,y)

	self[1] = x
	self[2] = y
	
	return self
end

function v2.newAngle(angle)
	local vector = v2.new()
	local a = angle or 0
	
	vector[1] = cos(a)
	vector[2] = sin(a)
	
	return vector
end

function v2.__add(a,b)
	local c = v2.new()
	
	c[1] = a[1] + b[1]
	c[2] = a[2] + b[2]
	
	return c
end

function v2.__sub(a,b)
	local c = v2.new()
	
	c[1] = a[1] - b[1]
	c[2] = a[2] - b[2]
	
	return c
end

function v2.__unm(a)
	local c = v2.new()
	
	c[1] = -a[1]
	c[2] = -a[2]
	
	return c
end

function v2.__mul(a,b)
	local c = v2.new()
	local a,b = a,b
	
	if isa(a,v2) then
		a,b = b,a
	end

	if type(a) ~= "number" then
		error("Trying to multiply vector by ".. type(a) )
	end

	c[1] = a*b[1]
	c[2] = a*b[2]
	
	return c
end

function v2.__div(a,b)
	local c = v2.new()
	
	if type(b) ~= "number" then 
		error("Incorrect type in vector scalar division")
	end
	
	c[1] = a[1]/b
	c[2] = a[2]/b
	
	return c
end

function v2.__eq(a,b)

	return a[1] == b[1] and a[2] == b[2]
end


-- used also a:dot(b)
function v2.dot(a,b)
	
	return a[1]*b[1] + a[2]*b[2]
end

-- used also a:cross(b)
function v2.cross(a,b)

	return a[1]*b[2] - a[2]*b[1]
end

-- rotate by angle around center 'vector'
function v2:rotate(angle,vector)
	local a = angle
	local vector = vector or v2.new(0,0)
	
	local cx = cos( a ) * (self[1] - vector[1])
	local cy = cos( a ) * (self[2] - vector[2])
	local sx = sin( a ) * (self[1] - vector[1])
	local sy = sin( a ) * (self[2] - vector[2])
	local x = vector[1] + (cx - sy)
	local y = vector[2] + (cy + sx)
	
	-- Return the new vector.
	return v2.new(x,y)
end

function v2:sqlen()
	
	return self:dot(self)
end

function v2:len()
	return sqrt(self:sqlen())
end

v2.norm = v2.len
v2.sqnorm = v2.sqlen

function v2:angle()
	return atan2(self[2],self[1])
end

function v2:normalize()
	local length = self:len()
	
	-- Check if the length is 0.
	if (length == 0) then length = 1 end
	
	return self/length
end

function v2:sqdist(b)
	return v2.sqlen(self - b)
end

function v2:dist(b)
	return sqrt(self:sqdist(b))
end

function v2.__tostring(a)
	local s = "("
	
	for i=1,#a-1 do
		s = s .. a[i] .. ","
	end
	
	return s .. a[#a] .. ")"
end
