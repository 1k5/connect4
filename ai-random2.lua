print('- Initializing random number generator.');
math.randomseed( os.time() )

function printf(...)
	return io.write(string.format(...))
end


function move()
	local player = C4:player()
	local width = C4:width()
	local height = C4:height()

	printf('- [%d] Making a move:', player)
	if (C4[1][4] == 0) then
		m = 4
	else
		repeat
			m = math.random(width)
		until(C4[height][m] == 0)
	end
	printf(' %d.\n', m)

	return m
end
