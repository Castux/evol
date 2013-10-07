local require = require
local ipairs = ipairs

local etah = {}
_ENV = etah

version = "0.1"

local submodules = {
	"class",
	"Director",
	"Scene"
}

for i,v in ipairs(submodules) do
	etah[v] = require("etah." .. v)
end

function run(first_scene)

	director = Director(first_scene)
	director:run()
	
end

return etah