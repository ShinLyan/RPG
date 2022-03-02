extends Control


onready var save_name = "" setget set_name # название сохранения
onready var save_file = preload("res://Resources/SaveData.gd") # загружаем скрипт


signal on_saved # сигнал о том, что мы игру сохранили, и нужно переключить панель


func _ready():
	hide() # скрываем панель при включении
	var _var = $Box/Naming.connect("text_changed", self, "set_name")
	_var = $Box/Scroll/ItemList.connect("item_selected", self, "change_line")
	_var = $Box/Buttons/Save.connect("pressed", self, "save")


func open(): # открываем панель
	show()
	update_saves()
	$Box/Naming.text = ""


func set_name(n):
	save_name = n


func change_line(ind): # при нажатии на сохранение в панели показывает название выделенного сохранения
	var sname = $Box/Scroll/ItemList.get_item_text(ind) # берем текст из названия сохранения
	$Box/Naming.text = sname # присваием название сохранения
	set_name(sname)


func update_saves(): # обновление сохранений в списке сохранений
	$Box/Scroll/ItemList.clear()
	
	var dir = Directory.new()
	if not dir.dir_exists(GlobalVars.save_dir): # если папка Saves не существует
		dir.make_dir_recursive(GlobalVars.save_dir) # если ее нет, то создаем папку
	dir.change_dir(GlobalVars.save_dir) # заходим в папку
	
	dir.list_dir_begin(true)
	var file = dir.get_next() # достает следующий файл в папке
	while file != "": # достаем следующий элемент пока он не пустой
		$Box/Scroll/ItemList.add_item(file.split('.')[0]) # добавляем файл сохранения с названием сохранения
		file = dir.get_next() # достаем следующий элемент
	
	dir.list_dir_end()


func save(): # сохранение файла
	if save_name != "": # у файла должно быть название
		var file = save_file.new()
		file.set_name(save_name)
		
		var able_to_save = get_tree().get_nodes_in_group(GlobalVars.saving_group)
		file.set_data(able_to_save[0],save())
		
		var save_path = GlobalVars.save_dir.plus_file(GlobalVars.save_temp % save_name) # полный путь сохранения файла
		
		var _var = ResourceSaver.save(save_path, file)
		
		emit_signal("on_saved") # активируем сигнал о том, что произошло сохранение
