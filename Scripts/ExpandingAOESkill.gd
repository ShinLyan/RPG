extends Area2D


var skill_name
var damage
var radius
var expansion_time
var damaged_targets = []


var circle_shape = preload("res://Resources/CircleShape.res")


func _ready():
	damage = DataImport.skill_data[skill_name].SkillDamage
	radius = DataImport.skill_data[skill_name].SkillRadius
	expansion_time = float(DataImport.skill_data[skill_name].SkillExpansionTime)
	var skill_texture = load("res://assets/skills/" + skill_name + ".png")
	$Sprite.set_texture(skill_texture)
	AOE_attack()


func AOE_attack():
	get_node("AnimationPlayer").play(skill_name)
	var radius_step = radius / (expansion_time / 0.05)
	print(expansion_time)
	while get_node("CollisionShape2D").get_shape().radius <= radius:
		var shape = circle_shape.duplicate()
		shape.set_radius(get_node("CollisionShape2D").get_shape().radius + radius_step)
		$CollisionShape2D.set_shape(shape)
		var targets = get_overlapping_bodies()
		for target in targets:
			if damaged_targets.has(target):
				continue
			else:
				target.reduce_hp(damage)
				damaged_targets.append(target)
		yield(get_tree().create_timer(0.05), "timeout")
		continue
	queue_free()
