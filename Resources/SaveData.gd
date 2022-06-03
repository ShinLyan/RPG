extends Resource

# сохранение игры
export var save_name = "" setget set_name, get_save_name # название сохранения
export var data = {} setget set_data, get_data # набор сохраненных данных


func set_name(n):
	save_name = n


func get_save_name():
	return save_name


func set_data(d):
	data = d.duplicate(true) # делаем копию


func get_data():
	return data
