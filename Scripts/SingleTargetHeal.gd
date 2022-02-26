extends Node2D


var skill_name
var heal_amount


func _ready():
	heal_amount = DataImport.skill_data[skill_name].SkillHeal
	var skill_texture = load("res://assets/skills/" + skill_name + ".png")
	$Sprite.set_texture(skill_texture)
	heal()


func heal():
	$AnimationPlayer.play(skill_name)
	get_parent().increase_hp(heal_amount)
	yield(get_tree().create_timer(0.6), "timeout")
	queue_free()
