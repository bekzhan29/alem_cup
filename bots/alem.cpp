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
typedef pair<ll,ll> pll;
typedef pair<pll,ll> plll;
typedef pair<double,double> pdd;
const ll N = 21, K = 8, US = 0, ENEMY = 1, COINS = 2, MONSTERS = 3, DAGGERS = 4, BONUSES = 5, SAFE = 6, START = 7;
double DEC_PW =1.2;
const ll RIGHT = 0, DOWN = 1, LEFT = 2, UP = 3, STAY = 4, NO_ANSWER = -1;
ll n = 11, m = 13, px, py, ex, ey, x, y, d[K][N][N], ans, neighbors[N][N], player_id, tick, safe_column[N];
ll dx[5] = {0, 1, 0, -1, 0}, dy[5] = {1, 0, -1, 0, 0}, ord[5];
ll cnt_coins, last_coins, dagger_left, bonus_left, our_score, enemy_score, cur_mid, mid_coins;
pll pr[K][N][N];
bool block_monsters = 0;
vector<pll> daggers, bonuses, monsters, coins;
queue<pll> q[K];
char c[N][N], lc[N][N], start_c[N][N];
ll bad_angle[N][N], near_monster[N][N], weight[N][N];
string s[5] = {"right", "down", "left", "up", "stay"};
bool enemy_alive, go_dagger, left_right, lleft, rright;
ll last_coin = -1, cnt_maps, last_cnt_maps, map_id;
plll map_hash;
map<plll, ll> hash_to_id;
plll id_to_hash[N];
ll safe_cells[N][N][N], decoded[N][N], changed;
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());


const bool beast_mode = 0, are_you_sure = 0;
const bool silent_mode = 1;


