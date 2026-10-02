extends Resource

class_name Team
var name: String
var players: Array[Player]

func _init() -> void:
	for i in range(4):
		players.append(Player.new())
