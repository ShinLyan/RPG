#pragma once
//решить вопрос с инвентарем


#include <core/Godot.hpp>
#include <core/String.hpp>
#include <TextureRect.hpp>
#include <Input.hpp>
#include <Label.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <string>
using namespace std;


namespace godot {
	class InventItem : public Control {
		GODOT_CLASS(InventItem, Control)
	private:

	public:

		InventItem();
		~InventItem();
		String item_name = "";
		real_t item_amount = 0;
		Dictionary properties;
		Control* inventory = NULL;
		//signal on_picked;

		void remove_amount(real_t am);
		void pick();
		void unpick();
		void use();
		void set_inventory(int val);
		void set_item(String Item_name, real_t amount, Dictionary props);
		void add_amount(real_t am);

		//геттеры
		String get_item_name();
		real_t get_amount();
		Dictionary get_props();

		//сеттеры

		void _on_InventItem_gui_input(InputEvent* event);

		Dictionary duplicate(bool p_deep) const;
	};
}

