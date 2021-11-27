#include<bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<pair<ll, ll>, ll> plll;

const int N = 55;

int dx[5] = {-1, 0,0, 1, 0};
int dy[5] = {0, -1, 1, 0, 0};

namespace SafeCellGenerator {
    int decoded[N][N];
    map<plll, ll> hash_to_id;
    plll id_to_hash[N];
    int cnt_maps;
    ll safe_cells[N][N][N];
    int map_id;
    pair<pair<ll, ll>, ll> encode(char c[N][N], int n, int m) {
        ll h1 = 0, h2 = 0, h3 = 0;
        for (ll i = 0; i < n; i++)
            for (ll j = 0; j < m; j++) {
                if (i < 4 && c[i][j] == '!')
                    h1 |= (1LL << (i * m + j));
                else if (4 <= i && i < 8 && c[i][j] == '!')
                    h2 |= (1LL << ((i - 4) * m + j));
                else if (8 <= i && c[i][j] == '!')
                    h3 |= (1LL << ((i - 8) * m + j));
            }
        return {{h1, h2}, h3};
    }
    void decode(pair<pair<ll, ll>, ll> a, int n, int m) {
        ll h1 = a.first.first, h2 = a.first.second, h3 = a.second, cur = 0, x;
        for (ll i = 0; i < n; i++)
            for (ll j = 0; j < m; j++) {
                if (i < 4)
                    x = h1;
                else if (i < 8)
                    x = h2;
                else
                    x = h3;
                decoded[i][j] = ((x >> cur) & 1);
                cur++;
                cur %= 4 * m;
            }
    }
    void init(ll a[N][N], plll b, int n, int m) {
        decode(b, n, m);
        for (ll i = 0; i < n; i++)
            for (ll j = 0; j < m; j++)
                a[i][j] = decoded[i][j];
    }

    inline void init_all(int n, int m) {
        cnt_maps = 0;
        cnt_maps++;
        id_to_hash[1] = {{1412887495131136, 1412876746391616}, 21053664};
        init(safe_cells[1], {{3703033829523455, 3700685667086401}, 549755812067},n,m);
        cnt_maps++;
        id_to_hash[2] = {{141877870592, 148319537314}, 9766980};
        init(safe_cells[2], {{3944984362744847, 3944990267473063}, 482103131207},n,m);
        cnt_maps++;
        id_to_hash[3] = {{35257659752448, 35253387052036}, 4785220};
        init(safe_cells[3], {{106614510059519, 35257682019396}, 549755812935},n,m);
        cnt_maps++;
        id_to_hash[4] = {{565405475176448, 565153273479232}, 8425198};
        init(safe_cells[4], {{830131278970879, 688309313208544}, 549755813887},n,m);
        cnt_maps++;
        id_to_hash[5] = {{245753184256, 15063171296}, 527950};
        init(safe_cells[5], {{4230891749015047, 4239716836704255}, 413374479967},n,m);
        cnt_maps++;
        id_to_hash[6] = {{79730114560, 79742649508}, 4785316};
        init(safe_cells[6], {{124208843587583, 123229339927780}, 549755813095},n,m);
        cnt_maps++;
        id_to_hash[7] = {{149671164805120, 149675468539970}, 10258434};
        init(safe_cells[7], {{255563707449343, 184859841153090}, 549755811907},n,m);
        cnt_maps++;
        id_to_hash[8] = {{221601890304, 73293136964}, 8424678};
        init(safe_cells[8], {{3944994561915911, 3949445766971391}, 481564949743},n,m);
        cnt_maps++;
        id_to_hash[9] = {{321057404256256, 321194978740226}, 8945664};
        init(safe_cells[9], {{391439292825599, 321199273707586}, 549755805760},n,m);
        cnt_maps++;
        id_to_hash[10] = {{565198922465280, 565159714619552}, 16794344};
        init(safe_cells[10], {{4068193022771199, 4067658299867361}, 549755813887},n,m);
        for (ll i = 1; i <= cnt_maps; i++)
            hash_to_id[id_to_hash[i]] = i;
    }

    inline void set_map_id(int n, int m, char c[N][N]) {
        plll map_hash = encode(c, n, m);
        if (!hash_to_id.count(map_hash)) {
            assert(0);
        }
        map_id = hash_to_id[map_hash];
    }

    bool is_safe(int x, int y) {
        return safe_cells[map_id][x][y];
    }
}

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
        CoinBFS = 4,
        SafeBFS = 5,
        Finish = 6,
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
        return SafeCellGenerator::is_safe(x, y);
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
    } bfs[6];

    inline int get_distance(BFSType bfs_type, int x, int y) {
        return bfs[bfs_type].get(x, y);
    }

    inline void init_bfs() {
        for (int i = 0; i < BFSType::Finish; ++i)
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
                case SafeBFS:
                    for(int i = 0; i < n; ++i)
                        for(int j = 0; j < m; ++j)
                            if(SafeCellGenerator::is_safe(i, j))
                                starts.emplace_back(i,j);
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
        if(current_tick == 1) SafeCellGenerator::init_all(n, m);
        cerr << n << ' ' << m << ' ' << me.id << ' ' << current_tick << endl;
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < m; ++j) {
                cin >> c[i][j];
                identify(i, j);
            }
        if(current_tick == 1) SafeCellGenerator::set_map_id(n, m, c);
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

    inline void run_away_to_safe() {
        if(answer != NO_ANSWER) return;
        for(int i = 0; i <= 4; ++i) {
            int tox = GameBoard::me.x + dx[i], toy = GameBoard::me.y + dy[i];
            if(GameBoard::can_move(tox, toy) && is_safe(tox, toy)) {
                answer = Answer(i);
            }
        }
    }

    bool is_better(int xa, int ya, int xb, int yb) {
        GameBoard::BFS m_bfs = GameBoard::bfs[GameBoard::MonsterBFS], s_bfs = GameBoard::bfs[GameBoard::SafeBFS], c_bfs = GameBoard::bfs[GameBoard::CoinBFS];
        if (m_bfs.get(xa, ya) > m_bfs.get(xb, yb)) return true;
        if (m_bfs.get(xa, ya) < m_bfs.get(xb, yb)) return false;
        if (s_bfs.get(xa, ya) < s_bfs.get(xb, yb)) return true;
        if (s_bfs.get(xa, ya) > s_bfs.get(xb, yb)) return false;
        return c_bfs.get(xa, ya) <= c_bfs.get(xb, yb);
    }
    inline void run_away_from_monster() {
        if (answer != NO_ANSWER) return;
        for (int i = 0; i <= 4; ++i) {
            int tox = GameBoard::me.x + dx[i], toy = GameBoard::me.y + dy[i];
            if (GameBoard::can_move(tox, toy)) {
                if(answer == NO_ANSWER) {
                    answer = Answer(i);
                } else {
                    int px = GameBoard::me.x + dx[answer], py = GameBoard::me.y + dy[answer];
                    if(is_better(tox, toy, px, py))
                        answer = Answer(i);
                }
            }
        }
    }

    inline void make_moves() {
        go_to_coin();
        go_to_safe();
        run_away_to_safe();
        run_away_from_monster();
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
