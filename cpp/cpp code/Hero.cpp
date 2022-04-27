#include "Hero.h"
#include <Position2D.hpp>
#include <Area2D.hpp>
#include <algorithm>
#include <File.hpp>
#include <JSON.hpp>
#include <JSONParseResult.hpp>


using namespace godot;


Person::State Hero::get_state()
{
	return state;
}


void godot::Hero::_register_methods()
{
	register_method("range_attack_speed", &Hero::range_attack_state);
	register_method("_ready", &Hero::_ready);
	register_method("_physics_process", &Hero::_physics_process);
	register_method("get_direction", &Hero::get_direction);
	register_method("_init", &Hero::_init);
	register_method("move_state", &Hero::move_state);
	register_method("attack_state", &Hero::attack_state);
	register_method("attack_animation_finished", &Hero::attack_animation_finished);
	register_method("death_state", &Hero::death_state);
	register_method("death_animation_finished", &Hero::death_animation_finished);
	register_signal<Hero>((char*)"on_death");
}


// HP bar
void Hero::_ready()
{
	state = State::Move;
	this->hp = 200; // исходное здоровье игрока
	this->max_hp = 200;
	this->mp = 100;
	this->max_mp = 100;
	set_start_hp(hp, max_hp); // задаем hp персонажу
	create_inventory();
	inventory->connect("on_changed", this, "update_inventory");
	/*func _ready() :
		var skill_data_file = File.new()
		skill_data_file.open("res://Data/SkillData - Sheet1.json", File.READ)
		var skill_data_json = JSON.parse(skill_data_file.get_as_text())
		skill_data_file.close()
		skill_data = skill_data_json.result
		*/



}

void godot::Hero::_unhandled_input(InputEvent* event) //не может быть включен во время отладки
{
	Input* _input = Input::get_singleton();

	if (_input == nullptr) return;
	if (state == State::Death) return;
	ResourceLoader* loader;
	
	// инвентарь
	if (_input->is_action_pressed("inventory")) {
		Node* ui = get_node("Root/UI/Control");
		ui->call("toggle_inventory", inventory);
	}


	// ближний бой
	if (_input->is_action_pressed("left_click")) { // нажатие на ЛКМ
		state = State::Attack;

		// DamagePos должен двигаться в направлении мыши, но при этом не уходить слишком далеко от игрока
		Position2D* DamagePos = (Position2D*)get_node("DamagePos");
		DamagePos->set_position(get_global_mouse_position() - this->get_position());
		
		real_t x = DamagePos->get_position().x;
		real_t y = DamagePos->get_position().y;
		DamagePos->set_position(Vector2((clip(x, -35, 33)), (clip(y, -35, 33))).normalized());
		
		// при нажатии кнопки создаем DamageArea и добавляем его на карту
		Ref<PackedScene> s = loader->get_singleton()->load("res://Scenes/DamageArea.tscn");
		Node* attackNode = s->instance();
		Area2D* attack = static_cast<Area2D*>(attackNode);

		attack->call("set_damage", 10);// 10 урона наносим одной атакой
		get_parent()->add_child(attack);
		attack->set_position(get_position() + DamagePos->get_position());
	}


}

void godot::Hero::_process(float delta)
{
	skill_loop();
}

void Hero::_physics_process(float delta)
{
	if (state == State::Move) {
		move_state(delta);
	}
	else
		if (state == State::Attack) {
			attack_state(delta);
		}
		else if (state == State::Death) {
			death_state(delta);
		}
		else if (state == State::Range) {
			range_attack_state(delta);
		}
}

void godot::Hero::_init(){}



void godot::Hero::update_inventory()
{
	Node* ui = get_node("Root/UI/Control");
	ui->call("update_inventory", inventory);
}

bool godot::Hero::pick(int item)
{
	bool is_picked = Person::pick(item);
	return is_picked;
}

void godot::Hero::drop_item(int link)
{
	Node* world = get_node("Root/StartWorld");
	world->call("add_lying_item", link, get_position().x, get_position().y);
	inventory->call("remove_item", link);
}

