// #include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include <stack>
#include <chrono>
#include <random>

using namespace std;
#define pb push_back
#define mp make_pair
#define INF ll(2e6)
#define mod 998244353
#define eps 1e-9
#define abs(x) ((x)>=0?(x):-(x))
#define y1 solai
#define fi first
#define se second
typedef long long ll;
typedef long double ld;
typedef pair<ll, ll> pll;
typedef pair<pll, ll> plll;
typedef pair<double, double> pdd;
const ll N = 21, MAX_DEPTH = 16, K = 10, US = 0, ENEMY = 1, COINS = 2, MONSTERS = 3, DAGGERS = 4;
const ll SAFE = 5, BONUSES = 6, FREEZE = 7, IMMUNE = 8, START = 9;
double DEC_PW = 1.4;
const ll RIGHT = 0, DOWN = 1, LEFT = 2, UP = 3, STAY = 4, NO_ANSWER = -1;
ll n = 11, m = 13, px, py, ex, ey, x, y, d[K][N][N], ans, neighbors[N][N], player_id, tick, safe_column[N];
ll dx[5] = {0, 1, 0, -1, 0}, dy[5] = {1, 0, -1, 0, 0}, ord[5];
ll cnt_coins, last_coins, dagger_left, bonus_left, enemy_bonus_left, our_score, enemy_score, cur_mid, mid_coins;
pll pr[K][N][N];
bool block_monsters = 0;
vector<pll> daggers, bonuses[K], monsters, coins;
queue<pll> q[K];
char c[N][N], lc[N][N], start_c[N][N];
ll bad_angle[N][N], near_monster[N][N], weight[N][N], time_left[N][N];
string s[5] = {"right", "down", "left", "up", "stay"};
bool enemy_alive, go_dagger, left_right, lleft, rright;
ll last_coin = -1, cnt_maps, last_cnt_maps, map_id, bonus_type, enemy_bonus_type;
plll map_hash;
map<plll, ll> hash_to_id;
plll id_to_hash[N];
ll safe_cells[N][N][N], decoded[N][N], changed, temp[N][N], steps;
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
double max_time, cur_time;


const bool beast_mode = 1, are_you_sure = 1;
const bool silent_mode = 0;


ll in_box(ll x, ll y) {
    return (0 <= x && x < n && 0 <= y && y < m);
}

ll border_dist(ll x, ll y) {
    return min({x, y, n - x - 1, m - y - 1});
}

bool mid_zone(int x, int y) {
    return x > 1 && x < n - 2 && y * 2 + 1 == m;
}

ll dist(pll a, pll b) {
    return abs(a.fi - b.fi) + abs(a.se - b.se);
}

ll dist(ll x1, ll y1, ll x2, ll y2) {
    return dist({x1, y1}, {x2, y2});
}

bool cmp(pll a, pll b) {
    return weight[a.fi][a.se] > weight[b.fi][b.se];
}

string tostring(plll a) {
    return "{{" + to_string(a.fi.fi) + ", " + to_string(a.fi.se) + "}, " + to_string(a.se) + "}";
}

