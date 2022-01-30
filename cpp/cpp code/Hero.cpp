#include "Hero.h"
#include <Position2D.hpp>

using namespace godot;

//void godot::Hero::_unhandled_input(event)
//{}

void Hero::range_attack_state(float delta)
{
	//DamagePos должен двигаться в направлении мыши, но при этом не уходить слишком далеко от игрока
	Position2D* FirePos = (Position2D*)get_node("FirePos");
	FirePos->get_position() = get_global_mouse_position() - position;
	//$FirePos.position.x = clamp($FirePos.position.x, -35, 33)
	//$FirePos.position.y = clamp($FirePos.position.y, -35, 33)

	animationTree->set("parameters/Cast/blend_position", FirePos->get_position());
	animationState->travel("Cast");
}

// HP bar
void Hero::_ready()
{
	// self - аналог this->в C++
	this->hp = 100; // исходное здоровье игрока
	set_start_hp(this->hp, this->max_hp); // задаем hp персонажу
	//add_to_group(GlobalVars.entity_group) //ytn ukj,fkmys[ pyfxtybq gjrf xnj
}

void Hero::_physics_process(float delta)
{
	switch (state)
	{
	case State::Move:
		move_state(delta);
	case State::Attack:
		attack_state(delta);
	 case State::Death:
		death_state(delta);
	case State::Range:
		range_attack_state(delta);
	}
}

Vector2 Hero::get_direction()
{
	return (Vector2(
		i->get_action_strength("ui_right") - i->get_action_strength("ui_left"),
		i->get_action_strength("ui_down") - i->get_action_strength("ui_up")
	)).normalized();
}

void Hero::move_state(float delta)
{
	direction = get_direction();
	if (direction != Vector2()) {
		animationTree->set("parameters/Idle/blend_position", direction);
		animationTree->set("parameters/Run/blend_position", direction);
		animationTree->set("parameters/Death/blend_position", direction);
		animationState->travel("Run");
	}
	else {
		animationState->travel("Idle");
		move_and_slide(direction * speed);
	}

	if (i->is_action_just_pressed("left_click")) {
		state = State::Attack;
	}
	if (i->is_action_just_pressed("ui_fire")) {
		state = State::Range;
	}
}

void Hero::attack_state(float delta)
{
	// DamagePos должен двигаться в направлении мыши, но при этом не уходить слишком далеко от игрока
	Position2D* DamagePos = (Position2D*)get_node("DamagePos");
	DamagePos->get_position() = get_global_mouse_position() - position;
	//$DamagePos.position.x = clamp($DamagePos.position.x, -35, 33)
	//$DamagePos.position.y = clamp($DamagePos.position.y, -35, 33)

	animationTree->set("parameters/Attack/blend_position", DamagePos->get_position());
	animationState->travel("Attack");
}

void Hero::attack_animation_finished()
{
	state = State::Move;
}

void godot::Hero::death_state(float delta)
{
	animationTree->set("parameters/Death/blend_position", direction);
	animationState->travel("Death");
}

void Hero::death_animation_finished()
{
	emit_signal("on_death"); //  выпускаем сигнал смерти игрока (DOESN'T WORK NOW)
	get_parent()->remove_child(this); // удаляем узел
	queue_free(); // освобождаем память от него
}
