# Eto lyam ewe lyam sverhu

This is our team's code for [alem cup](https://cup.alem.school/) where we won 3rd place. Alem cup is AI programming competition.

This year game's goal was to collect more coins than your opponent. [Example](https://cup.alem.school/main/playback/4d709248-f7e9-487e-9d3e-0c7ab1da95e4)

[Results](https://cup.alem.school/main/leaderboard)

## main.py

Этот скрипт тестирует решение `main.cpp` против себя на сайте alem.cup.

Чтобы проверить разные стратегии нужно прописать разный код для `player_id = 1` и для `player_id = 2`.

Для запуска скрипта используйте команду снизу и результаты запишутся в `main.txt`.

```
python3 main.py > main.txt
```

## parser.py

Этот скрипт парсит все игры определенной команды. Из всех игр команды смотрит на каких клетках были монстры и помечает их опасными.

Чтобы спарсить игры определенной команды нужно поменять в скрипте параметр `team_id`.

Для запуска скрипта используйте команду снизу и ответ запишется в `parser.txt`.

```
python3 parser.py > parser.txt
```

## analyze.cpp

Этот скрипт берет маску безопасных клеток из файла `analyze.txt` и добавляет в уже существующую маску.

Перед запуском этого скрипта нужно получить ответ от `parser.py` и скопировать последние 10 карт из файла `parser.txt` в `analyze.txt`

Для запуска скрипта используйте команду снизу и ответ запишется в стандартный вывод `stdout`.

```
g++ analyze.cpp
./a.out
```
