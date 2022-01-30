#include "Person.h"
namespace godot {
	class Hero : public Person {
		GODOT_CLASS(Hero, KinematicBody2D)
	private:
		Vector2 position;
	public:
		//var
		Vector2 velocity = Vector2();
		//Передвижение персонажа на клавиши
		Vector2 direction = Vector2();
		AnimationTree * animationTree = (AnimationTree*)get_node("AnimationTree");
		AnimationNodeStateMachinePlayback* animationState = animationTree->get("Parametries/playback");
		Input* i = Input::get_singleton();
		//Смерть игрока
		//signal on_death; // сигнал смерти персонажа

		//void
		//void pick(item);
		//void _unhandled_input(event);
		void range_attack_state(float delta);
		void _ready();
		void _physics_process(float delta);

		Vector2 get_direction();

		void move_state(float delta);

		void attack_state(float delta);
		void attack_animation_finished();
		void death_state(float delta);
		void death_animation_finished();
	};
}



