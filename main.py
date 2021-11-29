import requests
import json
import time
import sys

bad_cells = {}
hash_to_map = {}

id_to_hash = []
hash_to_id = {}
win_from_map = []
diff_from_map = []
cnt_maps = 0


def parse_game(link):
	res = requests.get(url=link).json()
	return res


def get_map(res):
	return res["initial_state"]["walls"]


def init_maps():
	global id_to_hash
	global hash_to_id
	global cnt_maps
	cnt_maps += 1
	id_to_hash.append((1412887495131136, 1412876746391616, 21053664))
	cnt_maps += 1
	id_to_hash.append((141877870592, 148319537314, 9766980))
	cnt_maps += 1
	id_to_hash.append((35257659752448, 35253387052036, 4785220))
	cnt_maps += 1
	id_to_hash.append((565405475176448, 565153273479232, 8425198))
	cnt_maps += 1
	id_to_hash.append((245753184256, 15063171296, 527950))
	cnt_maps += 1
	id_to_hash.append((79730114560, 79742649508, 4785316))
	cnt_maps += 1
	id_to_hash.append((149671164805120, 149675468539970, 10258434))
	cnt_maps += 1
	id_to_hash.append((221601890304, 73293136964, 8424678))
	cnt_maps += 1
	id_to_hash.append((321057404256256, 321194978740226, 8945664))
	cnt_maps += 1
	id_to_hash.append((565198922465280, 565159714619552, 16794344))
	for i in range(0, cnt_maps):
		hash_to_id[id_to_hash[i]] = i
		win_from_map.append({"first": 0, "second": 0})
		diff_from_map.append({"scores": []})


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


win1 = 0
win2 = 0


def get_score():
	global win1
	global win2
	headers = {
		'Host': 'cup.alem.school',
		'Connection': 'keep-alive',
		'sec-ch-ua': '"Google Chrome";v="95", "Chromium";v="95", ";Not A Brand";v="99"',
		'Authorization': 'Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJiZWtqYW4yOThAbWFpbC5ydSIsInVzZXJuYW1lIjoiYmVremhhbjI5IiwiaWQiOjM4MiwiZXhwIjoxNjQ4OTAyMzE3fQ.lTBsDO9Q7JtcpNnTK2UlAxtPecqnlkNhQe0r82GCXVY',
		'sec-ch-ua-mobile': '?0',
		'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.69 Safari/537.36',
		'sec-ch-ua-platform': '"macOS"',
		'Accept': '*/*',
		'Sec-Fetch-Site': 'same-origin',
		'Sec-Fetch-Mode': 'cors',
		'Sec-Fetch-Dest': 'empty',
		'Referer': 'https://cup.alem.school/main/profile/383',
		'Accept-Encoding': 'gzip, deflate, br',
		'Accept-Language': 'en-US,en;q=0.9,ru;q=0.8',
		'Cookie': '_ym_uid=1635431987715230997; _ym_d=1635431987'
	}

	url = 'https://cup.alem.school/api/ondemand/send_solution'
	file = open("main.cpp", "r").read()

	response = requests.post(url, json={"code": file, "lang": "cpp", "mode": "SELF"}, headers=headers)
	key = response.json()

	# key = "636548a9-97e7-4c15-9097-539340ae3a9d"
	log_url = f'https://cup.alem.school/api/game/logs/{key}'
	while True:
		res = requests.get(log_url, headers=headers).json()
		print(res['finished'], file=sys.stderr, flush=True)
		if res['finished'] is True:
			break
		time.sleep(1)

	logs_url = f"https://s3.alem.school/storage/gamesessions/{key}.json"
	res = requests.get(logs_url)
	# with open("answer.json", "w") as out:
	# 	out.write(str(json.dumps(res.json(), indent=4)))
	score = {"p1": 0, "p2": 0}
	if res.status_code == 200:
		frames = res.json()['frames']

		hash = get_hash(get_map(res.json()))
		map = hash_to_id[hash]
		last_frame = 0
		for frame in frames:
			last_frame = max(last_frame, frame["f"])
		frames = list(filter(lambda x: x['d'], frames))
		for frame in frames:
			if frame['d']:
				for result in frame['d']:
					if result['n'] == '#':
						for action in result['p']:
							score[action['n']] = max(score[action['n']], action['c'])
		if score['p1'] > score['p2']:
			win_from_map[map]['first'] += 1
			win1 += 1
		elif score['p2'] > score['p1']:
			win_from_map[map]['second'] += 1
			win2 += 1
		else:
			win_from_map[map]['first'] += 0.5
			win_from_map[map]['second'] += 0.5
			win1 += 0.5
			win2 += 0.5
		diff_from_map[map]['scores'].append(score['p1'] - score['p2'])
		print(f"Coins: {score['p1']} : {score['p2']}", file=sys.stderr, flush=True)
		print(f"Wins: {win1} : {win2}", file=sys.stderr, flush=True)
		print(f"Ticks: {last_frame}", file=sys.stderr, flush=True)


init_maps()
for i in range(10000):
	try:
		get_score()
	except:
		time.sleep(1)
	print(i, file=sys.stderr, flush=True)
	print(json.dumps(win_from_map, indent=4), flush=True)
	if i % 10 == 0:
		print(json.dumps(diff_from_map, indent=4), flush=True)
