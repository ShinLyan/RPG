extends "res://Scripts/Character.gd"


func _ready(): # функция, вызывающая при создании существа
	speed = default_speed # устанавливаем обычную скорость мобов
	
	# Устанавливаем hp bar мобам        self - аналог this-> в С++
	self.hp = 40 
	self.max_hp = 40 # делаем здоровье немного меньше чем у игрока
	set_start_hp(self.hp, self.max_hp)
	# Добавляем моба в группы (для работы клавиши Alt)
	add_to_group(GlobalVars.entity_group)
	add_to_group(GlobalVars.troll_group)
	
	
	# добавляем предмет в троля
	var item = ItemMachine.generate_inventory_item("coins", 10)
	self.inventory.add_item(item)


func _physics_process(delta):
	match state:
		MOVE:
			move_state(delta)
		DEATH:
			death_state(delta)


# Передвижение моба
var stands = true # существо стоит и не движется
var destination = Vector2() # вектор координат цели, к которой будет идти моб
var velocity = Vector2() # вектор, на который будет перемещаться моб
var prev_pos = Vector2() # вектор, предыдущей позиции
var target = null # цель к которой ходит моб, изначально у моба нет таргета
var default_speed = 45 # обычная скорость моба


func move_state(_delta): # Передвижение моба
	if velocity: # если у существа есть скорость
		prev_pos = position # сперва фиксируем предыдущую позицию
		move_and_slide(velocity) # двигаем его на величину скорости
	wander() # бродим


func set_destination(dest): # устанавливаем место назначения
	destination = dest
	# normalized() - нормирует вектор
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
	$StandingTimer.start(4) # запускаем таймер. Мобы думают куда пойти 2 секунды
	target = null


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
################################################################################


# Смерть моба
func death_state(_delta):
	animationTree.set("parameters/Death/blend_position", velocity)
	animationState.travel("Death")


func death_animation_finished():
	get_parent().remove_child(self) # удаляем узел
	queue_free() # освобождаем память от него
################################################################################


# Таймеры
func _on_StandingTimer_timeout(): # таймер стояния моба
	stands = true
