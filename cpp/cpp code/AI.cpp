#include "AI.h"

using namespace godot;

// hp functions
void AI::set_start_hp(real_t hp, real_t max_hp) { // ���������� hp bar � ��������
	HP_bar = (TextureProgress*)get_node("HP_bar");
	HP_bar->set_value(hp); // ������� �������� hp
	HP_bar->set_max(max_hp);

}
void AI::toggle_hp_bar() {// ��� / ���� hp bar
	HP_bar = (TextureProgress*)get_node("HP_bar");
	if (HP_bar->is_visible()) {
		HP_bar->set_visible(false);
	}
	else HP_bar->set_visible(true);
}
bool AI::reduce_hp(real_t val)
{
	if (Person::reduce_hp(val)) return true;
	else return false;
	update_hp();
}
void AI::update_hp() { // ��������� �������� hp bar �� �������
	HP_bar = (TextureProgress*)get_node("HP_bar");
	Tween* HP_bar_Tween = (Tween*)get_node("HP_bar/Tween");
	HP_bar_Tween->interpolate_property(HP_bar, "value", HP_bar->get_value(), hp, 0.1, godot::Tween::TransitionType::TRANS_LINEAR, godot::Tween::EaseType::EASE_IN_OUT);
	HP_bar_Tween->start();
}
void AI::increase_hp(real_t val) { // ����������� �������� hp bar �� �������
	Person::increase_hp(val);
	update_hp();
}


void AI::wander() { // �������
	Vector2 pos = this->get_position();
	if (stands) { //���� �������� ����� � �� ��������
		srand(time(NULL));// ���������� ��������� ����� �����������
		int x = int(GetRandomNumber(int(pos.x) - 5, int(pos.x) + 5));
		int y = int(GetRandomNumber(int(pos.y) - 5, int(pos.y) + 5));
		//�������� ���� � 
		set_destination(Vector2(x, y));
		if (x == 0 && y == 0) {
			cancel_movement();
		}
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

void AI::set_destination(Vector2 dest) { // ������������� ����� ����������
	AnimationTree* animationTree = (AnimationTree*)get_node_or_null("AnimationTree");
	AnimationNodeStateMachinePlayback* animationState = animationTree->get("parameters/playback");

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
	}
	stands = false;
}

void AI::cancel_movement() { // ������������� ��������
	Timer* StandingTimer = (Timer*)get_node("StandingTimer");
	animationState->travel("Idle"); // ��������� �� ���

	// �������� �������� � ����� ����������
	velocity = Vector2();
	destination = Vector2();
	speed = default_speed; // ���������� �������� �� ���������
	StandingTimer->start(2); // ��������� ������.���� ������ ���� ����� 2 �������
	target = NULL;
}


void AI::bite(KinematicBody2D* targ) { // ����� ����
	if (targ != NULL) {
		bool is_alive = false;
		if (targ->has_method("reduce_hp") && can_bite) {
			is_alive = targ->call("reduce_hp", this, bite_strength);
		}
		can_bite = false;
		Timer* BiteCooldown = (Timer*)get_node("BiteCooldown");
		BiteCooldown->start(0.5); // ������ ������� �������� = 1,5 ���
		if (!is_alive) {
			cancel_movement();
		}
	}
}

void AI::heal(KinematicBody2D* targ) {
	if (targ != NULL) {
		bool _is_alive = false;
		if (targ->has_method("increase_hp")) {
			_is_alive = targ->call("increase_hp", this, heal_strength);
		}
		can_bite = false;
		Timer* BiteCooldown = (Timer*)get_node("BiteCooldown");
		BiteCooldown->start(0.5); // ������ ������� �������� = 1,5 ���
		real_t t_hp, t_max_hp;
		if (targ->has_method("get_hp")) {
			t_hp = targ->call("get_hp", this);
		}
		if (targ->has_method("get_max_hp")) {
			t_max_hp = targ->call("get_max_hp", this);
		}
		if (t_hp == t_max_hp) {
			cancel_movement();
			target_intercepted = false;
		}
	}
}



void AI::non_combat_state(float _delta) {
	if (velocity != Vector2()) { // ���� � �������� ���� ��������
		prev_pos = get_position(); // ������ ��������� ���������� �������
		Vector2 _var = move_and_slide(velocity); // ������� ��� �� �������� ��������
	}
	wander(); // ������
}
void AI::combat_state(float _delta) { ; }
void AI::regrouping_state(float _delta) { ; }
	
// ������ npc
void AI::death_state(float delta) {
	AnimationTree* animationTree = (AnimationTree*)get_node_or_null("AnimationTree");
	AnimationNodeStateMachinePlayback* animationState = animationTree->get("parameters/playback");

	animationTree->set("parameters/Death/blend_position", velocity);
	animationState->travel("Death");
}

void AI::death_animation_finished() {
	get_parent()->remove_child(this); // ������� ����
	queue_free(); // ����������� ������ �� ����
}



