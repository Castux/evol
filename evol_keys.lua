-- evols.keys.lua -- keysyms definitions
-- Copyright (C) 2008 Noé Falzon

-- This program is  free software:  you can redistribute it and/or modify it
-- under  the terms  of the  GNU General Public License  as published by the
-- Free Software Foundation, version 3 of the License.
--
-- This program  is  distributed  in the hope  that it will  be useful,  but
-- WITHOUT  ANY WARRANTY;  without  even the implied  warranty of MERCHANTA-
-- BILITY  or  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
-- License for more details.
--
-- You should have received a copy of the  GNU General Public License  along
-- with this program. If not, see <http://www.gnu.org/licenses/>.

-- This is a direct translation of SDL's keysym definitions in SDL's keysym.h


-- the keyboard syms have been cleverly chosen to map to acsii
keys = {}

keys.unknown = 0
keys.first = 0
keys.backspace = 8
keys.tab = 9
keys.clear = 12
keys['return'] = 13
keys.pause = 19
keys.escape = 27
keys.space = 32
keys.exclaim = 33
keys.quotedbl = 34
keys.hash = 35
keys.dollar = 36
keys.ampersand = 38
keys.quote = 39
keys.leftparen = 40
keys.rightparen = 41
keys.asterisk = 42
keys.plus = 43
keys.comma = 44
keys.minus = 45
keys.period = 46
keys.slash = 47
keys['0'] = 48
keys['1'] = 49
keys['2'] = 50
keys['3'] = 51
keys['4'] = 52
keys['5'] = 53
keys['6'] = 54
keys['7'] = 55
keys['8'] = 56
keys['9'] = 57
keys.colon = 58
keys.semicolon = 59
keys.less = 60
keys.equals = 61
keys.greater = 62
keys.question = 63
keys.at = 64

-- skip uppercase letters

keys.leftbracket = 91
keys.backslash = 92
keys.rightbracket = 93
keys.caret = 94
keys.underscore = 95
keys.backquote = 96
keys.a = 97
keys.b = 98
keys.c = 99
keys.d = 100
keys.e = 101
keys.f = 102
keys.g = 103
keys.h = 104
keys.i = 105
keys.j = 106
keys.k = 107
keys.l = 108
keys.m = 109
keys.n = 110
keys.o = 111
keys.p = 112
keys.q = 113
keys.r = 114
keys.s = 115
keys.t = 116
keys.u = 117
keys.v = 118
keys.w = 119
keys.x = 120
keys.y = 121
keys.z = 122
keys.delete = 127
-- end of ascii mapped keysyms

-- international keyboard syms
keys.world_0 = 160 -- 0xa0
keys.world_1 = 161
keys.world_2 = 162
keys.world_3 = 163
keys.world_4 = 164
keys.world_5 = 165
keys.world_6 = 166
keys.world_7 = 167
keys.world_8 = 168
keys.world_9 = 169
keys.world_10 = 170
keys.world_11 = 171
keys.world_12 = 172
keys.world_13 = 173
keys.world_14 = 174
keys.world_15 = 175
keys.world_16 = 176
keys.world_17 = 177
keys.world_18 = 178
keys.world_19 = 179
keys.world_20 = 180
keys.world_21 = 181
keys.world_22 = 182
keys.world_23 = 183
keys.world_24 = 184
keys.world_25 = 185
keys.world_26 = 186
keys.world_27 = 187
keys.world_28 = 188
keys.world_29 = 189
keys.world_30 = 190
keys.world_31 = 191
keys.world_32 = 192
keys.world_33 = 193
keys.world_34 = 194
keys.world_35 = 195
keys.world_36 = 196
keys.world_37 = 197
keys.world_38 = 198
keys.world_39 = 199
keys.world_40 = 200
keys.world_41 = 201
keys.world_42 = 202
keys.world_43 = 203
keys.world_44 = 204
keys.world_45 = 205
keys.world_46 = 206
keys.world_47 = 207
keys.world_48 = 208
keys.world_49 = 209
keys.world_50 = 210
keys.world_51 = 211
keys.world_52 = 212
keys.world_53 = 213
keys.world_54 = 214
keys.world_55 = 215
keys.world_56 = 216
keys.world_57 = 217
keys.world_58 = 218
keys.world_59 = 219
keys.world_60 = 220
keys.world_61 = 221
keys.world_62 = 222
keys.world_63 = 223
keys.world_64 = 224
keys.world_65 = 225
keys.world_66 = 226
keys.world_67 = 227
keys.world_68 = 228
keys.world_69 = 229
keys.world_70 = 230
keys.world_71 = 231
keys.world_72 = 232
keys.world_73 = 233
keys.world_74 = 234
keys.world_75 = 235
keys.world_76 = 236
keys.world_77 = 237
keys.world_78 = 238
keys.world_79 = 239
keys.world_80 = 240
keys.world_81 = 241
keys.world_82 = 242
keys.world_83 = 243
keys.world_84 = 244
keys.world_85 = 245
keys.world_86 = 246
keys.world_87 = 247
keys.world_88 = 248
keys.world_89 = 249
keys.world_90 = 250
keys.world_91 = 251
keys.world_92 = 252
keys.world_93 = 253
keys.world_94 = 254
keys.world_95 = 255 -- 0xff

-- numeric keypad
keys.kp0 = 256
keys.kp1 = 257
keys.kp2 = 258
keys.kp3 = 259
keys.kp4 = 260
keys.kp5 = 261
keys.kp6 = 262
keys.kp7 = 263
keys.kp8 = 264
keys.kp9 = 265
keys.kp_period = 266
keys.kp_divide = 267
keys.kp_multiply = 268
keys.kp_minus = 269
keys.kp_plus = 270
keys.kp_enter = 271
keys.kp_equals = 272

-- arrows + home/end pad
keys.up = 273
keys.down = 274
keys.right = 275
keys.left = 276
keys.insert = 277
keys.home = 278
keys['end'] = 279
keys.pageup = 280
keys.pagedown = 281

-- function keys
keys.f1 = 282
keys.f2 = 283
keys.f3 = 284
keys.f4 = 285
keys.f5 = 286
keys.f6 = 287
keys.f7 = 288
keys.f8 = 289
keys.f9 = 290
keys.f10 = 291
keys.f11 = 292
keys.f12 = 293
keys.f13 = 294
keys.f14 = 295
keys.f15 = 296

-- key state modifier keys
keys.numlock = 300
keys.capslock = 301
keys.scrollock = 302
keys.rshift = 303
keys.lshift = 304
keys.rctrl = 305
keys.lctrl = 306
keys.ralt = 307
keys.lalt = 308
keys.rmeta = 309
keys.lmeta = 310
keys.lsuper = 311
keys.rsuper = 312
keys.mode = 313
keys.compose = 314

-- miscellaneous function keys
keys.help = 315
keys.print = 316
keys.sysreq = 317
keys['break'] = 318
keys.menu = 319
keys.power = 320 -- power macintosh power key
keys.euro = 321 -- some european keyboards
keys.undo = 322 -- atari keyboard has undo


-- reverse table, stored in the same place

for k,v in pairs(keys) do
	keys[v] = k
end
