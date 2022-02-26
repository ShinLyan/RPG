extends "res://Scripts/Character.gd"


func _ready(): # функция, вызывающая при создании существа
	speed = default_speed # устанавливаем обычную скорость мобов
	
	# Устанавливаем hp bar мобам        self - аналог this-> в С++
	self.hp = 60 
	self.max_hp = 60 # делаем здоровье немного меньше чем у игрока
	self.bite_strength = 10
	set_start_hp(self.hp, self.max_hp)
	# Добавляем моба в группы (для работы клавиши Alt)
	add_to_group(GlobalVars.entity_group)
	#add_to_group(GlobalVars.troll_group)
	
	var item = load("res://Scenes/InventItem.tscn").instance()
	item.set_item("coins", 10, {"can_stack":true})
	self.inventory.add_item(item)


func _physics_process(delta):
	match state:
		MOVE:
			move_state(delta)
		ATTACK:
			attack_state(delta)
		DEATH:
			death_state(delta)


# Передвижение моба
var stands = true # существо стоит и не движется
var destination = Vector2() # вектор координат цели, к которой будет идти моб
var velocity = Vector2() # вектор, на который будет перемещаться моб
var prev_pos = Vector2() # вектор, предыдущей позиции
var target = null # цель к которой ходит моб, изначально у моба нет таргета
var default_speed = 45


func move_state(_delta): # Передвижение моба
	if velocity: # если у существа есть скорость
		prev_pos = position # сперва фиксируем предыдущую позицию
		move_and_slide(velocity) # двигаем его на величину скорости
		
		# ограничиваем его координаты в рамках карты
		position.x = clamp(position.x, -516, 1412) 
		position.y = clamp(position.y, -209, 756)
	
	wander() # бродим
	search_for_target() # ищем таргет
	
	if target_intercepted and can_bite: # если моб может атаковать, то атакуем
		state = ATTACK
		if target:
			if target.is_in_group(GlobalVars.troll_group):
				bite(target)
			else:
				heal(target)


func search_for_target(): # функция, ищущая местоположение игрока
	var pl = get_parent().get_parent().get_player() # достаем игрока
	
	# проверяем существуют ли игрок
	if pl:
		if pl.hp < pl.max_hp:
			if target: # если в таргете игрок, т.е. моб преследует игрока
				# если игрок далеко
				#print(position.distance_to(target.position))
				if position.distance_to(target.position) > 200:
					cancel_movement() # отменяем предыдущее движение
				else: # если рядом
					#set_destination3(target.position)
					set_destination(target.position)
			
			elif position.distance_to(pl.position) < 200: # проверяем, рядом ли находится игрок
				cancel_movement()
				if speed == default_speed:
					speed = default_speed * 2 # увеличиваем скорость моба атакующего игрока
				
				# задаём новый таргет - игрока
				target = pl
		else:
			if target:
				if position.distance_to(target.position) > 200:
					cancel_movement() # отменяем предыдущее движение
				else: # если рядом
					set_destination(target.position)


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


######################################
var _path = []


func convertTileMapCoordToWorld(coordinates):
	var isoX = coordinates[0]
	var isoY = coordinates[1]
	var x = (isoX - isoY) * 32
	var y = 16 + (isoX + isoY) * 16
	return [x, y]


var _target_point_world = Vector2()

func set_destination3(dest):
	
	_path = get_parent().get_parent().get_parent().get_node("Ground").get_astar_path(self.position, dest)
	#print("path ======= ", _path)
	
	var _arrived_to_next_point = _move_to(_target_point_world)
	#print("target world = ", _target_point_world)
	
	
	
	if _arrived_to_next_point:
		_path.remove(0)
		#if len(_path) == 0:
			#_path = get_parent().get_parent().get_parent().get_node("Ground").get_astar_path(position, dest)
			#cancel_movement()
			#print(1)
		if not _path.empty():
			_target_point_world = _path[0]
	
	
	#_path = get_parent().get_parent().get_parent().get_node("Ground").get_astar_path(position, dest)
	#print(_path)
	
	if not _path or len(_path) == 1:
		#_path = get_parent().get_parent().get_parent().get_node("Ground").get_astar_path(position, dest)
		#print("afaaaaaaaaaaaa", 1)
		#return
		pass
	else:
		_target_point_world = _path[1]
	
	
	if velocity != Vector2.ZERO:
		animationTree.set("parameters/Idle/blend_position", velocity)
		animationTree.set("parameters/Run/blend_position", velocity)
		animationState.travel("Run")
	else:
		animationState.travel("Idle")
	stands = false


