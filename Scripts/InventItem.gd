extends Control


var item_name = ""
var item_amount = 0
var properties = {} # свойства предмета


func set_item(item_name, amount, props):
	self.item_name = item_name
	self.item_amount = amount
	self.properties = props.duplicate()
	
	$Box/Texture.texture = load("res://Sprites/Items/%s.png" % item_name)
	$Box/Amount.text = str(amount)


func can_stack(): 
	# не все предметы могут стакаться, например 2 меча, должны храниться в разных ячейках
	return properties["can_stack"]


func add_amount(am):
	item_amount += am
	$Box/Amount.text = str(item_amount)


# Геттеры
func get_item_name():
	return item_name


func get_amount():
	return item_amount


func get_props():
	return properties


# Сохранение
func save():
	return [item_name, item_amount, properties]
