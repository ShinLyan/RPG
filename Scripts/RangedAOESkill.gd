extends Area2D


var skill_name
var damage
var damage_delay_time
var remove_delay_time
var origin


func _ready():
	if origin == "Player":
		damage = DataImport.skill_data[skill_name].SkillDamage
		damage_delay_time = float(DataImport.skill_data[skill_name].SkillDamageDelayTime)
		remove_delay_time = DataImport.skill_data[skill_name].SkillRemoveDelayTime
		set_collision_mask_bit(1, false)
	get_node("CollisionShape2D").get_shape().radius = DataImport.skill_data[skill_name].SkillRadius
	var skill_texture = load("res://assets/skills/" + skill_name + ".png")
	$Sprite.set_texture(skill_texture)
	AOE_attack()


func AOE_attack():
	get_node("AnimationPlayer").play(skill_name)
	yield(get_tree().create_timer(damage_delay_time), "timeout")
	var targets = get_overlapping_bodies()
	for target in targets:
		if target.is_in_group("Enemies"):
			target.reduce_hp(damage)
	yield(get_tree().create_timer(remove_delay_time), "timeout")
	self.queue_free()