const MASS = 10.0
const ARRIVE_DISTANCE = 15.0


func _move_to(world_position):
	var desired_velocity = (world_position - position).normalized() * speed
	var steering = desired_velocity - velocity
	velocity += steering / MASS
	position += velocity * get_process_delta_time()
	return position.distance_to(world_position) < ARRIVE_DISTANCE


########################


func cancel_movement(): # останавливает существо
	animationState.travel("Idle") # переходим на шаг
	
	# обнуляем скорость и точку назначения
	velocity = Vector2()
	destination = Vector2()
	speed = default_speed # сбрасываем скорость на дефолтное
	$StandingTimer.start(2) # запускаем таймер. Мобы думают куда пойти 2 секунды
	target = null


func wander(): # бродить
	var pos = position
	if stands: # если существо стоит и не движется
		randomize() # генерируем рандомные числа координатам

		var x = int(rand_range(pos.x - 150, pos.x + 150))
		var y = int(rand_range(pos.y - 150, pos.y + 150))

		# устанавливаем границы координатам
		x = clamp(x, 0, 10000)
		y = clamp(y, 0, 10000)
		
		set_destination(Vector2(x, y))
	
	# Проверка на движение, если он движется, то нужно остановиться
	elif velocity != Vector2() and not target:
		if pos.distance_to(destination) <= speed:
			cancel_movement()
		elif pos.distance_to(prev_pos) <= 0.6: 
		# если моб будет толкаться и практически не двигаться, то моб перестает двигаться
			cancel_movement()


# Атака моба
var target_intercepted = false # есть ли какая-то цель в зоне досягаемости
var can_bite = true # можно ли атаковать существо (перезарядка, чтобы моб постоянно не бил)

var heal_strength = 10


func heal(targ): # хил героя
	if targ != null:
		var _is_alive = targ.increase_hp(heal_strength) 
		can_bite = false
		$BiteCooldown.start(0.5) # Запуск таймера кулдауна = 0.5 сек

		if targ.hp == targ.max_hp:
			cancel_movement()
			target_intercepted = false


func bite(targ): # атака моба
	if targ != null:
		var is_alive = targ.reduce_hp(bite_strength) 
		can_bite = false
		$BiteCooldown.start(0.5) # Запуск таймера кулдауна = 0.5 сек

		if not is_alive:
			cancel_movement() 
			target_intercepted = false



func attack_state(_delta):
	animationTree.set("parameters/Cast/blend_position", velocity)
	animationState.travel("Cast")


func attack_animation_finished():
	animationState.travel("Idle")
	if can_bite:
		state = MOVE
################################################################################


# Смерть моба
func death_state(_delta):
	target_intercepted = false
	target = null
	animationTree.set("parameters/Death/blend_position", velocity)
	animationState.travel("Death")


func death_animation_finished():
	get_parent().remove_child(self) # удаляем узел
	queue_free() # освобождаем память от него
################################################################################


# area_entered() - проверяет пересечение с другой областью area2D
func _on_BiteArea_area_entered(area): 
	if area.get_parent() == target: # если в зоне таргет
		target_intercepted = true


# area_exited() - проверяет отсутствие пересечения с другой областью area2D
func _on_BiteArea_area_exited(area): 
	# если таргет покинул область атаки, то моб должен перестать атаковать
	if area.get_parent() == target:
		target_intercepted = false


# Таймеры
func _on_StandingTimer_timeout(): # таймер стояния моба
	stands = true


func _on_BiteCooldown_timeout(): # таймер кулдауна
	can_bite = true


# Перегруженная функция save()
func save():
	var data = .save() # вызываем родительскую функцию save()
	data["bite_strength"] = bite_strength
	data["default_speed"] = default_speed
	return data


func _on_Area2D_area_entered(area):
	for i in area.get_overlapping_bodies(): # по каждому телу, пересекающимся области проходим
		if i in get_tree().get_nodes_in_group(GlobalVars.troll_group): # если тело i принадлежит группе троллей
			target = i
	if area.get_parent() == target: # если в зоне таргет
		target_intercepted = true



func _on_Area2D_area_exited(area):
	if area.get_parent() == target:
		target_intercepted = false
