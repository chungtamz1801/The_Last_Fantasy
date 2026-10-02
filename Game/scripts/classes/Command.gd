extends Resource

class_name Command
 
enum CommandType{
	ATTACK,
	MOVING,
	COUNTER,
	STATENESS
}

var targetArea: TargetArea
var targets: Array[Player]
var commandType: CommandType

func target()->void:
	if(targetArea == null):
		pass
	#Sua
	targets = targetArea.get_targets(Vector2(0,0))
	
func excute()->void:
	pass
