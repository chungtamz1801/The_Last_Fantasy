extends TargetArea

class_name CircleArea
var radius: float

func _init(radius: float):
	self.radius = radius

func get_targets(
	position: Vector2
) -> Array[Player]:
	assert(false, "Subclass must override get_targets()")
	return []
