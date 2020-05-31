/**
 * code generated by JHelper
 * More info: https://github.com/AlexeyDmitriev/JHelper
 * @author Miguel Mini
 * @tag dfs, dp, segment tree
 * @idea
 *      - get the unique cycle
 *      - solve to every tree with root in the cycle
 *      - answer with a sweep windows the maximum length with
 *      fixed root, I use a segment tree to find this.
 */

#include <bits/stdc++.h>
#define sz(x) (int)x.size()
#define trav(v, x) for (auto v : x)
#define re(x, y, z) for (int x=y; x<z; ++x)
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()
#define set_to(x, v) fill(all(x), v)
#define eb emplace_back
#define lso(x) ((x)&-(x))
using namespace std;
using ll = long long;
using ii = pair<int, int>;
using vi = vector<int>;
const int mod = 1e9 + 7;
const int maxn = 2e5 + 10;
vi g[maxn];

int state[maxn];
vi path, cycle;
int m;
void get_cycle(int x, int p) {
    state[x] = 1;
    path.eb(x);
    trav(v, g[x]) {
        if (v == p) continue;
        if (state[v] == 0) {
            get_cycle(v, x);
        } else if (state[v] == 1) {
            for (int i = sz(path)-1; i >= 0; --i) {
                cycle.eb(path[i]);
                if (path[i] == v) break;
            }
        }
    }
    path.pop_back();
    state[x] = 2;
}

using T = pair<int, ll>;
T maxi = {-1, 0};
bool vis[maxn];
T maxLen[maxn];
vector<T> memo[maxn];
T id = {0, 1ll};
void dfs(int x) {
    memo[x] = {id};
    vis[x] = 1;
    trav(v, g[x]) {
        if (vis[v]) continue;
        dfs(v);
        auto q = maxLen[v];
        q.first += 1;
        memo[x].eb(q);
        sort(rall(memo[x]));
        int temp_len = memo[x][0].first + memo[x][1].first;
        if (temp_len > maxi.first) {
            maxi.first = temp_len;
            maxi.second = memo[x][0].second * memo[x][1].second;
        } else if (temp_len == maxi.first) {
            maxi.second += memo[x][0].second * memo[x][1].second;
        }
        if (memo[x][0].first == memo[x][1].first) {
            memo[x][0].second += memo[x][1].second;
        }
        memo[x].pop_back();
    }
    maxLen[x] = memo[x][0];
}

T merge(T p, T q) {
    if (p.first < q.first) return q;
    if (p.first > q.first) return p;
    p.second += q.second;
    return p;
}

T st[8 * maxn];
int lazy[8 * maxn];
void build(int nx, int l, int r) {
    if (l == r) {
        st[nx] = maxLen[cycle[r % m]];
        st[nx].first += abs(r - (m - 1) / 2);
        return;
    }
    int mid = (l + r) / 2;
    build(nx << 1, l, mid);
    build(nx << 1 | 1, mid+1, r);
    st[nx] = merge(st[nx<<1], st[nx<<1|1]);
}

void unit_upd(int nx, int delta) {
    lazy[nx] += delta;
    st[nx].first += delta;
}

void push(int nx) {
    unit_upd(nx<<1, lazy[nx]);
    unit_upd(nx<<1|1, lazy[nx]);
    lazy[nx] = 0;
}

void update(int ll, int rr, int delta, int nx, int l, int r) {
    if (rr < l or r < ll) return;
    if (ll <= l and r <= rr) {
        unit_upd(nx, delta);
        return;
    }
    int mid = (l + r) / 2;
    push(nx);
    update(ll, rr, delta, nx<<1, l, mid);
    update(ll, rr, delta, nx<<1|1, mid+1, r);
    st[nx] = merge(st[nx<<1], st[nx<<1|1]);
}

T query(int ll, int rr, int nx, int l, int r) {
    if (rr < l or r < ll) return {-1, 0};
    if (ll <= l and r <= rr) return st[nx];
    int mid = (l+r) / 2;
    push(nx);
    return merge(
                query(ll, rr, nx<<1, l, mid),
                query(ll, rr, nx<<1|1, mid+1, r)
            );
}

class IZHO_14_Shymbulak {
public:
    void solveOne(istream& in, ostream& out) {
        int n;
        in >> n;
        re(i, 0, n) {
            int a, b;
            in >> a >> b;
            g[a].eb(b);
            g[b].eb(a);
        }
        get_cycle(1, 0);
        trav(v, cycle) vis[v] = 1;
        trav(v, cycle) dfs(v);
        //trav(v, cycle) {
        //    out << maxLen[v].first << " " << maxLen[v].second << endl;
        //}
        maxi.second *= 2;
        //out << maxi.first << " " << maxi.second << endl;
        m = sz(cycle);
        build(1, 0, 2 * m - 1);
        int pos = (m - 1) / 2;
        for (int i = 0; i < m; ++i) {
            auto L = query(i, pos+i-1, 1, 0, 2 * m - 1);
            auto R = query(pos+i+1, m-1+i, 1, 0, 2 * m - 1);
            if (L.first < R.first) swap(L, R);
            if (L.first == R.first) L.second += R.second;
            if (m % 2 == 0) {
                R = query(m-1+i, m-1+i, 1, 0, 2 * m - 1);
                if (L.first == R.first) L.second += R.second;
            }
            auto M = maxLen[cycle[(pos + i) % m]];
            int temp = M.first + L.first;
            if (temp > maxi.first) {
                maxi.first = temp;
                maxi.second = M.second * L.second;
            } else if (temp == maxi.first) {
                maxi.second += M.second * L.second;
            }
            update(0, pos+i, 1, 1, 0, 2 * m - 1);
            update(pos+i+1, 2*m-1, -1, 1, 0, 2 * m - 1);
        }
        //out << maxi.first << endl;
        out << maxi.second / 2 << endl;
    }

    void solve(istream& in, ostream& out) {
        int testNumber = 1;
        //in >> testNumber;
        re(tc, 1, testNumber+1) {
            //out << "Case #" << tc << ": ";
            solveOne(in, out);
        }
    }
};


int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
	IZHO_14_Shymbulak solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}