module(..., package.seeall)

version = "0.1"

require "etah.class"
require "etah.Director"

function init(scene)

	director = Director(scene)
	director:run()
	
end