package.path = package.path .. ";../etah/?.lua"

require "etah"
require "Game"

print("etah version:", etah.version)

evol.init()
evol.set_mode("fullscreen")

etah.run(Game)

evol.quit()