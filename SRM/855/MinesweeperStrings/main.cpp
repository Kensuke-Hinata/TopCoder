// BEGIN CUT HERE

// END CUT HERE
#line 5 "MinesweeperStrings.cpp"
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

class MinesweeperStrings {
    public:
        LL dp[70][4][4];

        LL recur(int idx, int i, int j, int n) {
            LL& res = dp[idx][i][j];
            if (idx == n) {
                res = 1;
                return 1;
            }
            if (res != -1) return res;
            res = 0;
            for (int k = 0; k < 4; ++ k) {
                if (idx == n - 1) {
                    if (k == 2) continue;
                    if (j != 3 && k == 1) continue;
                }
                if (j == 0 && k >= 2) continue;
                if (j == 1) {
                    if (i == 3 && k == 3) continue;
                    if (i != 3 && k != 3) continue;
                }
                if (j == 2) {
                    if (i != 3 || k != 3) continue;
                }
                if (j == 3 && k == 0) continue;
                auto ret = recur(idx + 1, j, k, n);
                res += ret;
            }
            return res;
        }

        string generate(int N, long long X) {
            if (N == 1) {
                if (X == 0) return "*";
                if (X == 1) return "0";
                return "";
            }
            memset(dp, 255, sizeof(dp));
            for (int i = 0; i < 4; ++ i) for (int j = 0; j < 4; ++ j) {
                if (N == 2) {
                    if (j == 2) continue;
                    if (i != 3 && j == 1) continue;
                }
                if (i == 0 && j >= 2) continue;
                if (i == 1 && j != 3) continue;
                if (i == 2) continue;
                if (i == 3 && j == 0) continue;
                recur(2, i, j, N);
            }
            auto c = vi{3, 0, 1, 2};
            ++ X;
            auto si = -1, sj = -1;
            for (int i = 0; i < 4; ++ i) {
                for (int j = 0; j < 4; ++ j) {
                    if (dp[2][c[i]][c[j]] == -1) continue;
                    if (dp[2][c[i]][c[j]] < X) {
                        X -= dp[2][c[i]][c[j]];
                        continue;
                    }
                    si = c[i];
                    sj = c[j];
                    break;
                }
                if (si != -1) break;
            }
            if (si == -1) return "";
            string res = "";
            if (si == 3) res += "*";
            else res += to_string(si);
            if (sj == 3) res += "*";
            else res += to_string(sj);
            for (int i = 2; i < N; ++ i) {
                for (int k = 0; k < 4; ++ k) {
                    if (sj == 0 && c[k] >= 2) continue;
                    if (sj == 1) {
                        if (si == 3 && c[k] == 3) continue;
                        if (si != 3 && c[k] != 3) continue;
                    }
                    if (sj == 2) {
                        if (si != 3 || c[k] != 3) continue;
                    }
                    if (sj == 3 && c[k] == 0) continue;
                    if (dp[i + 1][sj][c[k]] < X) {
                        X -= dp[i + 1][sj][c[k]];
                        continue;
                    }
                    if (c[k] == 3) res += "*";
                    else res += to_string(c[k]);
                    si = sj;
                    sj = c[k];
                    break;
                }
            }
            return res;
        }
};
