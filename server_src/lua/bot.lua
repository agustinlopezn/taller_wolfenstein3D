positionables = {}
blockings = {}
players = {}
self_id = 0
grid_size = 64
straight_line = {}
closest_target = {}
position = {}
in_sight = {}
in_sight_len = 0
angle = 0
angle_turn = 0
position = {x = 96, y = 96}

function addPositionable(x, y, _type)
	io.write("[LUA] Executing addPositionable("..x..", "..y..", ".._type..")\n")
	local coord_table = {math.floor(x), math.floor(y) }
	positionables[coord_table] = {type = _type}
	print(positionables[coord_table])
end

function addBlocking(x, y, _type)
	io.write("[LUA] Executing addBlocking("..x..", "..y..", ".._type..")\n")
	local coord_table = {math.floor(x), math.floor(y)}
	blockings[coord_table] = {type = _type}
end

function addPlayer(x, y, _id)
	io.write("[LUA] Executing addPlayers("..x..", "..y..", ".._id..")\n")
	local coord_table = {math.floor(x), math.floor(y)}
	players[coord_table] = {id = math.floor(_id)}
end

function setId(_id)
	io.write("[LUA] Executing setId(".._id..")\n")
	self_id = id
end

function printMap()
	print("[LUA] Executing printMap()")
	print("Positionables:")
	for k, v in pairs(positionables) do
		print(string.format("Coord (%s, %s) -> Type: %s", k[1], k[2], v.type))
	end
	print("Blockings:")
	for k, v in pairs(blockings) do
		print(string.format("Coord (%s, %s) -> Type: %s", k[1], k[2], v.type))
	end
	print("Players:")
	for k, v in pairs(players) do
		print(string.format("Coord (%s, %s) -> ID: %s", k[1], k[2], v.id))
	end
	-- PRUEBA DE BLOCKING PAPA --
	--[[
	for k, v in pairs(players) do
		if isABlockingItemAt(k[1], k[2]) then
			print("SI HAY BLOKING ITEM EN PLAYER")
		else
			print("NO HAY BLOCKING ITEM EN PLAYER")
		end
	end

	for k, v in pairs(blockings) do
		if isABlockingItemAt(k[1], k[2]) then
			print("SI HAY BLOKING ITEM EN BLOKING")
		else
			print("NO HAY BLOCKING ITEM EN BLOKIN")
		end
	end
	--]]
end

------------------------------- Calculos -------------------------------

function isInTable(tab, x, y)
	for key, value in pairs(tab) do
		if key[1] == x and key[2] == y then
			return true
		end
	end
	return false
end

function normalizedCoordinate(i)
	local normalized = (math.floor(i / grid_size)) * grid_size
	return normalized
end

function isABlockingItemAt(x, y)
	local x_norm = (normalizedCoordinate(x))
	local y_norm = (normalizedCoordinate(y))
	--io.write("[LUA] Coord: ("..x_norm..", "..y_norm..")\n")
	local coord_table = {x_norm, y_norm }
	return isInTable(blockings, x_norm, y_norm)
end

function getDiff(x_old, y_old, x_new, y_new)
	local x_diff = math.abs(x_old - x_new)
	local y_diff = math.abs(y_old - y_new)
	return (x_diff + y_diff)
end

-- TEST USE
function executeClosestTarget(x_old, y_old, x_new, y_new)
	io.write("[LUA] Calling isInSight("..x_old..", "..y_old..", "..x_new..", "..y_new..")\n")
	return_value = isInSight(x_old,y_old,x_new,y_new)
	return_value = math.floor(return_value)
	if return_value == 1 then
		io.write("[LUA] isInSight returned "..return_value.."\n")
	else
		io.write("[LUA] isInSight returned NOOO\n")
	end
	getDiff(1,2,3,4)
end




function closestTarget()
	in_sight_len = 0
	local min_difference = math.huge
	io.write("[LUA] Executing closestTarget\n")

	for coord, player in pairs(players) do
		io.write("[LUA] Calling isInSight("..coord[1]..", "..coord[2]..", "..position.x..", "..position.y..")\n")
		found_in_sight = math.floor(isInSight(coord[1],coord[2],position.x,position.y))
		io.write("[LUA] isInSight returned "..found_in_sight.."\n")

		if (found_in_sight == 1) then
			in_sight[coord] = true
			in_sight_len = in_sight_len + 1
		end
	end
	if in_sight_len > 0 then
		for coord, _ in pairs(in_sight) do
			difference = getDiff(coord[1], coord[2], position.x, position.y)
			print(string.format("For (in_sight) iteration with diff: %s", difference))
			if difference < min_difference then -- Aca muere, no hace mas nada
				io.write("[LUA] Min diff is: "..difference.."\n[LUA] Closest target is: "..coord[1]..", "..coord[2].."\n")
				min_difference = difference
				closest_target = coord
			end
		end
	else
		for coord, _ in pairs(players) do
			difference = getDiff(coord[1], coord[2], position.x, position.y)
			print(string.format("For (players) iteration (no players in sight) with diff: %s", difference))
			if difference < min_difference then
				io.write("[LUA] Min diff is: "..difference.."\n[LUA] Closest target is: "..coord[1]..", "..coord[2].."\n")
				min_difference = difference
				closest_target = coord
			end
		end
	end

	print("Finished closestTarget, calling tryMove")
	tryMove(closest_target[1], closest_target[2], min_difference)
