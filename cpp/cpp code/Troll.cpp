#include "Troll.h"
#include <algorithm>

#include <iostream>
//���� ����� ��������� �������� ���������� �� ����
// ���������� ���������� ??? 


//using namespace std;
using namespace godot;

AI::State Troll::get_state()
{
	return state;
}

void Troll::_register_methods() {//z t,fk 'nj ujdybot
	
	register_method((char*)"_ready", &Troll::_ready);
	register_method((char*)"_physics_process", &Troll::_physics_process);
	register_method((char*)"_process", &Troll::_process);
	register_method((char*)"_init", &Troll::_init);
	
	register_method((char*)"move_state", &Troll::move_state);
	register_method((char*)"attack_state", &Troll::attack_state); 
	register_method((char*)"attack_animation_finished", &Troll::attack_animation_finished); 
	register_method((char*)"death_state", &Troll::death_state); 
	register_method((char*)"death_animation_finished", &Troll::death_animation_finished); 
	register_method((char*)"bite", &Troll::bite);
	register_method((char*)"search_for_target", &Troll::search_for_target);	
	register_method((char*)"wander", &Troll::wander);	
	register_method((char*)"set_destination", &Troll::set_destination);	
	register_method((char*)"set_start_hp", &Troll::set_start_hp);
	register_method((char*)"_on_StandingTimer_timeout", &Troll::_on_StandingTimer_timeout);
	register_method((char*)"_on_BiteArea_area_entered", &Troll::_on_BiteArea_area_entered);
	register_method((char*)"_on_BiteArea_area_exited", &Troll::_on_BiteArea_area_exited);
	register_method((char*)"_on_BiteCooldown_timeout", &Troll::_on_BiteCooldown_timeout);
}

Troll::Troll() {}

void Troll::_init() {};

Troll::~Troll() {}

void Troll::_ready() {  //�������, ���������� ��� �������� ��������
	state = State::Move;

	default_speed = 45; // ������� �������� ����
	speed = default_speed; // ������������� ������� �������� �����

	
	// ������������� hp bar �����        self - ������ this->� �++
	this->hp = 60;
	this->max_hp = 60; // ������ �������� ������� ������ ��� � ������
	this->bite_strength = 5; // ���� ����� ����(5 ������ �������� �� ����)
	set_start_hp(this->hp, this->max_hp);

	ResourceLoader* loader;
	Ref<PackedScene> s = loader->get_singleton()->load("res://Scenes/Singletones/GlobalVars.tscn");
	Node* GlobalVars = s->instance();
	if (GlobalVars->has_method("set_num_trolls")) {
		int k = GlobalVars->call("get_num_trolls", this);
		k++;
		GlobalVars->call("set_num_trolls", this, k);
	}
	// ��������� ���� � ������(��� ������ ������� Alt)
	GlobalVars->add_to_group("entity_group");
	GlobalVars->add_to_group("troll_group");

	//��������� ������� � �����
	Ref<PackedScene> d = loader->get_singleton()->load("res://Scenes/Singletones/ItemMachine.tscn");
	Node* ItemMachine = d->instance();
	Node* item = ItemMachine->call("generate_inventory_item", "coins", 10);
	this->inventory->call("add_item", item);
}

void Troll::_physics_process(float delta) {
	Godot::print("physicqiqi");
	State state = get_state();

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
}


//�� ������ ������� ��������� �-���

KinematicBody2D* Troll::get_player() {
	//��������� ���������� �� ���� ������
	if (get_parent()->get_parent()->has_node("YSort/Hero")) {
		KinematicBody2D* Hero = (KinematicBody2D*)get_node_or_null("../YSort/Hero");
		return (Hero);
		//Godot::print_error();
	}
	else return NULL;
}
//���������� � ������������ � ����� ����
void Troll::search_for_target() { //�������, ������ �������������� ������
	KinematicBody2D* pl = get_player(); // ������� ������

	//��������� ���������� �� �����
	if (pl) {
		if (target) { // ���� � ������� �����, �.�.��� ���������� ������
	// ���� ������ ������
			Vector2 targ_pos = target->get_position();
			if (position.distance_to(targ_pos) > 200) {
				cancel_movement();
			}// �������� ���������� ��������
			else {
				KinematicBody2D* temp = target;
				cancel_movement();
				target = temp;
				speed = default_speed * 2; // ����������� �������� ���� ���������� ������
				set_destination(target->get_position());
			}
		}
	}
}

