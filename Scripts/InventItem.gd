extends Control

onready var border = $Border # границы иконки инвентаря
var item_name = ""
var item_amount = 0
var properties = {} # свойства предмета
var inventory = null
signal on_picked # сигнал, когда игрок нажимает на предмет на панели инвентаря


func remove_amount(am):
	item_amount -= am
	$Box/Amount.text = str(item_amount) # обновляем значение количества предемета
	if item_amount == 0:
		inventory.remove_item(self)


func pick(): # включение границ
	border.show()


func unpick(): # выключение границ
	border.hide()


func use():
	var target = inventory.get_inv_owner()
	if "function" in properties.keys(): # есть ли у предмета функция
		match properties["function"]:
			"heal":
				target.increase_hp(properties["heal_val"]) # хилим
				if properties["expandable"]:
					remove_amount(properties["exp_rate"]) # отнимаем количество предмета


func set_inventory(val):
	inventory = val


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


func _on_InventItem_gui_input(event):
	if event.is_action_pressed("left_click"):
		emit_signal("on_picked")
