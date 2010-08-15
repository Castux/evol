import glob

env = Environment()

env.Append(CCFLAGS = ['-Wall', '-O2'])

env.Append(CPPPATH = '/usr/include/lua5.1')

env.ParseConfig('sdl-config --cflags --libs')
env.Append(LIBS = ['SDL_image', 'SDL_ttf', 'SDL_gfx', 'SDL_mixer'])


env.SharedLibrary(target = 'evolcore', source = 'evolcore.c', SHLIBPREFIX='')
env.Install('/usr/local/lib/lua/5.1', ['evolcore.so','evol.lua','evol_keys.lua'])
env.Alias('install','/usr/local/lib/lua/5.1')

env.Append(LIBS = ['SDL_image', 'SDL_ttf', 'SDL_gfx', 'SDL_mixer','lua5.1'])
env.Program(target = 'evol_dingux_pc', source = ['evolcore.c','evol_dingux.c'])
