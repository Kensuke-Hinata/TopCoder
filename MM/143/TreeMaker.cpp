#include <bits/stdc++.h>

using namespace std;

#define INF (1 << 30)
#define SZ(v) ((int)(v).size())
#define PB push_back
#define MP make_pair
#define F(x) (x).first
#define S(x) (x).second

typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<bool> vb;
typedef vector<string> vs;

const string dirs[4] = {"U", "R", "D", "L"};
const int d[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
const vi dmask = vi{43690, 52428, 61680, 65280};

int tg[32][32];
int tc[32][32];
int grid[32][32];
int colors[32][32];

bool is_valid(int x, int y, int n) {
    return x >= 0 && x < n && y >= 0 && y < n;
}

pii move(int tile, int tile_color, int idx, int didx, int n) {
    int x, y;
    if (didx == 0) x = n - 1, y = idx; 
    else if (didx == 1) x = 0, y = idx;
    else if (didx == 2) x = idx, y = n - 1;
    else x = idx, y = 0;
    while (is_valid(x, y, n)) {
        swap(grid[x][y], tile);
        swap(colors[x][y], tile_color);
        x += d[didx][0];
        y += d[didx][1];
    }
    return MP(tile, tile_color);
}

void dfs(int x, int y, int n, vector<vb>& f) {
    f[x][y] = true;
    if (x > 0 && ((1 << grid[x][y]) & dmask[0]) != 0 && !f[x - 1][y]) {
        if (((1 << grid[x - 1][y]) & dmask[2]) != 0 && colors[x][y] == colors[x - 1][y]) {
            dfs(x - 1, y, n, f);
        }
    }
    if (x < n - 1 && ((1 << grid[x][y]) & dmask[2]) != 0 && !f[x + 1][y]) {
        if (((1 << grid[x + 1][y]) & dmask[0]) != 0 && colors[x][y] == colors[x + 1][y]) {
            dfs(x + 1, y, n, f);
        }
    }
    if (y < n - 1 && ((1 << grid[x][y]) & dmask[1]) != 0 && !f[x][y + 1]) {
        if (((1 << grid[x][y + 1]) & dmask[3]) != 0 && colors[x][y] == colors[x][y + 1]) {
            dfs(x, y + 1, n, f);
        }
    }
    if (y > 0 && ((1 << grid[x][y]) & dmask[3]) != 0 && !f[x][y - 1]) {
        if (((1 << grid[x][y - 1]) & dmask[1]) != 0 && colors[x][y] == colors[x][y - 1]) {
            dfs(x, y - 1, n, f);
        }
    }
}

int count_broken_pipes(int n) {
    auto res = 0;
    for (int x = 0; x < n; ++ x) for (int y = 0; y < n; ++ y) {
        if (grid[x][y] == 0) ++ res;
        if (((1 << grid[x][y]) & dmask[0]) != 0) {
            if (x == 0) {
                ++ res;
            } else {
                if (((1 << grid[x - 1][y]) & dmask[2]) == 0 || colors[x][y] != colors[x - 1][y]) {
                    ++ res;
                }
            }
        }
        if (((1 << grid[x][y]) & dmask[1]) != 0) {
            if (y == n - 1) {
                ++ res;
            } else {
                if (((1 << grid[x][y + 1]) & dmask[3]) == 0 || colors[x][y] != colors[x][y + 1]) {
                    ++ res;
                }
            }
        }
        if (((1 << grid[x][y]) & dmask[2]) != 0) {
            if (x == n - 1 || ((1 << grid[x + 1][y]) & dmask[0]) == 0) ++ res;
        }
        if (((1 << grid[x][y]) & dmask[3]) != 0) {
            if (y == 0 || ((1 << grid[x][y - 1]) & dmask[1]) == 0) ++ res;
        }
    }
    return res;
}

int main(int argc, char** argv) {
    int N, C, P;
    cin >> N >> C >> P;
    for (int row = 0; row < N; ++ row) for (int col = 0; col < N; ++ col) {
        cin >> grid[row][col] >> colors[row][col];
    }
    int tile, tile_color;
    cin >> tile >> tile_color;
    vs output;
    vector<vb> f(N, vb(N, false));
    vi cnt(10, 0);
    auto best = INF, cidx = -1;
    for (int i = 0; i < 10000; ++ i) {
        auto tt = tile, ttc = tile_color;
        for (int x = 0; x < N; ++ x) for (int y = 0; y < N; ++ y) {
            tg[x][y] = grid[x][y];
            tc[x][y] = colors[x][y];
        }
        auto ibest = INF, idx = -1, didx = -1, ntile = -1, ntile_color = -1;
        for (int j = 0; j < N; ++ j) for (int di = 0; di <= 3; ++ di) {
            for (int x = 0; x < N; ++ x) for (int y = 0; y < N; ++ y) {
                grid[x][y] = tg[x][y];
                colors[x][y] = tc[x][y];
            }
            tile = tt;
            tile_color = ttc;
            auto ret = move(tile, tile_color, j, di, N);
            for (int k = 0; k < N; ++ k) fill(f[k].begin(), f[k].end(), false);
            fill(cnt.begin(), cnt.end(), 0);
            for (int x = 0; x < N; ++ x) for (int y = 0; y < N; ++ y) {
                if (!f[x][y]) {
                    dfs(x, y, N, f);
                    ++ cnt[colors[x][y]];
                }
            }
            auto E = count_broken_pipes(N);
            auto cost = i + 1;
            cost += E * P;
            for (int k = 0; k < C; ++ k) cost += cnt[k] * cnt[k];
            if (cost < ibest) {
                ibest = cost;
                idx = j;
                didx = di;
                ntile = F(ret);
                ntile_color = S(ret);
            }
        }
        if (ibest < best) cidx = i;
        tile = ntile;
        tile_color = ntile_color;
        output.PB(dirs[didx] + " " + to_string(idx));
    }
    cout << cidx + 1 << endl;
    for (int i = 0; i <= cidx; ++ i) cout << output[i] << endl;
    return 0;
}
