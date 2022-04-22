#pragma once
#include "AI.h"
namespace godot {
	class Heroine : public AI {
		GODOT_CLASS(Heroine, KinematicBody2D)
	private:
		State state = State::Move;
	public:
		// var
			// ???

		bool can_heal = true;
		bool target_in_range = false;
		bool target_in_sight = false;
		Vector2 target_position = Vector2();

		bool can_fire = true;
		real_t fire_direction;			

			//Переменные, связанные с движением моба
		bool stands = true;
		Vector2 destination = Vector2();
		Vector2 velocity = Vector2();
		Vector2 prev_pos;

		real_t default_speed = 45;
		KinematicBody2D* target = NULL;

			//Переменные для атаки моба
		bool target_intercepted = false; // есть ли какая - то цель в зоне досягаемости
		bool can_bite = true; // можно ли атаковать существо(перезарядка, чтобы моб постоянно не бил)
		int bite_strength = 10; // сила удара моба(10 единиц здоровья за удар)

		//methods
		State get_state();
		static void _register_methods();
		void _ready();
		void _process(float delta);
		void _init();
		void _physics_process(float delta); //
		void attack_state(float delta);
		void attack_animation_finished();
		void combat_state(float _delta);
		void non_combat_state(float _delta);

		void sight_check();
		void self_heal();
		void sel_heal_at();

		void set_can_fire();
		void set_can_heal();
			
		//signals
		void _on_StandingTimer_timeout();
		void _on_BiteCooldown_timeout();
		void _on_Sight_body_entered(KinematicBody2D* body);
		void _on_Sight_body_exited(KinematicBody2D* body);
		
		Heroine();
		~Heroine();
		};
	}