#include <core/Godot.hpp>
#include <core/String.hpp>
#include <TextureRect.hpp>
#include <Input.hpp>
#include <Label.hpp>
#include <KinematicBody2D.hpp>
#include <gen/ResourceLoader.hpp>
#include <gen/PackedScene.hpp>
#include <Sprite.hpp>
#include <map>

namespace godot {
	class Item : public Node2D {
		GODOT_CLASS(Item, Node2D)
	private:

	public:

		std::map<String, Array> properties; // свойства предметов
		String item = ""; // название предмета
		real_t amount = 1; // количество предметов
		real_t stack_limit = 8; // предметы группируются связками по 8 штук


		void _input(InputEvent* event);

		// Сеттеры
		void set_amount(real_t am);
		void set_item(Array props); // генерируем предмет, задаём свойства
		
		// Геттеры
		String get_item();
		real_t get_amount();
		real_t get_item_stack();
		//getting player 
		KinematicBody2D* get_player();
	};
}