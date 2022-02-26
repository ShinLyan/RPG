extends RigidBody2D


var projectile_speed
var life_time = 0.5
var fire_direction
var damage
var skill_name



func _ready():
	damage = DataImport.skill_data[skill_name].SkillDamage
	projectile_speed = DataImport.skill_data[skill_name].SkillProjectileSpeed
	var skill_texture = load("res://assets/skills/" + skill_name + ".png")
	get_node("Projectile").set_texture(skill_texture)
	
	
	apply_impulse(Vector2(), Vector2(projectile_speed, 0).rotated(rotation))
	self_destruct()


func self_destruct():
	yield(get_tree().create_timer(life_time), "timeout")
	queue_free()



func _on_Spell_body_entered(body):
	get_node("CollisionPolygon2D").set_deferred("disabled", true)
	if body.is_in_group("Enemies"):
		body.reduce_hp(damage)
	self.hide()
