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

require "class"

v3 = class()

-- Use:
-- v3(1,2,3)
-- v3({1,2,3})
-- v3{1,2,3)
-- v3() --> defaults to (0,0,0)

function v3:init(...)
	local tab = {...}

	if type(tab[1]) == 'table' then
		tab = tab[1]
	elseif tab[1] == nil then
		tab = {0,0,0}
	end

	for i=1,3 do
		self[i] = tab[i]
	end

	return self
end


function v3.__add(a,b)
	local c = v3.new()

	for i=1,3 do
		c[i] = a[i] + b[i]
	end

	return c
end

function v3.__sub(a,b)
	local c = v3.new()

	for i=1,3 do
		c[i] = a[i] - b[i]
	end

	return c
end

function v3.__unm(a)
	local c = v3.new()

	for i=1,3 do
		c[i] = -a[i]
	end

	return c
end

function v3.__mul(a,b)
	local c = v3.new()
	local a,b = a,b
	
	if isa(a,v3) then
		a,b = b,a
	end

	if type(a) ~= "number" then
		error("Trying to multiply vector by ".. type(a) )
	end

	for i=1,3 do
		c[i] = a*b[i]
	end

	return c
end

function v3.__div(a,b)
	local c = v3.new()

	if not isa(a,v3) or type(b) ~= "number" then
		error("Wrong types in vector scalar division")
	end

	for i=1,3 do
		c[i] = a[i]/b
	end

	return c
end

function v3.__eq(a,b)

	for i=1,3 do
		if a[i] ~= b[i] then return false end
	end

	return true
end


-- used also a:dot(b)
function v3.dot(a,b)
	local c = 0

	for i=1,3 do
		c = c + a[i]*b[i]
	end
	return c
end

-- used also a:cross(b)
function v3.cross(a,b)
	local c = v3.new()

	c[1] = a[2]*b[3] - a[3]*b[2]
	c[2] = a[3]*b[1] - a[1]*b[3]
	c[3] = a[1]*b[2] - a[2]*b[1]

	return c
end


function v3:sqlen()

	return self:dot(self)
end

function v3:len()

	return sqrt(self:sqlen())
end

v3.sqnorm = v3.sqlen
v3.norm = v3.len

function v3:normalize()
	local length = self:len();

	-- Check if the length is 0.
	if (length == 0) then length = 1 end

	return self/length
end

function v3:sqdist(b)
	return v3.sqlen(self - b)
end

function v3:dist(b)
	return sqrt(self:sqdist(b))
end

-- takes in a Rotation, or the same arguments as Rotation:init()
function v3:rotate(...)

	local r = {...}

	if not isa(r[1],Rotation) then
		r = Rotation(...)
	else
		r = r[1]
	end

	return r:apply(self)
end

function v3.__tostring(a)

	return "(" .. tostring(a[1]) .. "," .. tostring(a[2]) .. ",".. tostring(a[3]) .. ")"
end

