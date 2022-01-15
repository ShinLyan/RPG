extends NinePatchRect


# при отображении инвентаря, загружаем этот предмет
onready var item = preload("res://Scenes/InventItem.tscn") 
onready var container = $Scroll/Grid


func _ready():
	clear() # чистим инвентарь
	visible = false # делаем инвентарь невидимым


func clear(): # удаляем узлы предметов
	for i in container.get_children():
		container.remove_child(i)
		i.queue_free()


# inventory['book'] = [15, 8] 15 - количество предметов, 8 - ограничение связки
func show_inventory(inventory):
	clear()
	for i in inventory.keys():
		var amount = inventory[i][0] # количество предмета
		for j in range(amount / inventory[i][1] + 1):
			var new_item = item.instance()
			if amount >= inventory[i][1]: 
				# если количество предметов позволяет создать полную свзяку
				container.add_child(new_item)
				new_item.set_item(i, inventory[i][1])
				amount -= inventory[i][1]
			elif amount > 0: # количество предмета меньше ограничения связки
				container.add_child(new_item)
				new_item.set_item(i, amount)
				amount = 0
