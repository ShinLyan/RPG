extends TileMap


export(Vector2) var map_size = Vector2.ONE * 31




var obstacles = []


onready var _half_cell_size = cell_size / 2


var flag = true

var walkable_cells_list


func _process(delta):
	if flag:
		flag = false
		walkable_cells_list = get_parent().get_walkable_cells_list()
		#astar_add_walkable_cells(walkable_cells_list)
		astar_connect_walkable_cells_diagonal(walkable_cells_list)
		
		


onready var astar_node = AStar.new()
onready var rect = $EdgeOfMap.get_used_rect()


func astar_add_walkable_cells(walkable_cells_list = []): # формирует список точек, по которым можно ходить
	for point in walkable_cells_list:
		var point_index = calculate_point_index(point)
		astar_node.add_point(point_index, Vector3(point.x, point.y, 0.0))


func calculate_point_index(point):
	return point.x + rect.size[0] * (point.y + abs(rect.position[1]))


func astar_connect_walkable_cells_diagonal(walkable_cells_list = []):
	for point in walkable_cells_list:
		var point_index = calculate_point_index(point)
		astar_node.add_point(point_index, Vector3(point.x, point.y, 0.0))
		###
		#print("x = ", point.x, " y = ", point.y, " index = ", point_index)
		####
		
		for local_y in range(3):
			for local_x in range(3):
				var point_relative = Vector2(point.x + local_x - 1, point.y + local_y - 1)
				#print("point_relative = ", point_relative)
				var point_relative_index = calculate_point_index(point_relative)
				if point_relative == point:
					continue
				if not astar_node.has_point(point_relative_index):
					continue
				astar_node.connect_points(point_index, point_relative_index, true)


func transform2dToIso(VecList):
	var isoX = VecList[0]
	var isoY = VecList[1]
	var X2D = (isoX - isoY) * 32
	var Y2D = 16 + (isoX + isoY) * 16
	return Vector2(X2D, Y2D)



var path_start_position = Vector2() setget _set_path_start_position
var path_end_position = Vector2() setget _set_path_end_position
var _point_path = []


func get_astar_path(world_start, world_end):
	self.path_start_position = world_to_map(world_start)
	self.path_end_position = world_to_map(world_end)
	_recalculate_path()
	var path_world = []
	for point in _point_path:
		var point_world = map_to_world(Vector2(point.x, point.y)) + _half_cell_size
		path_world.append(point_world)
	return path_world


func _recalculate_path():
	var start_point_index = calculate_point_index(path_start_position)
	#print("begin ", start_point_index)
	var end_point_index = calculate_point_index(path_end_position)
	#print("end ", end_point_index)
	# This method gives us an array of points. Note you need the start and end points' indices as input.
	_point_path = astar_node.get_point_path(start_point_index, end_point_index)
	#print(_point_path)


func _set_path_start_position(value):
	if value in obstacles:
		return
	path_start_position = value
	if path_end_position and path_end_position != path_start_position:
		_recalculate_path()


func _set_path_end_position(value):
	if value in obstacles:
		return
	path_end_position = value
	if path_start_position != value:
		_recalculate_path()
