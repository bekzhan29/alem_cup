#include<bits/stdc++.h>

using namespace std;

int dx[5] = {-1, 0,0, 1, 0};
int dy[5] = {0, -1, 1, 0, 0};

namespace GameBoard {

    const int GameDuration = 300;

    int n, m;
    int current_tick;
    int q;

    char c[55][55];

    enum BonusType {
        Double = 1,
        Immune = 2,
        Freeze = 3
    };

    enum CellType {
        Money,
        Wall,
        Empty,
        Bonus,
        Dagger
    };

    enum BFSType {
        MonsterBFS = 0,
        PlayerBFS = 1,
        EnemyBFS = 2,
        BonusBFS = 3,
        CoinBFS = 4
    };


    struct Bonus {
        int x, y;
        BonusType type;
    };


    struct Player {
        int x, y;
        bool is_bonus;
        int bonus_left;
        BonusType bonus_type;
        int id;

        bool is_dagger;
        int dagger_left;

        inline void set(int x, int y, int param_1, int param_2) {
            this->x = x;
            this->y = y;
            is_dagger = param_1 > 0;
            is_bonus = param_2 > 0;
            bonus_type = BonusType(param_2);
        }
    } me, enemy;

    struct Monster {
        int x, y;
        Monster(int x, int y) {
            this->x = x;
            this->y = y;
        }
    };

    vector<Monster> monsters;

    struct Cell {
        CellType cell_type = Empty;
        bool is_dangerous = 0;
        BonusType bonus_type;

        bool is_money() const {
            return cell_type == Money;
        }

        bool is_dagger() const {
            return cell_type == Dagger;
        }

        bool is_bonus() const {
            return cell_type == Bonus;
        }

        bool is_wall() const {
            return cell_type == Wall;
        }
    } a[55][55];

    inline void identify(int x, int y) {
        if(c[x][y] == '!') {
            a[x][y].cell_type = Wall;
        } else if(c[x][y] == '#') {
            a[x][y].cell_type = Money;
        } else if(c[x][y] != '.') {
            a[x][y].cell_type = Bonus;
            a[x][y].bonus_type = (c[x][y] == 'b' ? Double : c[x][y] == 'f' ? Freeze : Immune);
        }
    }

    inline bool is_safe_cell(int x, int y) {
        return false;
    }

    inline bool inside(int x, int y) {
        return 0 <= x && x < n && 0 <= y && y < m;
    }

    inline bool can_move(int x, int y) {
        return inside(x, y) && !a[x][y].is_wall();
    }

    struct BFS {
        int d[55][55];
        BFSType bfs_type;
        inline void init_bfs(const vector<pair<int, int> >& starts) {
            queue<pair<int, int> > qu;
            for(int i = 0; i < n; ++i)
                for(int j = 0; j < m; ++j)
                    d[i][j] = 1e9;
            for(const auto &start : starts) {
                qu.push(start);
                d[start.first][start.second] = 0;
            }
            while(!qu.empty()) {
                auto cur = qu.front();
                qu.pop();
                for(int i = 0; i < 4; ++i) {
                    int tox = cur.first + dx[i], toy = cur.second + dy[i];
                    if(can_move(tox, toy) && d[tox][toy] > d[cur.first][cur.second] + 1) {
                        qu.push({tox, toy});
                        d[tox][toy] = d[cur.first][cur.second] + 1;
                    }
                }
            }
        }
        inline int get(int x, int y) {
            return d[x][y];
        }
    } bfs[5];

    inline int get_distance(BFSType bfs_type, int x, int y) {
        return bfs[bfs_type].get(x, y);
    }

