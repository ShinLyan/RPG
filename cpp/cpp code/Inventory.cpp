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
	bool added = false;
	if (item->has_method("can_stack")) {
		// если предмет может стакаться
		if (item->call("can_stack")) {
			for (int i = 0; i < items.size(); i++) {
				if (items[i].call("get_item_name") == item->call("get_item_name")) {
					real_t tmp = item->call("get_amount");
					items[i].call("add_amount", tmp);
					added = true;
					item->queue_free();
					break;
				}
			}
		}
	}
	if (!(item->has_method("can_stack")) or !added) {
		items.push_back(item);
		item->call("set_inentory", this);
	}
	emit_signal("on_changed");
}

void godot::Inventory::remove_item(Node2D* link)
{
	Array items;
	items.erase(items.find(link));
	link->queue_free();
	emit_signal("on_changed");
}

Array godot::Inventory::get_items()
{
	return items;
}
