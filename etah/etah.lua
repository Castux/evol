module(..., package.seeall)

version = "0.1"

require "etah.class"
require "etah.Director"
require "etah.Scene"

function run(first_scene)

	director = Director(first_scene)
	director:run()
	
end