    inline void init_bfs() {
        for (int i = 0; i < 5; ++i)
            bfs[i].bfs_type = BFSType(i);
        for (auto &bf : bfs) {
            vector<pair<int, int> > starts;
            switch (bf.bfs_type) {
                case MonsterBFS:
                    std::transform(monsters.begin(), monsters.end(), back_inserter(starts), [](const Monster &monster) {
                                       return make_pair(monster.x, monster.y);
                                   }
                    );
                    break;
                case PlayerBFS:
                    starts.emplace_back(me.x, me.y);
                    break;
                case EnemyBFS:
                    starts.emplace_back(enemy.x, enemy.y);
                    break;
                case BonusBFS:
                    for (int i = 0; i < n; ++i)
                        for (int j = 0; j < m; ++j)
                            if (a[i][j].is_bonus())
                                starts.emplace_back(i, j);
                    break;
                case CoinBFS:
                    for (int i = 0; i < n; ++i)
                        for (int j = 0; j < m; ++j)
                            if (a[i][j].is_money())
                                starts.emplace_back(i, j);
                    break;
            }
            bf.init_bfs(starts);
        }
    }

    inline void init() {
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < m; ++j)
                a[i][j].cell_type = Empty;
        monsters.clear();
    }

    void read() {
        cin >> m >> n >> me.id >> current_tick;
        cerr << n << ' ' << m << ' ' << me.id << ' ' << current_tick << endl;
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < m; ++j) {
                cin >> c[i][j];
                identify(i, j);
            }
        cin >> q;
        for(int i = 0; i < q; ++i) {
            string type;
            int id, y, x, param_1, param_2;
            cin >> type >> id >> y >> x >> param_1 >> param_2;
            if(type == "p") {
                if(id == me.id) {
                    me.set(x, y, param_1, param_2);
                } else {
                    enemy.id = id;
                    enemy.set(x, y, param_1, param_2);
                }
            } else {
                monsters.emplace_back(Monster(x, y));
            }
        }
        cerr << me.id << ' ' << me.x << ' ' << me.y << endl;
        init_bfs();
    }
}

class BaseStrategy {
    enum Answer {
        UP,
        LEFT,
        RIGHT,
        DOWN,
        STAY,
        NO_ANSWER
    };
public:
    Answer answer;
    static string get_answer_string(Answer answer) {
        switch (answer) {
            case UP:
                return "up";
            case DOWN:
                return "down";
            case LEFT:
                return "left";
            case RIGHT:
                return "right";
            default:
                return "stay";
        }
    }
    static bool can_escape(int x, int y) {
       for(int i = 0; i < 4; ++i) {
           int nx = x + dx[i], ny = y + dy[i];
           if(GameBoard::can_move(nx, ny) && GameBoard::get_distance(GameBoard::MonsterBFS, nx, ny) >= 4) return true;
       }
       return false;
    }
    static bool is_safe(int x, int y) {
        if(GameBoard::is_safe_cell(x, y) || GameBoard::me.is_dagger) return true;
        return can_escape(x, y);
    }

    inline void init_answer() {
        answer = NO_ANSWER;
    }

    inline void go_to_coin() {
        if(answer != NO_ANSWER) return;
        for(int i = 0; i < 4; ++i) {
            int tox = GameBoard::me.x + dx[i], toy = GameBoard::me.y + dy[i];
            if(GameBoard::can_move(tox, toy) && is_safe(tox, toy)) {
                if(GameBoard::bfs[GameBoard::CoinBFS].get(tox, toy) < GameBoard::bfs[GameBoard::CoinBFS].get(GameBoard::me.x, GameBoard::me.y)) {
                    answer = Answer(i);
                    cerr << to_string(answer) << ' ' << endl;
                }
            }
        }
    }

    inline void go_to_safe() {
        if(answer != NO_ANSWER) return;
        for(int i = 0; i < 4; ++i) {
            int tox = GameBoard::me.x + dx[i], toy = GameBoard::me.y + dy[i];
            if(GameBoard::can_move(tox, toy) && is_safe(tox, toy)) {
                answer = Answer(i);
            }
        }
    }

    inline void make_moves() {
        go_to_coin();
        go_to_safe();
    }
    inline void print_answer() {
        cout << get_answer_string(answer) << endl;
    }

    void move() {
        init_answer();
        make_moves();
        print_answer();
    }
};

class MainStrategy : public BaseStrategy {
};

class NextStrategy : public BaseStrategy {

};

int main() {
    while(true) {
        GameBoard::init();
        GameBoard::read();
        if(GameBoard::me.id == 1) {
            MainStrategy().move();
        } else {
            NextStrategy().move();
        }
    }
    return 0;
}
