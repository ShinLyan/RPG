#pragma once
#include "AI.h"
#include <RandomNumberGenerator.hpp>
#include <Node2D.hpp>
#include <Area2D.hpp>
#include <vector>

namespace godot {
	class Troll : public AI {
		GODOT_CLASS(Troll, AI)
	private:
		Vector2 position;
		State state = State::Move;
	public:
		// ???
		bool target_in_range = false;
		bool target_in_sight = false;
		std::vector<KinematicBody2D*> sees_enemies;

		Vector2 target_position = Vector2();

		//methods
		State get_state();
		static void _register_methods();
		void _ready();
		void _process(float delta);
		void _init();
		void _physics_process(float delta); //
		void move_state(float delta); //moving
		void attack_state(float delta); 
		void attack_animation_finished();
		void death_state(float delta);
		void death_animation_finished();
		void search_for_target();

		void sight_check();

		//signals
		void _on_BiteArea_area_entered(Variant area);
		void _on_BiteArea_area_exited(Variant area);
		void _on_StandingTimer_timeout();
		void _on_BiteCooldown_timeout();
		void _on_Sight_body_exited(KinematicBody2D* body);
		void _on_Sight_body_entered(KinematicBody2D* body);
		
		//getting player 
		KinematicBody2D* get_player();

		Troll();
		~Troll();


	};
}