end

--[[
Siempre tiene un objetivo: closest target carga el mas cercano y
 llamo a moveToTarget que ejecuta un numero random entre 1 y 5 el tryMove
 para tratar de acercarse

Las funciones de tryMove usan el move to position del col handler

funciones de crear eventos son llamadas por las funciones q mueven, devuelven
eventos liso y llano con los ints

]]--

function moveToPosition(x, y, angle)
	local new_x, new_y = move(position.x, position.y, angle)
	io.write("[LUA] moveToPosition new recv Coord: ("..math.floor(new_x)..", "..math.floor(new_y)..")\n")
	return math.floor(new_x), math.floor(new_y)
end

function tryMove(destiny_x, destiny_y, min_difference)
	io.write("[LUA] Executing tryMove with ("..destiny_x..", "..destiny_y..") and min_diff: "..min_difference.."\n")

	-- Create all tries to move and get distances
	local x_move_front, y_move_front = moveToPosition(position.x, position.y, angle)
	local diff_self_front = getDiff(x_move_front, y_move_front, position.x, position.y)
	local diff_front = getDiff(x_move_front, y_move_front, destiny_x, destiny_y)
	if diff_self_front == 0 then
		io.write("diff_self_front is 0, changing to inf\n")
		diff_front = math.huge
	end
	io.write("diff_front is now: "..diff_front.."\n")

	local x_move_right, y_move_right = moveToPosition(position.x, position.y, angle - math.pi/2)
	local diff_self_right = getDiff(x_move_right, y_move_right, position.x, position.y)
	local diff_right = getDiff(x_move_right, y_move_right, destiny_x, destiny_y)
	if diff_self_right == 0 then
		io.write("diff_self_right is 0, changing to inf\n")
		diff_right = math.huge
	end
	io.write("diff_right is now: "..diff_right.."\n")

	local x_move_left, y_move_left = moveToPosition(position.x, position.y, angle + math.pi/2)
	local diff_self_left = getDiff(x_move_left, y_move_left, position.x, position.y)
	local diff_left = getDiff(x_move_left, y_move_left, destiny_x, destiny_y)
	if diff_self_left == 0 then
		io.write("diff_self_left is 0, changing to inf\n")
		diff_left = math.huge
	end
	io.write("diff_left is now: "..diff_left.."\n")

	local x_move_back, y_move_back = moveToPosition(position.x, position.y, angle + math.pi)
	local diff_self_back = getDiff(x_move_back, y_move_back, position.x, position.y)
	local diff_back = getDiff(x_move_back, y_move_back, destiny_x, destiny_y)
	if diff_self_back == 0 then
		io.write("diff_self_back is 0, changing to inf\n")
		diff_back = math.huge
	end
	io.write("diff_back is now: "..diff_back.."\n")

	-- Verify which is the lowest
	if diff_front <= diff_left and diff_front <= diff_right and diff_front <= diff_back then
		io.write("diff_front is the lowest: "..diff_front.."\n")
		--tryFront()
		--create_moveEvent()
	elseif diff_left <= diff_front and diff_left <= diff_right and diff_left <= diff_back then
		io.write("diff_left is the lowest: "..diff_left.."\n")
		--angles_move = tryLeft()
		--create_TurnLeftEvent(4)
		--create_moveEvent()
	elseif diff_right <= diff_front and diff_right <= diff_left and diff_right <= diff_back then
		io.write("diff_right is the lowest: "..diff_right.."\n")
		--angles_move = tryRight()
		--create_TurnRightEvent(4) // crea 4 eventos de giro derecha
		--create_moveEvent() // crea 1 evento de avanzar
	else
		io.write("diff_back is the lowest: "..diff_back.."\n")
	end

	--if inRange(closest_target)
	--	create_pikanazoEvent()

end


----------------------------------- AUX -----------------------------------

function is_int(n)
	return (type(n) == "number") and (math.floor(n) == n)
end

function split (inputstr, sep)
	if sep == nil then
		sep = "%s"
	end
	local t={}
	for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
		table.insert(t, str)
	end
	return t
end