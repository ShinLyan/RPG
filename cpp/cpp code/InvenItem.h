#pragma once

#include <core/Godot.hpp>
#include <core/String.hpp>
#include <TextureRect.hpp>
#include <Input.hpp>
#include <Label.hpp>


using namespace std;


namespace godot {
	class InventItem : public TextureRect {
		GODOT_CLASS(InventItem, TextureRect)
	private:

	public:

		InventItem();
		~InventItem();

		Label Amount;

		void set_item(String item_name, Label &Amount);
	};
}

