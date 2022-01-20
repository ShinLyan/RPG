extends KinematicBody2D

const SPEED = 350

var velocity = Vector2()

export var direction = Vector2()





func _physics_process(delta):
	var collision_info = move_and_collide(velocity.normalized() * delta * SPEED)


func _ready():
	$Timer.start(2) # область, которая будет существовать 2 сек


func _on_Timer_timeout():
	# удаление объекта области
	if get_parent() != null:
		get_parent().remove_child(self)
		queue_free()
