#include <iostream>
#include <cstdio>
#include <algorithm>
#include <climits>
using namespace std;

const int N = 200005;
int n, k, s;
long long t;
long long c[N], v[N], g[N], a[N];

bool p(long long x){

	long long acum = 0;
	for(int i = 0; i <= k; ++i)
		if(a[i] > x) return false;
		else acum += 2LL*a[i] - min(a[i], x-a[i]);
	return acum <= t;
}

int main(){

	scanf("%d %d %d %lld", &n, &k, &s, &t);
	for(int i = 0; i < n; ++i)
		scanf("%lld %lld", c+i, v+i);

	for(int i = 0; i < k; ++i)
		scanf("%lld", g+i);
	
	sort(g, g+k);
	a[0] = g[0];
	for(int i = 1; i < k; ++i)
		a[i] = g[i] - g[i-1];
	a[k] = s - g[k-1];
	
	int lo = 0, hi = 1000000000;
	while(lo < hi){
		int mid = lo + (hi-lo)/2;
		if(p(mid)) hi = mid;
		else lo = mid+1;
	}

	if(p(lo)){
		long long ans = INT_MAX;
		for(int i = 0; i < n; ++i)
			if(v[i] >= lo) ans = min(ans, c[i]);

		if(ans != INT_MAX) printf("%lld\n", ans);
		else puts("-1");

	}
	else puts("-1");

	return 0;
}
