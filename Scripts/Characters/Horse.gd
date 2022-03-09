extends "res://Scripts/Characters/AI.gd"


func _ready(): # функция, вызывающая при создании существа
	default_speed = 45 # обычная скорость моба
	speed = default_speed # устанавливаем обычную скорость мобов
	
	# Устанавливаем hp bar мобам        self - аналог this-> в С++
	self.hp = 40 
	self.max_hp = 40 # делаем здоровье немного меньше чем у игрока
	set_start_hp(self.hp, self.max_hp)
	
	# Добавляем моба в группы (для работы клавиши Alt)
	add_to_group(GlobalVars.entity_group)
	
	# добавляем предмет в NPC
	var item = ItemMachine.generate_inventory_item("coins", 10)
	self.inventory.add_item(item)


func _physics_process(delta):
	match state:
		NON_COMBAT:
			non_combat_state(delta)
		
		DEATH:
			death_state(delta)


# Таймеры
func _on_StandingTimer_timeout(): # таймер стояния моба
	stands = true
