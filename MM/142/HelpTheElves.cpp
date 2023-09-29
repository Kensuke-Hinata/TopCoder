#include <bits/stdc++.h>

using namespace std;

#define INF (1 << 30)
#define SZ(v) ((int)(v).size())
#define PB push_back
#define MP make_pair
#define F(x) ((x).first)
#define S(x) ((x).second)
#define ALL(v) (v).begin(), v.end()

typedef vector<int> vi;
typedef vector<bool> vb;
typedef vector<double> vd;
typedef pair<int, int> pii;

int N;
int C;
double elfP;
int Money;

static const int dr[] = {0, -1, 0, 1};
static const int dc[] = {-1, 0, 1, 0};    

char grid[32][32];

bool inGrid(int r, int c) {
    return (r >= 0 && r < N && c >= 0 && c < N);
}

void makeMove(int r, int c, int r2, int c2) {
    if (grid[r][c] != 'e') {
        grid[r2][c2] = grid[r][c];
    } else {
        if (grid[r2][c2] == '.') grid[r2][c2] = 'e';
        else if (grid[r2][c2] == 'b') grid[r2][c2] = 'B';
        else if (grid[r2][c2] == 'P') grid[r2][c2] = 'E';
    }
    grid[r][c] = '.';
}

char getDirection(int sx, int sy, int dx, int dy) {
    if (sx == dx) return (sy + 1 == dy ? 'R' : 'L');
    return (sx + 1 == dx ? 'D' : 'U');
}

bool randomChoose(pii e, char target, string& moves) {
    vi ids;
    for (int i = 0; i < 4; ++ i) {
        auto dx = F(e) + dr[i], dy = S(e) + dc[i];
        if (inGrid(dx, dy) && grid[dx][dy] == target) ids.PB(i);
    }
    if (SZ(ids) == 0) return false;
    random_shuffle(ALL(ids));
    auto dx = F(e) + dr[ids[0]], dy = S(e) + dc[ids[0]];
    makeMove(F(e), S(e), dx, dy);
    auto direction = getDirection(F(e), S(e), dx, dy);
    moves += to_string(F(e)) + " " + to_string(S(e)) + " " + direction + " ";
    return true;
}

void probChoose(vector<pii>& p, vd& prob, pii e, string& moves) {
    auto idx = -1;
    double sp = 0;
    auto r = (double)(rand() % 100) / 100;
    for (int i = 0; i < SZ(p); ++ i) {
        sp += prob[i];
        if (r <= sp) {
            idx = S(p[i]);
            break;
        }
    }
    if (idx == -1) idx = S(p[SZ(p) - 1]);
    auto dx = F(e) + dr[idx], dy = S(e) + dc[idx];
    makeMove(F(e), S(e), dx, dy);
    auto direction = getDirection(F(e), S(e), dx, dy);
    moves += to_string(F(e)) + " " + to_string(S(e)) + " " + direction + " ";
}

bool searchTarget(pii e, char target, string& moves) {
    vi ids, steps;
    for (int i = 0; i < 4; ++ i) {
        auto c = 0;
        auto x = F(e) + dr[i], y = S(e) + dc[i];
        while (inGrid(x, y) && grid[x][y] == '.') {
            x += dr[i];
            y += dc[i];
            ++ c;
        }
        if (inGrid(x, y) && grid[x][y] == target) {
            ids.PB(i);
            steps.PB(c);
        }
    }
    if (SZ(ids) == 0) return false;
    vector<pii> p(SZ(ids));
    for (int i = 0; i < SZ(ids); ++ i) p[i] = MP(steps[i], ids[i]);
    sort(ALL(p));
    auto sum = 0;
    for (int i = 0; i < SZ(ids); ++ i) sum += steps[i];
    vd prob(SZ(ids));
    for (int i = 0; i < SZ(ids); ++ i) prob[i] = 1 - (double)steps[i] / sum;
    probChoose(p, prob, e, moves);
    return true;
}

void search(pii e, string& moves) {
    for (int i = 0; i < 4; ++ i) {
        auto dx = F(e) + dr[i], dy = S(e) + dc[i];
        if (inGrid(dx, dy)) {
            if (grid[dx][dy] == 'P') {
                makeMove(F(e), S(e), dx, dy);
                auto direction = getDirection(F(e), S(e), dx, dy);
                moves += to_string(F(e)) + " " + to_string(S(e)) + " " + direction + " ";
                return;
            }
        }
    }
    auto ret = searchTarget(e, 'P', moves);
    if (ret) return;
    ret = searchTarget(e, 'b', moves);
    if (!ret) randomChoose(e, '.', moves);
}

void leave(pii e, string& moves) {
    vi ids, sids, steps;
    auto best = INF;
    for (int i = 0; i < 4; ++ i) {
        auto x = F(e) + dr[i], y = S(e) + dc[i];
        auto c = 0;
        while (inGrid(x, y) && grid[x][y] == '.') {
            x += dr[i];
            y += dc[i];
            ++ c;
        }
        if (!inGrid(x, y)) {
            if (c < best) {
                best = c;
                sids = {i};
            } else if (c == best) {
                sids.PB(i);
            }
        }
        if (c >= 1) {
            ids.PB(i);
            steps.PB(-c);
        }
    }
    if (SZ(sids) > 0) {
        random_shuffle(ALL(sids));
        auto dx = F(e) + dr[sids[0]], dy = S(e) + dc[sids[0]];
        if (!inGrid(dx, dy) || grid[dx][dy] == '.') {
            if (inGrid(dx, dy)) makeMove(F(e), S(e), dx, dy);
            auto direction = getDirection(F(e), S(e), dx, dy);
            moves += to_string(F(e)) + " " + to_string(S(e)) + " " + direction + " ";
        }
        return;
    }
    if (SZ(ids) == 0) return;
    vector<pii> p(SZ(ids));
    for (int i = 0; i < SZ(ids); ++ i) p[i] = MP(steps[i], ids[i]);
    sort(ALL(p));
    auto sum = 0;
    for (int i = 0; i < SZ(ids); ++ i) sum += steps[i];
    vd prob(SZ(ids));
    for (int i = 0; i < SZ(ids); ++ i) prob[i] = (double)steps[i] / sum;
    probChoose(p, prob, e, moves);
}

int main() {
    cin >> N >> C >> elfP >> Money;

    srand((unsigned)time(NULL));

    for (int Turn = 1; Turn <= N * N; ++ Turn) {      
        int elapsedTime;
        cin >> elapsedTime >> Money;

        for (int r = 0; r < N; ++ r) for (int c = 0; c < N; ++ c) cin >> grid[r][c];

        vector<pii> e, d;
        for (int r = 0; r < N; ++ r) {
            for (int c = 0; c < N; ++ c) {
                if (grid[r][c] == 'e') e.PB(MP(r, c));
                else if (grid[r][c] == 'B' || grid[r][c] == 'E') d.PB(MP(r, c));
            }
        }

        string moves = "";

        for (int i = 0; i < SZ(d); ++ i) leave(d[i], moves);
        for (int i = 0; i < SZ(e); ++ i) search(e[i], moves);

        if (moves.length() == 0) moves = "-1";

        cout << moves << endl;
        cout.flush();
    }

    return 0;
}
