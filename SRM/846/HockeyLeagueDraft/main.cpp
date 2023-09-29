// BEGIN CUT HERE

// END CUT HERE
#line 5 "HockeyLeagueDraft.cpp"
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

class HockeyLeagueDraft {
    public:
        int draft(int P, int T, int seed, int MP, int MT) {
            LL state = seed;

            vi A(P), D(P);
            for (int i = 0; i < P; ++ i) {
                state = (state * 1103515245 + 12345) % (1LL << 31);
                A[i] = (state / 10) % MP;
                state = (state * 1103515245 + 12345) % (1LL << 31);
                D[i] = (state / 10) % MP;
            }

            vi TA(T), TD(T);
            for (int j = 0; j < T; ++ j) {
                state = (state * 1103515245 + 12345) % (1LL << 31);
                TA[j] = (state / 10) % MT;
                state = (state * 1103515245 + 12345) % (1LL << 31);
                TD[j] = (state / 10) % MT;
            }

            vector<pii> sa, sd;
            for (int i = 0; i < P; ++ i) {
                sa.PB(make_pair((-A[i]), i));
                sd.PB(make_pair((-D[i]), i));
            }
            sort(ALL(sa));
            sort(ALL(sd));
            vb f(P);
            auto ai = 0, di = 0, res = 0;
            for (int i = 0; i < T; ++ i) {
                while (ai < P && f[sa[ai].second]) ++ ai;
                while (di < P && f[sd[di].second]) ++ di;
                auto aval = (LL)(TA[i] - sa[ai].first) * (TA[i] - sa[ai].first) + (LL)TD[i] * TD[i];
                auto dval = (LL)TA[i] * TA[i] + (LL)(TD[i] - sd[di].first) * (TD[i] - sd[di].first);
                if (aval > dval) {
                    f[sa[ai].second] = true;
                    res = sa[ai].second;
                    ++ ai;
                } else if (aval < dval) {
                    f[sd[di].second] = true;
                    res = sd[di].second;
                    ++ di;
                } else {
                    if (sa[ai].second < sd[di].second) {
                        f[sa[ai].second] = true;
                        res = sa[ai].second;
                        ++ ai;
                    } else {
                        f[sd[di].second] = true;
                        res = sd[di].second;
                        ++ di;
                    }
                }
            }

            return res;
        }
};
