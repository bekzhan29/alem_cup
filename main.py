import requests

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

win1 = 0
win2 = 0

def get_score():
	global win1
	global win2
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

	url = 'https://cup.alem.school/api/ondemand/send_solution'
	file = open("main.cpp", "r").read()

	response = requests.post(url, json={"code": file, "lang": "cpp", "mode": "SELF"}, headers = headers)
	key = response.json()

	# key = "636548a9-97e7-4c15-9097-539340ae3a9d"
	log_url = f'https://cup.alem.school/api/game/logs/{key}'
	import time
	while True:
		res =requests.get(log_url, headers=headers)
		print(res.json())
		if res.json()['finished'] == True:
			break
		time.sleep(1)

	logs_url = f"https://s3.alem.school/storage/gamesessions/{key}.json"
	res = requests.get(logs_url)
	score = {"p1": 0, "p2" : 0}
	if res.status_code == 200:
		frames = res.json()['frames']
		frames = list(filter(lambda x: x['d'], frames))
		for frame in frames:
			if frame['d']:
				for result in frame['d']:
					if result['n'] == '#':
						for action in result['p']:
							score[action['n']] = max(score[action['n']], action['c'])
		if score['p1'] > score['p2']:
			win1 += 1
		elif score['p2'] > score['p1']:
			win2 += 1
		else:
			win1 += 0.5
			win2 += 0.5
		print(f"{score['p1']}: {score['p2']}")

for i in range(100):
	get_score()
print(win1, win2)