#include "InvenItem.h"

using namespace godot;


InventItem::~InventItem() {}


void set_item(String item_name, Label &Amount) {
	texture = load("res://Sprites/Items/%s.png" % item_name);
	Amount.text = str(amount);
}
