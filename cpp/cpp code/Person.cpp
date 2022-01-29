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



void Person::set_start_hp(real_t hp, real_t max_hp) { // ���������� hp bar � ��������
	//HP_bar = (TextureProgress*)get_node("HP_bar");
	HP_bar->set_value(hp); // ������� �������� hp
	HP_bar->set_max(max_hp);
	
}

void Person::update_hp() { // ��������� �������� hp bar �� �������
	HP_bar->set_value(hp);
}

void Person::toggle_hp_bar() {// ��� / ���� hp bar
	if (HP_bar->is_visible()) {
		HP_bar->set_visible(false);
	}
	else HP_bar->set_visible(true);
}


// ����� ����������

bool Person::reduce_hp(real_t val) { // ��������� hp ����������
	hp -= val;
//	update_hp();

	//������� ������ ���������
	if (hp <= 0) {
		die(); //�������� �������
		return false;
	}
	return true;
}
void Person::die() { //������ ���������
	state = State::Death;
}

//s
int Person::GetRandomNumber(int min, int max)
{
	// ���������� ��������� ��������� �����
	srand(time(NULL));

	// �������� ��������� ����� - �������
	int num = min + rand() % (max - min + 1);

	return num;
}