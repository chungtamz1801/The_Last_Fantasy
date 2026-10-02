extends Node



class_name BattleFieldManager
var players: Array[Player]
static var turn_manager := TurnManager.new()

func _ready() -> void:
	pass
	#SaveSystem.data_readed.connect(_on_data_readed)
	
#func _on_data_readed(data_readed:Variant):
	
