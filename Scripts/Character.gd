extends KinematicBody2D


enum {
	MOVE,
	ATTACK,
	DEATH,
	IDLE
}

var state = MOVE


onready var animationPlayer = $AnimationPlayer
onready var animationTree = $AnimationTree
onready var animationState = animationTree.get("parameters/playback")

onready var ui = get_viewport().get_node("Root/UI/Control") # ссылка на интерфейс


var speed = 100


# Полоса здоровья
onready var hp = 50 # текущее значение hp
export var max_hp = 100


func set_start_hp(hp, max_hp): # настройска hp bar у существа
	$HP_bar.value = hp # текущее значение hp
	$HP_bar.max_value = max_hp


func update_hp(): # обновляет значение hp bar на текущее
	$HP_bar.value = hp


func _ready():
	set_start_hp(hp, max_hp)


func toggle_hp_bar(): # вкл/выкл hp bar
	$HP_bar.visible = !$HP_bar.visible


# Атака персонажей

func reduce_hp(val): # уменьшает hp персонажам
	self.hp -= val
	update_hp()

	# условие смерти персонажа
	if self.hp <= 0:
		die() # персонаж умирает
		return false
	return true


func die(): # смерть персонажа
	#play_animation()
	get_parent().remove_child(self) # удаляем узел
	queue_free() # освобождаем память от него


func save():
	var data = {
		"filename": get_filename(),
		"position": position,
		"speed": speed,
		"hp": hp,
		"max_hp": max_hp
	}
	return data


func play_animation():
	pass
