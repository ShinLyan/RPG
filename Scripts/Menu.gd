extends Control


func _ready():
	var _var = $Buttons/Button.connect("pressed", self, "change_scene", [$Buttons/Button.scene_to_open])
	# Первый параметр  - название сигнала в "". Сигнал принадлежит узлу, который вызывает функция connect().
	# (в данном случае к кнопке)
	# Второй параметр - подсоединяемый узел, тот, в который приходит результат при активации сигнала
	# (в данном случае Menu)
	# Третий параметр - запускаемая функция. Функция, которая будет выполняться при активации сингала.
	# (в данном случае change_scene)
	# Последний параметр - список передаваемых параметров. Параметры передаются в запускаемую функцию.
	# (в данном случае в списке только scene_to_open)
	
	_var = $Buttons/Open_load.connect("pressed", $Load, "open") # открываем Load
	_var = $Buttons/Open_load.connect("pressed", $Buttons, "hide") # скрываем Buttons
	
	_var = $Load/Box/Buttons/Cancel.connect("pressed", $Load, "hide")
	_var = $Load/Box/Buttons/Cancel.connect("pressed", $Buttons, "show")
	
	_var = $Buttons/Quit.connect("pressed", self, "quit")


func quit():
	get_tree().quit()


func change_scene(path): # смена сцены
	SceneChanger.change_scene(path)


func _process(_delta):
	if $AudioStreamPlayer2D.playing == false:
		$AudioStreamPlayer2D.play()
