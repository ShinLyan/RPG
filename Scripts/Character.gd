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
	inventory.set_inv_owner(self)


func pick(item):
	self.inventory.add_item(item)
	return true
###################################


###########################################
# Полоса здоровья
var hp # текущее значение hp
var max_hp


# настройка hp bar у персонажей
func set_start_hp(HP, max_HP): 
	$HP_bar.value = HP # текущее значение hp
	$HP_bar.max_value = max_HP

onready var hp_bar = $HP_bar

func update_hp(): # обновляет значение hp bar на текущее
	# делаем плавный переход для хп бара
	$HP_bar/Tween.interpolate_property(hp_bar, 'value', hp_bar.value, hp, 0.1, Tween.TRANS_LINEAR, Tween.EASE_IN_OUT)
	$HP_bar/Tween.start()


func toggle_hp_bar(): # вкл/выкл hp bar
	$HP_bar.visible = !$HP_bar.visible
###########################################

var bite_strength # сила удара персонажа


func reduce_hp(val): # уменьшает hp персонажам
	self.hp -= val
	update_hp()

	# условие смерти персонажа
	if self.hp <= 0:
		die() # персонаж умирает
		return false
	return true


func increase_hp(val): # увеличивает hp персонажам
	self.hp = min(self.hp + val, self.max_hp)
	update_hp()


# Смерть персонажа
func die(): 
	if state != DEATH: # предметы при смерти выпадают из персонажа
		randomize() 
		for i in inventory.get_items():
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
"""


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
