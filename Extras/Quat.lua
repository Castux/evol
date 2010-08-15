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

local sqrt = math.sqrt
local cos = math.cos
local sin = math.sin

require "class"
require "v3"

Quat = class()

-- Use:
-- Quat(1,2,3,4)
-- Quat({1,2,3,4})
-- Quat{1,2,3,4}

-- Scalar + Vector form:
-- Quat(1,{2,3,4})

-- Quat() --> defaults to (0,0,0,0)

function Quat:init(...)
	local tab = {...}

	if type(tab[1]) == 'table' then
		tab = tab[1]
	elseif tab[1] == nil then
		tab = {0,0,0,0}
	end

	if type(tab[2]) == 'table' then
		self[1],self[2],self[3],self[4] =
			tab[1],tab[2][1],tab[2][2],tab[2][3]

	else
		for i=1,4 do
			self[i] = tab[i]
		end
	end

	return self
end

function Quat.fromSV(s,v)

	return Quat(s,v[1],v[2],v[3])
end

-- takes norm, unit v3 and angle
function Quat.fromPolar(len,n,theta)

	return len * (Quat(0,n)*theta):exp()
end


function Quat.__add(a,b)
	local c = Quat.new()

	for i=1,4 do
		c[i] = a[i] + b[i]
	end

	return c
end

function Quat.__sub(a,b)
	local c = Quat.new()

	for i=1,4 do
		c[i] = a[i] - b[i]
	end

	return c
end

function Quat.__unm(a,b)
	local c = Quat.new()

	for i=1,4 do
		c[i] = -a[i]
	end

	return c
end

function Quat.__mul(a,b)

	-- Quaternion multiplication

	if isa(a,Quat) and isa(b,Quat) then

		--[[ Slow as fuck but pretty

		local va = v3(a[2],a[3],a[4])
		local vb = v3(b[2],b[3],b[4])

		local s = a[1]*b[1] - va:dot(vb)
		local v = va*b[1] + a[1]*vb + va:cross(vb)

		return Quat(s,v[1],v[2],v[3])

		--]]

		local c = Quat.new()

		c[1] = a[1]*b[1] - a[2]*b[2] - a[3]*b[3] - a[4]*b[4]
		c[2] = a[2]*b[1] + a[1]*b[2] + a[3]*b[4] - a[4]*b[3]
		c[3] = a[3]*b[1] + a[1]*b[3] + a[4]*b[2] - a[2]*b[4]
		c[4] = a[4]*b[1] + a[1]*b[4] + a[2]*b[3] - a[3]*b[2]

		return c
	end

	-- multiplication by scalar

	if isa(a,Quat) then
		a,b = b,a
	end

	if type(a) ~= "number" then
		error("Trying to multiply quaternion by ".. type(a) )
	end

	local c = Quat.new()

	for i=1,4 do
		c[i] = a*b[i]
	end

	return c
end

function Quat.__div(a,b)
	local c = Quat.new()

	if not isa(a,Quat) or type(b) ~= "number" then
		error("Wrong types in quaternion scalar division")
	end

	for i=1,4 do
		c[i] = a[i]/b
	end

	return c
end

function Quat.__eq(a,b)

	for i=1,4 do
		if a[i] ~= b[i] then return false end
	end

	return true
end

-- used also a:dot(b)
function Quat.dot(a,b)
	local c = 0

	for i=1,4 do
		c = c + a[i]*b[i]
	end
	return c
end

function Quat:sqlen()

	return self:dot(self)
end

function Quat:len()

	return sqrt(self:sqlen())
end

Quat.sqnorm = Quat.sqlen
Quat.norm = Quat.len

function Quat:normalize()
	local length = self:len();

	-- Check if the length is 0.
	if (length == 0) then length = 1 end

	return self/length
end

function Quat:sqdist(b)
	return Quat.sqlen(self - b)
end

function Quat:dist(b)
	return sqrt(self:sqdist(b))
end

function Quat:conj()

	return Quat(self[1],-self[2],-self[3],-self[4])
end

function Quat:inv()
	return self:conj() / self:sqlen()
end

function Quat:exp()
	local s = self[1]
	local v = v3(self[2],self[3],self[4])

	local vlen = v:len()

	local s2 = cos(vlen)
	local v2 = v:normalize()*sin(vlen)

	return math.exp(s) * Quat(s2,v2[1],v2[2],v2[3])
end

-- return norm, unit vector, angle
function Quat:polar()

	local len = self:len()
	local n = v3(self[2],self[3],self[4]):normalize()
	local theta = math.acos(self[1]/len)

	return len,n,theta
end


function Quat.__tostring(a)

	return "(" .. tostring(a[1]) .. "," .. tostring(a[2])
		.. ",".. tostring(a[3]) .. ",".. tostring(a[4]) .. ")"
end

