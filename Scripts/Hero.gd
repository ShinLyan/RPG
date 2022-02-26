extends "res://Scripts/Character.gd"


# позволяет обрабатывать те сигналы, которые не были обработаны другими узлами
func _unhandled_input(event): 
	# инвентарь
	if event.is_action_pressed("inventory"): # нажатие на кнопку I
		ui.toggle_inventory(inventory)
	
	# атака игрока
	# ближний бой
	if state != DEATH: # если герой жив
		if event.is_action_pressed("left_click"): # нажатие на ЛКМ
			state = ATTACK
			# DamagePos должен двигаться в направлении мыши, но при этом не уходить слишком далеко от игрока
			$DamagePos.position = get_global_mouse_position() - position
			$DamagePos.position.x = clamp($DamagePos.position.x, -35, 33)
			$DamagePos.position.y = clamp($DamagePos.position.y, -35, 33)
			# при нажатии кнопки создаем DamageArea и добавляем его на карту
			var attack = load("res://Scenes/DamageArea.tscn").instance() 
			attack.set_damage(10) # 10 урона наносим одной атакой
			get_parent().add_child(attack)
			attack.position = position + $DamagePos.position


func update_inventory():
	ui.update_inventory(inventory)



func pick(item):
	var is_picked = .pick(item)
	return is_picked


func drop_item(link):
	world.add_lying_item(link, position.x, position.y)
	inventory.remove_item(link)


# HP bar
func _ready():
	self.hp = 100 # исходное здоровье игрока
	self.max_hp = 100
	set_start_hp(self.hp, self.max_hp) # задаем hp персонажу
	#add_to_group(GlobalVars.entity_group)
	create_inventory()
	inventory.connect("on_changed", self, "update_inventory")



##################################

var can_fire = true
var rate_of_fire = 0.8


var shooting = false


var selected_skill

func _process(_delta):
	skill_loop()


var fire_direction

func skill_loop():
	if Input.is_action_pressed("ui_fire") and can_fire: # нажатие на ПКМ
		can_fire = false
		state = RANGE
		shooting = true
		# DamagePos должен двигаться в направлении мыши, но при этом не уходить слишком далеко от игрока
		$FirePos.position = get_global_mouse_position() - position
		$FirePos.position.x = clamp($FirePos.position.x, -35, 33)
		$FirePos.position.y = clamp($FirePos.position.y, -35, 33)
		
		fire_direction = (get_angle_to(get_global_mouse_position()) / 3.14) * 180
		get_node("TurnAxis").rotation = get_angle_to(get_global_mouse_position())
		
		match DataImport.skill_data[selected_skill].SkillType:
			"RangedSingleTargetSkill":
				var skill = load("res://Scenes/RangedSingleTargetSkill.tscn")
				var skill_instance = skill.instance()
				skill_instance.skill_name = selected_skill
				skill_instance.fire_direction = fire_direction
				skill_instance.rotation = get_angle_to(get_global_mouse_position())
				skill_instance.position = get_node("TurnAxis/CastPoint").get_global_position()
				get_parent().add_child(skill_instance)
			
			"RangedAOESkill":
				var skill = load("res://Scenes/RangedAOESkill.tscn")
				var skill_instance = skill.instance()
				skill_instance.skill_name = selected_skill
				skill_instance.position = get_global_mouse_position()
				get_parent().add_child(skill_instance)
			
			"ExpandingAOESkill":
				var skill = load("res://Scenes/ExpandingAOESkill.tscn")
				var skill_instance = skill.instance()
				skill_instance.skill_name = selected_skill
				skill_instance.position = get_global_position()
				get_parent().add_child(skill_instance)
			
			"SingleTargetHeal":
				var skill = load("res://Scenes/SingleTargetHeal.tscn")
				var skill_instance = skill.instance()
				skill_instance.skill_name = selected_skill
				add_child(skill_instance)
		
		yield(get_tree().create_timer(rate_of_fire), "timeout")
		can_fire = true
		shooting = false


#################################



func _physics_process(delta):
	match state:
		MOVE:
			move_state(delta)
		ATTACK:
			attack_state(delta)
		RANGE:
			range_attack_state(delta)
		DEATH:
			death_state(delta)
	HP_regen(delta)


var hp_regen = 1

func HP_regen(delta):
	self.hp += hp_regen * delta
	if self.hp >= self.max_hp:
		self.hp = self.max_hp



################################################################################
# Передвижение игрока
var velocity = Vector2()
var direction = Vector2.ZERO

func move_state(_delta):
	direction = get_direction()
	if direction != Vector2.ZERO:
		animationTree.set("parameters/Idle/blend_position", direction)
		animationTree.set("parameters/Run/blend_position", direction)
		animationTree.set("parameters/Death/blend_position", direction)
		animationState.travel("Run")
	else:
		animationState.travel("Idle")
	move_and_slide(direction * speed)


func get_direction():
	return (Vector2(
		Input.get_action_strength("ui_right") - Input.get_action_strength("ui_left"),
		Input.get_action_strength("ui_down") - Input.get_action_strength("ui_up")
	)).normalized()
################################################################################


################################################################################
# Атака игрока

# Ближний бой
func attack_state(_delta):
	animationTree.set("parameters/Attack/blend_position", $DamagePos.position)
	animationState.travel("Attack")


func attack_animation_finished():
	state = MOVE


# Дальний бой
func range_attack_state(_delta):
	if shooting:
		animationTree.set("parameters/Cast/blend_position", $FirePos.position)
		animationState.travel("Cast")

################################################################################


################################################################################
# Смерть игрока
signal on_death # сигнал смерти персонажа


func death_state(_delta):
	animationTree.set("parameters/Death/blend_position", direction)
	animationState.travel("Death")


func death_animation_finished():
	emit_signal("on_death") # выпускаем сигнал смерти игрока
	get_parent().remove_child(self) # удаляем узел
	queue_free() # освобождаем память от него
################################################################################


# Перегруженная функция save()
func save():
	var data = .save() # вызываем родительскую функцию save()
	return data
