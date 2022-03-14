extends KinematicBody2D


func _ready():
	create_inventory()


# Поля класса
var hp # текущее значение hp
var max_hp # максимальное значение hp
var shield = 0 # текущее значение щита
var max_shield = 50 # максимальное значение щита
var mp # текущее значение mp
var max_mp # максимальное значение mp
var bite_strength # сила удара персонажа
var speed = 100 # скорость персонажа
var inventory
var state # текущее состояние


onready var pre_inv = preload("res://Scenes/Inventory.tscn") # чертеж инвентаря
onready var pre_item = preload("res://Scenes/InventItem.tscn")
onready var world = get_viewport().get_node("Root/StartWorld")
onready var animationTree = $AnimationTree
onready var animationState = animationTree.get("parameters/playback")
onready var ui = get_viewport().get_node("Root/UI/Control") # ссылка на интерфейс


# Состояния персонажа
enum {
	COMBAT,
	NON_COMBAT,
	REGROUPING,
	DEATH,
	
	
	MOVE,
	ATTACK,
	RANGE
}



func create_inventory():
	inventory = pre_inv.instance()
	add_child(inventory)
	inventory.set_inv_owner(self)
func pick(item):
	self.inventory.add_item(item)
	return true
func reduce_hp(val): # уменьшает hp персонажам
	if shield == 0:
		self.hp -= val
		if self.hp <= 0: # условие смерти персонажа
			die() # персонаж умирает
			return false
	else:
		self.shield = max(self.shield - val, 0)
	return true
func increase_hp(val): # увеличивает hp персонажам
	self.hp = min(self.hp + val, self.max_hp)
func magic_shield(val):
	self.shield = min(self.shield + val, self.max_shield)
func die(): # state DEATH - Смерть персонажа
	if state != DEATH: # предметы при смерти выпадают из персонажа
		for i in inventory.get_items():
			randomize()
			var x_coord = rand_range(-1, 1) * 10 + self.position.x
			var y_coord = rand_range(-1, 1) * 10 + self.position.y
			world.add_lying_item(i, x_coord, y_coord)
	state = DEATH

















"""
##############################
var sees_enemies = []

# Выбор приоритетного таргета
func priority_target():
	var weight
	var max_weight
	var max_weight_id = 0
	for i in sees_enemies:
		pass
		#weight = 


################################



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
"""
