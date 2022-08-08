#include <string>
#include <vector>
#include <iostream>

using namespace std;

#define LEN(s) ((int)s.length())

typedef long long LL;

class PalindromePermutations {
    public:
        double palindromeProbability(string w) {
            auto n = LEN(w);
            vector<int> cnt(26);
            fill(cnt.begin(), cnt.end(), 0);
            for (int i = 0; i < n; ++ i) {
                ++ cnt[w[i] - 'a'];
            }
            auto odd = 0;
            for (int i = 0; i < 26; ++ i) {
                if (cnt[i] & 1) {
                    ++ odd;
                    if (odd > 1 || !(n & 1)) {
                        return (double)0;
                    }
                }
            }
            vector<vector<LL>> c(n + 1);
            for (int i = 0; i <= n; ++ i) {
                c[i] = vector<LL>(n + 1);
                fill(c[i].begin(), c[i].end(), 0);
            }
            for (int i = 0; i <= n; ++ i) {
                c[i][0] = 1;
            }
            for (int i = 1; i <= n; ++ i) {
                for (int j = 1; j <= i; ++ j) {
                    c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
                }
            }
            LL ac = 1, pc = 1;
            auto an = n, pn = n >> 1;
            for (int i = 0; i < 26; ++ i) {
                if (cnt[i] > 0) {
                    ac *= c[an][cnt[i]];
                    an -= cnt[i];
                    pc *= c[pn][cnt[i] >> 1];
                    pn -= cnt[i] >> 1;
                }
            }
            return (double)pc / ac;
        }
};

int main(int argc, char** argv) {
    PalindromePermutations obj;
    string w;
    while (cin >> w) {
        cout << obj.palindromeProbability(w) << endl;
    }
    return 0;
}
