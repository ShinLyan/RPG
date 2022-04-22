#pragma once
#include "Person.h"
namespace godot {
	class Hero : public Person {
		GODOT_CLASS(Hero, KinematicBody2D)
	private:
		Vector2 position;
		State state = State::Move;
	public:
		// var
		bool can_fire = true;
		real_t rate_of_fire = 0.8;
		bool shooting = false;
		int selected_skill; //yb xthnf 
		real_t fire_direction;

		// Передвижение персонажа на клавиши
		Vector2 velocity = Vector2();
		Vector2 direction = Vector2();
		Input* i = NULL;

		int hp_regen = 1;
		int mp_regen = 2;

		AnimationTree* animationTree = NULL;
		AnimationNodeStateMachinePlayback* animationState = NULL;

		//signal on_death; а как какать

		State get_state();

		static void _register_methods();
		
		void _ready();
		void _process(float delta);
		void _physics_process(float delta);
		void _init();

		Vector2 get_direction();

		void _unhandled_input(InputEvent* event);

		//инвентарь
		void update_inventory();
		bool pick(int item);
		void drop_item(int link);

		void skill_loop();

		void HP_regen(float _delta);
		void MP_regen(float _delta);

		void move_state(float delta);

		//атака игрока
		void attack_state(float delta);
		void attack_animation_finished();
		void range_attack_state(float delta);

		void death_state(float delta);
		void death_animation_finished();


		void DateImport(String selected_skill);
	};
}