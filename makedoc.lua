file = io.open("evol.lua")

line = file:read()

entries = {}

while line do
	
	line = string.match(line,"^%-%-%- (.*)")
	if line then
		entry = {}
		
		-- add description
		entry.desc = line
		
		
		line = file:read()
		if string.match(line,"^%-%- (.*)") then
		
			-- add use
			entry.use = string.sub(line,4)
		
			-- add anything until we find until a non comment line
			line = file:read()
			while string.match(line,"^%-%- (.*)") do
				table.insert(entry, string.sub(line,4))
				line = file:read()
			end
		end
		
		table.insert(entries,entry)
	end
	
	
	line = file:read()
end

-- output

out = io.open("API-reference.txt","w")

-- quick ref

out:write("Quick reference\n===============\n\n")

for k,v in pairs(entries) do
	if v.use then
		out:write(v.use .. '\n')
	else
		out:write('\n' .. v.desc .. '\n')
	end
end

out:write("\n\nDetailed reference\n==================\n\n")

for k,v in pairs(entries) do

	if v.use then
		out:write(v.use .. '\n\t' .. v.desc .. '\n\n')
	else
		out:write(v.desc .. '\n')
	end
	
	for i,l in ipairs(v) do
		out:write('\t' .. l .. '\n')
	end
	out:write('\n')
end
