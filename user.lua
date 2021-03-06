function printf(...)
	return io.write(string.format(...))
end


function move()
	local player = C4:player()
	local width = C4:width()
	local height = C4:height()

	for i = height,1,-1 do
		printf('|')
		for j = 1,width do
			printf(' %d |', C4[i][j])
		end
		printf('\n')
	end

	printf('- [%d] Make your move:\n', player)
	repeat
		m = io.read('*n')
		if m == nil or m < 1 or m > width or C4[height][m] ~= 0 then
			printf('- Try again!\n')
			m = nil
		end
	until m ~= nil

	return m
end
