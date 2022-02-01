#pragma once
#include "Person.h"
#include <Vector2.hpp>
#include <Timer.hpp>
#include <RandomNumberGenerator.hpp>
#include <Node2D.hpp>
#include <Area2D.hpp>


namespace godot {
	class Troll : public Person {
		GODOT_CLASS(Troll, Person)
	private:
		Vector2 position;
	public:
		


		//����������, ��������� � ��������� ����
		bool stands = true;
		Vector2 destination = Vector2();
		Vector2 velocity;
		Vector2 prev_pos;
		
		int default_speed = 45;
		KinematicBody2D* target = NULL;

		//ref
		//Timer* BiteCooldown = null;
		//AnimationTree* animationTree = (AnimationTree*)get_node("AnimationTree");
		//AnimationNodeStateMachinePlayback* animationState = animationTree->get("Parametries/playback");
		//Area2D* BiteArea = (Area2D*)get_node("BiteArea");
		//KinematicBody2D* target = NULL;
		//TextureProgress* HP_bar = (TextureProgress*)get_node("HP_bar");

		//���������� ��� ����� ����
		bool target_intercepted = false; // ���� �� ����� - �� ���� � ���� ������������
		bool can_bite = true; // ����� �� ��������� ��������(�����������, ����� ��� ��������� �� ���)
		int bite_strength = 10; // ���� ����� ����(10 ������ �������� �� ����)

		//methods
		static void _register_methods();
		void _ready();
		//void _process(float delta);
		void _init();
		void _physics_process(float delta); //
		void move_state(float delta); //moving
		void attack_state(float delta); 
		void attack_animation_finished();
		void death_state(float delta);
		void death_animation_finished();

		void bite(KinematicBody2D* targ);
		void search_for_target();
		void cancel_movement();
	
		void wander();
		void set_destination(Vector2 dest);
		void set_start_hp(int hp, int max_hp);
		//signals
		//void _on_BiteArea_area_entered(Area2D area);
		//void _BiteArea_entered(Area2D area);
		//getting player 
		KinematicBody2D* get_player();

		Troll();
		~Troll();


	};
}