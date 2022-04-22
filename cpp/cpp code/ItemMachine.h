#pragma once

#include "core/Godot.hpp"
#include "Node.hpp"
#include <Control.hpp>

namespace godot {
	class ItemMachine : public Node {
		GODOT_CLASS(ItemMachine, Node)
	private:

	public:
		Dictionary items;
		// словарь предметов с названиями и связками
		// ключ предмет, количество предметов в связке, может стакаться или нет
		// ключ function означает, что у предмета есть способность
		// expandable - тратиться ли предмет при использовании
		// exp_rate - в каком кол - ве тратяться предметы
		items = ["book", 8, { "can_stack": true }];
		items = { "book" };
	"scroll" : ["scroll", 8, {"can_stack": true}] ,
	"hp_potion" : ["hp_potion", 8, {
		"can_stack": true,
		"function" : "heal",
		"heal_val" : 10,
		"expandable" : true, "exp_rate" : 1}
	] ,
	"coins": ["coins", 100, {"can_stack": true}]
		}

		String has_item(String item_name); // функция, проверяющая есть ли предмет в базе данных
		int generate_item(String item_name, real_t item_amount);
		Control* generate_inventory_item(String item_name, real_t item_amount);
		Dictionary  get_item_properties(String item_name);
		Array get_openworld_items(); // предметы, которые могут спавниться на карте
	};
}

# Инвентарь и предметы
onready var item = preload("res://Scenes/Item.tscn")
onready var inv_item = preload("res://Scenes/InventItem.tscn")


var items = { # словарь предметов с названиями и связками
	# ключ предмет, количество предметов в связке, может стакаться или нет
	# ключ function означает, что у предмета есть способность
	# expandable - тратиться ли предмет при использовании
	# exp_rate - в каком кол - ве тратяться предметы
	"book": ["book", 8, {"can_stack": true}] ,
	"scroll" : ["scroll", 8, {"can_stack": true}] ,
	"hp_potion" : ["hp_potion", 8, {
		"can_stack": true,
		"function" : "heal",
		"heal_val" : 10,
		"expandable" : true, "exp_rate" : 1}
	] ,
	"coins": ["coins", 100, {"can_stack": true}]
}


func has_item(item_name) : # функция, проверяющая есть ли предмет в базе данных
return item_name in items.keys()


func generate_item(item_name, item_amount = 1) :
	if has_item(item_name) :
		var new_item = item.instance()
		new_item.set_item(items[item_name])
		new_item.set_amount(item_amount)
		return new_item
		return null


		func generate_inventory_item(item_name, item_amount = 1) :
		if has_item(item_name) :
			var new_item = inv_item.instance()
			new_item.set_item(item_name, item_amount, items[item_name][2])
			return new_item
			return null


			func get_item_properties(item_name) :
			if has_item(item_name) :
				return items[item_name]
				return[]


				func get_openworld_items() : # предметы, которые могут спавниться на карте
				return["book", "scroll", "hp_potion"]
