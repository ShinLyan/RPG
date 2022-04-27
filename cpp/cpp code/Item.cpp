#include "Item.h"
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <Sprite.hpp>
#include <Image.hpp>

void godot::Item::_input(InputEvent* event)
{
	Input* _input = Input::get_singleton();
	if (_input->is_action_pressed("e_click")) {
		KinematicBody2D* pl = get_player();
		ResourceLoader* loader;
		Ref<PackedScene> pre_inv = loader->get_singleton()->load("res://Scenes/InventItem.tscn");

	
		// проверяем рядом ли игрок
		if (abs(pl->get_position().x - this->get_position().x) < 30 and abs(pl->get_position().y - this->get_position().y) < 30) {

		}
		Control* new_item = (Control*)pre_inv->instance();
		new_item->call("set_item", item, amount, properties); // создаем новый объект

		bool is_picked = pl->call("pick", new_item); // просим игрока поднять предмет(случай когда нет места в инвентаре)
		if (is_picked) { // если предмет поднят
			get_parent()->remove_child(this); // удаляем предмет с карты(подбираем)
			queue_free();
		}
		else new_item->call("queue_free");
	}
}

void godot::Item::set_amount(real_t am)
{
	amount = am;
}

void godot::Item::set_item(Array props)
{
	ResourceLoader* loader = ResourceLoader::get_singleton();
	Sprite* Sprite;
	/*if (props.has("book")) {
		Ref<Texture> temp = loader->load("res://Sprites/Items/book.png");
		Sprite->set_texture(temp);
	}
	else if (props.has("coins")) {
		Ref<Image> temp = loader->load("res://Sprites/Items/coins.png");
		Sprite->set_texture(temp);
	}
	else if (props.has("hp_potion")) {
		Ref<Image> temp = loader->load("res://Sprites/Items/hp_potion.png");
		Sprite->set_texture(temp);
	}
	else if (props.has("scroll")) {
		Ref<Image> temp = loader->load("res://Sprites/Items/scroll.png");
		Sprite->set_texture(temp);
	}*/

	item = props[0];
	stack_limit = props[1];
	this->properties[item] = props[2];
}

godot::String godot::Item::get_item()
{
	return item;
}

real_t godot::Item::get_amount()
{
	return amount;
}

real_t godot::Item::get_item_stack()
{
	return stack_limit;
}


godot::KinematicBody2D* godot::Item::get_player()
{
	//проверяем существует ли узел игрока
	if (get_parent()->get_parent()->has_node("YSort/Hero")) {
		KinematicBody2D* Hero = (KinematicBody2D*)get_node_or_null("../YSort/Hero");
		return (Hero);
		//Godot::print_error();
	}
	else return NULL;
}
