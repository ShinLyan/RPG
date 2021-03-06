extends Control


func _ready():
	hide() # скрываем панель при включении
	
	# кнопка Resume, закрывает меню
	var _var = $Panel/Main/Buttons/Resume.connect("pressed", self, "open") 
	
	# кнопка Quit, выходит из игры в главное меню
	_var = $Panel/Main/Buttons/Quit.connect("pressed", SceneChanger, "change_scene", ["res://Scenes/Menu.tscn"])
	
	# кнопка Save, открывает соответствующую панель и закрывает меню
	_var = $Panel/Main/Buttons/Save.connect("pressed", $Panel/Main, "hide")	
	_var = $Panel/Main/Buttons/Save.connect("pressed", $Panel/Save, "open")	
	
	# кнопка Load, открывает соответствующую панель и закрывает меню
	_var = $Panel/Main/Buttons/Load.connect("pressed", $Panel/Main, "hide")	
	_var = $Panel/Main/Buttons/Load.connect("pressed", $Panel/Load, "open")	
	
	# кнопка Cancel для Save
	_var = $Panel/Save/Box/Buttons/Cancel.connect("pressed", $Panel/Save, "hide")
	_var = $Panel/Save/Box/Buttons/Cancel.connect("pressed", $Panel/Main, "show")
	
	# кнопка Cancel для Load 
	_var = $Panel/Load/Box/Buttons/Cancel.connect("pressed", $Panel/Load, "hide")
	_var = $Panel/Load/Box/Buttons/Cancel.connect("pressed", $Panel/Main, "show")
	
	_var = $Panel/Save.connect("on_saved", $Panel/Main, "show")
	_var = $Panel/Save.connect("on_saved", $Panel/Save, "hide")

	_var = $Panel/Load.connect("on_close_menu", self, "open")


func open(): # переключает видимость меню
	if visible: # если открыто
		hide() # закрываем окно
		get_parent().cursor_show()
		get_tree().paused = false
	else:
		# пауза при открытии меню
		get_tree().paused = true
		show() # показываем окно
		$Panel/Load.hide()
		$Panel/Save.hide()
		$Panel/Main.show() # показываем панель Main, остальные скрываем
