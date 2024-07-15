// BEGIN CUT HERE

// END CUT HERE
#line 5 "BusTravel.cpp"
#include <bits/stdc++.h>

using namespace std;

#define LEN(s) ((int)(s).length())
#define SZ(v) ((int)(v).size())
#define INF32 (1 << 30)
#define INF64 (1LL << 60)
#define PB push_back
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

class BusTravel {
    public:
        int dp[1 << 16][16];
        int d[16][16][16];
        vi c[1 << 16][2];

        int recur(int mask, int idx, int n) {
            if (idx < 0) return 0;
            int& res = dp[mask][idx];
            if (res != -1) return res;
            res = INF32;
            for (int i = 0; i < SZ(c[mask][0]); ++ i) {
                auto pi = c[mask][0][i];
                for (int j = 0; j < SZ(c[mask][1]); ++ j) {
                    auto pj = c[mask][1][j];
                    auto ret = recur(mask | (1 << pi), idx - 1, n);
                    if (ret != INF32) res = min(res, ret + d[pi][pj][idx]);
                }
                for (int j = 0; j < SZ(c[mask][0]); ++ j) if (i != j) {
                    auto pj = c[mask][0][j];
                    auto ret = recur(mask | (1 << pi) | (1 << pj), idx - 1, n);
                    if (ret != INF32) res = min(res, ret + min(d[pi][pj][idx], d[pj][pi][idx]));
                }
            }
            return res;
        }

        int optimize(int N, int S, int M, int A) {
            for (int i = 0; i < (1 << N); ++ i) {
                for (int j = 0; j < 2; ++ j) CLR(c[i][j]);
                for (int j = 0; j < N; ++ j) {
                    if (i & (1 << j)) c[i][1].PB(j);
                    else c[i][0].PB(j);
                }
            }
            for (int i = 0; i < N; ++ i) for (int j = i + 1; j < N; ++ j) {
                for (int x = 1; x <= N - S; ++ x) {
                    d[i][j][x - 1] = ((x * (i + 4) * (i + j + A)) % M) + 1;
                    d[j][i][x - 1] = ((x * (j + 4) * (j + i + A)) % M) + 1;
                }
            }
            memset(dp, 255, sizeof(dp));
            return recur(0, N - S - 1, N);
        }
};

int main(int argc, char** argv) {
    BusTravel obj;

    cout << obj.optimize(4, 4, 47, 7) << endl;
    cout << obj.optimize(4, 3, 47, 7) << endl;
    cout << obj.optimize(4, 2, 47, 7) << endl;
    cout << obj.optimize(4, 1, 47, 7) << endl;
    cout << obj.optimize(4, 1, 47, 17) << endl;
    cout << obj.optimize(14, 4, 1, 0) << endl;
    cout << obj.optimize(16, 1, 1, 0) << endl;
    cout << obj.optimize(16, 8, 1, 0) << endl;
    cout << obj.optimize(16, 16, 1, 0) << endl;

    return 0;
}
