#include<bits/stdc++.h>

using namespace std;

#define pb push_back

typedef long long ll;

const ll N = 55;
ll dx[] = {-1,0,0, 1,0}, dy[] = {0,1,-1,0,0};

namespace Grid {
    ll n, m, player_id, tick;
    char c[N][N];
    char start_c[N][N];
    char lc[N][N];
    ll cnt_coins;
    ll last_coins;

    inline void read() {
        cin >> m >> n >> player_id >> tick;

        for (ll i = 0; i < n; i++)
            for (ll j = 0; j < m; j++) {
                lc[i][j] = c[i][j];
            }
        last_coins = cnt_coins;
            cnt_coins = 0;
        for (ll i = 0; i < n; i++) {
            for (ll j = 0; j < m; j++) {
                cin >> c[i][j];
                cnt_coins += c[i][j] == '#';
                if(tick == 1) start_c[i][j] = c[i][j];
            }
        }
    }
}

class Position {
public:
    ll x, y;
    string type;
    ll param_1;
    ll param_2;
    ll p_id;
    ll score = 0;
    ll bonus_left = 0;
    bool has_dagger = 0;
    ll dagger_left = 0;
    ll has_bonus = 0;
    ll last_tick = -1;
    ll last_coin = -1;

    Position() {this->x = 0; this->y = 0;}
    Position(ll x, ll y) {
        this->x = x;
        this->y = y;
    }

    Position(string type, ll p_id, ll x, ll y, ll param_1, ll param_2) {
        this->type = type;
        this->p_id = p_id;
        this->x = x;
        this->y = y;
        this->param_1 = param_1;
        this->param_2 = param_2;
        score += ((Grid::lc[x][y] == '#') + (Grid::cnt_coins > Grid::last_coins && Grid::start_c[x][y] == '#')) * (param_2 + 1);
        if(((Grid::lc[x][y] == '#') + (Grid::cnt_coins > Grid::last_coins && Grid::start_c[x][y] == '#')) * (param_2 + 1) > 0) {
            last_coin = Grid::tick;
        }
        if (!has_dagger && param_1)
            dagger_left = 15;
        else
            dagger_left--;
        dagger_left = max(dagger_left, 0ll);
        if (!has_bonus && param_2)
            bonus_left = 30;
        else
            bonus_left--;
        bonus_left = max(bonus_left, 0ll);
        has_dagger = param_1;
        has_bonus = param_2;
    }

    bool inside() {
        return 0 <= this->x && this-> x < Grid::n && 0 <= this->y && this->y < Grid::m && Grid::c[this->x][this->y] != '!';
    }

    bool operator == (const Position &b) {
        return x == b.x && y == b.y;
    }

    vector<Position> move_cells() {
        vector<Position> result;
        for(ll i = 0; i <= 4; ++i) {
            ll nx = this->x + dx[i];
            ll ny = this->y + dy[i];
            Position nxt = Position(nx, ny);
            if(nxt.inside())
                result.pb( Position(nx, ny));
        }
        return result;
    }


    bool is_alive() {
        return this->last_tick == Grid::tick;
    }


    inline string get_direction(Position to) {
        if(to.x > this->x) return "down";
        if(to.y > this->y) return "right";
        if(to.x < this->x) return "up";
        if(to.y < this->y) return "left";
        return "stay";
    }
    inline void prll() {
        cerr << "Position " << this->x << ' ' << this->y << endl;
        return;
    }

    bool is_safe(Position nxt);
    ll get_score();
} player, enemy_player;
constexpr bool operator<(const Position& lhs, const Position& rhs){
return lhs.x < rhs.x || lhs.x == rhs.x && lhs.y < rhs.y;
}

struct BFS {
    ll distance[N][N];
    queue<Position> q;
    map<Position, Position> par;

    inline void init(vector<Position> starts) {
        while(!q.empty()) q.pop();
        par.clear();
        for(ll i = 0; i < Grid::n; ++i)
            for(ll j = 0 ; j < Grid::m; ++j)
                distance[i][j] = 1e9;

        for(auto &pos : starts) {
            distance[pos.x][pos.y] = 0;
            par[pos] = pos;
            q.push(pos);
        }
    }

    inline ll get(Position pos) {
        return distance[pos.x][pos.y];
    }

