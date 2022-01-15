extends Node


onready var entity_group = "ENTITY" # название группы всех сущностей
onready var troll_group = "TROLLS"

onready var saving_group = "TO_BE_SAVED"


onready var save_dir = "res://Saves" # адерс папки, где лежат сохранения
onready var save_temp = "%s.tres" # шаблон для сохранения
