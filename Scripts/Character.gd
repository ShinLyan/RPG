extends KinematicBody2D


func _ready():
	create_inventory()


# Состояния персонажа
enum {
	MOVE,
	ATTACK,
	DEATH,
	RANGE
}

var state = MOVE # текущее состояние

var speed = 100 # скорость персонажа


onready var animationPlayer = $AnimationPlayer
onready var animationTree = $AnimationTree
onready var animationState = animationTree.get("parameters/playback")

onready var ui = get_viewport().get_node("Root/UI/Control") # ссылка на интерфейс


###########################################
# Инвентарь
onready var pre_inv = preload("res://Scenes/Inventory.tscn") # чертеж инвентаря
onready var pre_item = preload("res://Scenes/InventItem.tscn")
onready var world = get_viewport().get_node("Root/StartWorld")
var inventory


func create_inventory():
	inventory = pre_inv.instance()
	add_child(inventory)


func pick(item):
	if item.item_name == "hp_potion":
		self.hp += 10
		update_hp()
	else:
		self.inventory.add_item(item)
	
	return true
###################################


###########################################
# Полоса здоровья
onready var hp = 50 # текущее значение hp
export var max_hp = 100


# настройка hp bar у персонажей
func set_start_hp(hp, max_hp): 
	$HP_bar.value = hp # текущее значение hp
	$HP_bar.max_value = max_hp


func update_hp(): # обновляет значение hp bar на текущее
	$HP_bar.value = hp


func toggle_hp_bar(): # вкл/выкл hp bar
	$HP_bar.visible = !$HP_bar.visible
###########################################


# Атака персонажей
func reduce_hp(val): # уменьшает hp персонажам
	self.hp -= val
	update_hp()

	# условие смерти персонажа
	if self.hp <= 0:
		die() # персонаж умирает
		return false
	return true


# Смерть персонажа
func die(): 
	if state != DEATH: # предметы при смерти выпадают из персонажа
		randomize() 
		for i in inventory.get_items():
			var x_coord = rand_range(-1, 1) * 10 + self.position.x
			var y_coord = rand_range(-1, 1) * 10 + self.position.y
			world.add_lying_item(i, x_coord, y_coord)
	state = DEATH



# Сохранение игры
func save():
	var data = {
		"filename": get_filename(),
		"position": position,
		"speed": speed,
		"hp": hp,
		"max_hp": max_hp,
		"inventory": self.inventory.get_saves()
	}
	return data


# Загрузка игры
func load_from_data(data):
	position = data["position"]
	speed = data["speed"]
	hp = data["hp"]
	max_hp = data["max_hp"]
	set_start_hp(hp, max_hp)
	create_inventory()
	
	for item in data["inventory"]:
		var new_item = pre_item.instance()
		new_item.set_item(item[0], item[1], item[2])
		self.inventory.add_item(new_item)
