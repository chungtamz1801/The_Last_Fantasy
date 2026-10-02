extends Command

class_name NormalAttack

func _init(range: float):
	targetArea = CircleArea.new(range)