void godot::Hero::skill_loop()
{
	ResourceLoader* loader;
	Input* _input = Input::get_singleton();
	if (_input->is_action_pressed("ui_fire") and can_fire){ // нажатие на ПКМ
		can_fire = false;
		state = State::Range;
		shooting = true;

		Position2D* FirePos = (Position2D*)get_node("FirePos");
		FirePos->set_position(get_global_mouse_position() - this->get_position());
		fire_direction = (get_angle_to(get_global_mouse_position()) / 3.14) * 180;
		Position2D * tmp = (Position2D*)get_node("TurnAxis");
		tmp->set_rotation_degrees(get_angle_to(get_global_mouse_position()));
		/*Node* DataImport = get_node("res://Data/DataImport.tscn");
		int mana_cost = DataImport->call("skill_data", selected_skill, MP); //ujdjyj 
				if mana_cost <= self.mp:
			match DataImport.skill_data[selected_skill].SkillType:
				"RangedSingleTargetSkill":
					var skill = load("res://Scenes/RangedSingleTargetSkill.tscn")
					var skill_instance = skill.instance()
					skill_instance.skill_name = selected_skill
					skill_instance.fire_direction = fire_direction
					skill_instance.rotation = get_angle_to(get_global_mouse_position())
					skill_instance.position = get_node("TurnAxis/CastPoint").get_global_position()
					skill_instance.origin = "Player" # выстрел совершил игрок
					get_parent().add_child(skill_instance)
				
				"RangedAOESkill":
					var skill = load("res://Scenes/RangedAOESkill.tscn")
					var skill_instance = skill.instance()
					skill_instance.skill_name = selected_skill
					skill_instance.position = get_global_mouse_position()
					skill_instance.origin = "Player" # выстрел совершил игрок
					get_parent().add_child(skill_instance)
				
				"ExpandingAOESkill":
					var skill = load("res://Scenes/ExpandingAOESkill.tscn")
					var skill_instance = skill.instance()
					skill_instance.skill_name = selected_skill
					skill_instance.position = get_global_position()
					skill_instance.origin = "Player" # выстрел совершил игрок
					get_parent().add_child(skill_instance)
				
				"SingleTargetHeal":
					var skill = load("res://Scenes/SingleTargetHeal.tscn")
					var skill_instance = skill.instance()
					skill_instance.skill_name = selected_skill
					add_child(skill_instance)
				
				"SingleTargetShield":
					if GlobalVars.num_shields == 0: # если щитов нет
						var skill = load("res://Scenes/SingleTargetShield.tscn")
						var skill_instance = skill.instance()
						skill_instance.skill_name = selected_skill
						skill_instance.origin = self # щит создал игрок
						add_child(skill_instance)
					else:
						mana_cost = 0
			self.mp -= mana_cost
		yield(get_tree().create_timer(rate_of_fire), "timeout")
		can_fire = true
		shooting = false*/






		Ref<PackedScene> s = loader->get_singleton()->load("res://Scenes/Fire.tscn");
		Node* fireNode = s->instance();
		KinematicBody2D* fire = static_cast<KinematicBody2D*>(fireNode);
		
		get_parent()->add_child(fire);
		fire->set_position(position + FirePos->get_position());
	}
}

void godot::Hero::HP_regen(float _delta)
{
	this->hp = fmin(this->hp + hp_regen * _delta, this->max_hp);
}

void godot::Hero::MP_regen(float _delta)
{
	this->mp = fmin(this->mp + mp_regen * _delta, this->max_mp);
}


void Hero::move_state(float delta)
{
	AnimationTree* animationTree = (AnimationTree*)get_node("AnimationTree");
	AnimationNodeStateMachinePlayback* animationState = animationTree->get("parameters/playback");
	Input* i = Input::get_singleton();

	direction = get_direction();
	if (direction != Vector2()) {
		animationTree->set("parameters/Idle/blend_position", direction);
		animationTree->set("parameters/Run/blend_position", direction);
		animationTree->set("parameters/Death/blend_position", direction);
		animationState->travel("Run");
	}
	else {
		animationState->travel("Idle");
	}
	move_and_slide(direction * speed);
}


Vector2 Hero::get_direction()
{
	real_t right = 0, left = 0, down = 0, up = 0;
	Input* i = Input::get_singleton();
	if (i->is_action_pressed("ui_right")) {
		right = i->get_action_strength("ui_right");
	}
	if (i->is_action_pressed("ui_left")) {
		left = i->get_action_strength("ui_left");
	}
	if (i->is_action_pressed("ui_down")) {
		down = i->get_action_strength("ui_down");
	}
	if (i->is_action_pressed("ui_up")) {
		up = i->get_action_strength("ui_up");
	}
	return (Vector2(
		right - left,
		down - up
	)).normalized();
}


void Hero::attack_state(float delta)
{
	AnimationTree* animationTree = (AnimationTree*)get_node("AnimationTree");
	AnimationNodeStateMachinePlayback* animationState = animationTree->get("parameters/playback");

	// DamagePos должен двигаться в направлении мыши, но при этом не уходить слишком далеко от игрока
	Position2D* DamagePos = (Position2D*)get_node("DamagePos");

	animationTree->set("parameters/Attack/blend_position", DamagePos->get_position());
	animationState->travel("Attack");
}

void Hero::attack_animation_finished()
{
	state = State::Move;
}

void Hero::range_attack_state(float delta) {
	AnimationTree* animationTree = (AnimationTree*)get_node("AnimationTree");
	AnimationNodeStateMachinePlayback* animationState = animationTree->get("parameters/playback");
	Position2D* FirePos = (Position2D*)get_node("FirePos");
	if (shooting) {
		animationTree->set("parameters/Cast/blend_position", FirePos->get_position());
		animationState->travel("Cast");
	}
}


void Hero::death_state(float delta)
{
	AnimationTree* animationTree = (AnimationTree*)get_node("AnimationTree");
	AnimationNodeStateMachinePlayback* animationState = animationTree->get("parameters/playback");

	animationTree->set("parameters/Death/blend_position", direction);
	animationState->travel("Death");
}

void Hero::death_animation_finished()
{
	emit_signal("on_death"); //  выпускаем сигнал смерти игрока (DOESN'T WORK NOW)
	get_parent()->remove_child(this); // удаляем узел
	queue_free(); // освобождаем память от него
}

void godot::Hero::DateImport(String selected_skill)
{
	File skill_data_file;
	skill_data_file.open("res://Data/SkillData - Sheet1.json", 1);
	Ref<JSONParseResult> skill_data_json_ref;
	skill_data_json_ref = JSON::get_singleton()->parse(skill_data_file.get_as_text());
	JSONParseResult* skill_data_json;
	skill_data_json = skill_data_json_ref.instance();
	skill_data_file.close();
	Array skill_data = skill_data_json->get_result();

}
