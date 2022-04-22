#include "Person.h"
#include <iostream>
#include <Viewport.hpp>

using namespace godot;
/*
onready var pre_inv = preload("res://Scenes/Inventory.tscn") # ������ ���������
onready var pre_item = preload("res://Scenes/InventItem.tscn")
onready var world = get_viewport().get_node("Root/StartWorld")

onready var ui = get_viewport().get_node("Root/UI/Control") # ������ �� ���������*/

void Person::_register_methods() {
	register_method((char*)"_init", &Person::_init);
	//register_method((char*)"_process", &Person::_process);
}

void godot::Person::_ready()
{
}


void Person::_init() {};

void godot::Person::_process(float delta){}

Person::Person() {}
Person::~Person() {}

void godot::Person::create_inventory()
{
	ResourceLoader* loader;
	Ref<PackedScene> s = loader->get_singleton()->load("res://Scenes/Inventory.tscn");
	Node* pre_inv_node = s->instance();
	Control* inventory = static_cast<Control*>(pre_inv_node);
	add_child(inventory);
	inventory->call("set_inv_owner", this);
}

bool godot::Person::pick(int item)// ����� ����� ���?
{
	this->inventory->call("add_item", item);
	return true;
}


void Person::set_start_hp(real_t hp, real_t max_hp) { // ���������� hp bar � ��������
	HP_bar = (TextureProgress*)get_node("HP_bar");
	HP_bar->set_value(hp); // ������� �������� hp
	HP_bar->set_max(max_hp);
	
}

void Person::increase_hp(real_t val) { // ����������� �������� hp bar �� �������
	if (this->hp + val > this->max_hp) {
		this->hp = this->max_hp;
	}
	else this->hp = this->hp + val;
}

void Person::magic_shield(real_t val) {
	if (this->shield + val > this->max_shield) {
		this->shield = this->max_shield;
	}
	else this->shield = this->shield + val;
}
// ����� ����������

bool Person::reduce_hp(real_t val) { // ��������� hp ����������
	if (shield == 0) {
		this->hp -= val;
		if (this->hp <= 0) {
			die();
			return false;
		}
	}
	else {
		if (this->shield - val > 0) {
			this->shield = this->shield - val;
		}
		else this->shield = 0;
		return true;
	}
}
real_t Person::get_hp()
{
	return hp;
}
real_t Person::get_max_hp()
{
	return max_hp;
}
// ��������
void Person::die() { //������ ���������
	if (state != State::Death) {
		int temp = inventory->call("get_items", this);
		for (int i = 0; i < temp; i++) {
			real_t x_coord = GetRandomNumber(-1, 1) * 10 + this->get_position().x;
			real_t y_coord = GetRandomNumber(-1, 1) * 10 + this->get_position().y;
			Node* world = get_viewport()->get_node("Root/StartWorld");
			world->call("add_lying_item", i, x_coord, y_coord);
			state = State::Death;

		}
	}
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
real_t Person::clip(real_t n, real_t lower, real_t upper) {
	return std::max(lower, std::min(n, upper));
}