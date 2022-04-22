#pragma once
#include "Person.h"
#include <Vector2.hpp>
#include <Tween.hpp>
#include <Timer.hpp>
#include <vector>

namespace godot {
	class AI : public Person {
		GODOT_CLASS(AI, Person)
	private:

	public:
		enum class State20 {
			Pursuit,
			Hit
		};

		//����������, ��������� � ��������� ����
		bool stands = true;
		Vector2 destination = Vector2();
		Vector2 velocity = Vector2();
		Vector2 prev_pos;
		real_t default_speed = 45;
		KinematicBody2D* target = NULL;

		//���������� ��� ����� ����
		bool target_intercepted = false; // ���� �� ����� - �� ���� � ���� ������������
		bool can_bite = true; // ����� �� ��������� ��������(�����������, ����� ��� ��������� �� ���)
		real_t heal_strength;
		//int bite_strength = 10; // ���� ����� ����(10 ������ �������� �� ����)
		std::vector<KinematicBody2D*> sees_enemies;

		void set_start_hp(real_t hp, real_t max_hp);
		void update_hp();
		void increase_hp(real_t val);
		void toggle_hp_bar();
		bool reduce_hp(real_t val);

		void wander();
		void set_destination(Vector2 dest);
		void cancel_movement();

		void bite(KinematicBody2D* targ);
		void heal(KinematicBody2D* targ);

		void non_combat_state(float _delta);
		void combat_state(float _delta);
		void regrouping_state(float _delta);

		void death_state(float delta);
		void death_animation_finished();

		AI();
		~AI();
	};
}