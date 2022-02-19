extends Area2D

var damaged = false # урон еще был не нанесен, чтобы урон не нанести дважды
var damage = 0 # количество урона


func _ready():
	$Timer.start(0.1) # область нанесенеия урона будет существовать 0.1 сек


func set_damage(dmg):
	damage = dmg


func _process(_delta):
	if not damaged and get_overlapping_bodies() != []: # если какое-то тело находится в области
		for i in get_overlapping_bodies(): # по каждому телу, пересекающимся области проходим
			if i in get_tree().get_nodes_in_group(GlobalVars.troll_group): # если тело i принадлежит группе троллей
				i.reduce_hp(damage) # наносим урон
		damaged = true


func _on_Timer_timeout():
	# удаление объекта области
	get_parent().remove_child(self)
	queue_free()
