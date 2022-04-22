#pragma once
#include <cstdlib>
#include <time.h>

#include <core/Godot.hpp>
#include <KinematicBody2D.hpp>
#include <Input.hpp>
#include <TextureProgress.hpp>
#include <AnimationTree.hpp>
#include <AnimationNodeStateMachinePlayback.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <Control.hpp>

namespace godot{
	class Person : public KinematicBody2D {
		GODOT_CLASS(Person, KinematicBody2D)
	private:
		
	public:

		real_t hp = 50;
		real_t max_hp = 100;
		real_t speed = 100;
		real_t shield = 0; // текущее значение щита
		real_t max_shield = 50; // максимальное значение щита
		real_t mp; // текущее значение mp
		real_t max_mp; // максимальное значение mp
		real_t bite_strength; // сила удара персонажа
		

		enum class State { 
			Combat,
			Non_combat,
			Regrouping,
			Death,

			Move,
			Attack,
			Range
		};
		State state = State::Move;
		
		TextureProgress* HP_bar = NULL;//добавить связь
		AnimationTree* animationTree = NULL;
		AnimationNodeStateMachinePlayback* animationState = NULL;
		Control* inventory = nullptr;

		
		static void _register_methods();
		void _ready();
		void _init();
		void _process(float delta);

		Person();
		~Person();

		void create_inventory();
		bool pick(int item);
		
		void set_start_hp(real_t hp, real_t max_hp);
		void increase_hp(real_t val);
		bool reduce_hp(real_t val);
		real_t get_hp();
		real_t get_max_hp();
		void die();
		void magic_shield(real_t val);

		//s
		int GetRandomNumber(int min, int max);
		real_t clip(real_t n, real_t lower, real_t upper);
	};
}

