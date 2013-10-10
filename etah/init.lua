local require = require
local ipairs = ipairs

local etah = {}
_ENV = etah

version = "0.1"

local submodules = {
	"class",
	"Director",
	"Entity",
	"Scene",
	"Color"
}

for i,v in ipairs(submodules) do
	etah[v] = require("etah." .. v)
end

function run(first_scene)

	director = Director(first_scene)
	director:run()
	
end

function new_scene()
	return etah.class(etah.Scene)
end

function new_entity()
	return etah.class(etah.Entity)
end

return etah