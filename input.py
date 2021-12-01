import requests

player_id = 2

a = [["." for j in range(13)] for i in range(11)]

player_param = [[0, 0], [0, 0]]
param_2 = ["bonus", "frost", "immunity"]

players = [[0, 0], [0, 0]]
monsters = [[0, 0], [0, 0]]
is_dead_monster = [0, 0]
is_dead_player = [0, 0]

id = "c8af0d84-529a-4928-b3d6-e77af7e769d2"
link = f"https://s3.alem.school/storage/gamesessions/{id}.json"
# link = "https://s3.alem.school/storage/gamesessions/c59c0829-d1d7-4216-af78-5317d7e9e26c.json"
# link = "https://s3.alem.school/storage/gamesessions/0db03ef8-5108-40e6-8e6d-ab6bec6a9472.json"

def get_json(link):
	res = requests.get(url = link).json()	
	return res

def init_map(json):
	global a
	global players
	global monsters
	
	for coin in json['initial_state']['coins']:
		x = coin['x']
		y = coin['y']

		a[y][x] = '#'

	for wall in json['initial_state']['walls']:
		x = wall['x']
		y = wall['y']

		a[y][x] = '!'
	
	for player in json['initial_state']['players']:
		x = player['x']
		y = player['y']
		id = int(player['n'][1]) - 1

		players[id] = [x, y]

	for monster in json['initial_state']['monsters']:
		x = monster['x']
		y = monster['y']
		id = int(monster['n'][1]) - 1

		monsters[id] = [x, y]

def delete_disappeared(disappeared):
	global a

	for update in disappeared:
		name = update['n']
		x = update['y']
		y = update['x']

		a[x][y] = '.'

def add_features(features):
	global a

	for update in features:
		name = update['n']
		x = update['y']
		y = update['x']

		if name == "bonus":
			a[x][y] = 'b'
		elif name == "dagger":
			a[x][y] = 'd'
		elif name == "frost":
			a[x][y] = 'f'
		elif name == "immunity":
			a[x][y] = 'i'

def process_features(disappeared):
	global player_param

	for event in disappeared:
		name = event['n']
		if name == '#':
			continue

		# event is for a player
		if 'p' in event and len(event['p']) > 0 and 'c' in event['p'][0]:
			player_id = 0
			
			# print(name, event['p'])

			if event['p'][0]['n'] == "p2":
				player_id = 1

			if name == "dagger":
				player_param[player_id][0] = event['p'][0]['c']
			elif name in param_2:
				param_id = param_2.index(name) + 1
				player_param[player_id][1] = event['p'][0]['c'] * param_id
			else:
				# print(name)
				continue

def check_if_someone_died(disappeared):
	global is_dead_monster
	global is_dead_player

	for event in disappeared:
		name = event['n']
		if name[0] == 'm':
			id = int(name[1]) - 1
			is_dead_monster[id] = 1
		elif name[0] == 'p':
			id = int(name[1]) - 1
			is_dead_player[id] = 1

def process_moves(moves):
	global players
	global monsters

	for move in moves:
		id = int(move['n'][1]) - 1
		x = move['x']
		y = move['y']

		if move['n'][0] == 'p':
			players[id] = [x, y]
		else:
			monsters[id] = [x, y]


def print_input(tick):
	print(13, 11, player_id, tick)
	for i in range(11):
		for j in range(13):
			print(a[i][j], end = '')
		print()
	
	total = 4 - is_dead_monster[0] - is_dead_monster[1] - is_dead_player[0] - is_dead_player[1]
	print(total)

	for i in range(2):
		if is_dead_player[i] == 0:
			print(f"p {i + 1}", players[i][0], players[i][1], player_param[i][0], player_param[i][1])

	for i in range(2):
		if is_dead_monster[i] == 0:
			print(f"m {i + 1}", monsters[i][0], monsters[i][1], 0, 0)

def get_input(json):
	global a

	ind = 0
	for frame in json['frames']:
		tick = frame['f']

		disappeared = frame['d'] 
		features = frame['ft']
		
		# process moves
		if ind + 1 < len(json['frames']):
			moves = json['frames'][ind + 1]['m']
			process_moves(moves)

		# map was reset
		if 'c' in frame:
			init_map(json)

		# add features to the map
		add_features(features)
		

		#TODO: input
		print_input(tick)

		# check if someone just died
		check_if_someone_died(disappeared)

		# delete items from the map
		delete_disappeared(disappeared)

		# process features for players
		process_features(disappeared)

		# print(tick)
		# print("Players", players)
		# print("Monsters", monsters)

		ind += 1
		

def print_map():
	for i in range(11):
		print(a[i])

json = get_json(link)	

init_map(json)

get_input(json)
