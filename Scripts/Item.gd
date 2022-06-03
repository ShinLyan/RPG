extends Node2D


onready var pre_inv = preload("res://Scenes/InventItem.tscn")


var properties = {} # свойства предметов
var item = "" # название предмета
var amount = 1 # количество предметов
var stack_limit = 8 # предметы группируются связками по 8 штук


func _input(event):
	if event.is_action_pressed("e_click"): # на клик клавиши "E" совершается подбор предмета
		var pl = get_parent().get_parent().get_player()
		
		# проверяем рядом ли игрок
		if abs(pl.position.x - position.x) < 30 and abs(pl.position.y - position.y) < 30:
			var new_item = pre_inv.instance()
			new_item.set_item(item, amount, properties) # создаем новый объект
			
			var is_picked = pl.pick(new_item) # просим игрока поднять предмет (случай когда нет места в инвентаре)
			if is_picked: # если предмет поднят
				get_parent().remove_child(self) # удаляем предмет с карты (подбираем)
				queue_free()
			else:
				new_item.queue_free()


# Сеттеры
func set_amount(am):
	amount = am


func set_item(props): # генерируем предмет, задаём свойства
	$Sprite.texture = load("res://Sprites/Items/%s.png" % props[0])
	item = props[0]
	stack_limit = props[1]
	self.properties = props[2]


# Геттеры
func get_item():
	return item


func get_amount():
	return amount


func get_item_stack():
	return stack_limit


# Сохранение и загрузка
func save():
	var data = {
		"filename": get_filename(),
		"position": position,
		"item": item,
		"amount": amount,
		"stack_limit": stack_limit,
		"properties": properties
	}
	return data


func load_from_data(data):
	position = data["position"]
	set_item([data["item"], data["stack_limit"], data["properties"]])
	amount = data["amount"]
