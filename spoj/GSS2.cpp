#include <bits/stdc++.h>
using namespace std;
const int MAX_N = 1e5 + 10;
struct Node 
{
	long long sum, lazy, sumH, lazyH;
} st[MAX_N<<3];
int a[MAX_N];
long long ans[MAX_N];
int n, q;
void upd (int nx, long long x) 
{
	Node& p = st[nx];
	p.lazy += x;
	p.lazyH = max(p.lazyH, p.lazy);
	p.sum += x;
	p.sumH = max(p.sumH, p.sum);
}
void push (int nx) 
{
	Node& p = st[nx];
	upd(nx<<1, p.lazyH);
	upd(nx<<1, p.lazy - p.lazyH);	
	upd(nx<<1|1, p.lazyH);
	upd(nx<<1|1, p.lazy - p.lazyH);	
	p.lazy = p.lazyH = 0;
} 
Node merge(Node p, Node q) {	
	bool less = p.sumH < q.sumH;
	return {less ? q.sum : p.sum, 0ll, less ? q.sumH : p.sumH , 0ll};
}
void update(int a, int b, int c, int nx = 1, int l = 1, int r = n) 
{
	if (r < a or b < l) return;
	if (a <= l and r <= b) {
		upd(nx, c);	
		return;
	}
	int mid = (l+r)>>1;
	push(nx);
	update(a, b, c, nx<<1, l, mid);	
	update(a, b, c, nx<<1|1, mid+1, r);
	st[nx] = merge(st[nx<<1], st[nx<<1|1]);
}
long long query (int a, int b, int nx = 1, int l = 1, int r = n) 
{
	if (r < a or b < l) return 0;
	if (a <= l and r <= b) return st[nx].sumH;
	int mid = (l+r)>>1;
	push(nx);
	long long L = query(a, b, nx<<1, l, mid);
	long long R = query(a, b, nx<<1|1, mid+1, r);	
	st[nx] = merge(st[nx<<1], st[nx<<1|1]);
	return max(L, R);
}

struct Query 
{
	int l, r, id;
	bool operator<(const Query p) const {
		return r < p.r;
	}
} Q[MAX_N];

int main () {
	scanf ("%d", &n);
	for (int i = 1; i <= n; ++i) {
		scanf ("%d", a+i);
	}
	scanf ("%d", &q);
	for (int i = 0; i < q; ++i) {
		int l, r;
		scanf ("%d %d", &l, &r);
		Q[i] = {l, r, i};
	}
	sort (Q, Q+q);
	map<int, int> id;
	for (int j = 1, i = 0; i < q; ++i) {
		while (j <= Q[i].r) {
			update(id[a[j]]+1, j, a[j]);
			id[a[j]] = j;	
			j += 1;
		}
		ans[Q[i].id] = query (Q[i].l, Q[i].r);
	}
	for (int i = 0; i < q; ++i) {
		printf ("%lld\n", ans[i]);
	}
	return 0;
}