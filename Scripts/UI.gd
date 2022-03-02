extends CanvasLayer


onready var health_globe = $HealthGlobe/GlobeFull/TextureProgress
onready var health_globe_tween = $HealthGlobe/GlobeFull/TextureProgress/Tween


onready var hero = get_viewport().get_node("Root/StartWorld").get_player() # ссылка на игрока



onready var shortcuts_path = "SkillBar/Background/HBoxContainer/"
var loaded_skills = {
	"Shortcut1": "Ice_Spear", 
	"Shortcut2": "Fireball", 
	"Shortcut3": "Lava_Bomb",
	"Shortcut4": "Ice_Nova",
	"Shortcut5": "Heal"
}


func _ready():
	health_globe.max_value = hero.max_hp
	health_globe.value = hero.hp
	
	var skill_icon = load("res://assets/skills/" + loaded_skills["Shortcut1"] + "_icon.png")
	get_node(shortcuts_path + "/SelectedSkill/TextureRect").set_texture(skill_icon)
	hero.selected_skill = loaded_skills["Shortcut1"]
	
	load_shortcuts()
	
	for shortcut in get_tree().get_nodes_in_group("Shortcuts"):
		shortcut.connect("pressed", self, "select_shortcut", [shortcut.get_parent().get_name()])
		

func load_shortcuts():
	for shortcut in loaded_skills.keys():
		var skill_icon = load("res://assets/skills/" + loaded_skills[shortcut] + "_icon.png")
		get_node(shortcuts_path + shortcut + "/TextureButton").set_normal_texture(skill_icon)



func select_shortcut(shortcut):
	var skill_icon = load("res://assets/skills/" + loaded_skills[shortcut] + "_icon.png")
	get_node(shortcuts_path + "/SelectedSkill/TextureRect").set_texture(skill_icon)
	hero.selected_skill = loaded_skills[shortcut]



func _process(_delta):
	if !$Control/DeathScreen.is_visible_in_tree():
		update_globes()
	else:
		$HealthGlobe.hide()
		$ManaGlobe.hide()


func update_globes():
	var new_hp = hero.hp
	health_globe_tween.interpolate_property(health_globe, "value", health_globe.value, new_hp, 0.1, Tween.TRANS_LINEAR, Tween.EASE_IN_OUT)
	health_globe_tween.start()
