#pragma once

#include <core/Godot.hpp>
#include <core/String.hpp>
#include <TextureRect.hpp>
#include <Input.hpp>
#include <Label.hpp>
#include <gen/KinematicBody2D.hpp>
#include "Item.h"

namespace godot {
	class Inventory : public Control {
		GODOT_CLASS(Inventory, Control)
	private:

	public:
		Inventory();
		~Inventory();

		Array items;
		KinematicBody2D* inventory_owner = NULL;
		void set_inv_owner(KinematicBody2D* val); 
		KinematicBody2D* get_inv_owner();

		//signal on_changed

		void add_item(Node2D* item);
		void remove_item(Node2D* link);

		Array get_items();
	};
	
}