plll encode(char c[N][N]) {
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

plll encode(ll a[N][N]) {
    ll h1 = 0, h2 = 0, h3 = 0;
    for (ll i = 0; i < n; i++)
        for (ll j = 0; j < m; j++) {
            if (i < 4 && a[i][j])
                h1 |= (1LL << (i * m + j));
            else if (4 <= i && i < 8 && a[i][j])
                h2 |= (1LL << ((i - 4) * m + j));
            else if (8 <= i && a[i][j])
                h3 |= (1LL << ((i - 8) * m + j));
        }
    return {{h1, h2}, h3};
}

// result is in array "decoded"
void decode(plll a) {
    ll h1 = a.fi.fi, h2 = a.fi.se, h3 = a.se, cur = 0, x;
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

void print_map(ll id) {
    if (silent_mode) return;
//    cerr << "Map hashes:" << endl;
//    cerr << "id_to_hash[" + to_string(id) + "] = " + tostring(id_to_hash[id]) + ";" << endl;
//    plll encoded = encode(safe_cells[id]);
//    cerr << "init(safe_cells[" + to_string(id) + "], " + tostring(encoded) + ");" << endl;
//    if (cnt_maps != last_cnt_maps)
//        cerr << "------ NEW LEVEL ----------" << endl;
//    if (changed)
//        cerr << "------ CHANGED SAFE MASK ----------" << endl;
//    cerr << endl;
}

void print_safe_cells(ll id) {
    if (silent_mode) return;
//    for (ll i = 0; i < n; i++, cerr << endl)
//        for (ll j = 0; j < m; j++)
//            cerr << safe_cells[id][i][j];
}

void and_mask(ll a[N][N], ll b[N][N]) {
    for (ll i = 0; i < n; i++)
        for (ll j = 0; j < m; j++) {
            if (a[i][j] && !b[i][j])
                changed = 1;
            a[i][j] &= b[i][j];
        }
}

void init(ll a[N][N], plll b) {
    decode(b);
    for (ll i = 0; i < n; i++)
        for (ll j = 0; j < m; j++)
            a[i][j] = decoded[i][j];
}


bool is_safe1(int x, int y) {
    for (ll i = 0; i < 4; i++) {
        ll tx = x + dx[i], ty = y + dy[i];
        if (in_box(tx, ty) && c[tx][ty] != '!' && d[MONSTERS][tx][ty] >= 4)
            return 1;
    }
    return 0;
}

bool is_safe(int x, int y) {
//    if (beast_mode && are_you_sure && player_id == 2)
//        return (d[MONSTERS][x][y] > 2);
    if (safe_cells[map_id][x][y] || dagger_left > 3)
        return 1;
    if(bonus_type == 2 && bonus_left > 3 && d[MONSTERS][x][y] != 0)
        return 1;
    return is_safe1(x, y);
}

void bfs(ll cur, bool is_coin = false) {
    ll x, y, tx, ty;
    for (; !q[cur].empty();) {
        x = q[cur].front().fi;
        y = q[cur].front().se;
        q[cur].pop();
        for (ll i = 0; i < 4; i++) {
            tx = x + dx[i];
            ty = y + dy[i];
            if(!is_safe(tx, ty) && d[MONSTERS][tx][ty] <= 2 && cur == COINS) continue;

            if (in_box(tx, ty) && c[tx][ty] != '!')
                if (d[cur][tx][ty] > d[cur][x][y] + 1) {
                    d[cur][tx][ty] = d[cur][x][y] + 1;
                    q[cur].push({tx, ty});
                    pr[cur][tx][ty] = {x, y};
                }
        }
    }
}

void init_safe_cells() {
    cnt_maps = 0;
    cnt_maps++;
    id_to_hash[1] = {{1412887495131136, 1412876746391616}, 21053664};
    init(safe_cells[1], {{3703033829523455, 3700685667086401}, 549755812067});
    cnt_maps++;
    id_to_hash[2] = {{141877870592, 148319537314}, 9766980};
    init(safe_cells[2], {{3944984362744847, 3944990267473063}, 482103131207});
    cnt_maps++;
    id_to_hash[3] = {{35257659752448, 35253387052036}, 4785220};
    init(safe_cells[3], {{106614510059519, 35257682019396}, 549755812935});
    cnt_maps++;
    id_to_hash[4] = {{565405475176448, 565153273479232}, 8425198};
    init(safe_cells[4], {{830131278970879, 688309313208544}, 549755813887});
    cnt_maps++;
    id_to_hash[5] = {{245753184256, 15063171296}, 527950};
    init(safe_cells[5], {{4230891749015047, 4239716836704255}, 413374479967});
    cnt_maps++;
    id_to_hash[6] = {{79730114560, 79742649508}, 4785316};
    init(safe_cells[6], {{124208843587583, 123229339927780}, 549755813095});
    cnt_maps++;
    id_to_hash[7] = {{149671164805120, 149675468539970}, 10258434};
    init(safe_cells[7], {{255563707449343, 184859841153090}, 549755811907});
    cnt_maps++;
    id_to_hash[8] = {{221601890304, 73293136964}, 8424678};
    init(safe_cells[8], {{3944994561915911, 3949445766971391}, 481564949743});
    cnt_maps++;
    id_to_hash[9] = {{321057404256256, 321194978740226}, 8945664};
    init(safe_cells[9], {{391439292825599, 321199273707586}, 549755805760});
    cnt_maps++;
    id_to_hash[10] = {{565198922465280, 565159714619552}, 16794344};
    init(safe_cells[10], {{4068193022771199, 4067658299867361}, 549755813887});
    last_cnt_maps = cnt_maps;
    for (ll i = 1; i <= cnt_maps; i++)
        hash_to_id[id_to_hash[i]] = i;
}


void count_empty_neighbors() {
    for (ll i = 0; i < n; i++)
        for (ll j = 0; j < m; j++) {
            neighbors[i][j] = 0;
            for (ll k = 0; k < 4; k++)
                if (in_box(i + dx[k], j + dy[k]) && c[i + dx[k]][j + dy[k]] != '!')
                    neighbors[i][j]++;
            if (neighbors[i][j] == 2) {
                for (int k = 1; k <= 4; k++) {
                    int cur = k % 4;
                    int pre = (k - 1);
                    if (in_box(i + dx[cur], j + dy[cur]) && c[i + dx[cur]][j + dy[cur]] != '!')
                        if (in_box(i + dx[pre], j + dy[pre]) && c[i + dx[pre]][j + dy[pre]] != '!')
                            bad_angle[i][j] = 1;
                }
            }
        }
}

namespace bfs_distance {
    int get_weight(int x, int y, int radius) {
        if (c[x][y] == '!')
            return 0;
        vector<vector<int>> dist(n, vector<int>(m, -1));
        int cnt = 0;
        dist[x][y] = 0;
        queue<pair<int, int>> cur_q;
        cur_q.push({x, y});
        while (cur_q.size()) {
            int cur_x = cur_q.front().first;
            int cur_y = cur_q.front().second;
            cur_q.pop();

            if (dist[cur_x][cur_y] <= radius) {
                cnt += (c[cur_x][cur_y] == '#');
            }
            for (int i = 0; i < 4; i++) {
                int nx = cur_x + dx[i];
                int ny = cur_y + dy[i];
                if (in_box(nx, ny) && dist[nx][ny] == -1 && c[nx][ny] != '!') {
                    dist[nx][ny] = dist[cur_x][cur_y] + 1;
                    cur_q.push({nx, ny});
                }
            }
        }
        return cnt;
    }
}

void calculate_weights() {
    ll r = 5;
    for (ll i = 0; i < n; i++)
        for (ll j = 0; j < m; j++)
            weight[i][j] = bfs_distance::get_weight(i, j, r);
}

void kill_enemy() {
    if (ans != NO_ANSWER || bonus_type != 2 || enemy_bonus_type == 3)
        return;
    if (!enemy_alive || dist(px, py, ex, ey) != 2 || safe_cells[map_id][ex][ey])
        return;
    for (pll monster:monsters) {
        x = monster.fi;
        y = monster.se;
        if (dist(px, py, x, y) > 2 && d[ENEMY][x][y] <= 5) {
            ans = STAY;
            return;
        }
    }
}

bool check_time(pll item) {
    x = item.fi;
    y = item.se;
    return (d[US][x][y] < time_left[x][y]);
}

inline bool better(int x, int y) {
    int tx = px + dx[ans];
    int ty = py + dy[ans];
    if(d[BONUSES][x][y] < d[BONUSES][tx][ty]) return true;
    if(d[BONUSES][x][y] > d[BONUSES][tx][ty]) return false;
    if(d[COINS][x][y] < d[COINS][tx][ty]) return true;
    if(d[COINS][x][y] > d[COINS][tx][ty]) return false;
    if(d[MONSTERS][x][y] > d[MONSTERS][tx][ty]) return true;
    if(d[MONSTERS][x][y] < d[MONSTERS][tx][ty]) return false;
    return d[SAFE][x][y] < d[SAFE][tx][ty];
}

void go_to_bonus(ll type) {
    if (ans != NO_ANSWER || d[type][px][py] >= 300 - tick)
        return;
    shuffle(ord, ord + 4, rnd);
    for (ll j = 0; j < 4; j++) {
        ll i = ord[j], tx = px + dx[i], ty = py + dy[i];
        if (in_box(tx, ty) && d[type][tx][ty] + 1 == d[type][px][py] && is_safe(tx, ty)) {
            if(ans == NO_ANSWER || better(tx, ty)) {
                ans = i;
                if (!silent_mode) {
                    cerr << "Moving towards bonus" << endl;
                }
            }
        }
    }
}

void go_to_dagger() {
    if (ans != NO_ANSWER || d[DAGGERS][px][py] >= 300 - tick)
        return;
    if (go_dagger && !monsters.empty() && !daggers.empty()) {
        shuffle(ord, ord + 4, rnd);
        for (ll j = 0; j < 4; j++) {
            ll i = ord[j], tx = px + dx[i], ty = py + dy[i];
            if (in_box(tx, ty) && d[DAGGERS][tx][ty] + 1 == d[DAGGERS][px][py] && is_safe(tx, ty)) {
                if(ans == NO_ANSWER || better(tx, ty)) {
                    ans = i;
                    if (!silent_mode) {
                        cerr << "Moving towards dagger" << endl;
                    }
                }
            }
        }
    }
}



void go_kill() {
    if (d[MONSTERS][px][py] >= 300 - tick)
        return;
    if (ans == NO_ANSWER && dagger_left > 3 && go_dagger) {
        ans = STAY;
        shuffle(ord, ord + 4, rnd);
        for (ll j = 0; j < 4; j++) {
            ll i = ord[j];
            x = px + dx[i];
            y = py + dy[i];
            if (in_box(x, y) && c[x][y] != '!')
                if (d[MONSTERS][x][y] < d[MONSTERS][px + dx[ans]][py + dy[ans]]) {
                    if(ans == NO_ANSWER || better(x, y)) {
                        ans = i;
                        if (!silent_mode) {
                            cerr << "Moving towards monster" << endl;
                        }
                    }
                }
        }
    }
}

double cost[N][N];
int dc[N][N];

void go_to_coin() {
    if (ans != NO_ANSWER)
        return;
    if (cnt_coins <= 1 && our_score > enemy_score + 1 && enemy_alive) {
        if(is_safe(px, py)) ans = STAY;
    }
    x = pr[COINS][px][py].fi;
    y = pr[COINS][px][py].se;
    if (in_box(x, y) && c[x][y] != '!' && is_safe(x, y)) {
        for (ll i = 0; i < 4; i++)
            if (px + dx[i] == x && py + dy[i] == y)
                ans = i;
        return;
    }
    shuffle(ord, ord + 4, rnd);
    for (ll j = 0; j < 4; j++) {
        ll i = ord[j];
        x = px + dx[i];
        y = py + dy[i];
        if (in_box(x, y) && c[x][y] != '!')
            if (d[COINS][x][y] + 1 == d[COINS][px][py] && is_safe(x, y)) {
                ans = i;
                if (!silent_mode) {
                    cerr << "Moving towards coin: " << x << " " << y << " " << d[MONSTERS][x][y] << " "
                         << neighbors[x][y] << endl;
                }
            }
    }
}

void run_away() {
    if (ans == NO_ANSWER) {
        shuffle(ord, ord + 4, rnd);
        for (ll j = 0; j <= 4; j++) {
            ll i = ord[j];
            x = px + dx[i];
            y = py + dy[i];
            if (in_box(x, y) && c[x][y] != '!' && safe_cells[map_id][x][y]) {
                if(ans == NO_ANSWER || better(x, y)) {
                    ans = i;
                    if (!silent_mode) {
                        cerr << "Moving towards safe cell " << x << " " << y << endl;
                    }
                }
            }
        }
    }

    if (ans == NO_ANSWER) {
        shuffle(ord, ord + 4, rnd);
        for (ll j = 0; j <= 4; j++) {
            ll i = ord[j];
            x = px + dx[i];
            y = py + dy[i];
            if (in_box(x, y) && c[x][y] != '!')
                if (neighbors[x][y] > 1 && is_safe(x, y)) {
                    if(ans == NO_ANSWER || better(x, y)) {
                        ans = i;
                        if (!silent_mode) {
                            cerr << "Moving away from monster to safe cell: " << x << " " << y << endl;
                        }
                    }
                }
        }
    }

    if (ans == NO_ANSWER) {
        ll mx = 0, mn = INF;
        shuffle(ord, ord + 4, rnd);
        for (ll j = 0; j <= 4; j++) {
            ll i = ord[j];
            x = px + dx[i];
            y = py + dy[i];
            if (in_box(x, y) && c[x][y] != '!')
                if (neighbors[x][y] > 1)
                    if (d[MONSTERS][x][y] > mx) {
                        if(ans == NO_ANSWER || better(x, y)) {
                            ans = i, mx = d[MONSTERS][x][y];
                            if (!silent_mode) {
                                cerr << "Moving away from monster: " << x << " " << y << " " << d[MONSTERS][x][y] << endl;
                            }
                        }
                    }
        }
    }
    if (d[MONSTERS][px][py] == 2 && beast_mode && are_you_sure && (ans == STAY && !is_safe(px, py)) || (ans == NO_ANSWER)) {
        for(auto &mon : monsters) {
            x = mon.fi, y = mon.se;
            for(int j = 0; j < 4; ++j) {
                ll nx = x + dx[j];
                ll ny = y + dy[j];
                if(d[US][nx][ny] < d[US][x][y] && !safe_cells[map_id][nx][ny]) {
                    x = nx;
                    y = ny;
                    break;
                }
            }
            mon.fi = x;
            mon.se = y;
        }
        for(int j = 0; j < 4; ++j) {
            x = px + dx[j];
            y = py + dy[j];
            int ok = 0;
            for(auto &z : monsters) {
                if(z.fi != x && z.se != y) {
                    ok ++;
                }
            }
            if(ok == monsters.size()) {
                if(ans == NO_ANSWER || ans == STAY && !is_safe(px,py) || better(x, y))
                    ans = j;
            }
        }
        if (ans != NO_ANSWER)
            cerr << "BEAST MODE" << endl;
    }
}
inline bool is_bonus_or_dagger(char c)
{
    return (c == 'b' || c == 'f' || c == 'i' || c == 'd');
}



int main() {
    for (ll i = 0; i < m; i++)
        safe_column[i] = 1;
    for (ll i = 0; i < 5; i++)
        ord[i] = i;
    init_safe_cells();
    for (;;) {
        cin >> m >> n >> player_id >> tick;
        cerr << n << " " << m << " " << player_id << " " << tick << endl;
        block_monsters = 1;
        // read map
        coins.clear();
        daggers.clear();

        bonuses[BONUSES].clear();
        bonuses[FREEZE].clear();
        bonuses[IMMUNE].clear();

        cnt_coins = 0;

        for (ll k = 0; k < K; k++) {
            if (k == START && tick > 1)
                break;
            for (ll i = 0; i < n; i++)
                for (ll j = 0; j < m; j++) {
                    d[k][i][j] = INF;
                    pr[k][i][j] = {-1, -1};
                }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cin >> c[i][j];
                if (tick == 1)
                    start_c[i][j] = c[i][j];
                near_monster[i][j] = 0;
                // dagger
                if (c[i][j] == 'd') {
                    // d[DAGGERS][i][j] = 0;
                    // q[DAGGERS].push({i, j});
                    daggers.pb({i, j});
                }
                // bonus
                if (c[i][j] == 'b') {
                    // d[BONUSES][i][j] = 0;
                    // q[BONUSES].push({i, j});
                    bonuses[BONUSES].pb({i, j});
                }
                // freeze
                if (c[i][j] == 'f') {
                    // d[FREEZE][i][j] = 0;
                    // q[FREEZE].push({i, j});
                    bonuses[FREEZE].pb({i, j});
                }
                // immune
                if (c[i][j] == 'i') {
                    // d[IMMUNE][i][j] = 0;
                    // q[IMMUNE].push({i, j});
                    bonuses[IMMUNE].pb({i, j});
                }

                // coin
                if (c[i][j] == '#') {
                    cnt_coins++;
                    coins.pb({i, j});
                }
                if (is_bonus_or_dagger(c[i][j])) {
                    if (time_left[i][j] == 0)
                        time_left[i][j] = 15;
                    else
                        time_left[i][j]--;
                } else  time_left[i][j] = 0;

            }
        }
        cerr << endl;
        if (tick == 1) {
            int cx = n / 2;
            int cy = m / 2;
            map_hash = encode(start_c);
            if (!hash_to_id.count(map_hash)) {
                hash_to_id[map_hash] = ++cnt_maps;
                id_to_hash[cnt_maps] = map_hash;
            }
            map_id = hash_to_id[map_hash];
            mid_coins = 0;
            for (ll i = cx - 1; i <= cx + 1; ++i) {
                for (ll j = cy - 1; j <= cy + 1; ++j) {
                    if (c[i][j] == '#')
                        ++mid_coins;
                }
            }
        }




        for (ll i = 0; i < n; i++)
            for (ll j = 0; j < m; j++)
                if (safe_cells[map_id][i][j]) {
                    d[SAFE][i][j] = 0;
                    q[SAFE].push({i, j});
                }

        print_safe_cells(map_id);

        print_map(map_id);

        // count empty neighbors
        count_empty_neighbors();

        // calculate cell weights
        calculate_weights();

        // number of entities
        int k;
        cin >> k;

        cerr << "k:" << k << endl;

        enemy_alive = 0;
        monsters.clear();
        // read entities
        for (int i = 0; i < k; i++) {
            string type;
            ll p_id, cx, cy, param_1, param_2;

            cin >> type >> p_id >> cy >> cx >> param_1 >> param_2;
            // player
            if (type == "p") {
                // us
                if (p_id == player_id) {
                    px = cx, py = cy;
                    our_score +=
                            ((lc[px][py] == '#') + (cnt_coins > last_coins && start_c[cx][cy] == '#')) * ((param_2 == 1) + 1);
                    if (lc[px][py] == 'd' && param_1)
                        dagger_left = 15;
                    else
                        dagger_left--;
                    if (!param_1)
                        dagger_left = 0;
                    dagger_left = max(dagger_left, 0LL);
                    if (lc[px][py] == 'b' && param_2 == 1)
                        bonus_left = 30;
                    else if (lc[px][py] == 'f' && param_2 == 2)
                        bonus_left = 15;
                    else if (lc[px][py] == 'i' && param_2 == 3)
                        bonus_left = 15;
                    else
                        bonus_left--;
                    if (!param_2)
                        bonus_left = 0;
                    bonus_left = max(bonus_left, 0LL);
                    bonus_type = param_2;
                }
                if (p_id != player_id) {
                    enemy_alive = 1;
                    ex = cx;
                    ey = cy;
                    enemy_score +=
                            ((lc[ex][ey] == '#') + (cnt_coins > last_coins && start_c[cx][cy] == '#')) * ((param_2 == 1) + 1);
                    if (lc[ex][ey] == 'b' && param_2 == 1)
                        enemy_bonus_left = 30;
                    else if (lc[ex][ey] == 'f' && param_2 == 2)
                        enemy_bonus_left = 15;
                    else if (lc[ex][ey] == 'i' && param_2 == 3)
                        enemy_bonus_left = 15;
                    else
                        enemy_bonus_left--;
                    if (!param_2)
                        enemy_bonus_left = 0;
                    enemy_bonus_left = max(enemy_bonus_left, 0LL);
                    enemy_bonus_type = param_2;
                }

            }
            // monster
            if (type == "m") {
                if (safe_cells[map_id][cx][cy])
                    changed = 1;
                safe_cells[map_id][cx][cy] = 0;
                monsters.pb({cx, cy});
                d[MONSTERS][cx][cy] = 0;
                q[MONSTERS].push({cx, cy});
            }
        }
        if (!silent_mode) {
            cerr << "Map id: " << map_id << endl;
            cerr << "Our score: " << our_score << endl;
            cerr << "Enemy score: " << enemy_score << endl;
        }

        q[US].push({px, py});
        d[US][px][py] = 0;

        q[ENEMY].push({ex, ey});
        d[ENEMY][ex][ey] = 0;

        // player
        bfs(US);

        // enemy
        bfs(ENEMY);

        // monsters
        bfs(MONSTERS);

        for (ll type = BONUSES; type <= IMMUNE; type++) {
            for (pll bonus:bonuses[type]) {
                x = bonus.fi;
                y = bonus.se;
                cerr << "BONuS HERE" << x << ' ' << y << ' '  << d[US][x][y] << ' ' << time_left[x][y] << endl;
                if (d[US][x][y] < time_left[x][y]) {
                    q[type].push({x, y});
                    d[type][x][y] = 0;
                }
            }
        }
        for (pll dagger:daggers) {
            x = dagger.fi;
            y = dagger.se;
            if (d[US][x][y] < d[MONSTERS][x][y] && d[US][x][y] < time_left[x][y]) {
                q[DAGGERS].push({x, y});
                d[DAGGERS][x][y] = 0;
            }
        }

        sort(coins.begin(), coins.end(), &cmp);

        for (pll coin:coins) {
            ll i = coin.fi, j = coin.se;
            ll x = i, y = j;
            bool bad = map_id != 8 && map_id != 4 && map_id != 7 && map_id != 9 && map_id != 3 && map_id != 5 && map_id != 1;
            ll cx = n / 2, cy = m / 2;
            if(!(cx - 1 <= i && i <= cx + 1 && cy - 1 <= j && j <= cy + 1 && !safe_cells[map_id][i][j])) bad = 0;
            if(map_id == 1 && (x == 0 || x == n - 1) && y == m / 2)
                bad = 1;
            if(map_id == 2 && (x == 1 || x == n - 2) && y == m / 2) bad = 1;
            if(map_id == 3 && (x == 0 && y == 0 || x == n - 1 && y == m - 1)) bad = 1;
            if(map_id == 4 && (y == 1 || y == m - 2)) bad = 1;
            if(map_id == 8 && (y == 1 && x == n - 2  || y == m - 2 && x == 1)) bad = 1;
            // good coin
            if (d[US][i][j] <= d[ENEMY][i][j] && !bad) {
                d[COINS][i][j] = 0;
                q[COINS].push({i, j});
            }
        }
        if (q[COINS].empty())
            for (pll coin:coins) {
                ll i = coin.fi, j = coin.se;
                // all coins
                d[COINS][i][j] = 0;
                q[COINS].push({i, j});
            }


        // good coins
        bfs(COINS, true);

        // daggers
        bfs(DAGGERS);

        // bonuses
        bfs(BONUSES);

        bfs(FREEZE);
        bfs(IMMUNE);

        // safe cells
        bfs(SAFE);

        // start map
        if (tick == 1) {
            ll mx = 0;
            for (pll coin:coins) {
                ll i = coin.fi, j = coin.se;
                mx = max(mx, weight[i][j]);
            }
            for (pll coin:coins) {
                ll i = coin.fi, j = coin.se;
                if (weight[i][j] != mx)
                    continue;
                q[START].push({i, j});
                d[START][i][j] = 0;
            }
            bfs(START);
        }
//

        ans = NO_ANSWER;

        cerr << "Dagger left: " << dagger_left << endl;

        go_dagger = 0;
        if (enemy_alive == 0 || (tick - last_coin > 20 && our_score - 3 <= enemy_score)) {
            go_dagger = 1;
        }
        if (monsters.empty()) {
            go_dagger = 0;
        }

        last_coins = cnt_coins;

        go_to_bonus(BONUSES);
        if(d[IMMUNE][px][py] <= 5 && bonus_left < d[IMMUNE][px][py]) go_to_bonus(IMMUNE);
        go_to_dagger();

        // try to kill a monster
        if (!enemy_alive) go_kill();

        // try to go to a coin
        go_to_coin();

        // run away from a monster
        run_away();

        cerr << "debug code" << endl;
        cerr << "player_id: " << player_id << endl;
        cerr << s[ans] << " " << px + dx[ans] << " " << py + dy[ans] << endl;
        cerr << d[COINS][px + dx[ans]][py + dy[ans]] << " " << d[COINS][px][py] << " "
             << d[MONSTERS][px + dx[ans]][py + dy[ans]] << endl;
        if (d[COINS][px + dx[ans]][py + dy[ans]] == 0)
            last_coin = tick;
        cout << s[ans] << endl;

        for (ll i = 0; i < n; i++)
            for (ll j = 0; j < m; j++)
                lc[i][j] = c[i][j];
    }

    return 0;
}
