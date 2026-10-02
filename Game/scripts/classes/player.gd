extends Node

func save():
	var save_dict = {
		"filename": get_scene_file_path(),
		"parent": get_parent().get_path(),
 		"player_name": "Makoto",
		"player_avatar": "Eldlich",
		"max_hp":0,
		"current_hp":0,
		"atk":0,
		"def":0,
		"mag":0,
		"res":0,
		"spd":0,
		"mp":0,
		"mrg":0,
		"normal_attack":"",
		"skills": ["dark_flame","golden_castle"],
		"status": [],
		"pos_x": 500,
		"pos_y": 500
	}
	
	return save_dict