ll in_box(ll x, ll y)
{
    return (0 <= x && x < n && 0 <= y && y < m);
}
ll border_dist(ll x, ll y)
{
    return min({x, y, n - x - 1, m - y - 1});
}
bool mid_zone(int x, int y)
{
    return x > 1 && x < n - 2 && y * 2 + 1 == m;
}
ll dist(pll a, pll b)
{
    return abs(a.fi - b.fi) + abs(a.se - b.se);
}
ll dist(ll x1, ll y1, ll x2, ll y2)
{
    return dist({x1, y1}, {x2, y2});
}
bool cmp(pll a, pll b)
{
    return weight[a.fi][a.se] > weight[b.fi][b.se];
}
string tostring(plll a)
{
    return "{{" + to_string(a.fi.fi) + ", " + to_string(a.fi.se) + "}, " + to_string(a.se) + "}";
}
void init_DEC_PW()
{
    if (map_id == 4)
        DEC_PW = 1.2;
    if (map_id == 5 || map_id == 9)
        DEC_PW = 1.6;
    if (map_id >= 6 && map_id <= 8)
        DEC_PW = 2;
}
plll encode(char c[N][N])
{
    ll h1 = 0, h2 = 0, h3 = 0;
    for (ll i = 0; i < n; i++)
        for (ll j = 0; j < m; j++)
        {
            if (i < 4 && c[i][j] == '!')
                h1 |= (1LL << (i * m + j));
            else if (4 <= i && i < 8 && c[i][j] == '!')
                h2 |= (1LL << ((i - 4) * m + j));
            else if (8 <= i && c[i][j] == '!')
                h3 |= (1LL << ((i - 8) * m + j));
        }
    return {{h1, h2}, h3};
}
plll encode(ll a[N][N])
{
    ll h1 = 0, h2 = 0, h3 = 0;
    for (ll i = 0; i < n; i++)
        for (ll j = 0; j < m; j++)
        {
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
void decode(plll a)
{
    ll h1 = a.fi.fi, h2 = a.fi.se, h3 = a.se, cur = 0, x;
    for (ll i = 0; i < n; i++)
        for (ll j = 0; j < m; j++)
        {
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
void print_map(ll id)
{
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
void print_safe_cells(ll id)
{
    if (silent_mode) return;
//    for (ll i = 0; i < n; i++, cerr << endl)
//        for (ll j = 0; j < m; j++)
//            cerr << safe_cells[id][i][j];
}

void and_mask(ll a[N][N], ll b[N][N])
{
    for (ll i = 0; i < n; i++)
        for (ll j = 0; j < m; j++)
        {
            if (a[i][j] && !b[i][j])
                changed = 1;
            a[i][j] &= b[i][j];
        }
}

void init(ll a[N][N], plll b)
{
    decode(b);
    for (ll i = 0; i < n; i++)
        for (ll j = 0; j < m; j++)
            a[i][j] = decoded[i][j];
}

void bfs(ll cur, bool is_coin = false)
{
    ll x, y, tx, ty;
    for (;!q[cur].empty();)
    {
        x = q[cur].front().fi;
        y = q[cur].front().se;
        q[cur].pop();
        for (ll i = 0; i < 4; i++)
        {
            tx = x + dx[i];
            ty = y + dy[i];
            bool fail = 0;
            for (auto &M : monsters) {
                if (dist({tx, ty}, M) <= 1)
                    fail = 1;
            }
            if(fail && block_monsters && is_coin)
                continue;

            if (in_box(tx, ty) && c[tx][ty] != '!')
                if (d[cur][tx][ty] > d[cur][x][y] + 1)
                {
                    d[cur][tx][ty] = d[cur][x][y] + 1;
                    q[cur].push({tx, ty});
                    pr[cur][tx][ty] = {x, y};
                }
        }
    }
}
void init_safe_cells()
{
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
bool is_safe1(int x, int y)
{
    for (ll i = 0; i < 4; i++)
    {
        ll tx = x + dx[i], ty = y + dy[i];
        if (in_box(tx, ty) && c[tx][ty] != '!' && d[MONSTERS][tx][ty] >= 4)
            return 1;
    }
    return 0;
}
bool is_safe(int x, int y)
{
    if (beast_mode && are_you_sure && player_id == 2)
        return (d[MONSTERS][x][y] > 2);
    if (safe_cells[map_id][x][y] || dagger_left > 3)
        return 1;
    return is_safe1(x, y);
}
void count_empty_neighbors()
{
    for (ll i = 0; i < n; i++)
        for (ll j = 0; j < m; j++)
        {
            neighbors[i][j] = 0;
            for (ll k = 0; k < 4; k++)
                if (in_box(i + dx[k], j + dy[k]) && c[i + dx[k]][j + dy[k]] != '!')
                    neighbors[i][j]++;
            if(neighbors[i][j] == 2)
            {
                for(int k = 1; k <= 4; k++)
                {
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
        vector <vector<int>> dist(n, vector<int>(m, -1));
        int cnt = 0;
        dist[x][y] = 0;
        queue <pair<int,int>> cur_q;
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

void calculate_weights()
{
    ll r = 5;
    for (ll i = 0; i < n; i++)
        for (ll j = 0; j < m; j++)
            weight[i][j] = bfs_distance :: get_weight(i, j, r);
}
void go_to_bonus()
{
    if (d[BONUSES][px][py] >= 300 - tick)
        return;
    if (!bonuses.empty())
    {
        for (pll bonus:bonuses)
        {
            x = bonus.fi;
            y = bonus.se;
            // TODO: maybe change
            if ((d[BONUSES][px][py] < d[MONSTERS][x][y] || d[MONSTERS][px][py] > 4) && d[BONUSES][px][py] <= 10)
            {
                shuffle(ord, ord + 4, rnd);
                for (ll j = 0; j < 4; j++)
                {
                    ll i = ord[j], tx = px + dx[i], ty = py + dy[i];
                    if (in_box(tx, ty) && d[BONUSES][tx][ty] + 1 == d[BONUSES][px][py] && is_safe(tx, ty))
                    {
                        ans = i;
                        if (!silent_mode) {
                            cerr << "Moving towards bonus" << endl;
                        }
                    }
                }
            }
        }
    }
}
void go_to_dagger()
{
    if (d[DAGGERS][px][py] >= 300 - tick)
        return;
    if (go_dagger && ans == NO_ANSWER && !monsters.empty() && !daggers.empty())
    {
        for (pll dagger:daggers)
        {
            x = dagger.fi;
            y = dagger.se;
            if ((d[DAGGERS][px][py] < d[MONSTERS][x][y] || d[MONSTERS][px][py] > 4) && d[DAGGERS][px][py] < 15)
            {
                shuffle(ord, ord + 4, rnd);
                for (ll j = 0; j < 4; j++)
                {
                    ll i = ord[j], tx = px + dx[i], ty = py + dy[i];
                    if (in_box(tx, ty) && d[DAGGERS][tx][ty] + 1 == d[DAGGERS][px][py] && is_safe(tx, ty))
                    {
                        ans = i;
                        if (!silent_mode) {
                            cerr << "Moving towards dagger" << endl;
                        }
                    }
                }
            }
        }
    }
}
void go_kill()
{
    if (d[MONSTERS][px][py] >= 300 - tick)
        return;
    if (ans == NO_ANSWER && dagger_left > 3 && go_dagger)
    {
        ans = STAY;
        shuffle(ord, ord + 4, rnd);
        for (ll j = 0; j < 4; j++)
        {
            ll i = ord[j];
            x = px + dx[i];
            y = py + dy[i];
            if (in_box(x, y) && c[x][y] != '!')
                if (d[MONSTERS][x][y] < d[MONSTERS][px + dx[ans]][py + dy[ans]])
                {
                    ans = i;
                    if (!silent_mode) {
                        cerr << "Moving towards monster" << endl;
                    }
                }
        }
    }
}
double cost[N][N];
int dc[N][N];

void go_to_coin()
{
    if (ans != NO_ANSWER)
        return;
    if (cnt_coins <= 1 && our_score > enemy_score + 1 && enemy_alive)
    {
//        cerr<<our_score << ' ' << enemy_score <<' ' << enemy_alive << ' ' << cnt_coins << endl;
        x = pr[START][px][py].fi;
        y = pr[START][px][py].se;
        if (in_box(x, y) && c[x][y] != '!' && is_safe(x, y))
        {
            for (ll i = 0; i < 4; i++)
                if (px + dx[i] == x && py + dy[i] == y)
                    ans = i;
        }
        else if (x == -1 && y == -1 && is_safe(px, py))
        {
            cerr << "Pears" << endl;
            ans = STAY;
        }
        return;
    }
    if (map_id == 4 || map_id == 5 || map_id == 7)
    {
        if (map_id != 4 || (map_id == 4 && d[COINS][px][py] >= 2 && d[COINS][px][py] <=  7 && d[MONSTERS][px][py] >= 4)) {
            shuffle(ord, ord + 5, rnd);
            for (ll j = 0; j <= 4; j++) {
                ll i = ord[j];
                x = px + dx[i];
                y = py + dy[i];
                if (in_box(x, y) && c[x][y] != '!' && is_safe(x, y))
                    if (ans == NO_ANSWER || cost[x][y] > cost[px + dx[ans]][py + dy[ans]] || cost[x][y] == cost[px + dx[ans]][py + dy[ans]] && d[COINS][x][y] < d[COINS][px + dx[ans]][py + dy[ans]])
                        ans = i;
            }
            if(ans != NO_ANSWER)
                return;
        }
    }
    x = pr[COINS][px][py].fi;
    y = pr[COINS][px][py].se;
    if (in_box(x, y) && c[x][y] != '!' && is_safe(x, y))
    {
        for (ll i = 0; i < 4; i++)
            if (px + dx[i] == x && py + dy[i] == y)
                ans = i;
        return;
    }
    shuffle(ord, ord + 4, rnd);
    for (ll j = 0; j < 4; j++)
    {
        ll i = ord[j];
        x = px + dx[i];
        y = py + dy[i];
        if (in_box(x, y) && c[x][y] != '!')
            if (d[COINS][x][y] + 1 == d[COINS][px][py] && is_safe(x, y))
            {
                ans = i;
                if (!silent_mode) {
                    cerr << "Moving towards coin: " << x << " " << y << " " << d[MONSTERS][x][y] << " " << neighbors[x][y] << endl;
                }
            }
    }
}
void run_away()
{
    if (ans == NO_ANSWER)
    {
        shuffle(ord, ord + 4, rnd);
        for (ll j = 0; j <= 4; j++)
        {
            ll i = ord[j];
            x = px + dx[i];
            y = py + dy[i];
            if (in_box(x, y) && c[x][y] != '!' && safe_cells[map_id][x][y])
            {
                ans = i;
                if (!silent_mode) {
                    cerr << "Moving towards safe column " << x << " " << y << endl;
                }
            }
        }
    }

    if (ans == NO_ANSWER)
    {
        shuffle(ord, ord + 4, rnd);
        for (ll j = 0; j <= 4; j++)
        {
            ll i = ord[j];
            x = px + dx[i];
            y = py + dy[i];
            if (in_box(x, y) && c[x][y] != '!')
                if(neighbors[x][y] > 1 && is_safe(x, y))
                {
                    ans = i;
                    if (!silent_mode) {
                        cerr << "Moving away from monster to safe cell: " << x << " " << y << endl;
                    }
                }
        }
    }

    if (ans == NO_ANSWER)
    {
        ll mx = 0, mn = INF;
        shuffle(ord, ord + 4, rnd);
        for (ll j = 0; j <= 4; j++)
        {
            ll i = ord[j];
            x = px + dx[i];
            y = py + dy[i];
            if (in_box(x, y) && c[x][y] != '!')
                if(neighbors[x][y] > 1)
                    if(d[MONSTERS][x][y] > mx || (d[MONSTERS][x][y] == mx && d[SAFE][x][y] < mn))
                    {
                        ans = i;
                        mx = d[MONSTERS][x][y];
                        mn = d[SAFE][x][y];
                        if (!silent_mode) {
                            cerr << "Moving away from monster: " << x << " " << y << " " << d[MONSTERS][x][y] << " " << d[SAFE][x][y] << endl;
                        }
                    }
        }
    }
    if (d[MONSTERS][px][py] == 2 && beast_mode && are_you_sure && ans == NO_ANSWER && player_id == 2)
    {
        for (pll monster:monsters)
        {
            x = monster.fi;
            y = monster.se;
            // upper left
            if (x == px - 1 && y == py - 1 && c[px - 1][py] != '!' && c[px][py - 1] != '!')
            {
                ans = LEFT;
            }
            // upper right
            if (x == px - 1 && y == py + 1 && c[px - 1][py] != '!' && c[px][py + 1] != '!')
            {
                ans = UP;
            }
            // bottom left
            if (x == px + 1 && y == py - 1 && c[px + 1][py] != '!' && c[px][py - 1] != '!')
            {
                ans = LEFT;
            }
            // bottom right
            if (x == px + 1 && y == py + 1 && c[px + 1][py] != '!' && c[px][py + 1] != '!')
            {
                ans = RIGHT;
            }
        }
        if (ans != NO_ANSWER)
            cerr << "BEAST MODE" << endl;
    }
}
inline void clean_costs() {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cost[i][j] = 0;
}
inline void make_costs(int x, int y, double multi) {
    queue<pair<int, int> > mq;
    if(d[MONSTERS][x][y] <= 1) return;
    mq.push({x, y});
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            dc[i][j] = INF;
    dc[x][y] = 0;
    while (!mq.empty()) {
        int x = mq.front().first;
        int y = mq.front().second;
        mq.pop();
        for (ll i = 0; i < 4; i++)
        {
            int tx = x + dx[i];
            int ty = y + dy[i];
            if (in_box(tx, ty) && c[tx][ty] != '!'&&d[MONSTERS][tx][ty] > 1)
                if (dc[tx][ty] > dc[x][y] + 1)
                    dc[tx][ty] = dc[x][y] + 1, mq.push({tx, ty});
        }
    }
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if (dc[i][j] < INF)
                cost[i][j] += multi * pow(DEC_PW, -dc[i][j]);
}
inline void unmake(int x, int y) {
    queue<pair<int, int > > mq;
    mq.push({x, y});
    for (int i = 0; i < n; ++i)
        for (int j = 0 ; j < m; ++j)
            dc[i][j] = INF;
    dc[x][y] = 0;
    while (!mq.empty()) {
        int x = mq.front().first;
        int y = mq.front().second;
        mq.pop();
        for (ll i = 0; i < 4; i++)
        {
            int tx = x + dx[i];
            int ty = y + dy[i];
            if (in_box(tx, ty) && c[tx][ty] != '!')
                if (dc[tx][ty] > dc[x][y] + 1)
                    dc[tx][ty] = dc[x][y] + 1, mq.push({tx, ty});
        }
    }
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if (dc[i][j] < INF)
                cost[i][j] -= pow(DEC_PW, -dc[i][j]);
}
int main()
{
    for (ll i = 0; i < m; i++)
        safe_column[i] = 1;
    for (ll i = 0; i < 5; i++)
        ord[i] = i;
    init_safe_cells();
    for(;;)
    {
        cin >> m >> n >> player_id >> tick;
        cerr << n << " " << m << " " << player_id << " " << tick << endl;
        // if(player_id == 2) {
        // } else DEC_PW = 1.8;
        block_monsters = 1;
        clean_costs();
        // read map
        coins.clear();
        daggers.clear();
        bonuses.clear();
        cnt_coins = 0;

        for (ll k = 0; k < K; k++)
        {
            if (k == START && tick > 1)
                break;
            for (ll i = 0; i < n; i++)
                for (ll j = 0; j < m; j++)
                {
                    d[k][i][j] = INF;
                    pr[k][i][j] = {-1, -1};
                }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                cin >> c[i][j];
                if (tick == 1)
                    start_c[i][j] = c[i][j];
                near_monster[i][j] = 0;
                // dagger
                if (c[i][j] == 'd')
                {
                    d[DAGGERS][i][j] = 0;
                    q[DAGGERS].push({i, j});
                    daggers.pb({i, j});
                }
                // bonus
                if (c[i][j] == 'b')
                {
                    d[BONUSES][i][j] = 0;
                    q[BONUSES].push({i, j});
                    bonuses.pb({i, j});
                }
                // coin
                if (c[i][j] == '#')
                {
                    cnt_coins++;
                    coins.pb({i, j});
                }
            }
        }
        last_coins = cnt_coins;
        cerr << endl;
        if (tick == 1) {
            int cx = n / 2;
            int cy = m / 2;
            map_hash = encode(start_c);
            if (!hash_to_id.count(map_hash))
            {
                hash_to_id[map_hash] = ++cnt_maps;
                id_to_hash[cnt_maps] = map_hash;
            }
            map_id = hash_to_id[map_hash];
            init_DEC_PW();
            mid_coins = 0;
            for(ll i = cx - 1; i <= cx + 1; ++i) {
                for (ll j = cy - 1; j <= cy + 1; ++j) {
                    if (c[i][j] == '#')
                        ++mid_coins;
                }
            }
        }
        if (player_id <= 2) {
            int cx = n / 2;
            int cy = m / 2;
            cur_mid = 0;

            for(ll i = cx - 1; i <= cx + 1; ++i) {
                for(ll j = cy - 1; j <= cy + 1; ++j) {
                    if (c[i][j] == '#') {
                        ++cur_mid;
                    }
                }
            }
            if(cnt_coins > cur_mid + 2 && mid_coins <= 4 && monsters.size() > 0 && 
                map_id != 8 && map_id != 4 && map_id != 6 && map_id != 7) {
                for(ll i = cx - 1; i <= cx + 1; ++i) {
                    for(ll j = cy - 1; j <= cy + 1; ++j) {
                        if (c[i][j] == '#') {
                            coins.erase(find(coins.begin(), coins.end(), make_pair(i, j)));
                        }
                    }
                }
            }
        }

        for (ll i = 0; i < n; i++)
            for (ll j = 0; j < m; j++)
                if (safe_cells[map_id][i][j])
                {
                    d[SAFE][i][j] = 0;
                    q[SAFE].push({i, j});
                }

        print_safe_cells(map_id);

        cerr << endl;

        if (!silent_mode) {
            cerr << "Hash: " << tostring(map_hash) << endl;
        }

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
            if (type == "p")
            {
                // us
                if (p_id == player_id)
                {
                    px = cx, py = cy;
                    our_score += ((lc[px][py] == '#') + (cnt_coins > last_coins && start_c[cx][cy] == '#')) * (param_2 + 1);
                    if (lc[px][py] == 'd' && param_1)
                        dagger_left = 15;
                    else
                        dagger_left--;
                    dagger_left = max(dagger_left, 0LL);
                    if (lc[px][py] == 'b' && param_2)
                        bonus_left = 30;
                    else
                        bonus_left--;
                    bonus_left = max(bonus_left, 0LL);
                }

                // enemy
                if (p_id != player_id)
                {
                    enemy_alive = 1;
                    ex = cx;
                    ey = cy;
                    enemy_score += ((lc[ex][ey] == '#') + (cnt_coins > last_coins && start_c[cx][cy] == '#')) * (param_2 + 1);
                }
            }
            // monster
            if (type == "m")
            {
                if (safe_cells[map_id][cx][cy])
                    changed = 1;
                safe_cells[map_id][cx][cy] = 0;
                monsters.pb({cx, cy});
                for (ll xx = max(0LL, cx - 1); xx < min(n, cx + 2); xx++)
                    for (ll yy = max(0LL, cy - 1); yy < min(m, cy + 2); yy++)
                    {
                        near_monster[xx][yy] = 1;
                        // if (player_id == 2)
                        //  d[COINS][xx][yy] = INF;
                    }
                d[MONSTERS][cx][cy] = 0;
                q[MONSTERS].push({cx, cy});
                if (tick == 1)
                {
                    for(ll j = max(0LL, cy - 3); j <= min(m - 1, cy + 3); j++)
                        safe_column[j] = 0;
                    if (cy < 5 && abs(cx - 5) <= 2)
                        lleft = 1;
                    if (cy > 7 && abs(cx - 5) <= 2)
                        rright = 1;
                }
            }
            cerr << type << " " << p_id << " " << cx << " " << cy << " " << param_1 << " " << param_2 << endl;
        }
        if (monsters.size() == 2 && map_id == 2)
            c[3][6] = c[7][6] = '!';
        cerr << "Player pos: " << px << " " << py << endl;

        if (!silent_mode) {
            cerr << "Map id: " << map_id << endl;
            cerr << "Our score: " << our_score << endl;
            cerr << "Enemy score: " << enemy_score << endl;
        }

        if (tick == 1 && lleft && rright)
        {
            left_right = 1;
            for (ll j = 0; j < m; j++)
                safe_column[j] = 0;
            safe_column[m / 2] = 1;
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
        // if (player_id == 2)
        sort(coins.begin(), coins.end(), &cmp);
        for (pll coin:coins)
        {
            ll i = coin.fi, j = coin.se;
            // good coin
            if (d[US][i][j] <= d[ENEMY][i][j])
            {
                d[COINS][i][j] = 0;
                q[COINS].push({i, j});
                make_costs(i, j, +1);
            }
        }
        if (q[COINS].empty())
            for (pll coin:coins)
            {
                ll i = coin.fi, j = coin.se;
                // all coins
                d[COINS][i][j] = 0;
                q[COINS].push({i, j});
                make_costs(i, j, +1);
            }

        if (!silent_mode)
        {
            for(int i = 0; i < n; ++i, cerr<<endl)
                for(int j = 0; j < m; ++j)
                    cerr<<cost[i][j] <<' ';
        }

        // good coins
        bfs(COINS, true);

        // daggers
        bfs(DAGGERS);

        // bonuses
        bfs(BONUSES);

        // safe cells
        bfs(SAFE);

        // start map
        if (tick == 1)
        {
            ll mx = 0;
            for (pll coin:coins)
            {
                ll i = coin.fi, j = coin.se;
                mx = max(mx, weight[i][j]);
            }
            for (pll coin:coins)
            {
                ll i = coin.fi, j = coin.se;
                if (weight[i][j] != mx)
                    continue;
                q[START].push({i, j});
                d[START][i][j] = 0;
            }
            bfs(START);
        }
//        if(player_id == 2) {
//            for (auto &x : monsters) {
//                unmake(x.first, x.second);
//            }
//            unmake(ex, ey);
//        }


        ans = NO_ANSWER;

        cerr << "Dagger left: " << dagger_left << endl;

        go_dagger = 0;
        if (enemy_alive == 0 || (tick - last_coin > 35 && our_score - 3 <= enemy_score)) {
            go_dagger = 1;
        }
        if (monsters.empty()) {
            go_dagger = 0;
        }

        if (map_id == 1 || map_id == 3)
        {
            // try to go to a dagger
            go_to_dagger();

            // try to go to a bonus
            go_to_bonus();
        }
        else
        {
            // try to go to a bonus
            go_to_bonus();

            // try to go to a dagger
            go_to_dagger();
        }

        // try to kill a monster
        if (!enemy_alive && (map_id == 1 || map_id == 3))
            go_kill();
        else if (map_id != 1 && map_id != 3)
            go_kill();

        // try to go to a coin
        go_to_coin();

        // run away from a monster
        run_away();

        cerr << "debug code" << endl;
        cerr << "player_id: " << player_id << endl;
        cerr << s[ans] << " " << px + dx[ans] << " " << py + dy[ans] << endl;
        cerr << d[COINS][px + dx[ans]][py + dy[ans]] << " " << d[COINS][px][py] << " " << d[MONSTERS][px + dx[ans]][py + dy[ans]] << endl;
        if (d[COINS][px + dx[ans]][py + dy[ans]] == 0)
            last_coin = tick;
        // bot action
        cout << s[ans] << endl;
        // if (!silent_mode) {
        //     cerr.precision(3);
        //     for (ll i = 0; i < n; i++, cerr << endl)
        //         for (ll j = 0; j < m; j++) {
        //             cerr << fixed << cost[i][j] << ' ';
        //         }
        // }

        for (ll i = 0; i < n; i++)
            for (ll j = 0; j < m; j++)
                lc[i][j] = c[i][j];
    }

    return 0;
}
