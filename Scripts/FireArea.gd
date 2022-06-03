extends Area2D

var damaged = false # урон еще был не нанесен, чтобы урон не нанести дважды


func _ready():
	damaged = false


func _process(_delta):
	if not damaged and get_overlapping_bodies() != []: # если какое-то тело находится в области
		for i in get_overlapping_bodies(): # по каждому телу, пересекающимся области проходим
			if i in get_tree().get_nodes_in_group(GlobalVars.troll_group): # если тело i принадлежит группе троллей
				i.reduce_hp(10) # наносим урон
				damaged = true
