extends RigidBody2D


var projectile_speed
var life_time = 0.5
var fire_direction
var damage
var skill_name
var origin
var animation
#var mana_cost


func _ready():
	if origin == "Player": # если стреляет игрок, то одни параметры, если нет то другие
		damage = DataImport.skill_data[skill_name].SkillDamage
		projectile_speed = DataImport.skill_data[skill_name].SkillProjectileSpeed
		set_collision_mask_bit(1, false)
	elif origin == "Allies":
		damage = 10
		projectile_speed = 300
		set_collision_mask_bit(1, false)
		set_collision_mask_bit(3, false)
	var skill_texture = load("res://assets/skills/" + skill_name + ".png")
	get_node("Projectile").set_texture(skill_texture)
	
	
	apply_impulse(Vector2(), Vector2(projectile_speed, 0).rotated(rotation))
	self_destruct()
	#set_animation()


func set_animation():
	if fire_direction <= 15 and fire_direction >= -15:
		animation = "Fire_E"
	elif fire_direction <= 60 and fire_direction >= 15:
		animation = "Fire_SE"
		get_node("Projectile").rotation_degrees -= 30
	elif fire_direction <= 120 and fire_direction >= 60:
		animation = "Fire_S"
		get_node("Projectile").rotation_degrees -= 90
	elif fire_direction <= 165 and fire_direction >= 120:
		animation = "Fire_SW"
		get_node("Projectile").rotation_degrees -= 150
	elif fire_direction <= -60 and fire_direction >= -15:
		animation = "Fire_NE"
		get_node("Projectile").rotation_degrees += 30
	elif fire_direction <= -120 and fire_direction >= -60:
		animation = "Fire_N"
		get_node("Projectile").rotation_degrees += 90
	elif fire_direction <= -165 and fire_direction >= -120:
		animation = "Fire_NW"
		get_node("Projectile").rotation_degrees += 150
	elif fire_direction <= -165 or fire_direction >= 165:
		animation = "Fire_W"
		get_node("Projectile").rotation_degrees -= 180
	if animation:
		$AnimationPlayer.play(animation)
	#get_node("AnimationPlayer").play(animation)


func self_destruct():
	yield(get_tree().create_timer(life_time), "timeout")
	queue_free()



func _on_Spell_body_entered(body):
	get_node("CollisionPolygon2D").set_deferred("disabled", true)
	
	if body.is_in_group("Enemies"):
		body.reduce_hp(damage)
	elif body.is_in_group("Player") and origin == "Enemy":
		body.reduce_hp(damage)
	self.hide()
