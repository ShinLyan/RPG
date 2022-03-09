extends "res://Scripts/Characters/AI.gd"


func _ready(): # функция, вызывающая при создании существа
	default_speed = 45
	speed = default_speed # устанавливаем обычную скорость мобов
	heal_strength = 10
	# Устанавливаем hp bar мобам        self - аналог this-> в С++
	self.hp = 80 
	self.max_hp = 80 # делаем здоровье немного меньше чем у игрока
	self.bite_strength = 10
	set_start_hp(self.hp, self.max_hp)
	# Добавляем моба в группы (для работы клавиши Alt)
	add_to_group(GlobalVars.entity_group)
	#add_to_group(GlobalVars.troll_group)
	
	var item = load("res://Scenes/InventItem.tscn").instance()
	item.set_item("coins", 10, {"can_stack":true})
	self.inventory.add_item(item)



var can_heal = true
var target_in_range = false
var target_in_sight = false
var sees_enemies = []
var target_position
var can_fire = true
var fire_direction


func _physics_process(delta):	
	match state:
		NON_COMBAT:
			non_combat_state(delta)
		
		COMBAT:
			combat_state(delta)
		
		#REGROUPING:
			#regrouping_state(delta)
		
		DEATH:
			death_state(delta)


func _process(_delta):
	print("sees enemies = ", len(sees_enemies), "  target = ", target)
	if target == null and len(sees_enemies) > 0:
		target = sees_enemies[0]
	if self.hp < 0:
		state = DEATH
	sight_check()
	if self.hp < self.max_hp / 2 and can_heal:
		self_heal()




func sight_check():
	if target_in_range and target:
		var space_state = get_world_2d().direct_space_state
		var sight_check = space_state.intersect_ray(position, target.position, [self], collision_mask)
		if sight_check: 
			target_in_sight = true
			target_position = target.get_global_position()
			state = COMBAT
		else:
			target_in_sight = false
			state = NON_COMBAT

func non_combat_state(_delta):
	.non_combat_state(_delta)
	if self.hp < self.max_hp and can_heal:
		self_heal()
func self_heal():
	can_heal = false
	yield(get_tree().create_timer(0.25), "timeout") # задержка перед хилом
	if state != DEATH: # если персонаж живой
		cancel_movement()
		state = ATTACK
		var skill = load("res://Scenes/SingleTargetHeal.tscn")
		var skill_instance = skill.instance()
		skill_instance.skill_name = "Heal"
		add_child(skill_instance)
		yield(get_tree().create_timer(1), "timeout")
		can_heal = true
func _on_Sight_body_entered(body):
	if body.is_in_group("Enemies"):
		target_in_range = true
		sees_enemies.append(body)
func _on_Sight_body_exited(body):
	if body.is_in_group("Enemies"):
		target = null # убираем таргет
		sees_enemies.remove(sees_enemies.find(body))
		if len(sees_enemies) == 0:
			target_in_range = false


func combat_state(_delta):
	if can_fire:
		attack_state(_delta)
func attack_state(_delta):
	can_fire = false
	fire_direction = (get_angle_to(target_position) / 3.14) * 180
	var vel = (target_position - position).normalized() * speed # скорость движения к цели
	animationTree.set("parameters/Cast/blend_position", vel)
	animationState.travel("Cast")
	get_node("TurnAxis").rotation = get_angle_to(target_position)
	var skill = load("res://Scenes/RangedSingleTargetSkill.tscn")
	var skill_instance = skill.instance()
	skill_instance.skill_name = "Ice_Spear"
	skill_instance.fire_direction = fire_direction
	skill_instance.rotation = get_angle_to(target_position)
	skill_instance.position = get_node("TurnAxis/CastPoint").get_global_position()
	skill_instance.origin = "Allies" # выстрел совершила героиня
	get_parent().add_child(skill_instance)
	yield(get_tree().create_timer(0.6), "timeout")
	can_fire = true
func attack_animation_finished():
	animationState.travel("Idle")
	state = NON_COMBAT












"""
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

"""






# Таймеры
func _on_StandingTimer_timeout(): # таймер стояния моба
	stands = true


func _on_BiteCooldown_timeout(): # таймер кулдауна
	can_bite = true