void Troll::_on_Sight_body_entered(KinematicBody2D* body) {
	if (body->is_in_group("Player") or body->is_in_group("Allies")) {
		target_in_range = true;
		sees_enemies.push_back(body);
	}
}
void Troll::_on_Sight_body_exited(KinematicBody2D* body) {
	if (body->is_in_group("Player") or body->is_in_group("Allies")) {
		target = NULL; // ������� ������
		std::vector<KinematicBody2D*>::iterator it = find(sees_enemies.begin(), sees_enemies.end(), body);
		if (it != sees_enemies.end()) {
			sees_enemies.erase(it);
			if (sizeof(sees_enemies) == 0) {
				target_in_range = false;
			}
		}
	}
}
void Troll::sight_check() {// ,kznm 
	if (target_in_range && target) {
		target_position = target->get_global_position();
		state = State::Combat;
	}
}


void Troll::_process(float _delta) {
	if (target == NULL and sizeof(sees_enemies) > 0) {
		target = sees_enemies[0];
	}
}


void Troll::attack_state(float delta) {
	AnimationTree* animationTree = (AnimationTree*)get_node_or_null("AnimationTree");
	AnimationNodeStateMachinePlayback* animationState = animationTree->get("parameters/playback");

	animationTree->set("parameters/Attack/blend_position", velocity);
	animationState->travel("Attack");
}

void Troll::attack_animation_finished() {
	animationState->travel("Idle");
	if (can_bite) {
		state = State::Move;
	}
}


void Troll::move_state(float delta) {
	// ������������ ����
	if (velocity != Vector2(0, 0)) { // ���� � �������� ���� ��������
		prev_pos = this->get_position(); // ������ ��������� ���������� �������

		Vector2 tmp = move_and_slide(velocity); // ������� ��� �� �������� ��������
	}
	wander(); // ������
	search_for_target(); // ���� ������

	if (target_intercepted and can_bite) { // ���� ��� ����� ���������, �� �������
		state = State::Attack;
		bite(target);
	}
}



// ������ ����
void Troll::death_state(float delta) {
	AI::death_state(delta);
	target_intercepted = false;
	target = NULL;
}

void Troll::death_animation_finished() {
	AI::death_animation_finished();
	ResourceLoader* loader;
	Ref<PackedScene> s = loader->get_singleton()->load("res://Scenes/Singletones/GlobalVars.tscn");
	Node* GlobalVars = s->instance();
	if (GlobalVars->has_method("set_num_trolls")) {
		int k = GlobalVars->call("get_num_trolls", this);
		k--;
		GlobalVars->call("set_num_trolls", this, k);
	}
}


//area_entered() - ��������� ����������� � ������ �������� area2D
void Troll::_on_BiteArea_area_entered(Variant area) {
	Area2D* area2 = static_cast<godot::Area2D*>(area);
	if (area2->get_parent() == target) { //���� � ���� ������
		target_intercepted = true;
	}
}


	//area_exited() - ��������� ���������� ����������� � ������ �������� area2D
void Troll::_on_BiteArea_area_exited(Variant area) {
	Area2D* area2 = static_cast<godot::Area2D*>(area);
	//���� ������ ������� ������� �����, �� ��� ������ ��������� ���������
	if (area2->get_parent() == target) {
		target_intercepted = false;
	}
}

// �������
void Troll::_on_StandingTimer_timeout() { // ������ ������� ����
	stands = true;
}


void Troll::_on_BiteCooldown_timeout() { // ������ ��������
	can_bite = true;
}