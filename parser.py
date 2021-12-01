import requests
import json

bad_cells = {}
hash_to_map = {}


def parse_game(link):
	res = requests.get(url=link).json()
	return res


def get_map(res):
	return res["initial_state"]["walls"]


def get_hash(res):
	dict = {}
	h1 = 0
	h2 = 0
	h3 = 0
	for wall in res:
		x = wall['y']
		y = wall['x']

		if x < 4:
			h1 += 2 ** (x * 13 + y)
		elif x < 8:
			h2 += 2 ** ((x - 4) * 13 + y)
		else:
			h3 += 2 ** ((x - 8) * 13 + y)

	# print(h1, h2, h3)
	return (h1, h2, h3)


def update_bad_cells(res):
	cur_hash = get_hash(get_map(res))

	if cur_hash not in hash_to_map:
		hash_to_map[cur_hash] = get_map(res)
		bad_cells[cur_hash] = {}

	for frames in res["frames"]:
		for u in frames["m"]:
			if u["n"] == "m1" or u["n"] == "m2":
				x = u['y']
				y = u['x']
				bad_cells[cur_hash][(x, y)] = 1


def print_json_game(id):
	link = f"https://s3.alem.school/storage/gamesessions/{id}.json"
	res = parse_game(link)
	with open("debug.json", "w") as out:
		out.write(json.dumps(res, indent=4))


def parse_page(page):
	team_id = 111
	url = f"https://cup.alem.school/api/arena/last_games/{team_id}?skip={(page - 1) * 5}&limit=10"

	payload = {}
	headers = {
		'Host': 'cup.alem.school',
		'Connection': 'keep-alive',
		'sec-ch-ua': '"Google Chrome";v="95", "Chromium";v="95", ";Not A Brand";v="99"',
		'Authorization': 'Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJ6aGFuLmt1cmJhbmJheWV2QGdtYWlsLmNvbSIsInVzZXJuYW1lIjoicnRta3oiLCJpZCI6MTMzLCJleHAiOjE2NDgzOTIxNjR9.fXTtHZukVhX0Cebj3WQ6G2nXvdQn37BZ104Mdfir0tk',
		'sec-ch-ua-mobile': '?0',
		'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.69 Safari/537.36',
		'sec-ch-ua-platform': '"macOS"',
		'Accept': '*/*',
		'Sec-Fetch-Site': 'same-origin',
		'Sec-Fetch-Mode': 'cors',
		'Sec-Fetch-Dest': 'empty',
		'Referer': 'https://c...content-available-to-author-only...m.school/main/profile/383',
		'Accept-Encoding': 'gzip, deflate, br',
		'Accept-Language': 'en-US,en;q=0.9,ru;q=0.8',
		'Cookie': '_ym_uid=1635431987715230997; _ym_d=1635431987'
	}

	response = requests.request("GET", url, headers=headers)

	next_page = 0
	for game in response.json():
		id = game['game_session_id']

		link = f"https://s3.alem.school/storage/gamesessions/{id}.json"
		res = parse_game(link)

		update_bad_cells(res)

		next_page = 1
	return next_page


# link = "https://s...content-available-to-author-only...m.school/storage/gamesessions/c609d882-4627-475d-9141-7f85ebaf2117.json"
# res = update_bad_cells(parse_game(link))

print_json_game("32878d7e-0f8b-4f42-9689-0b57fd088e52")
exit()

page = 1
while True:
	print (f"Processing page {page}")
	if parse_page(page) == 0:
		break
	print (f"Processed page {page}")
	page = page + 1

	for key, val in bad_cells.items():
		print(key)
		for i in range(0, 11):
			for j in range(0, 13):
				if (i, j) in val:
					print(0, end="")
				else:
					print(1, end="")
			print()
	print()
