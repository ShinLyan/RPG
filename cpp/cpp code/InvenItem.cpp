#include "InvenItem.h"
#include <KinematicBody2D.hpp>

using namespace godot;


InventItem::~InventItem() {}

void godot::InventItem::remove_amount(real_t am)
{
	item_amount -= am;
	Label* amount = (Label*)get_node("Box/Amount");
	amount->set_text(std::to_string(item_amount).std::string::c_str());
	if (item_amount == 0) {
		//а где а кто
		Label* amount = (Label*)get_node("Box/Amount");
		inventory->call("remove_item", this);
	}
}

void godot::InventItem::pick()
{
	TextureRect* border = (TextureRect*)get_node("Border");
	border->show();
}

void godot::InventItem::unpick()
{
	TextureRect* border = (TextureRect*)get_node("Border");
	border->hide();
}

void godot::InventItem::use()
{
	KinematicBody2D* target = inventory->call("get_inv_owner");
	if (properties.has("function")) {
		String temp = properties["function"];
		if (temp == "heal") {
			target->call("increase_hp", properties["heal_val"]);
			if (properties.has("expandable")) {
				remove_amount(properties["exp_rate"]);
			}
		}
	}
}

void godot::InventItem::set_inventory(int val)
{//че
	//inventory  = val;
}

void godot::InventItem::set_item(String Item_name, real_t amount, Dictionary props)
{
	this->item_name = Item_name;
	this->item_amount = amount;
	this->properties = props;
}

void godot::InventItem::add_amount(real_t am)
{
	item_amount += am;
	Label* amount = (Label*)get_node("Box/Amount");
	amount->set_text(std::to_string(item_amount).std::string::c_str());
}

String godot::InventItem::get_item_name()
{
	return item_name;
}

real_t godot::InventItem::get_amount()
{
	return item_amount;
}

Dictionary godot::InventItem::get_props()
{
	return properties;
}

void godot::InventItem::_on_InventItem_gui_input(InputEvent* event)
{
	Input* _input = Input::get_singleton();
	if (_input->is_action_pressed("left_click")) {
		emit_signal("on_picked");
	}
}


Dictionary godot::InventItem::duplicate(bool p_deep) const {
	Dictionary n;



	return n;
}