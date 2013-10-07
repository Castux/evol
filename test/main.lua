package.path = package.path .. ";../etah/?.lua"

etah = require "etah"
require "Game"

print("etah version:", etah.version)

evol.init()
evol.set_mode("windowed", 400, 300)

etah.run(Game)

evol.quit()