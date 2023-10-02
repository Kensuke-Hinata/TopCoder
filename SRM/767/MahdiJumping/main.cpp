// BEGIN CUT HERE

// END CUT HERE
#line 5 "MahdiJumping.cpp"
#include <bits/stdc++.h>

using namespace std;

#define LEN(s) ((int)(s).length())
#define SZ(v) ((int)(v).size())
#define INF32 (1 << 30)
#define INF64 (1LL << 60)
#define PB push_back
#define MP make_pair
#define CLR(v) ((v).clear())
#define ALL(v) (v).begin(), (v).end()
#define ZERO(v) memset(v, 0, sizeof(v))

typedef long long LL;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<LL> vll;
typedef vector<double> vd;
typedef vector<string> vs;
typedef vector<bool> vb;

class MahdiJumping {
    public:
        LL minDis(int n, int A, int B, int a, int b) {
            priority_queue<pair<LL, int>, vector<pair<LL, int>, greater<pair<LL, int>> q;
            vector<bool> f(n, false);
            vector<LL> dp(n, -1);
            dp[0] = 0;
            q.push(MP(0LL, 0));
            LL c;
            int idx;
            while (q.size() > 0) {
                auto p = q.top();
                q.pop();
                c = p.first;
                idx = p.second;
                if (f[idx]) continue;
                f[idx] = true;
                dp[idx] = c;
                if (idx == n - 1) break;
                auto nidx = idx + 1;
                if (dp[nidx] == -1) {
                    auto nc = c + a;
                    q.push(MP(nc, nidx));
                }
                nidx = (int)(((LL)A * idx + B) % n);
                if (dp[nidx] == -1) {
                    auto nc = c + b;
                    q.push(MP(nc, nidx));
                }
            }
            return dp[n - 1];
        }
};
