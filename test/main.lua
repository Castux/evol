package.path = package.path .. ";../etah/?.lua"

require "etah"

print(etah.version)

require "Game"

game = Game()