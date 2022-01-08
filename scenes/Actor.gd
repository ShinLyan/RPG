extends KinematicBody2D
class_name Actor

export (int) var speed: int = 100

var direction: Vector2 = Vector2.ZERO
onready var animationTree: AnimationTree = $AnimationTree
onready var stateMachine = $AnimationTree.get("parameters/playback")

func _physics_process(delta: float) -> void:
	direction = get_direction()
	
	if direction != Vector2.ZERO:
		animationTree.set("parameters/Idle/blend_position", direction)
		animationTree.set("parameters/Run/blend_position", direction)
		stateMachine.travel("Run")
	else:
		stateMachine.travel("Idle")
	
	
	move_and_slide(direction * speed)
	
func get_direction() -> Vector2:
	return (Vector2(
		Input.get_action_strength("ui_right") - Input.get_action_strength("ui_left"),
		Input.get_action_strength("ui_down") - Input.get_action_strength("ui_up")
	)).normalized()
