extends Area2D

var damaged = false # урон еще был не нанесен, чтобы урон не нанести дважды


func _on_FireArea_body_entered(body):
	if get_overlapping_bodies() != []: # если какое-то тело находится в области
		for i in get_overlapping_bodies(): # по каждому телу, пересекающимся области проходим
			if i in get_tree().get_nodes_in_group(GlobalVars.troll_group): # если тело i принадлежит группе троллей
				i.reduce_hp(10) # наносим урон
	$Timer2.start(0.1)


func _on_Timer2_timeout():
	emit_signal("fire_damage") # выпускаем сигнал что нужно удалять узел