    inline void set(Position pos, ll value) {
        distance[pos.x][pos.y] = value;
    }

    inline void bfs(const vector<Position> &starts) {
        init(starts);

        while(!q.empty()) {
            auto pos = q.front();
            q.pop();
            for(auto &nxt : pos.move_cells()) {
                if(get(nxt) > get(pos) + 1) {
                    par[nxt] = pos;
                    set(nxt, get(pos) + 1);
                    q.push(nxt);
                }
            }

        }
    }

} monster_bfs, enemy_bfs, my_bfs, coin_bfs, bonus_bfs, dagger_bfs, start_bfs;

namespace DistanceCalculator {
    ll d[N][N][N][N];
    inline void calc_all() {
        for(ll i = 0; i < Grid::n; ++i)
            for(ll j =0 ;j < Grid::m; ++j) {
                BFS bfs = BFS();
                bfs.init(vector<Position> (1, Position(i, j)));
                for(ll x = 0 ; x < Grid::n; ++x)
                    for(ll y = 0; y < Grid::m; ++y) {
                        d[i][j][x][y] = bfs.distance[x][y];
                    }
            }
    }

    inline ll get(Position from, Position to) {
        return d[from.x][from.y][to.x][to.y];
    }
}

namespace Status {
    bool is_dagger;
    ll dagger_tick;
    bool is_bonus;
    ll bonus_tick;
}

namespace Entities {
    ll n;
    vector<Position> units;
    vector<Position> monsters;
    vector<Position> enemy;
    vector<Position> me;
    vector<Position> daggers;
    vector<Position> bonuses;
    vector<Position> coins;

    inline void read() {
        monsters.clear(); units.clear(); me.clear();
        enemy.clear();
        daggers.clear();
        bonuses.clear();
        coins.clear();
        cin >> n;
        for (ll i = 0; i < n; i++) {
            string type;
            ll p_id, x, y, param_1, param_2;

            cin >> type >> p_id >> x >> y >> param_1 >> param_2;
            units.pb(Position(type, p_id, y, x, param_1, param_2));
            cerr << type << p_id << x << y << param_1 << param_2 << endl;
        }
    }

    inline void identify() {
        for(ll i = 0; i < n; ++i) {
            if(units[i].type == "m") {
                monsters.pb(units[i]);
            } else if(units[i].type == "p") {
                if(units[i].p_id == Grid::player_id) {
                    me.pb(units[i]);
                    player = units[i];
                    player.last_tick = Grid::tick;
                } else {
                    enemy.pb(units[i]);
                    enemy_player = units[i];
                    enemy_player.last_tick = Grid::tick;
                }
            }
        }
        for(ll i = 0; i < Grid::n; ++i) {
            for(ll j = 0; j < Grid::m; ++j) {
                if(Grid::c[i][j] == '#') {
//                    cerr << i << ' ' << j<< endl;
                    coins.pb(Position(i,j));
                }else if(Grid::c[i][j] == 'b') {
                    bonuses.pb(Position(i, j));
                }else if(Grid::c[i][j] == 'b') {
                    daggers.pb(Position(i, j));
                }
            }
        }
    }
}

bool Position::is_safe(Position nxt) {
    auto nxt_moves = nxt.move_cells();
    for(auto after : nxt_moves) {
        cerr << "monster dist is " << monster_bfs.get(after) << endl;
        if(monster_bfs.get(after) >= 4) {
            return true;
        }
    }
    return false;
}

ll Position::get_score() {
    return monster_bfs.get(*this) * (-10000) - bonus_bfs.get(*this) * (-100) - coin_bfs.get(*this);
}



namespace SmartMover {
    bool is_found_move = 0;
    string direction = "";
    bool go_dagger;

