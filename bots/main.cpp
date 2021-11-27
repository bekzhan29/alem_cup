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
        Bonus
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

        bool is_money() const {
            return cell_type == Money;
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
        inline void init_bfs(const vector<pair<int, int> >& starts) {

        }
        inline int get(int x, int y) {
            return d[x][y];
        }
    } bfs[5];

    inline int get_distance(BFSType bfs_type, int x, int y) {
        return bfs[bfs_type].get(x, y);
    }

    inline void init_bfs() {
        for(auto & bf : bfs) {
            vector<pair<int, int> > starts;
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
    }
}

class BaseStrategy {
public:
    static bool can_escape(int x, int y) {
       for(int i = 0; i < 4; ++i) {
           int nx = x + dx[i], ny = y + dy[i];
           if(GameBoard::can_move(nx, ny) && GameBoard::get_distance(GameBoard::MonsterBFS, nx, ny) <= 4) return true;
       }
       return false;
    }
    static bool is_safe(int x, int y) {
        if(GameBoard::is_safe_cell(x, y) || GameBoard::me.is_dagger) return true;
        return can_escape(x, y);
    }
};

class MainStrategy : BaseStrategy {

};

class NextStrategy : BaseStrategy {

};

int main() {
    while(true) {
        GameBoard::init();
        GameBoard::read();
    }
    return 0;
}