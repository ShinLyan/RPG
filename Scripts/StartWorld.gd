extends Node2D


func _ready():
	create_file_map()
	var walkable_cells_list = generate_walkable_cells()
	create_items(walkable_cells_list)
	create_trolls(walkable_cells_list)


func generate_walkable_cells(): # формирует список точек, по которым можно ходить
	var obstacles = $YSort/Tree.get_used_cells() # добавляем деревья
	obstacles += $YSort/Border.get_used_cells() # добавляем стены
	obstacles += $GroundBottom.get_used_cells() # добавляем озеро
	obstacles += $Bridge/Bridge.get_used_cells() # добавляем мост
	var rect = $YSort/Border.get_used_rect()
	var walkable_cells_list = []
	for y in range(rect.position[1], rect.position[1] + rect.size[1]):
		for x in range(rect.position[0], rect.position[0] + rect.size[0]):
			var point = Vector2(x, y)
			if point in obstacles:
				continue
			walkable_cells_list.append(point)
	return walkable_cells_list


# Инвентарь и предметы
onready var item = preload("res://Scenes/Item.tscn")


func add_lying_item(i, x, y):
	var new_item = self.item.instance()
	$YSort/Items.add_child(new_item)
	new_item.set_item([i.get_item_name(), 100, i.get_props()])
	new_item.set_amount(i.get_amount())
	new_item.position = Vector2(x, y)


func create_items(walkable_cells_list = []):
	var items = [ # список предметов с названиями и связками
		# предмет, количество предметов в связке, может стакаться или нет
		["book", 8, {"can_stack":true}], 
		["scroll", 8, {"can_stack":true}], 
		["hp_potion", 8, {"can_stack":false}],
		["coins", 100, {"can_stack":true}]
		] 
	
	var num_items = 100 # количество генерируемых предметов на карте
	
	# генерируем предметы на карте рандомным образом 
	for i in range(num_items):
		# instance - создает объект по примеру исходной сцены
		var new_item = item.instance() # создаём предмет
		
		# добавляем предмет на карту
		$YSort/Items.add_child(new_item)
		
		# выбираем рандомно число от 0 до 1 - это предметы, которые должны быть на карте
		randomize()
		var num_rand = int(rand_range(0, 3))
		
		# передаем список названий
		new_item.set_item(items[num_rand])
		
		# выбираем рандомно ячейку, в которую заспавним предмет
		var pos_rand = int(rand_range(0, len(walkable_cells_list) - 1))
		# задаём координаты прдемету
		new_item.position = transform2dToIso(walkable_cells_list[pos_rand])
##########################

func transform2dToIso(VecList):
	var isoX = VecList[0]
	var isoY = VecList[1]
	var X2D = (isoX - isoY) * 32
	var Y2D = 16 + (isoX + isoY) * 16
	return Vector2(X2D, Y2D)


onready var troll = preload("res://Scenes/Troll.tscn")


func create_trolls(walkable_cells_list = []):
	var num_trolls = 10 # количество генерируемых предметов на карте
	# генерируем троллей на карте рандомным образом 
	for i in range(num_trolls):
		# instance - создает объект по примеру исходной сцены
		var new_troll = troll.instance() # создаём тролля
		randomize()
		# выбираем рандомно ячейку, в которую заспавним предмет
		var pos_rand = int(rand_range(0, len(walkable_cells_list) - 1))
		
		new_troll.position = transform2dToIso(walkable_cells_list[pos_rand])
		$YSort.add_child(new_troll)


func get_player():
	# проверяем существует ли узел игрока
	if has_node("YSort/Hero"):
		return $YSort/Hero
	else:
		return false


func update_label(value):
	# перенаправляем данные в другой узел (в Root)
	get_parent().update_label(value)


# Создание карты
func create_file_map():
	# выгрузка карты в файл
	var rect = $YSort/Border.get_used_rect()
	var matrix = []
		
	for x in range(rect.size[0]):
		matrix.append([])
		matrix[x] = []
		for y in range(rect.size[1]):
			matrix[x].append([])
			matrix[x][y] = '0'
	
	var deltaX = rect.position[0] # смещение по оси X
	var deltaY = rect.position[1] # смещение по оси Y
	
	var ground = $YSort/Tree.get_used_cells()
	for i in ground:
		matrix[i[0] - deltaX][i[1] - deltaY] = '/'
	
	
	ground = $YSort/Border.get_used_cells()
	for i in ground:
		matrix[i[0] - deltaX][i[1] - deltaY] = '/'
	
	
	ground = $GroundBottom.get_used_cells()
	for i in ground:
		matrix[i[0] - deltaX][i[1] - deltaY] = '/'

	
	var my_file = File.new()
	my_file.open("map.txt", File.WRITE)
	
	for i in matrix:
		var temp = ''
		for j in i:
			temp += j
		my_file.store_line(temp)
	my_file.close()


func _unhandled_input(event): # позволяет обрабатывать те сигналы, которые не были обработаны другими узлами
	# Проверяем нажатие на клавишу Alt (сворачивание полосы здоровья)
	if event.is_action_pressed("Alt"):
		for i in get_tree().get_nodes_in_group(GlobalVars.entity_group):
			i.toggle_hp_bar()


# Сохранение игры
func save():
	var data = {
		"filename": get_filename(), 
		"player": $YSort/Hero.save(),
		"items": [],
		#"nature": [],
		"mobs": []
	}
	
	for item in $Items.get_children(): # цикл для заполения инфой предметов
		data["items"].append(item.save()) # заполняем список
	
	#for object in $YSort/Tilesets.get_children():
		#data["nature"].append(object.save())
	
	for mobs in $YSort/AngryMobs.get_children():
		data["mobs"].append(mobs.save())
	
	return data


# Загрузка игры
func load_from_data(data):
	for item in $Items.get_children():
		$Items.remove_child(item)
		item.queue_free()
	
	for object in $YSort.get_children():
		$YSort.remove_child(object)
		object.queue_free()
	
	
	for item in $Items.get_children():
		$Items.remove_child(item)
		item.queue_free()
