extends Resource

class_name AvatarFactory

static func create_avatar(data: Dictionary) -> Avatar:
	var avatar := Avatar.new()
	
	avatar.name = data["Name"]
	avatar.current_hp = data["HP"]
	
	
	return avatar

#static func create_avatar_data(data: Dictionary) -> AvatarData:
	#var avatar_data := AvatarData.new()
	#
	#return avatar_data
