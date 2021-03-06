/**
 * code generated by JHelper
 * More info: https://github.com/AlexeyDmitriev/JHelper
 * @author Miguel Mini
 * @tag mst, binary_lifting
 * @idea
 *      - Build maximum spanning tree of edges 
 *      
 *      - to improve, we can take edges (i * k, (i + 1) * k, k)
 *      these nodes are co-primes
 *      
 *      - the answer is the minimum edge in unique path
 *      from a to b in that spanning tree.
 * 
 * @complexity O((n + m) log n)
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
const int maxn = 1e5 + 10;
vector<ii> g[maxn];
int link[maxn];
int rnk[maxn];

void init(int x) {
    link[x] = x;
    rnk[x] = 0;
}

int get(int x) {
    if (x == link[x]) return x;
    return link[x] = get(link[x]);
}

bool merge(int x, int y) {
    x = get(x); y = get(y);
    if (x == y) return 0;
    if (rnk[x] < rnk[y]) {
        swap(x, y);
    }
    rnk[x] += rnk[x] == rnk[y];
    link[y] = x;
    return 1;
}

const int maxlog = 18;
int st[maxn][maxlog];
int min_edge[maxn][maxlog];
int height[maxn];

void dfs(int x, int p, int h = 0, int e = 0) {
    height[x] = h;
    st[x][0] = p;
    min_edge[x][0] = e;
    for (int k = 1; k < maxlog; ++k) {
        st[x][k] = st[st[x][k-1]][k-1];
        min_edge[x][k] = min(min_edge[x][k-1], min_edge[st[x][k-1]][k-1]);
    }
    for (auto node : g[x]) {
        if (node.first == p) continue;
        dfs(node.first, x, h + 1, node.second);
    }
}

int jump(int x, int len) {
    for (int k = 0; k < maxlog; ++k) {
        if (len & (1<<k)) {
            x = st[x][k];
        }
    }
    return x;
}

int lca(int x, int y) {
    if (height[x] > height[y]) swap(x, y);
    y = jump(y, height[y] - height[x]);
    if (x == y) return x;
    for (int k = maxlog-1; k >= 0; --k) {
        if (st[x][k] != st[y][k]) {
            x = st[x][k];
            y = st[y][k];
        }
    }
    return st[x][0];
}

int min_edge_jump(int x, int len) {
    int ans = 1e9;
    for (int k = 0; k < maxlog; ++k) {
        if (len & (1<<k)) {
            ans = min(ans, min_edge[x][k]);
            x = st[x][k];
        }
    }
    return ans;
}

class COCI_18_Pictionary {
public:

    void solveOne(istream& in, ostream& out) {
        int n, m, q;
        in >> n >> m >> q;
        re(i, 1, n + 1) {
            g[i].clear();
            init(i);
        }
        for (int i = m; i >= 1; --i) {
            for (int j = i; j + i <= n; j += i) {
                if (merge(j, j + i)) {
                    //out << j << " " << j + i << " " << i << endl;
                    g[j].push_back({j + i, i});
                    g[j + i].push_back({j, i});
                }
            }
        }
        dfs(1, 1);
        re(i, 0, q) {
            int a, b;
            in >> a >> b;
            int c = lca(a, b);
            int ans = min_edge_jump(a, height[a] - height[c]);
            ans = min(ans, min_edge_jump(b, height[b] - height[c]));
            out << max(1, m - ans + 1) << endl;
        }
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
	COCI_18_Pictionary solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}
