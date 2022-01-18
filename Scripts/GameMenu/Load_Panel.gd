extends Control

onready var file_to_load = ""

signal on_close_menu # сигнал о том, что после загрузки меню закрылось
signal on_loaded(data)


func _ready():
	hide() # скрываем панель при включении
	$Box/Scroll/ItemList.connect("item_selected", self, "set_file_to_load")
	$Box/Buttons/Load.connect("pressed", self, "load_file")
	connect("on_loaded", SceneChanger, "load_game")


func set_file_to_load(ind): # ind - индекс сохранения из списка
	var text = $Box/Scroll/ItemList.get_item_text(ind)
	file_to_load = text


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


func open(): # открываем панель
	show()
	update_saves()
	file_to_load = ""


func load_file(): # загрузка файла
	if file_to_load != "":
		var file_path = GlobalVars.save_dir.plus_file(GlobalVars.save_temp % file_to_load) # полный путь папки с сохранением
		var file = File.new()
		
		if file.file_exists(file_path): # если существует такой файл
			
			var saved_game = load(file_path)

			emit_signal("on_close_menu")
			emit_signal("on_loaded", saved_game.get_data())
