extends Control


onready var items = []


var inventory_owner = null setget set_inv_owner, get_inv_owner
signal on_changed

func set_inv_owner(val):
	inventory_owner = val


func get_inv_owner():
	return inventory_owner


func add_item(item):
	var added = false
	if item.can_stack(): # если предмет может стакаться
		for i in items:
			if i.get_item_name() == item.get_item_name(): # если предмет есть в инвентаре, то стакаем
				i.add_amount(item.get_amount())
				added = true
				item.queue_free()
				break
	if not item.can_stack() or not added:
		items.append(item) # добавляем предмет в инвентарь
		item.set_inventory(self)
	
	emit_signal("on_changed") # при добавлении предмета выпускаем сигнал


func remove_item(link):
	items.remove(items.find(link)) # удаляет элемент по позиции
	link.queue_free()
	emit_signal("on_changed") # при выбрасывании предмета выпускаем сигнал


func get_items():
	return items


func get_saves():
	var saves = []
	for i in items:
		saves.append(i.save())
	return saves
