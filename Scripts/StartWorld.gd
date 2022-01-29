extends Node2D

onready var item = preload("res://Scenes/Item.tscn")


func get_player():
	# проверяем существует ли узел игрока
	if has_node("YSort/Hero"):
		return $YSort/Hero
	else:
		return false


func update_label(value):
	# перенаправляем данные в другой узел (в Root)
	get_parent().update_label(value)

######################################
# смещение
var deltaX = 0
var deltaY = -18
######################################



func _ready():
	var items = [["book", 8], ["scroll", 8], ["coins", 1000]] # список предметов с названиями и связками
	
	# генерируем предметы на карте рандомным образом 
	for i in range(50): #(в скобках количество генерируемых предметов)
		randomize()
		var a = int(rand_range(0, 3))
		
		# instance - создает объект по примеру исходной сцены
		var new_item = item.instance() # создаём предмет
		
		# добавляем предмет на карту
		$Items.add_child(new_item)
		
		# передаем список названий
		new_item.set_item(items[a])
		
		new_item.position = Vector2(int(rand_range(0, 900)), int(rand_range(0, 500)))
	
	createMap()
	

	add_to_group(GlobalVars.saving_group) # добавляем в группу для сохранения игры



func createMap():
	# выгрузка карты в файл
	var rect = $YSort/Border.get_used_rect()
	var matrix = []
		
	for x in range(rect.size[0]):
		matrix.append([])
		matrix[x] = []
		for y in range(rect.size[1]):
			matrix[x].append([])
			matrix[x][y] = '0'
	
	deltaX = rect.position[0]
	deltaY = rect.position[1]
	
	#print(deltaX)
	#print(deltaY)
	
	var ground = $YSort/Tree.get_used_cells()
	for i in ground:
		matrix[i[0] - rect.position[0]][i[1] - rect.position[1]] = '/'
	
	
	ground = $YSort/Border.get_used_cells()
	for i in ground:
		matrix[i[0] - rect.position[0]][i[1] - rect.position[1]] = '/'
	
	
	ground = $GroundBottom.get_used_cells()
	for i in ground:
		matrix[i[0] - rect.position[0]][i[1] - rect.position[1]] = '/'

	
	var my_file = File.new()
	my_file.open("Map.txt", File.WRITE)
	
	for i in matrix:
		var temp = ''
		for j in i:
			temp += j
		my_file.store_line(temp)
	my_file.close()
	

# конвертирует файловые координаты в изометрические
func convertFileCoordToTileMap(x, y):
	x += deltaX
	y += deltaY
	return [x, y]







# позволяет обрабатывать те сигналы, которые не были обработаны другими узлами
func _unhandled_input(event):
	# Проверяем нажатие на клавишу Alt (сворачивание полосы здоровья)
	if event.is_action_pressed("Alt"):
		for i in get_tree().get_nodes_in_group(GlobalVars.entity_group):
			i.toggle_hp_bar()


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
	
	
