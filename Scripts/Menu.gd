extends Control


func _ready():
	$Buttons/Button.connect("pressed", self, "change_scene", [$Buttons/Button.scene_to_open])
	# Первый параметр  - название сигнала в "". Сигнал принадлежит узлу, который вызывает функция connect().
	# (в данном случае к кнопке)
	# Второй параметр - подсоединяемый узел, тот, в который приходит результат при активации сигнала
	# (в данном случае Menu)
	# Третий параметр - запускаемая функция. Функция, которая будет выполняться при активации сингала.
	# (в данном случае change_scene)
	# Последний параметр - список передаваемых параметров. Параметры передаются в запускаемую функцию.
	# (в данном случае в списке только scene_to_open)
	
	$Buttons/Open_load.connect("pressed", $Load, "open") # открываем Load
	$Buttons/Open_load.connect("pressed", $Buttons, "hide") # скрываем Buttons
	
	$Load/Box/Buttons/Cancel.connect("pressed", $Load, "hide")
	$Load/Box/Buttons/Cancel.connect("pressed", $Buttons, "show")


func change_scene(path): # смена сцены
	SceneChanger.change_scene(path)
