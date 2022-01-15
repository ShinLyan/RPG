extends Control

onready var pack = $Panel


func toggle_inventory(inventory): # функция вкл/выкл инвентаря
	if pack.visible:
		pack.clear()
		pack.visible = false
	else: # если инвентарь не видим
		pack.visible = true
		pack.show_inventory(inventory)


func update_inventory(inventory):
	if pack.visible:
		pack.show_inventory(inventory)


func set_death_screen(): # открытие экрана смерти игрока
	$Panel.hide() # сначала закрываем инвентарь, если он открыт
	$DeathScreen.show() # затем включаем экран смерти


func _unhandled_input(event):
	# Проверяем нажатие на клавишу Escape
	if event.is_action_pressed("esc"):
		$Menu.open() # переключает видимость меню
