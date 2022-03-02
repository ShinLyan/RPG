extends CanvasLayer


func change_scene(path): # смена сцены, path - путь
	var _var = get_tree().change_scene(path)
	get_tree().paused = false


func load_game(_data): # загрузка игры
	#print(data)
	pass
