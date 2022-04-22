#include "Inventory.h"
#include <vector>
#include "Item.h"

using namespace godot;

Inventory::~Inventory() {}

void godot::Inventory::set_inv_owner(KinematicBody2D* val)
{
	inventory_owner = val;
}

KinematicBody2D* godot::Inventory::get_inv_owner()
{
	return inventory_owner;
}

void godot::Inventory::add_item(Node2D* item)
{
	Dictianary 
	bool added = false;
	if (item->has_method("can_stack")) {
		// если предмет может стакаться
		if (item->call("can_stack")) {
			for (int i = 0; i < items.size(); i++) {
				items[i]
			}
		}
			
	}
}
