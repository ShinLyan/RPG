#pragma once
#include <cstdlib>
#include <time.h>
//#include <algorithm>

#include <core/Godot.hpp>
#include <KinematicBody2D.hpp>
#include <Input.hpp>
#include <TextureProgress.hpp>
#include <AnimationPlayer.hpp>
#include <AnimationTree.hpp>
#include <AnimationNodeStateMachinePlayback.hpp>
#include <ResourceLoader.hpp>

namespace godot{
	class Person : public KinematicBody2D {
		GODOT_CLASS(Person, KinematicBody2D)
	private:

	public:
		enum class State { Move, Attack, Death, Range };
		
		real_t hp = 50;
		real_t max_hp = 100;
		real_t speed = 100;
		//ref
		AnimationPlayer* animationPlayer = NULL;
		AnimationTree* animationTree = NULL;
		AnimationTree* animationState = NULL;
		
		TextureProgress* HP_bar = (TextureProgress*)get_node("HP_bar");//добавить связь

		State state;
		static void _register_methods();
		//void _ready();
		//virtual void _init();
		//void _process(float delta);

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

