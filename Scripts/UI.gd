extends CanvasLayer


onready var health_globe = $HealthGlobe/GlobeFull/TextureProgress
onready var health_globe_tween = $HealthGlobe/GlobeFull/TextureProgress/Tween

#onready var hero = get_node("")
onready var hero = get_viewport().get_node("Root/StartWorld").get_player() # ссылка на игрока



func _ready():
	#print(hero.max_hp)
	health_globe.max_value = hero.max_hp
	health_globe.value = hero.hp


func _process(delta):
	if !$Control/DeathScreen.is_visible_in_tree():
		update_globes()
	else:
		$HealthGlobe.hide()
		$ManaGlobe.hide()


func update_globes():
	var temp = get_viewport().get_node("Root/StartWorld").get_player()
	var new_hp = hero.hp
	health_globe_tween.interpolate_property(health_globe, "value", health_globe.value, new_hp, 0.1, Tween.TRANS_LINEAR, Tween.EASE_IN_OUT)
	health_globe_tween.start()
