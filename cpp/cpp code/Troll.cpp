#include "Troll.h"
#include <algorithm>

#include <iostream>
//надо везде прописать проверку указателей на ноль
// глобальные переменные ??? 


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

void Troll::_ready() {  //функци€, вызывающа€ при создании существа
	state = State::Move;

	default_speed = 45; // обычна€ скорость моба
	speed = default_speed; // устанавливаем обычную скорость мобов

	
	// ”станавливаем hp bar мобам        self - аналог this->в —++
	this->hp = 60;
	this->max_hp = 60; // делаем здоровье немного меньше чем у игрока
	this->bite_strength = 5; // сила удара моба(5 единиц здоровь€ за удар)
	set_start_hp(this->hp, this->max_hp);

	ResourceLoader* loader;
	Ref<PackedScene> s = loader->get_singleton()->load("res://Scenes/Singletones/GlobalVars.tscn");
	Node* GlobalVars = s->instance();
	if (GlobalVars->has_method("set_num_trolls")) {
		int k = GlobalVars->call("get_num_trolls", this);
		k++;
		GlobalVars->call("set_num_trolls", this, k);
	}
	// ƒобавл€ем моба в группы(дл€ работы клавиши Alt)
	GlobalVars->add_to_group("entity_group");
	GlobalVars->add_to_group("troll_group");

	//добавл€ем предмет в трол€
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


//на момент отладки выключена ф-ци€

KinematicBody2D* Troll::get_player() {
	//провер€ем существует ли узел игрока
	if (get_parent()->get_parent()->has_node("YSort/Hero")) {
		KinematicBody2D* Hero = (KinematicBody2D*)get_node_or_null("../YSort/Hero");
		return (Hero);
		//Godot::print_error();
	}
	else return NULL;
}
//переделать в соответствии с кодом ƒимы
void Troll::search_for_target() { //функци€, ищуща€ местоположение игрока
	KinematicBody2D* pl = get_player(); // достаем игрока

	//провер€ем существуют ли игрок
	if (pl) {
		if (target) { // если в таргете игрок, т.е.моб преследует игрока
	// если таргет далеко
			Vector2 targ_pos = target->get_position();
			if (position.distance_to(targ_pos) > 200) {
				cancel_movement();
			}// отмен€ем предыдущее движение
			else {
				KinematicBody2D* temp = target;
				cancel_movement();
				target = temp;
				speed = default_speed * 2; // увеличиваем скорость моба атакующего игрока
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
	// ѕередвижение моба
	if (velocity != Vector2(0, 0)) { // если у существа есть скорость
		prev_pos = this->get_position(); // сперва фиксируем предыдущую позицию

		Vector2 tmp = move_and_slide(velocity); // двигаем его на величину скорости
	}
	wander(); // бродим
	search_for_target(); // ищем таргет

	if (target_intercepted and can_bite) { // если моб может атаковать, то атакуем
		state = State::Attack;
		bite(target);
	}
}



// —мерть моба
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


//area_entered() - провер€ет пересечение с другой областью area2D
void Troll::_on_BiteArea_area_entered(Variant area) {
	Area2D* area2 = static_cast<godot::Area2D*>(area);
	if (area2->get_parent() == target) { //если в зоне таргет
		target_intercepted = true;
	}
}


	//area_exited() - провер€ет отсутствие пересечени€ с другой областью area2D
void Troll::_on_BiteArea_area_exited(Variant area) {
	Area2D* area2 = static_cast<godot::Area2D*>(area);
	//если таргет покинул область атаки, то моб должен перестать атаковать
	if (area2->get_parent() == target) {
		target_intercepted = false;
	}
}

// “аймеры
void Troll::_on_StandingTimer_timeout() { // таймер сто€ни€ моба
	stands = true;
}


void Troll::_on_BiteCooldown_timeout() { // таймер кулдауна
	can_bite = true;
}