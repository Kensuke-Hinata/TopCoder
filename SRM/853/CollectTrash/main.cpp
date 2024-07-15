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

class CollectTrash {
    public:
        int optimize(int P, int T, int M, int C, vector <int> Lprefix, int seed) {
            auto PL = SZ(Lprefix);
            vi L(T);
            for (int i = 0; i < PL; ++ i) L[i] = Lprefix[i];
            LL state = seed;
            const LL mod = 1LL << 31;
            for (int i = PL; i < T; ++ i) {
                if (C == 1 && i > 0) {
                    state = (state * 1103515245 + 12345) % mod;
                    auto old = L[state % i];
                    state = (state * 1103515245 + 12345) % mod;
                    int spread = (state % 21) - 10;
                    L[i] = max(0, min(M - 1, old + spread));
                } else {
                    state = (state * 1103515245 + 12345) % mod;
                    L[i] = state % M;
                }
            }

            sort(ALL(L));
            auto left = 0, right = INF32, res = -1;
            while (left <= right) {
                auto mid = (left + right) >> 1;
                int i, j, s = 0;
                for (i = 0, j = 0; i < P && j < T; ++ j) {
                    auto d = (j > 0 ? L[j] - L[j - 1] : L[j]);
                    if (s + d + 1 > mid) {
                        ++ i;
                        if (i >= P) break;
                        s = L[j] + 1;
                        if (s > mid) break;
                    } else {
                        s += (d + 1);
                    }
                }
                if (j < T) {
                    left = mid + 1;
                } else {
                    res = mid;
                    right = mid - 1;
                }
            }

            return res;
        }
};

int main(int argc, char** argv) {
    CollectTrash obj;

    auto P = 1, T = 4, M = 1000000000, C = 0, seed = 1000000000;
    auto Lprefix = vi{7, 4, 10, 4};
    cout << obj.optimize(P, T, M, C, Lprefix, seed) << endl;

    P = 47, T = 47003, M = 1, C = 0, seed = 47;
    Lprefix = vi();
    cout << obj.optimize(P, T, M, C, Lprefix, seed) << endl;

    P = 2, T = 5, M = 100000, C = 0, seed = 42;
    Lprefix = vi{1, 2, 3, 4, 5};
    cout << obj.optimize(P, T, M, C, Lprefix, seed) << endl;

    P = 3, T = 12, M = 7654321, C = 0, seed = 1234567;
    Lprefix = vi{1000, 2000, 3000};
    cout << obj.optimize(P, T, M, C, Lprefix, seed) << endl;

    P = 3, T = 12, M = 7654321, C = 1, seed = 1234567;
    Lprefix = vi{1000, 2000, 3000};
    cout << obj.optimize(P, T, M, C, Lprefix, seed) << endl;

    return 0;
}
