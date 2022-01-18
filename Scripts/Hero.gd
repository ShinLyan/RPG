extends "res://Scripts/Character.gd"

# Инвентарь
var inventory = {} # список инвернтаря
# inventory['Apple'] = [15, 8]


func pick(item):
	var it = item.get_item()
	if it in inventory.keys(): # если название предмета есть в инвентаре
		inventory[it][0] += item.get_amount() 
	else:
		inventory[it] = [item.get_amount(), item.get_item_stack()] # добавляем предмет
	ui.update_inventory(inventory)


# позволяет обрабатывать те сигналы, которые не были обработаны другими узлами
func _unhandled_input(event): 
	# инвентарь
	if event.is_action_pressed("inventory"): # нажатие на кнопку I
		ui.toggle_inventory(inventory)
	
	# атака игрока
	if event.is_action_pressed("left_click"): # нажатие на ЛКМ
		# при нажатии кнопки создаем DamageArea и добавляем его на карту
		var a = load("res://Scenes/DamageArea.tscn").instance() 
		a.set_damage(10) # 10 урона наносим одной атакой
		get_parent().add_child(a)
		a.position = position + $DamagePos.position


# HP bar
func _ready():
	# self - аналог this-> в C++
	self.hp = 100 # исходное здоровье игрока
	set_start_hp(self.hp, self.max_hp) # задаем hp персонажу
	add_to_group(GlobalVars.entity_group)


func _physics_process(delta):
	match state:
		MOVE:
			move_state(delta)
		
		ATTACK:
			attack_state(delta)
		
		DEATH:
			death_state(delta)


# Передвижение персонажа на клавиши
var direction = Vector2.ZERO


func get_direction():
	return (Vector2(
		Input.get_action_strength("ui_right") - Input.get_action_strength("ui_left"),
		Input.get_action_strength("ui_down") - Input.get_action_strength("ui_up")
	)).normalized()


func move_state(delta):

	direction = get_direction()
	
	if direction != Vector2.ZERO:
		animationTree.set("parameters/Idle/blend_position", direction)
		animationTree.set("parameters/Run/blend_position", direction)
		animationTree.set("parameters/Death/blend_position", direction)
		
		animationState.travel("Run")
	else:
		animationState.travel("Idle")
	
	move_and_slide(direction * speed)
	
	if Input.is_action_just_pressed("left_click"):
		state = ATTACK


func attack_state(pos):
	# DamagePos должен двигаться в направлении мыши, но при этом не уходить слишком далеко от игрока
	$DamagePos.position = get_global_mouse_position() - position
	$DamagePos.position.x = clamp($DamagePos.position.x, -35, 33)
	$DamagePos.position.y = clamp($DamagePos.position.y, -35, 33)
	
	animationTree.set("parameters/Attack/blend_position", $DamagePos.position)
	animationState.travel("Attack")


func attack_animation_finished():
	state = MOVE


# Смерть игрока
signal on_death # сигнал смерти персонажа


func death_state(delta):
	animationTree.set("parameters/Death/blend_position", direction)
	animationState.travel("Death")


func death_animation_finished():
	emit_signal("on_death") # выпускаем сигнал смерти игрока
	get_parent().remove_child(self) # удаляем узел
	queue_free() # освобождаем память от него


# Перегруженная функция save()
func save():
	var data = .save() # вызываем родительскую функцию save()
	data["inventory"] = inventory
	return data
