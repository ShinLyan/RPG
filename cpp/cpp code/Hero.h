#include "Person.h"
namespace godot {
	class Hero : public Person {
		GODOT_CLASS(Hero, KinematicBody2D)
	private:
		Vector2 position;
		State state = State::Move;
	public:

		// var
		Vector2 velocity = Vector2();

		// ������������ ��������� �� �������
		Vector2 direction = Vector2();
		AnimationTree * animationTree = NULL;
		AnimationNodeStateMachinePlayback* animationState = NULL;
		Input* i = NULL;

		State get_state();
		//void pick(item); ��������� ���� �� �������

		void _unhandled_input(Input* event);
		static void _register_methods();
		void range_attack_state(float delta);
		void _ready();
		void _physics_process(float delta);
		void _init();

		Vector2 get_direction();

		void move_state(float delta);

		void attack_state(float delta);
		void attack_animation_finished();
		void death_state(float delta);
		void death_animation_finished();
	};
}



/*# ���������
var inventory = {} # ������ ����������
# inventory['Apple'] = [15, 8]


func pick(item):
	var it = item.get_item()
	if it in inventory.keys(): # ���� �������� �������� ���� � ���������
		inventory[it][0] += item.get_amount() 
	else:
		inventory[it] = [item.get_amount(), item.get_item_stack()] # ��������� �������
	ui.update_inventory(inventory)









# ��������� ������������ �� �������, ������� �� ���� ���������� ������� ������
func _unhandled_input(event): 
	# ���������
	if event.is_action_pressed("inventory"): # ������� �� ������ I
		ui.toggle_inventory(inventory)
	
	# ����� ������
	if event.is_action_pressed("left_click"): # ������� �� ���
		# ��� ������� ������ ������� DamageArea � ��������� ��� �� �����
		var attack = load("res://Scenes/DamageArea.tscn").instance() 
		attack.set_damage(10) # 10 ����� ������� ����� ������
		get_parent().add_child(attack)
		attack.position = position + $DamagePos.position
	
	if event.is_action_pressed("ui_fire"): # ������� �� ���
		var fire = load("res://Scenes/Fire.tscn").instance()
		get_parent().add_child(fire)
		fire.position = position + $FirePos.position
		fire.velocity = get_global_mouse_position() - fire.position


func range_attack_state(delta):
	# DamagePos ������ ��������� � ����������� ����, �� ��� ���� �� ������� ������� ������ �� ������
	$FirePos.position = get_global_mouse_position() - position
	$FirePos.position.x = clamp($FirePos.position.x, -35, 33)
	$FirePos.position.y = clamp($FirePos.position.y, -35, 33)
	
	animationTree.set("parameters/Cast/blend_position", $FirePos.position)
	animationState.travel("Cast")


# HP bar
func _ready():
	# self - ������ this-> � C++
	self.hp = 100 # �������� �������� ������
	set_start_hp(self.hp, self.max_hp) # ������ hp ���������
	add_to_group(GlobalVars.entity_group)


func _physics_process(delta):
	#$FirePos.look_at(get_global_mouse_position())
	match state:
		MOVE:
			move_state(delta)
		
		ATTACK:
			attack_state(delta)
		
		DEATH:
			death_state(delta)
			
		RANGE:
			range_attack_state(delta)


# ������������ ��������� �� �������
var direction = Vector2.ZERO


func get_direction():
	return (Vector2(
		Input.get_action_strength("ui_right") - Input.get_action_strength("ui_left"),
		Input.get_action_strength("ui_down") - Input.get_action_strength("ui_up")
	)).normalized()

var velocity = Vector2()

func move_state(delta):
	direction = get_direction()
	if direction != Vector2.ZERO:
		animationTree.set("parameters/Idle/blend_position", direction)
		animationTree.set("parameters/Run/blend_position", direction)
		animationTree.set("parameters/Death/blend_position", direction)
		animationState.travel("Run")
	else:
		animationState.travel("Idle")
	move_and_slide(direction * speed)
	
	if Input.is_action_just_pressed("left_click"):
		state = ATTACK
	
	if Input.is_action_just_pressed("ui_fire"):
		state = RANGE


func attack_state(delta):
	# DamagePos ������ ��������� � ����������� ����, �� ��� ���� �� ������� ������� ������ �� ������
	$DamagePos.position = get_global_mouse_position() - position
	$DamagePos.position.x = clamp($DamagePos.position.x, -35, 33)
	$DamagePos.position.y = clamp($DamagePos.position.y, -35, 33)
	
	animationTree.set("parameters/Attack/blend_position", $DamagePos.position)
	animationState.travel("Attack")


func attack_animation_finished():
	state = MOVE


# ������ ������
signal on_death # ������ ������ ���������


func death_state(delta):
	animationTree.set("parameters/Death/blend_position", direction)
	animationState.travel("Death")


func death_animation_finished():
	emit_signal("on_death") # ��������� ������ ������ ������
	get_parent().remove_child(self) # ������� ����
	queue_free() # ����������� ������ �� ����


# ������������� ������� save()
func save():
	var data = .save() # �������� ������������ ������� save()
	data["inventory"] = inventory
	return data

*/