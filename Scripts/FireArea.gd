extends Area2D

var damaged = false # урон еще был не нанесен, чтобы урон не нанести дважды


func _process(delta):
	if not damaged and get_overlapping_bodies() != []: # если какое-то тело находится в области
		for i in get_overlapping_bodies(): # по каждому телу, пересекающимся области проходим
			if i in get_tree().get_nodes_in_group(GlobalVars.troll_group): # если тело i принадлежит группе троллей
				i.reduce_hp(10) # наносим урон
				get_parent().remove_and_skip()
				queue_free()
				



"""
func _on_FireArea_area_entered(area):
	if not damaged and get_overlapping_bodies() != []: # если какое-то тело находится в области
		for i in get_overlapping_bodies(): # по каждому телу, пересекающимся области проходим
			
			print(get_overlapping_bodies())
			
			if i in get_tree().get_nodes_in_group(GlobalVars.troll_group): # если тело i принадлежит группе троллей
				i.reduce_hp(10) # наносим урон
		#damaged = true
"""


func _on_FireArea_body_entered(body):
	pass
