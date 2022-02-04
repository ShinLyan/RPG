#include "Hero.h"
#include <Position2D.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <Area2D.hpp>


using namespace godot;

void godot::Hero::_unhandled_input(Input* event) //не может быть включен во время отладки
{
	ResourceLoader* loader;
	/*if (event->is_action_pressed("inventory")) { // нажатие на кнопку I
		Node* ui = get_viewport()->get_node("Root/UI/Control");
		ui->toggle_inventory(inventory);
	}*/
	if (event->is_action_pressed("left_click")) { // нажатие на ЛКМ
		// при нажатии кнопки создаем DamageArea и добавляем его на карту
		Ref<PackedScene> s = loader->get_singleton()->load("res://Scenes/DamageArea.tscn");
		Node* attackNode = s->instance();
		Area2D* attack = static_cast<Area2D*>(attackNode);
		Position2D* DamagePos = (Position2D*)get_node("DamagePos");
		attack->call("set_damage", this, 10);// 10 урона наносим одной атакой
		// 10 урона наносим одной атакой
		get_parent()->add_child(attack);
		attack->set_position(position + DamagePos->get_position());
	}
	if (event->is_action_pressed("ui_fire")) { // нажатие на ПКМ
		Ref<PackedScene> s = loader->get_singleton()->load("res://Scenes/Fire.tscn");
		Node* fireNode = s->instance();
		KinematicBody2D* fire = static_cast<KinematicBody2D*>(fireNode);
		Position2D* FirePos = (Position2D*)get_node("FirePos");

		get_parent()->add_child(fire);
		fire->set_position(position + FirePos->get_position());
	    Vector2 velocityOfFire = get_global_mouse_position() - fire->get_position();
		fire->call("velocity", this, velocityOfFire);
	}

}

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

void Hero::range_attack_state(float delta)
{
	AnimationTree* animationTree = (AnimationTree*)get_node("AnimationTree");
	AnimationNodeStateMachinePlayback* animationState = animationTree->get("parameters/playback");

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
	hp = 100; // исходное здоровье игрока
	set_start_hp(hp, max_hp); // задаем hp персонажу
	//add_to_group(GlobalVars.entity_group) //ytn ukj,fkmys[ pyfxtybq gjrf xnj
	//ui = get_viewport()->get_node("Root/UI/Control");
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
	/*switch (state)
	{
	case State::Move:
		move_state(delta);
	case State::Attack:
		attack_state(delta);
	 case State::Death:
		death_state(delta);
	case State::Range:
		range_attack_state(delta);
	default:
		move_state(delta);
	}*/
}

void godot::Hero::_init(){}

Vector2 Hero::get_direction()
{
	Input* i = Input::get_singleton();
	return (Vector2(
		i->get_action_strength("ui_right") - i->get_action_strength("ui_left"),
		i->get_action_strength("ui_down") - i->get_action_strength("ui_up")
	)).normalized();
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
		move_and_slide(direction * speed);
	}
	else {
		animationState->travel("Idle");
		
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
	AnimationTree* animationTree = (AnimationTree*)get_node("AnimationTree");
	AnimationNodeStateMachinePlayback* animationState = animationTree->get("parameters/playback");

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
