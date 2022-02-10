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


# inventory['book'] = [15, 8] 15 - количество предметов, 8 - ограничение связки
func show_inventory(inventory):
	clear()
	for i in inventory:
		container.add_child(i)
