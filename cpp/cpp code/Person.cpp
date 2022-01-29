#include "Person.h"
#include <iostream>

using namespace godot;

//debug
void Person::_register_methods() {
}
Person::Person(){}

Person::State state = Person::State::Move;

//void Person::_init() {};

Person::~Person() {}



void Person::set_start_hp(real_t hp, real_t max_hp) { // настройска hp bar у существа
	//HP_bar = (TextureProgress*)get_node("HP_bar");
	HP_bar->set_value(hp); // текущее значение hp
	HP_bar->set_max(max_hp);
	
}

void Person::update_hp() { // обновляет значение hp bar на текущее
	HP_bar->set_value(hp);
}

void Person::toggle_hp_bar() {// вкл / выкл hp bar
	if (HP_bar->is_visible()) {
		HP_bar->set_visible(false);
	}
	else HP_bar->set_visible(true);
}


// Атака персонажей

bool Person::reduce_hp(real_t val) { // уменьшает hp персонажам
	hp -= val;
//	update_hp();

	//условие смерти персонажа
	if (hp <= 0) {
		die(); //персонаж умирает
		return false;
	}
	return true;
}
void Person::die() { //смерть персонажа
	state = State::Death;
}

//s
int Person::GetRandomNumber(int min, int max)
{
	// Установить генератор случайных чисел
	srand(time(NULL));

	// Получить случайное число - формула
	int num = min + rand() % (max - min + 1);

	return num;
}