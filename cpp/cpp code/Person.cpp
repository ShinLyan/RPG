#include "Person.h"
#include <iostream>

using namespace godot;

void Person::_register_methods() {
	register_method((char*)"_init", &Person::_init);
	//register_method((char*)"_process", &Person::_process);
}


void Person::_init() {};

void godot::Person::_process(float delta){}

Person::Person() {}
Person::~Person() {}


void Person::set_start_hp(real_t hp, real_t max_hp) { // ���������� hp bar � ��������
	HP_bar = (TextureProgress*)get_node("HP_bar");
	HP_bar->set_value(hp); // ������� �������� hp
	HP_bar->set_max(max_hp);
	
}

void Person::update_hp() { // ��������� �������� hp bar �� �������
	HP_bar = (TextureProgress*)get_node("HP_bar");
	HP_bar->set_value(hp);
}

void Person::toggle_hp_bar() {// ��� / ���� hp bar
	HP_bar = (TextureProgress*)get_node("HP_bar");
	if (HP_bar->is_visible()) {
		HP_bar->set_visible(false);
	}
	else HP_bar->set_visible(true);
}


// ����� ����������

bool Person::reduce_hp(real_t val) { // ��������� hp ����������
	hp -= val;
	update_hp();

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