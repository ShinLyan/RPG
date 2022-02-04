#pragma once
#include <cstdlib>
#include <time.h>

#include <core/Godot.hpp>
#include <KinematicBody2D.hpp>
#include <Input.hpp>
#include <TextureProgress.hpp>
#include <AnimationTree.hpp>
#include <AnimationNodeStateMachinePlayback.hpp>

namespace godot{
	class Person : public KinematicBody2D {
		GODOT_CLASS(Person, KinematicBody2D)
	private:

	public:

		real_t hp = 50;
		real_t max_hp = 100;
		real_t speed = 100;

		enum class State { Move, Attack, Death, Range };
		
		TextureProgress* HP_bar = NULL;//добавить связь
		AnimationTree* animationTree = NULL;
		AnimationNodeStateMachinePlayback* animationState = NULL;

		State state = State::Move;
		static void _register_methods();
		//void _ready();
		void _init();
		void _process(float delta);

		Person();
		~Person();

		void set_start_hp(real_t hp, real_t max_hp);
		void update_hp();
		void toggle_hp_bar();
		bool reduce_hp(real_t val);
		void die();

		//s
		int GetRandomNumber(int min, int max);
	};
}

