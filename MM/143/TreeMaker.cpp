#include <bits/stdc++.h>

using namespace std;

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
    if (x > 0 && (grid[x][y] & dmask[0]) != 0 && !f[x - 1][y]) {
        if ((grid[x - 1][y] & dmask[2]) != 0 && colors[x][y] == colors[x - 1][y]) {
            dfs(x - 1, y, n, f);
        }
    }
    if (x < n - 1 && (grid[x][y] & dmask[2]) != 0 && !f[x + 1][y]) {
        if ((grid[x + 1][y] & dmask[0]) != 0 && colors[x][y] == colors[x + 1][y]) {
            dfs(x + 1, y, n, f);
        }
    }
    if (y < n - 1 && (grid[x][y] & dmask[1]) != 0 && !f[x][y + 1]) {
        if ((grid[x][y + 1] & dmask[3]) != 0 && colors[x][y] == colors[x][y + 1]) {
            dfs(x, y + 1, n, f);
        }
    }
    if (y > 0 && (grid[x][y] & dmask[3]) != 0 && !f[x][y - 1]) {
        if ((grid[x][y - 1] & dmask[1]) != 0 && colors[x][y] == colors[x][y - 1]) {
            dfs(x, y - 1, n, f);
        }
    }
}

int count_broken_pipes(int n) {
    auto res = 0;
    for (int x = 0; x < n; ++ x) for (int y = 0; y < n; ++ y) {
        if (grid[x][y] == 0) ++ res;
        if ((grid[x][y] & dmask[0]) != 0) {
            if (x == 0) {
                ++ res;
            } else {
                if ((grid[x - 1][y] & dmask[2]) == 0 || colors[x][y] != colors[x - 1][y]) {
                    ++ res;
                }
            }
        }
        if ((grid[x][y] & dmask[1]) != 0) {
            if (y == n - 1) {
                ++ res;
            } else {
                if ((grid[x][y + 1] & dmask[3]) == 0 || colors[x][y] != colors[x][y + 1]) {
                    ++ res;
                }
            }
        }
        if ((grid[x][y] & dmask[2]) != 0) {
            if (x == n - 1 || (grid[x + 1][y] & dmask[0]) == 0) ++ res;
        }
        if ((grid[x][y] & dmask[3]) != 0) {
            if (y == 0 || (grid[x][y - 1] & dmask[1]) == 0) ++ res;
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
    for (int i = 0; i < N; ++ i) for (int j = 0; j < N; ++ j) {
        tg[i][j] = grid[i][j];
        tc[i][j] = colors[i][j];
    }
    int tile, tile_color;
    cin >> tile >> tile_color;
    vector<vs> output(32);
    vector<vb> f(N, vb(N, false));
    vi cnt(10, 0);
    auto best = 1 << 30, tidx = -1, cidx = -1;
    srand(time(NULL));
    for (int t = 0; t < 32; ++ t) {
        for (int i = 0; i < 10000; ++ i) {
            auto idx = rand() % N, didx = rand() % 4;
            auto ret = move(tile, tile_color, idx, didx, N);
            tile = F(ret);
            tile_color = S(ret);
            for (int j = 0; j < N; ++ j) fill(f[j].begin(), f[j].end(), false);
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
            for (int j = 0; j < 4; ++ j) cost += cnt[j] * cnt[j];
            if (cost < best) {
                best = cost;
                tidx = t;
                cidx = i;
            }
            output[t].PB(dirs[didx] + " " + to_string(idx));
        }
        for (int i = 0; i < N; ++ i) for (int j = 0; j < N; ++ j) {
            grid[i][j] = tg[i][j];
            colors[i][j] = tc[i][j];
        }
    }
    cout << cidx + 1 << endl;
    for (int i = 0; i <= cidx; ++ i) cout << output[tidx][i] << endl;
    return 0;
}
