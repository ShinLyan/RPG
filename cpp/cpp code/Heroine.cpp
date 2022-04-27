#include "Heroine.h"
#include <Physics2DServer.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <Physics2DDirectSpaceState.hpp>
#include <Position2D.hpp>
#include <World2D.hpp>
#include <gen/RigidBody2D.hpp>
#include <algorithm>
//надо везде прописать проверку указателей на ноль
// глобальные переменные ??? 


using namespace godot;


void Heroine::_register_methods() {//z t,fk 'nj ujdybot

	register_method((char*)"_ready", &Heroine::_ready);
	register_method((char*)"_physics_process", &Heroine::_physics_process);
	register_method((char*)"_process", &Heroine::_process);
	register_method((char*)"_init", &Heroine::_init);

	
	register_method((char*)"attack_state", &Heroine::attack_state);
	register_method((char*)"attack_animation_finished", &Heroine::attack_animation_finished);
	register_method((char*)"_on_StandingTimer_timeout", &Heroine::_on_StandingTimer_timeout);
	register_method((char*)"_on_BiteCooldown_timeout", &Heroine::_on_BiteCooldown_timeout);
}

Heroine::Heroine() {}

void Heroine::_init() {};

Heroine::~Heroine() {}

void Heroine::_ready() {  //функци€, вызывающа€ при создании существа
	state = State::Move;
	default_speed = 45; // обычна€ скорость моба
	speed = default_speed; // устанавливаем обычную скорость мобов

	ResourceLoader* loader;
	Ref<PackedScene> s = loader->get_singleton()->load("res://Scenes/Singletones/GlobalVars.tscn");
	Node* GlobalVars = s->instance();

	// ”станавливаем hp bar мобам        self - аналог this->в —++
	this->hp = 80;
	this->max_hp = 80; // делаем здоровье немного меньше чем у игрока
	this->bite_strength = 10; // сила удара моба(5 единиц здоровь€ за удар)
	set_start_hp(this->hp, this->max_hp);
	// ƒобавл€ем моба в группы(дл€ работы клавиши Alt)
	GlobalVars->add_to_group("entity_group");

	//добавл€ем предмет в трол€

	Ref<PackedScene> r = loader->get_singleton()->load("res://Scenes/Singletones/ItemMachine.tscn");
	Node* ItemMachine = r->instance();
	Node* item = ItemMachine->call("generate_inventory_item", "coins", 10);
	this->inventory->call("add_item", item);
	HP_bar = (TextureProgress*)get_node_or_null("HP_bar");
	speed = default_speed; //устанавливаем обычную скорость мобов

	//”станавливаем hp bar мобам        self - аналог this->в —++
	hp = 100;
	max_hp = 60;  //делаем здоровье немного меньше чем у игрока
	set_start_hp(hp, max_hp);

	//Godot::print();
	State state = State::Move;
}

void Heroine::_physics_process(float delta) {
	State state = get_state();

	if (state == State::Non_combat) {
		non_combat_state(delta);
	}
	else
		if (state == State::Combat) {
			combat_state(delta);
		}
		else if (state == State::Death) {
			death_state(delta);
		}
}


void Heroine::_process(float _delta) {
	if (target == NULL and sizeof(sees_enemies) > 0) {
		target = sees_enemies[0];
	}
	if (this->hp < 0) {
		state = State::Death;
	}
	sight_check();
	if (this->hp < (this->max_hp / 2) && can_heal) {
		self_heal();
	}
}

void Heroine::sight_check() {// ,kznm 
	if (target_in_range && target) {
		Ref<World2D> tmp = get_world_2d();
		Physics2DDirectSpaceState* space_state = Physics2DServer::get_singleton()->space_get_direct_state((*tmp)->get_space());
		//возможно не работает
		Dictionary sight_check = space_state->intersect_ray(this->get_position(), target->get_position());
		if (sight_check.empty()) {
			target_in_sight = true;
			target_position = target->get_global_position();
			state = State::Combat;
		}
		else {
			target_in_sight = false;
			state = State::Non_combat;
		}
	}
}

