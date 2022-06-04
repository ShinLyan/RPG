extends "res://Scripts/Characters/Character.gd"


#############################################
func _ready():
	state = NON_COMBAT


enum {
	PURSUIT,
	HIT
}



# Полоса здоровья HP bar у NPC
onready var hp_bar = $HP_bar


func set_start_hp(HP, max_HP): 
	hp_bar.value = HP # текущее значение hp
	hp_bar.max_value = max_HP
func toggle_hp_bar(): # вкл/выкл hp bar
	hp_bar.visible = !hp_bar.visible
func reduce_hp(val): # уменьшает hp персонажам
	.reduce_hp(val)
	update_hp()
func increase_hp(val): # увеличивает hp персонажам
	.increase_hp(val)
	update_hp()
func update_hp(): # обновляет значение hp bar на текущее
	# делаем плавный переход для хп бара
	$HP_bar/Tween.interpolate_property(hp_bar, 'value', hp_bar.value, hp, 0.1, Tween.TRANS_LINEAR, Tween.EASE_IN_OUT)
	$HP_bar/Tween.start()

#############################################

# Передвижение NPC
var default_speed
var stands = true # существо стоит и не движется
var velocity = Vector2() # вектор, на который будет перемещаться моб
var destination = Vector2() # вектор координат цели, к которой будет идти моб
var prev_pos = Vector2() # вектор, предыдущей позиции
var target = null # цель к которой ходит моб, изначально у моба нет таргета



func wander(): # бродить
	var pos = position
	if stands: # если существо стоит и не движется
		randomize() # генерируем рандомные числа координатам
		var x = int(rand_range(pos.x - 150, pos.x + 150))
		var y = int(rand_range(pos.y - 150, pos.y + 150))
		set_destination(Vector2(x, y))
	
	# Проверка на движение, если он движется, то нужно остановиться
	elif velocity != Vector2() and not target:
		if pos.distance_to(destination) <= speed:
			cancel_movement()
		elif pos.distance_to(prev_pos) <= 0.6: 
		# если моб будет толкаться и практически не двигаться, то моб перестает двигаться
			cancel_movement()
func set_destination(dest): # устанавливаем место назначения
	destination = dest
	velocity = (destination - position).normalized() * speed # скорость движения к цели
	if velocity != Vector2.ZERO:
		animationTree.set("parameters/Idle/blend_position", velocity)
		animationTree.set("parameters/Run/blend_position", velocity)
		animationState.travel("Run")
	else:
		animationState.travel("Idle")
	stands = false
func cancel_movement(): # останавливает существо
	animationState.travel("Idle") # переходим на шаг
	# обнуляем скорость и точку назначения
	velocity = Vector2()
	destination = Vector2()
	speed = default_speed # сбрасываем скорость на дефолтное
	$StandingTimer.start(2) # запускаем таймер. Мобы думают куда пойти 2 секунды
	target = null
#############################################

# Атака и хилл NPC
var can_bite = true # можно ли атаковать существо (перезарядка, чтобы моб постоянно не бил)
var target_intercepted = false # есть ли какая-то цель в зоне досягаемости
var heal_strength
var sees_enemies = []

func bite(targ): # атака моба
	if targ != null:
		var is_alive = targ.reduce_hp(bite_strength) 
		can_bite = false
		$BiteCooldown.start(0.5) # Запуск таймера кулдауна = 0.5 сек
		
		if not is_alive:
			cancel_movement() 
func heal(targ): # хил героя
	if targ != null:
		var _is_alive = targ.increase_hp(heal_strength) 
		can_bite = false
		$BiteCooldown.start(0.5) # Запуск таймера кулдауна = 0.5 сек

		if targ.hp == targ.max_hp:
			cancel_movement()
			target_intercepted = false


#############################################
# Выбор приоритетного таргета
func set_priority_target():
	pass
#############################################
# Поведения NPC в зависимости от состояния

func non_combat_state(_delta):
	if velocity: # если у существа есть скорость
		prev_pos = position # сперва фиксируем предыдущую позицию
		var _var = move_and_slide(velocity) # двигаем его на величину скорости
	wander() # бродим


func combat_state(_delta):
	pass
func regrouping_state(_delta):
	pass


# Смерть NPC
func death_state(_delta):
	animationTree.set("parameters/Death/blend_position", velocity)
	animationState.travel("Death")
func death_animation_finished():
	get_parent().remove_child(self) # удаляем узел
	queue_free() # освобождаем память от него
#############################################


"""
# Перегруженная функция save()
func save():
	var data = .save() # вызываем родительскую функцию save()
	data["bite_strength"] = bite_strength
	data["default_speed"] = default_speed
	return data
"""
