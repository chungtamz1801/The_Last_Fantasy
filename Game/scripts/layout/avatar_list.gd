extends ScrollContainer

var avatar_data: Array
@onready var avatar_selection = get_tree().current_scene
@onready var avatar_name_label := $"../AvatarInfo/AvatarInfo/AvatarName"
@onready var avatar_image = $"../AvatarInfo/AvatarImage"
@onready var avatar_left_stat_label = $"../AvatarInfo/AvatarInfo/AvatarStat/LeftStat"
@onready var avatar_right_stat_label = $"../AvatarInfo/AvatarInfo/AvatarStat/RightStat"

func _ready() -> void:
	var avatar_list_container = $AvatarList
	#load_avatars()
	read_avatar_data("res://data/avatars.json")
	avatar_selection.avatar_changed.connect(_on_player_selected)
	
	
	
	for data in avatar_data:
		if not data is Dictionary:
			continue
		
		var new_button = Button.new()
		
		new_button.alignment = HORIZONTAL_ALIGNMENT_LEFT
		new_button.text = data["Name"]
		new_button.custom_minimum_size.y = 50
		new_button.set_anchors_preset(Control.PRESET_TOP_WIDE)
		new_button.pressed.connect(_on_button_pressed.bind(data))
		var style_box:StyleBox = new_button.get_theme_stylebox("normal").duplicate()
		style_box.content_margin_left = 15
		new_button.add_theme_stylebox_override("normal",style_box)
		
		avatar_list_container.add_child(new_button)


func _on_button_pressed(data: Dictionary) -> void:
	avatar_selection.set_avatar(data["Name"])

	
func read_avatar_data(path:String):
	var file := FileAccess.open(path, FileAccess.READ)
	
	if file == null:
		push_error("Cannot open file: "+path)
		
	var json = JSON.parse_string(file.get_as_text())
	
	if json == null:
		push_error("Invalid JSON: "+path)
		
	if not json is Array:
		push_error("Pokemon data must be an array: "+path)
	avatar_data = json
	
func _on_player_selected(avatar_id: String):
	for data in avatar_data:
		if data["Name"] == avatar_id:
			set_avatar_UI(data)
			break
		else:
			clear_avatar_UI()
	
func set_avatar_UI(data: Dictionary):
	avatar_name_label.text = data["Name"]
	avatar_image.texture = load("res://sprites/avatars/%s.png" % data["Name"])
	avatar_left_stat_label.text = """
HP: %d \n
ATK: %d \n
DEF: %d \n
MAG: %d \n
RES: %d \n""" % [
		data["HP"],
		data["ATK"],
		data["DEF"],
		data["MAG"],
		data["RES"]
	]
	
	avatar_right_stat_label.text = """
MP: %d \n
SPD: %d \n
CRIT: %d \n
MRG: %d \n
N.Attack: %s \n""" % [
		data["MP"],
		data["SPD"],
		data["CRIT"],
		data["MRG"],
		data["Normal Attack"]
	]
	
func clear_avatar_UI():
	avatar_name_label.text = ""
	avatar_image.texture = null
	avatar_left_stat_label.text = ""
	avatar_right_stat_label.text = ""
	
