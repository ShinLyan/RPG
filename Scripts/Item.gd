extends Node2D

var item = "" # название предмета
var amount = 1
var stack_limit = 8 # предметы группируются связками по 8 штук


func set_item(properties): # генерируем предмет, задаём свойства
	$Sprite.texture = load("res://Sprites/Items/%s.png" % properties[0])
	item = properties[0]
	stack_limit = properties[1]


func _ready():
	pass


func get_item():
	return item

func get_amount():
	return amount


func get_item_stack():
	return stack_limit


func _input(event):
	if event.is_action_pressed("e_click"): # на клик клавиши "E" совершается подбор предмета
		var pl = get_parent().get_parent().get_player()
		
		# проверяем рядом ли игрок
		if abs(pl.position.x - position.x) < 30 and abs(pl.position.y - position.y) < 30:
			get_parent().remove_child(self) # удаляем предмет с карты (подбираем)
			pl.pick(self) # подбираем предмет


func save():
	var data = {
		"filename": get_filename(),
		"position": position,
		"item": item,
		"amount": amount,
		"stack_limit": stack_limit
	}
	return data
