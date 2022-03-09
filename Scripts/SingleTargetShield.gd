extends Node2D


var skill_name
var shield_amount
var origin


func _ready():
	GlobalVars.num_shields += 1
	shield_amount = DataImport.skill_data[skill_name].SkillShield
	var skill_texture = load("res://assets/skills/" + skill_name + ".png")
	$Sprite.set_texture(skill_texture)
	shield()


func shield():
	$AnimationPlayer.play(skill_name)
	var temp_shield = origin.shield
	get_parent().shield(shield_amount)
	yield(get_tree().create_timer(20), "timeout")
	GlobalVars.num_shields -= 1
	queue_free()


func _process(delta):
	if origin.shield <= 0:
		self.hide()
