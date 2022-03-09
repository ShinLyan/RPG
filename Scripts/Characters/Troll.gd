extends "res://Scripts/Characters/AI.gd"


func _ready(): # функция, вызывающая при создании существа
	state = MOVE
	
	default_speed = 45 # обычная скорость моба
	speed = default_speed # устанавливаем обычную скорость мобов
	GlobalVars.num_trolls += 1
	# Устанавливаем hp bar мобам        self - аналог this-> в С++
	self.hp = 60 
	self.max_hp = 60 # делаем здоровье немного меньше чем у игрока
	self.bite_strength = 5 # сила удара моба (5 единиц здоровья за удар)
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
		
		ATTACK:
			attack_state(delta)
		
		#REGROUPING:
			#regrouping_state(delta)
		
		DEATH:
			death_state(delta)




func search_for_target(): # функция, ищущая местоположение игрока
	print(target)
	if target: # если в таргете игрок, т.е. моб преследует игрока
		# если таргет далеко
		if position.distance_to(target.position) > 200:
			cancel_movement() # отменяем предыдущее движение
		else: # если рядом
			var temp = target
			cancel_movement()
			target = temp
			speed = default_speed * 2 if speed == default_speed else speed # увеличиваем скорость моба атакующего игрока
			set_destination(target.position)



var target_in_range = false
var target_in_sight = false
var sees_enemies = []
var target_position


func _on_Sight_body_entered(body):
	if body.is_in_group("Player") or body.is_in_group("Allies"):
		target_in_range = true
		sees_enemies.append(body)
func _on_Sight_body_exited(body):
	if body.is_in_group("Player") or body.is_in_group("Allies"):
		target = null # убираем таргет
		sees_enemies.remove(sees_enemies.find(body))
		if len(sees_enemies) == 0:
			target_in_range = false
func sight_check():
	if target_in_range and target:
		target_position = target.get_global_position()
		state = COMBAT


func _process(_delta):
	#print("sees enemies = ", len(sees_enemies), "  target = ", target)
	if target == null and len(sees_enemies) > 0:
		target = sees_enemies[0] 
	#sight_check()



"""
	if position.distance_to(target.position) > 200:
		cancel_movement() # отменяем предыдущее движение
	else: # проверяем, рядом ли находится игрок
		cancel_movement()
		speed = default_speed * 2 if speed == default_speed else speed # увеличиваем скорость моба атакующего игрока
		set_destination(target.position)
	if velocity: # если у существа есть скорость
		prev_pos = position # сперва фиксируем предыдущую позицию
		var _var = move_and_slide(velocity) # двигаем его на величину скорости
	if target_intercepted and can_bite: # если моб может атаковать, то атакуем
		animationTree.set("parameters/Attack/blend_position", velocity)
		animationState.travel("Attack")
		bite(target)
"""



func attack_state(_delta):
	if target:
		animationTree.set("parameters/Attack/blend_position", target.position)
	animationState.travel("Attack")


func attack_animation_finished():
	animationState.travel("Idle")
	if can_bite:
		state = MOVE


func move_state(_delta): # Передвижение моба
	if velocity: # если у существа есть скорость
		prev_pos = position # сперва фиксируем предыдущую позицию
		var _var = move_and_slide(velocity) # двигаем его на величину скорости
	wander() # бродим
	search_for_target() # ищем таргет
	
	if target_intercepted and can_bite: # если моб может атаковать, то атакуем
		state = ATTACK
		bite(target)










func death_state(_delta):
	.death_state(_delta)
	target_intercepted = false
	target = null


func death_animation_finished():
	.death_animation_finished()
	GlobalVars.num_trolls -= 1
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
















######################################
var _path = []
var _target_point_world = Vector2()
const MASS = 10.0
const ARRIVE_DISTANCE = 15.0
func set_destination_A_star(dest):
	_path = get_parent().get_parent().get_node("Ground").get_astar_path(self.position, dest)
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
func _move_to(world_position):
	var desired_velocity = (world_position - position).normalized() * speed
	var steering = desired_velocity - velocity
	velocity += steering / MASS
	position += velocity * get_process_delta_time()
	return position.distance_to(world_position) < ARRIVE_DISTANCE
######################################

