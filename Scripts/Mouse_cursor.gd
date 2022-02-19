extends Sprite


func _ready():
	Input.set_mouse_mode(Input.MOUSE_MODE_HIDDEN)


func _process(_delta):
	self.position = self.get_global_mouse_position()
