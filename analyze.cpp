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
#define INF ll(2e9)
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
const ll N = 21;
ll n = 11, m = 13, cnt_maps, x, y, z, id, safe_cells[N][N][N], a[N][N], decoded[N][N], ans, res;
char ch;
plll id_to_hash[N];
map<plll,ll> hash_to_id;
string tostring(plll a)
{
    return "{{" + to_string(a.fi.fi) + ", " + to_string(a.fi.se) + "}, " + to_string(a.se) + "}";
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
   cout << "id_to_hash[" + to_string(id) + "] = " + tostring(id_to_hash[id]) + ";" << endl;
   plll encoded = encode(safe_cells[id]);
   cout << "init(safe_cells[" + to_string(id) + "], " + tostring(encoded) + ");" << endl;
   if (ans)
       cout << "------ CHANGED SAFE MASK ----------" << endl;
   cout << endl;
}
void init(ll a[N][N], plll b)
{
    decode(b);
    for (ll i = 0; i < n; i++)
        for (ll j = 0; j < m; j++)
            a[i][j] = decoded[i][j];
}
void init()
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
    for (ll i = 1; i <= cnt_maps; i++)
        hash_to_id[id_to_hash[i]] = i;
}
int main()
{
	freopen("mask.txt", "r", stdin);

	ios_base::sync_with_stdio(0);
	cin.tie(0);

	init();
	for (ll k = 1; k <= 10; k++)
	{
		cin >> ch >> x >> ch >> y >> ch >> z >> ch;
		id = hash_to_id[{{x, y}, z}];
        cout << id << endl;
		if (id == 0)
			cout << "Hash not found" << endl;
        ans = res = 0;
		for (ll i = 0; i < n; i++)
			for (ll j = 0; j < m; j++)
            {
				cin >> ch, a[i][j] = ch - '0';
                if (safe_cells[id][i][j] && !a[i][j])
                    ans++;
                if (!safe_cells[id][i][j] && a[i][j])
                    res++;
                safe_cells[id][i][j] &= a[i][j];
            }
        cout << ans << " " << res << endl;
        if (ans)
            print_map(id);
	}
}
/*
*/