void Heroine::self_heal() {
	can_heal = false;
	Timer* timer = nullptr;
	timer->_new();

	timer->connect("timeout", this, "self_heal_at");
	timer->set_wait_time(0.25);
	timer->start();// задержка перед хилом
}
void Heroine::sel_heal_at() {
	if (state != State::Death) { // если персонаж живой
		cancel_movement();
		state = State::Attack;
		ResourceLoader* loader;
		Ref<PackedScene> s = loader->get_singleton()->load("res://Scenes/SingleTargetHeal.tscn");
		Node* skill_instance = s->instance();
		Node2D* skill = static_cast<Node2D*>(skill_instance);
		if (skill->has_method("set_skill_name")) {
			skill->call("skill_name", this, "Heal");
		}
		add_child(skill);
		Timer* timer2 = nullptr;
		timer2->_new();

		timer2->connect("timeout", this, "set_can_heal");
		timer2->set_wait_time(1);
		timer2->start();
		
	}
}
void Heroine::_on_Sight_body_entered(KinematicBody2D* body) {
	if (body->is_in_group("Enemies")) {
		target_in_range = true;
		sees_enemies.push_back(body);
	}
}
void Heroine::_on_Sight_body_exited(KinematicBody2D* body){
	if (body->is_in_group("Enemies")){
		target = NULL; // убираем таргет
		std::vector<KinematicBody2D*>::iterator it = find(sees_enemies.begin(), sees_enemies.end(), body);
		if (it != sees_enemies.end()) {
			sees_enemies.erase(it);
			if (sizeof(sees_enemies) == 0) {
				target_in_range = false;
			}
		}
	}
}
	


void Heroine::combat_state(float _delta) {
	if (can_fire) {
		attack_state(_delta);
	}
}
void Heroine::attack_state(float _delta) {
	ResourceLoader* loader;

	can_fire = false;
	fire_direction = (get_angle_to(target_position) / 3.14) * 180;
	Vector2 vel = (target_position - this->get_position()).normalized() * speed; // скорость движени€ к цели

	AnimationTree* animationTree = (AnimationTree*)get_node_or_null("AnimationTree");
	AnimationNodeStateMachinePlayback* animationState = animationTree->get("parameters/playback");

	animationTree->set("parameters/Attack/blend_position", vel);
	animationState->travel("Cast");

	Position2D* tmp = (Position2D*)get_node("TurnAxis");
	tmp->set_rotation_degrees(get_angle_to(target_position));

	Ref<PackedScene> s = loader->get_singleton()->load("res://Scenes/RangedSingleTargetSkill.tscn");
	Node* skill_instance = s->instance();
	RigidBody2D* skill = static_cast<RigidBody2D*>(skill_instance);

	if (skill->has_method("set_skill_name")) {
		skill->call("set_skill_name", "Ice_Spear");
	}
	if (skill->has_method("set_fire_direction")) {
		skill->call("set_fire_direction", fire_direction);
	}
	if (skill->has_method("set_origin")) {
		skill->call("set_origin", "Allies"); // выстрел совершила героин€
	}

	skill->set_rotation_degrees(get_angle_to(target_position));
	tmp = (Position2D*)get_node("TurnAxis/CastPoint");
	skill->set_position(tmp->get_global_position());

	get_parent()->add_child(skill);

	Timer* timer = nullptr;
	timer->_new();

	timer->connect("timeout", this, "set_can_fire");
	timer->set_wait_time(0.6);
	timer->start();
}
void Heroine::attack_animation_finished() {
	AnimationNodeStateMachinePlayback* animationState = animationTree->get("parameters/playback");
	animationState->travel("Idle");
	state = State::Non_combat;
}

void Heroine::set_can_fire() {
		can_fire = true;
}

void Heroine::set_can_heal() {
	can_heal = true;
}

void Heroine::non_combat_state(float _delta) {
	AI::non_combat_state(_delta);
	if (this->hp < this->max_hp and can_heal) {
		self_heal();
	}
}


// “аймеры
void Heroine::_on_StandingTimer_timeout() { // таймер сто€ни€ моба
	stands = true;
}


void Heroine::_on_BiteCooldown_timeout() { // таймер кулдауна
	can_bite = true;
}