    inline void go_to_something(BFS bfs, bool condition) {
        if(bfs.get(player) >= 300 - Grid::tick) return;
        if(is_found_move) return;
        if(condition) {
            auto nxt_cells = player.move_cells();
            bool found_better = false;
            bool found_fair = false;
            Position best_move = player;
            for (auto nxt : nxt_cells) {
                if (player.is_safe(nxt)) {
                    if (bfs.get(nxt) < bfs.get(player)) {
                        if (found_better) {
                            if (monster_bfs.get(nxt) < monster_bfs.get(best_move))
                                best_move = nxt;
                        } else {
                            found_better = true;
                            best_move = nxt;
                        }
                    } else if (bfs.get(nxt) == bfs.get(player)) {
                        if (found_better) {

                        } else {
                            if (monster_bfs.get(nxt) < monster_bfs.get(best_move)) {
                                best_move = nxt;
                                found_fair = true;
                            }
                        }
                    }
                }
            }
            if(found_better || found_fair) {
                is_found_move = true;
                cerr<<bfs.get(best_move) << "next pos is " << ' ' << best_move.x << ' ' << best_move.y << endl;
                direction = player.get_direction(best_move);
            }
        }

    }

    inline void go_kill() {
        if(monster_bfs.get(player) >= 300 - Grid::tick)
            return;
        if(is_found_move) return;
        if(player.dagger_left > 3 && go_dagger) {
            auto nxt_cells = player.move_cells();
            for (auto nxt : nxt_cells) {
                if (monster_bfs.get(nxt) < monster_bfs.get(player)) {
                    is_found_move = true;
                    direction = player.get_direction(nxt);
                }
            }
        }
    }

    inline void go_to_best_pos() {
        if (Entities::coins.size() <= 1 && player.score > enemy_player.score + 1 && enemy_player.is_alive())
        {
            Position pos = start_bfs.par[player];
            if(pos.inside() && player.is_safe(pos)) {
                is_found_move = true;
                direction = player.get_direction(pos);
                cerr << "Moving to the best pos " << endl;
            }

            return;
        }
    }

    inline void run_away() {
        if(is_found_move) return;
        auto nxt_cells = player.move_cells();
        auto best_move = player;
        for (auto nxt : nxt_cells) {
            if (player.is_safe(nxt)) {
                if(nxt.get_score() < best_move.get_score() || !is_found_move) {
                    best_move = nxt;
                    is_found_move = true;
                    direction = player.get_direction(best_move);
                }
            }
        }
        if(is_found_move) return;
        for (auto nxt : nxt_cells) {
            if (nxt.get_score() < best_move.get_score() || !is_found_move) {
                best_move = nxt;
                is_found_move = true;
                direction = player.get_direction(best_move);
            }
        }
    }

    inline void get_next_move() {
        is_found_move = 0;
        cerr << player.x << ' ' << player.y << endl;
        go_dagger =  !enemy_player.is_alive() || (Grid::tick - player.last_coin > 35 && player.score - 3 <= enemy_player.score)
                                || (Entities::coins.size() <= 4 && player.score > enemy_player.score && Entities::monsters.size());

        go_to_something(bonus_bfs, !Entities::bonuses.empty() && player.bonus_left <= 5);
        cerr<<is_found_move<<' ' <<"BONUSE"<<endl;
        go_to_something(dagger_bfs, go_dagger && !is_found_move && !Entities::coins.empty() && !Entities::daggers.empty());
        cerr<<is_found_move<<' ' <<"DAGGER"<<endl;
        go_kill();
        cerr<<is_found_move<<' ' <<"KILL"<<endl;
        go_to_best_pos();
        go_to_something(coin_bfs, true);
        cerr<<is_found_move<<' ' <<"COIN"<<endl;

        run_away();
        for(int i =0 ; i< Grid::n;++i,cerr<<endl)
            for(int j = 0; j < Grid::m; ++j)
                cerr << bonus_bfs.distance[i][j] << ' ';
        cerr << player.x << ' ' << player.y << ' ' << endl;
        cerr<< direction<<endl;
        cout << direction << endl;
    }
}


int main()
{
    DistanceCalculator::calc_all();
    while (true) {
        Grid::read();
        Entities::read();
        Entities::identify();

        monster_bfs.bfs(Entities::monsters);
        my_bfs.bfs(Entities::me);
        enemy_bfs.bfs(Entities::enemy);
        if(Grid::tick == 1) {
            start_bfs.bfs(Entities::coins);
        }
        coin_bfs.bfs(Entities::coins);
        bonus_bfs.bfs(Entities::bonuses);
        dagger_bfs.bfs(Entities::daggers);

        SmartMover::get_next_move();
    }
	return 0;
}
