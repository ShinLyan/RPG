extends KinematicBody2D

const SPEED = 350

var velocity = Vector2()

export var direction = Vector2()


var prev_pos = Vector2()


func _physics_process(delta):
	prev_pos = position
	var _collision_info = move_and_collide(velocity.normalized() * delta * SPEED)
	if velocity != Vector2():
		if position.distance_to(prev_pos) <= 0.6:
			if get_parent() != null:
				get_parent().remove_child(self)
				queue_free()


func _ready():
	$Timer.start(1) # область, которая будет существовать 2 сек


func _on_Timer_timeout():
	# удаление объекта области
	if get_parent().get_child(self.get_index()) != null:
		get_parent().remove_child(self)
		queue_free()
