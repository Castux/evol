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

require "v3"
require "Quat"

local cos = math.cos
local sin = math.sin
local acos = math.acos

Rotation = class()

-- provide either:
--	vector + angle. vector doesn't have to be normal
--	vector whose norm is the angle.
--	nothing. gives the null rotation


function Rotation:init(v,a)
	local v,a = v,a

	-- if no arguments are given, put the no-rotation
	if not v and not a then
		a = 0
	end
	
	
	-- make sure v is a vector object
	if not isa(v,v3) then
		v = v3(v)
	end

	-- if no angle is given, use the norm, and normalize v
	if not a then
		a = v:len()
		v = v/a

	else
		v = v:normalize()
	end

	-- use a unit quaternion to represent the rotation
	self.q = Quat(cos(a/2), v*sin(a/2))

	return self
end

-- retrieve axis and angle of the rotation

function Rotation:axisAngle()

	local a = 2*acos(self.q[1])
	local n = v3(self.q[2],self.q[3],self.q[4])/sin(a/2)

	return n,a
end

function Rotation:axis()

	local n,a = self:axisAngle()

	return n
end

function Rotation:angle()

	return 2*acos(self.q[1])
end

-- rotation composition

function Rotation.__mul(a,b)
	local c = Rotation.new()

	c.q = a.q*b.q

	return c
end

-- application to vector

function Rotation:apply(v)

	local qv = Quat(0,v)

	local r = self.q * qv * self.q:inv()

	return v3(r[2],r[3],r[4])
end

function Rotation:localBase()

	return self:apply{1,0,0},
		self:apply{0,1,0},
		self:apply{0,0,1}
end
