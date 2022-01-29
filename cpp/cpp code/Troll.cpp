#include "Troll.h"
#include <algorithm>
#include <Area2D.hpp>
#include <iostream>


//using namespace std;
using namespace godot;

void Troll::_register_methods() {//z t,fk 'nj ujdybot
	register_method((char*)"_physics_process", &Troll::_physics_process);
	register_method((char*)"_ready", &Troll::_ready);
	register_method((char*)"_process", &Troll::_process);
	//register_signal<Troll>((char*)"area_entered", "area", GODOT_VARIANT_TYPE_OBJECT);
}
Troll::Troll() {}

//void Troll::_init() {};

Troll::~Troll() {}

void Troll::_process(float delta) {}


void Troll::_ready() {  //�������, ���������� ��� �������� ��������
	speed = default_speed; //������������� ������� �������� �����

	//������������� hp bar �����        self - ������ this->� �++
	hp = 100;
	max_hp = 60;  //������ �������� ������� ������ ��� � ������
	set_start_hp(hp, max_hp);

	//��������� ���� � ������(��� ������ ������� Alt)
	//add_to_group(GlobalVars.entity_group);
	//add_to_group(GlobalVars.troll_group);
}

void Troll::_physics_process(float delta) {
	switch (state)
	{
	case State::Move:
		move_state(delta);
	case State::Attack:
		attack_state(delta);
	// case State::Death:
		//death_state(delta);
	}
}

void Troll::set_start_hp(int hp, int max_hp) { // ���������� hp bar � ��������
	HP_bar = (TextureProgress*)get_node("HP_bar");
	HP_bar->set_value(hp); // ������� �������� hp
	HP_bar->set_max(max_hp);
}

void Troll::move_state(float delta) {
	// ������������ ����
	if (velocity != Vector2(0,0)) { // ���� � �������� ���� ��������
		prev_pos = get_position(); // ������ ��������� ���������� �������

		move_and_slide(velocity); // ������� ��� �� �������� ��������

		// ������������ ��� ���������� � ������ �����
		//position.x(clamp(position.x, 0, 10000));
		//position.y = clamp(position.y, 0, 10000);
	}
	wander(); // ������
	//search_for_target(); // ���� ������

	if (target_intercepted and can_bite) { // ���� ��� ����� ���������, �� �������
		state = State::Attack;
		//bite(target);
	}
}

void Troll::attack_state(float delta) {
	animationTree->set("parameters/Attack/blend_position", velocity);
	animationState->travel("Attack");
}

void Troll::attack_animation_finished() {
	state = State::Move;
}

void Troll::set_destination(Vector2 dest) { // ������������� ����� ����������
	destination = dest;
	//normalized() - ��������� ������
	velocity = (destination - get_position()).normalized() * speed; // �������� �������� � ����

	if (velocity != Vector2(0, 0)) {
		animationTree->set("parameters/Idle/blend_position", velocity);
		animationTree->set("parameters/Run/blend_position", velocity);
		animationState->travel("Run");
	}
	else {
		animationState->travel("Idle");
		stands = false;
	}
}

void Troll::wander() { // �������
	Vector2 pos = get_position();
	std::cout << "1";
	if (stands) { //���� �������� ����� � �� ��������
		srand(time(NULL));// ���������� ��������� ����� �����������
		int x = int(GetRandomNumber(pos.x - 150, pos.x + 150));
		int y = int(GetRandomNumber(pos.y - 150, pos.y + 150));

		//������������� ������� �����������
		//int x1 = clamp(x, 0, 10000);
		//int y1 = clamp(y, 0, 10000);

		set_destination(Vector2(x, y));
	}
	//�������� �� ��������, ���� �� ��������, �� ����� ������������
	else if (velocity != Vector2() && (target = NULL)) {
		if (pos.distance_to(destination) <= speed) {
			cancel_movement();
		}
		else if (pos.distance_to(prev_pos) <= 0.6) {
			//���� ��� ����� ��������� � ����������� �� ���������, �� ��� ��������� ���������
			cancel_movement();
		}
	}
}

//������ ����
void Troll::death_state(float delta) {
	target_intercepted = false;
	target = NULL;
	animationTree->set("parameters/Death/blend_position", velocity);
	animationState->travel("Death");
}

void Troll::death_animation_finished() {
	get_parent()->remove_child(this); // ������� ����
	queue_free(); // ����������� ������ �� ����
}


/*
//area_entered() - ��������� ����������� � ������ �������� area2D
void Troll::_on_BiteArea_area_entered(Area2D area) {
	BiteArea->connect("area_entered", this, "_BiteArea_entered");
}

void Troll::_BiteArea_entered(Area2D area) {
	if (area.get_parent() == target) { //���� � ���� ������
		target_intercepted = true;
	}
}*/

/*
	//area_exited() - ��������� ���������� ����������� � ������ �������� area2D
void Troll::_on_BiteArea_area_exited(area) {
	//���� ������ ������� ������� �����, �� ��� ������ ��������� ���������
	if (area.get_parent() == target) {
		target_intercepted = false;
	}
}*/

/*
// �������
void Troll::_on_StandingTimer_timeout() { // ������ ������� ����
	stands = true;
}


void Troll::_on_BiteCooldown_timeout() { // ������ ��������
	can_bite = true;
}*/


void Troll::cancel_movement() { // ������������� ��������
	animationState->travel("Idle"); // ��������� �� ���

	// �������� �������� � ����� ����������
	velocity = Vector2();
	destination = Vector2();
	speed = default_speed; // ���������� �������� �� ���������
	Timer* StandingTimer = (Timer*)get_node("StandingTimer");
	StandingTimer->start(2); // ��������� ������.���� ������ ���� ����� 2 �������
	target = NULL;
}


void Troll::bite(Person* targ) { // ����� ����
	bool is_alive = targ->reduce_hp(bite_strength);
	can_bite = false;
	Timer* BiteCooldown = (Timer *)get_node("BiteCooldown");
	BiteCooldown->start(1.5); // ������ ������� �������� = 1,5 ���
	if (!is_alive) {
		cancel_movement();
	}
}




/*
KinematicBody2D* Troll::get_player() {
	//��������� ���������� �� ���� ������
	if (get_parent()->get_parent()->has_node("YSort/Hero")) {
		KinematicBody2D* Hero = (KinematicBody2D*)get_node_or_null("../YSort/Hero");
		return (Hero);
	}
}*/
/*
void Troll::search_for_target() { //�������, ������ �������������� ������
	KinematicBody2D* pl = get_player(); // ������� ������

		//��������� ���������� �� �����
	if (pl) {
		if (target) { // ���� � ������� �����, �.�.��� ���������� ������
	// ���� ����� ������
			Vector2 targ_pos = target->get_position();
			if (position.distance_to(targ_pos) > 200) {
				cancel_movement();
			}// �������� ���������� ��������
			else if (position.distance_to(pl->get_position()) < 200) { // ���������, ����� �� ��������� �����
				cancel_movement();
				if (speed == default_speed) {
					speed = default_speed * 2; // ����������� �������� ���� ���������� ������
				}
				//����� ����� ������ - ������
				target = pl;
			}
			else { // ���� �����
				set_destination(targ_pos);
			}
		}
	}
}*/