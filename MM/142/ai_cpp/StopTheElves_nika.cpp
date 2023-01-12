#include <bits/stdc++.h>
int STANDARD = 1;
int SIM = 0;
int LATEST = 0;
using namespace std;
#define F0(i,n) for (int i=0; i<n; i++)
#define FR(i,n) for (int i=n-1; i>=0; i--)
#define F1(i,n) for (int i=1; i<=n; i++)
#define CL(a,x) memset(x, a, sizeof(x));
#define SZ(x) ((int)x.size())
const int inf = 1000009;
const double pi = acos(-1.0);
typedef pair<int, int> pii;
typedef long long ll;
typedef unsigned long long ull;
const double EPS = 1e-9;
#define PR(x) cerr << #x << "=" << x << endl
ostream& operator<<(ostream& os, const pii& p) { os << "(" << p.first << "," << p.second << ")"; return os; }
istream& operator>>(istream& is, pii& p) { is>>p.first>>p.second; return is; }
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v) {
    os << "["; F0(i,SZ(v)) { if (i>0) os << ","; os << v[i]; } os << "]"; return os;
}
void Print(set<int> v) {
    cerr << "{"; int f=1; for(int i:v) { if(f)f=0;else cerr << ","; cerr << i; }
    cerr << "}" << endl;
}

inline ll GetTSC() {
    ll lo, hi;
    asm volatile ("rdtsc": "=a"(lo), "=d"(hi));
    return lo + (hi << 32);
}
inline double GetSeconds() {
    return GetTSC() / 2.8e9;
}

const int MAX_RAND = 1 << 30;
struct Rand {
    ll x, y, z, w, o;
    Rand() {}
    Rand(ll seed) { reseed(seed); o = 0; }
    inline void reseed(ll seed) { x = 0x498b3bc5 ^ seed; y = 0; z = 0; w = 0;  F0(i, 20) mix(); }
    inline void mix() { ll t = x ^ (x << 11); x = y; y = z; z = w; w = w ^ (w >> 19) ^ t ^ (t >> 8); }
    inline ll rand() { mix(); return x & (MAX_RAND - 1); }
    inline int nextInt(int n) { return rand() % n; }
    inline int nextInt(int L, int R) { return rand()%(R - L + 1) + L; }
    inline int nextBool() { if (o < 4) o = rand(); o >>= 2; return o & 1; }
    double nextDouble() { return rand() * 1.0 / MAX_RAND; }
};
Rand my(2020);
double saveTime;
double t_o[20];
ll c_o[20];
void Init() {
    saveTime = GetSeconds();
    F0(i, 20) t_o[i] = 0.0;
    F0(i, 20) c_o[i] = 0;
}
double Elapsed() { return GetSeconds() - saveTime; }
void Report() {
    double tmp = Elapsed();
    cerr << "-------------------------------------" << endl;
    cerr << "Elapsed time: " << tmp << " sec" << endl;
    double total = 0.0; F0(i, 20) { if (t_o[i] > 0) cerr << "t_o[" << i << "] = " << t_o[i] << endl; total += t_o[i]; } cerr << endl; //if (total > 0) cerr << "Total spent: " << total << endl;
    F0(i, 20) if (c_o[i] > 0) cerr << "c_o[" << i << "] = " << c_o[i] << endl;
    cerr << "-------------------------------------" << endl;
}
struct AutoTimer {
    int x;
    double t;
    AutoTimer(int x) : x(x) {
        t = Elapsed();
    }
    ~AutoTimer() {
        t_o[x] += Elapsed() - t;
    }
};
#define AT(i) AutoTimer a##i(i)
//#define AT(i)

// CONSTANTS
const int N = 32;
const int ST = 32;
const int MAXSN = 256;
int SN = 16;
const int LOGN = 1 << 16;
double logs[LOGN];
enum { UP, RIGHT, DOWN, LEFT };
const int DX[]={-1,0,1,0};
const int DY[]={0,1,0,-1};
const string DS="URDL";

enum {
    ELF = 'e',
    ELF_BOX = 'B',
    ELF_PRESENT = 'E',
    PRESENT = 'P',
    EMPTY = '.',
    BOX = 'b',
    TREE = 'T'
};

bool changes;
int n, C, money, turn = 1, elapsedTime;
double elfP;
bool game_over;
char grid[N][N];
vector<pii> bmoves;
vector<pii> nei[N][N];
int s_turn[ST], s_money[ST];
char s_grid[ST][N][N];
vector<pii> slots;
Rand rng[MAXSN+1];
int elf_spawns[MAXSN+1][N*N+1], sid, slotn;
int orders[24][4];
const int MAXM = 1<<16;
const int MAXN = 1<<16;
int id[N][N], m, t;
vector<pii> gverts;
int ex[MAXM], ey[MAXM], ecap[MAXM];
vector<int> adj[MAXN];
int fv[MAXN];
int flow_dir = 1, flow_size;
int skip[N][N];
vector<pii> min_cut;
int actual_spawns;
pii MoveTo[4][N][N];
pii mode;
pii last_flow[1 << 16];
int last_flow_n, last_flow_cap;

int d1(int x, int y) { return min(abs(x-y), n-abs(x-y)); }
int dist_to_border[N][N];
bool good_start[N][N];
int d1(pii p1, pii p2) { return d1(p1.first,p2.first)+d1(p1.second,p2.second); }
pii Move(pii p, int k) { return pii(p.first + DX[k], p.second + DY[k]); }
bool In(pii p) { return p.first >= 0 && p.first < n && p.second >= 0 && p.second < n; }
bool In2(pii p) { return p.first >= 1 && p.first < n-1 && p.second >= 1 && p.second < n-1; }
//bool Good(pii p) { return In(p) && a[p.first][p.second] != -1; }
int GetDir(pii p1, pii p2) { F0(k, 4) if (d1(p1, p2) > d1(Move(p1, k), p2)) return k; return -1; }
int sqr(int x) { return x*x; }

int PresentsPresent() {
    int ret = 0;
    F0(i, n) F0(j, n) if (grid[i][j] == PRESENT || grid[i][j] == ELF_PRESENT) ret++;
    return ret;
}

void CalcGood() {
    queue<pii> Q;
    CL(0, good_start);
    F0(i, n) F0(j, n) if (grid[i][j] == PRESENT || grid[i][j] == BOX) {
        Q.push(pii(i, j));
        good_start[i][j] = true;
    }
    while (!Q.empty()) {
        pii p = Q.front(); Q.pop();
        for (pii q : nei[p.first][p.second]) if (!good_start[q.first][q.second]) {
            good_start[q.first][q.second] = true;
            Q.push(q);
        }
    }
}

void Prepare() {
    F0(i, LOGN) logs[i] = -log((i+0.5)/LOGN);

    F0(i, n) F0(j, n) {
        F0(k, 4) {
            int x = i + DX[k], y = j + DY[k];
            if (In(pii(x, y)) && grid[x][y] != TREE) nei[i][j].push_back(pii(x, y));
        }
    }
    queue<pii> Q;
    F0(i, n) F0(j, n) if (grid[i][j] != TREE && !In2(pii(i, j))) {
        slots.push_back(pii(i, j));
        slotn++;
        Q.push(pii(i, j));
        dist_to_border[i][j] = 1;
    }
    while (!Q.empty()) {
        pii p = Q.front(); Q.pop();
        for (pii q : nei[p.first][p.second]) if (!dist_to_border[q.first][q.second]) {
            dist_to_border[q.first][q.second] = dist_to_border[p.first][p.second] + 1;
            Q.push(q);
        }
    }
    F0(k, 4) orders[0][k] = k;
    F1(i, 23) {
        F0(k, 4) orders[i][k] = orders[i - 1][k];
        next_permutation(orders[i], orders[i] + 4);
    }

    F0(i, n) F0(j, n) F0(k, 4) {
        pii p = Move(pii(i, j), k);
        MoveTo[k][i][j] = In(p) ? p : pii(-1, -1);
    }

    F0(i, MAXSN) {
        rng[i].reseed(i + 1);
        F1(t, n*n) {
            elf_spawns[i][t] = -1;
            if (rng[i].nextDouble() < elfP) {
                elf_spawns[i][t] = rng[i].nextInt(slotn);
            }
        }
    }
}

int vis[N][N], Z, elfZ = 1, elfid[N][N], simZ = MAXSN;
void MoveElf(pii p, int k) {
    pii p2 = Move(p, k);
    char t = grid[p.first][p.second];
    grid[p.first][p.second] = EMPTY;
    if (In(p2)) {
        char t2 = grid[p2.first][p2.second];
        if (t2 == BOX) { assert(t == ELF); grid[p2.first][p2.second] = ELF_BOX; changes = true; }
        else if (t2 == PRESENT) { assert(t == ELF); grid[p2.first][p2.second] = ELF_PRESENT; changes = true; }
        else {
            assert(t2 == EMPTY);
            grid[p2.first][p2.second] = t;
            elfid[p2.first][p2.second] = elfid[p.first][p.second];
        }
    }
    elfid[p.first][p.second] = 0;
}

void PushElf(pii p) {
    Z++;
    queue<pair<int, pii>> Q;
    Q.push(make_pair(-1, p));
    int to_box = -1;
    char elf = grid[p.first][p.second];
    vis[p.first][p.second] = Z;
    while (!Q.empty()) {
        int fdir = Q.front().first;
        pii pos = Q.front().second;
        Q.pop();

        int ord = rng[sid].nextInt(24);
        F0(dd, 4) {
            int k = orders[ord][dd];
            pii npos = MoveTo[k][pos.first][pos.second];
            int nfdir = fdir == -1 ? k : fdir;
            if (npos.first != -1) {
                char t = grid[npos.first][npos.second];
                if (elf == ELF) {
                    if (t == PRESENT) {
                        MoveElf(p, nfdir);
                        return;
                    }
                    if (t == BOX && to_box == -1) to_box = nfdir;
                }
                if (t == EMPTY && Z != vis[npos.first][npos.second]) {
                    Q.push(make_pair(nfdir, npos));
                    vis[npos.first][npos.second] = Z;
                }
            } else {
                if (elf == ELF_BOX || elf == ELF_PRESENT) {
                    MoveElf(p, nfdir);
                    return;
                }
            }
        }
    }
    if (to_box != -1) {
        assert(elf == ELF);
        MoveElf(p, to_box);
    }
}

vector<pii> elves;
void SimElves() {
    elves.clear();
    F0(i, n) F0(j, n) if ((grid[i][j] == ELF && good_start[i][j])
                          || grid[i][j] == ELF_PRESENT
                          || grid[i][j] == ELF_BOX) elves.push_back(pii(i, j));
    for (pii p : elves) {
        PushElf(p);
    }
}

void MakeMove() {
    for (pii p : bmoves) {
        assert(grid[p.first][p.second] == EMPTY);
        grid[p.first][p.second] = BOX;
        money -= C;
    }
    changes = !bmoves.empty();
    SimElves();
    if (elf_spawns[sid][turn] != -1) {
        F0(sh, SZ(slots)) {
            pii npos = slots[(elf_spawns[sid][turn] + sh) % slotn];
            if (grid[npos.first][npos.second] == EMPTY) {
                grid[npos.first][npos.second] = ELF;
                break;
            }
        }
    }
    if (changes) CalcGood();
    money++;
    turn++;
}

void SaveState(int at = 0) {
    s_turn[at] = turn;
    s_money[at] = money;
    F0(i, n) F0(j, n) s_grid[at][i][j] = grid[i][j];
}

void LoadState(int at = 0) {
    turn = s_turn[at];
    money = s_money[at];
    F0(i, n) F0(j, n) grid[i][j] = s_grid[at][i][j];
}

void MarkBox(pii p) {
    grid[p.first][p.second] = BOX;
    money -= C;
    bmoves.push_back(p);
}

void UnmarkBoxes() {
    for (pii p : bmoves) {
        grid[p.first][p.second] = EMPTY;
        money += C;
    }
}

void RemoveEdge(int x, int y) {
    if (flow_dir) swap(x, y);
    assert(adj[y].back() == m-1); adj[y].pop_back(); m--;
    assert(adj[x].back() == m-1); adj[x].pop_back(); m--;
}

void AddEdge(int x, int y, int cap) {
    if (flow_dir) swap(x, y);
    ex[m] = x; ey[m] = y; ecap[m] = cap; adj[x].push_back(m); m++;
    ex[m] = y; ey[m] = x; ecap[m] = 0; adj[y].push_back(m); m++;
}

int rec(int i, int cap) {
    fv[i] = 1;
    if (i == 1 - flow_dir) return 1;
    for (int e : adj[i]) {
        int j = ey[e];
        if (ecap[e] >= cap && !fv[j] && rec(j, cap)) {
            last_flow[last_flow_n++] = pii(e, cap);
            ecap[e] -= cap;
            ecap[e^1] += cap;
            return 1;
        }
    }
    return 0;
}

int flow() {
    F0(i, t) fv[i] = 0;
    if (rec(flow_dir, inf)) return inf;
    int ret = 0;
    while (1) {
        F0(i, t) fv[i] = 0;
        if (rec(flow_dir, 1)) ret++; else break;
    }
    return ret;
}

void Cut(int i) {
    fv[i] = 1;
    for (int e : adj[i]) {
        int j = ey[e];
        if (!fv[j]) {
            if (ecap[e]) {
                Cut(j);
            }
        }
    }
}

void FlowGraph() {
    last_flow_n = 0;
    t = 2;
    m = 0;
    gverts.clear();
    F0(i, n) F0(j, n) {
        id[i][j] = -1;
        if (grid[i][j] == TREE) continue;
        if (grid[i][j] == BOX) continue;
        if (grid[i][j] == PRESENT) continue;
        if (grid[i][j] == ELF) continue;
        pii q(i, j);
        gverts.push_back(q);
        if (!In2(q)) {
            id[i][j] = 0;
        } else if ((grid[i][j] == ELF_BOX || grid[i][j] == ELF_PRESENT) && !skip[i][j]) {
            id[i][j] = 1;
        }
        else {
            id[i][j] = t;
            t += 2;
        }
    }
    F0(i, t) adj[i].clear();
    for (pii p1 : gverts) {
        int i1 = id[p1.first][p1.second];
        if (i1 == 1) continue;
        if (i1 > 1) {
            AddEdge(i1, i1 + 1, grid[p1.first][p1.second] != EMPTY ? inf : 1);
        }
        for (pii p2 : nei[p1.first][p1.second]) {
            int i2 = id[p2.first][p2.second];
            if (i2 <= 0) continue;
            AddEdge(i1 == 0 ? 0 : i1 + 1, i2, inf);
        }
    }

    flow_size=flow();
    if (flow_size == inf) return;
    //PR(flow_size);

    F0(i, t) fv[i] = 0;
    Cut(flow_dir);
    unordered_set<int> S;
    for (int i = 0; i < t; i++) if (fv[i]) for (int e : adj[i]) {
        int j = ey[e];
        if (!fv[j] && e%2==0) {
            S.insert(i);
        }
    }
    min_cut.clear();
    F0(i, n) {
        F0(j, n) {
            if (S.count(id[i][j]+flow_dir)) {
                min_cut.push_back(pii(i, j));
            }
        }
    }
    assert(SZ(min_cut) == flow_size);
}

vector<pii> spec;
int specZ, is_spec[N][N];
bool IsLeaving(char c) { return c == ELF_PRESENT || c == ELF_BOX; }
bool IsBlocker(char c) { return c == BOX || c == TREE; }
void MarkSpecial(int x, int y) {
    if (x > 0 && IsLeaving(grid[x-1][y]) && is_spec[x-1][y] != specZ) {
        is_spec[x-1][y] = specZ;
        spec.push_back(pii(x-1,y));
        MarkSpecial(x-1, y);
    }
    if (y > 0 && IsLeaving(grid[x][y-1]) && is_spec[x][y-1] != specZ) {
        is_spec[x][y-1] = specZ;
        spec.push_back(pii(x,y-1));
        MarkSpecial(x, y-1);
    }
}

void FlowGraph2(pii p, bool special = false) {
    last_flow_n = 0;
    min_cut.clear();
    t = 2;
    m = 0;
    gverts.clear();
    spec.clear();
    specZ++;
    if (special) {
        MarkSpecial(p.first, p.second);
    }
    F0(i, n) F0(j, n) {
        id[i][j] = -1;
        if (grid[i][j] == TREE) continue;
        if (grid[i][j] == BOX) continue;
        if (grid[i][j] == PRESENT) continue;
        if (is_spec[i][j] == specZ) continue;
        pii q(i, j);
        if (grid[q.first][q.second] == ELF || grid[q.first][q.second] == ELF_PRESENT || grid[q.first][q.second] == ELF_BOX) {
            if (q > p) continue;
        }
        gverts.push_back(q);
        if (!In2(q)) {
            id[i][j] = 0;
        } else if (q==p) {
            id[i][j] = 1;
        }
        else {
            id[i][j] = t;
            t += 2;
        }
    }
    F0(i, t) adj[i].clear();
    for (pii p1 : gverts) {
        int i1 = id[p1.first][p1.second];
        if (i1 == 1) continue;
        if (i1 > 1) {
            AddEdge(i1, i1 + 1, grid[p1.first][p1.second] != EMPTY ? inf : 1);
        }
        for (pii p2 : nei[p1.first][p1.second]) {
            int i2 = id[p2.first][p2.second];
            if (i2 <= 0) continue;
            AddEdge(i1 == 0 ? 0 : i1 + 1, i2, inf);
        }
    }

    for (pii q : spec) {
        if (q.first > 0 && grid[q.first-1][q.second] == EMPTY) {
            int i1 = id[q.first-1][q.second];
            AddEdge(i1 == 0 ? 0 : i1 + 1, 1, inf);
        }
        if (q.second > 0 && grid[q.first][q.second-1] == EMPTY) {
            int i1 = id[q.first][q.second-1];
            AddEdge(i1 == 0 ? 0 : i1 + 1, 1, inf);
        }
    }

    flow_size=flow();
    if (flow_size == inf) return;
}


vector<pii> ball;
vector<pair<int, pii>> issues;
void ChooseMove() {
    bmoves.clear();
    int present_thr = (mode.second * turn + (n*n-turn) * mode.first) / (n*n);
    if (money < C) return;
    if (present_thr <= 1) return;
    int ec = 0;

    issues.clear();
    F0(i, n) F0(j, n) if (grid[i][j] == ELF_PRESENT || grid[i][j] == ELF_BOX) {
        if (dist_to_border[i][j] - 1 + turn + (grid[i][j] == ELF_BOX) > n*n) continue;
        pii p(i, j);
        bool cant = false;
        ball.clear();
        for (pii q : nei[i][j]) {
            if (grid[q.first][q.second] == EMPTY) {
                if (!In2(q)) cant = true;
                if (grid[i][j] == ELF_PRESENT || q < p) ball.push_back(q);
            }
            if (grid[q.first][q.second] == ELF && q < p) cant = true;
        }
        if (cant || ball.empty()) continue;

        issues.push_back(make_pair(SZ(ball), pii(i, j)));

    }
    //sort(issues.rbegin(), issues.rend());
    for (auto issue : issues) {
        pii p = issue.second;
        int i = p.first, j = p.second;
        ball.clear();
        int extra = 0;
        for (pii q : nei[i][j]) {
            if (grid[q.first][q.second] == EMPTY) {
                if (grid[i][j] == ELF_PRESENT || q < p) ball.push_back(q);
                if (grid[i][j] == ELF_BOX && q > p) extra = ec;
            }
            if (grid[i][j] == ELF_PRESENT && (grid[q.first][q.second] == ELF_BOX || grid[q.first][q.second] == ELF)) {
                extra = ec;
            }
        }
        if (ball.empty()) continue;
        if (grid[i][j] == ELF_PRESENT && SZ(ball) >= present_thr) continue;
        if (grid[i][j] == ELF_BOX) {
            if (grid[i+1][j] != ELF_PRESENT && grid[i][j+1] != ELF_PRESENT) continue;
            if (SZ(ball) >= present_thr) continue;
        }

        if (SZ(ball) * C + extra <= money) {
            for (pii p : ball) {
                MarkBox(p);
            }
        }
    }
    UnmarkBoxes();
}

int HasPresent(char c) { return c == PRESENT || c == ELF_PRESENT; }
void ChooseMove2() {
    bmoves.clear();
    int pthr = (mode.second * turn + (n*n-turn) * mode.first) / (n*n);
    if (pthr <= 1) return;
    if (money < C) return;

    int ec = C-1;
    CL(0, skip);
    for (int present_thr = pthr; present_thr <= pthr; present_thr++)
    F0(i, n) F0(j, n) if (grid[i][j] == ELF_PRESENT || grid[i][j] == ELF_BOX) {
        skip[i][j] = 1;
        if (dist_to_border[i][j] - 1 + turn + (grid[i][j] == ELF_BOX) > n*n) continue;
        pii p(i, j);
        if (!In2(p)) continue;
        if (grid[i][j] == ELF_BOX && grid[i+1][j] != ELF_PRESENT && grid[i][j+1] != ELF_PRESENT) continue;
        bool cant = false;
        ball.clear();
        int extra = 0;
        for (pii q : nei[i][j]) {
            if (grid[q.first][q.second] == EMPTY) {
                if (!In2(q)) cant = true;
                if (grid[i][j] == ELF_PRESENT || q < p) ball.push_back(q);
                if (grid[i][j] == ELF_BOX && q > p) extra = ec;
            }
            if (grid[q.first][q.second] == ELF && q < p) cant = true;
            if (grid[i][j] == ELF_PRESENT && (grid[q.first][q.second] == ELF_BOX || grid[q.first][q.second] == ELF)) {
                extra = ec;
            }
        }
        if (cant) {
            continue;
        }
        if (ball.empty()) {
            continue;
        }
        FlowGraph2(p, true);
        if (flow_size >= inf) {
            continue;
        }
        if (flow_size == 0) {
            skip[i][j] = 0;
            continue;
        }
        if (flow_size <= SZ(ball)) {
            F0(i, t) fv[i] = 0;
            Cut(flow_dir);
            unordered_set<int> S;
            for (int i = 0; i < t; i++) if (fv[i]) for (int e : adj[i]) {
                int j = ey[e];
                if (!fv[j] && e%2==0) {
                    S.insert(i);
                }
            }
            F0(i, n) {
                F0(j, n) {
                    if (S.count(id[i][j]+flow_dir)) {
                        min_cut.push_back(pii(i, j));
                    }
                }
            }
            assert(SZ(min_cut) == flow_size);
            ball = min_cut;
        }
        if (ball.empty()) {
            continue;
        }

        if (money < C * SZ(ball) + extra) {
            continue;
        }

        if (SZ(ball) >= present_thr) {
            continue;
        }

        skip[i][j] = 0;

        for (pii p : ball) {
            MarkBox(p);
        }
    }
    UnmarkBoxes();

    if (1)
    if (!bmoves.empty()) {
        FlowGraph();
        if (flow_size <= SZ(bmoves)) {
            //if (flow_size < SZ(bmoves)) cerr << turn << "  " << flow_size << "/" << SZ(bmoves) << endl;
            bmoves = min_cut;
        }
    }
}

vector<pii> region;
int inside[N][N];
void CalcInside() {
    CL(0, inside);
    CL(0, vis);
    for (pii p : min_cut) { vis[p.first][p.second] = 1; inside[p.first][p.second] = 1; }
    F0(i, n) F0(j, n) if (grid[i][j] != TREE && !vis[i][j]) {
        queue<pii> Q;
        Q.push(pii(i, j));
        vis[i][j] = 1;
        region.clear();
        region.push_back(pii(i, j));
        bool has = false;
        bool outer = false;
        while (!Q.empty()) {
            pii p = Q.front(); Q.pop();
            for (pii q : nei[p.first][p.second]) {
                if (!vis[q.first][q.second]) {
                    if (HasPresent(grid[q.first][q.second])) {
                        has = true;
                    }
                    region.push_back(q);
                    vis[q.first][q.second] = 1;
                    Q.push(q);
                }
            }
        }
        for (pii p : region) if (!In2(p)) outer = true;
        if (has && !outer) for (pii p : region) inside[p.first][p.second] = 1;
    }
}

void Dumb() {
    bmoves.clear();
    if (money < C) return;
   // CalcInside();

    vector<pair<int,pii>> cand;
    for (pii p : min_cut) if (grid[p.first][p.second] == EMPTY) {
        if (dist_to_border[p.first][p.second] + turn > n*n) continue;
        int tot = 0;
        for (pii q : nei[p.first][p.second]) {
            char c = grid[q.first][q.second];
            if (c == ELF) {
                tot=max(tot,inside[q.first][q.second]?10:10);
            }
            if (c == ELF_PRESENT) {
                tot=max(tot,inside[q.first][q.second]?10:10);
            }
        }
        if (tot) cand.push_back(make_pair(-tot, p));
    }
    stable_sort(cand.begin(), cand.end());
    for (auto x : cand) if (money >= C) {
        /*
        pii p = x.second;
        int mask = 0;
        for (pii q : nei[p.first][p.second]) {
            if (inside[q.first][q.second]) mask |= 1; else mask |= 2;
        }
        if (mask != 3) continue;
        */
        MarkBox(x.second);
    }
    UnmarkBoxes();
}

void PrintState() {
    cerr << turn << endl;
    cerr << n << " " << C << " " << elfP << " " << money << endl;
    F0(i,n) { F0(j,n) cerr<<grid[i][j]; cerr << endl; }
}

vector<int> sims;
int done_sim = -1;
int cstrat;
int strat = -1;
int bt;
int sim_total;

int a[N*N+1][MAXSN], q, ssum[N*N+1];
vector<int> lft;

void Elim(int leave) {
    if (SZ(lft) <= leave) return;
    vector<pii> v;
    for (int i : lft) v.push_back(pii(ssum[i], i));
    sort(v.rbegin(), v.rend());
    lft.clear();
    F0(i, leave) lft.push_back(v[i].second);
}

void Simulate(bool fast = true) {
    SIM = 1;
    CalcGood();
    while (turn <= n * n) {
        //PrintState();
        int presents_now = PresentsPresent();
        if (!presents_now) break;
        if (cstrat == 0) {
            if (fast) ChooseMove();
            else ChooseMove2();
        } else if (cstrat == 2) {
            Dumb();
        }
        MakeMove();
    }
    int score = money + PresentsPresent() * 100;
    sims.push_back(score);
    SIM = 0;
}

void SimChoose() {
    cstrat = 0;
    SaveState(0);
    vector<pii> modes = {{1,1},{2,2},{3,3},{4,4},{5,5},{1,5},{2,6},{3,7},{4,8},{2,5}};
    pii bmode;
    int m = SZ(modes);

    lft.clear();
    F0(i, m) {
        lft.push_back(i);
        ssum[i] = 0;
    }
    for (sid = 0; sid < SN; sid++) {
        sims.clear();
        for (int i : lft) {
            rng[sid].reseed(sid + 1);
            mode = modes[i];
            Simulate();
            ssum[i] += sims.back();
            LoadState(0);
        }
        if (sid + 1 == SN/8) Elim(4);
        if (sid + 1 == SN/2) Elim(2);
    }
    Elim(1);
    int i = lft[0];
    int total = ssum[i];
    if (total > bt) {
        //cerr << modes[i] << " " << total/SN << endl;
        bt = total;
        bmode = modes[i];
        strat = 0;
    }
    mode = bmode;
    if (done_sim == -1) {
        if (n <= 20) {
            bt = 0;
            sims.clear();
            for (sid = 0; sid < SN; sid++) {
                rng[sid].reseed(sid + 1);
                Simulate(false);
                bt += sims.back();
                LoadState(0);
            }
        } else {
            bt *= 1.1;
        }
    }
}

void AddEnd(pii p) {
    int i1 = id[p.first][p.second];
    AddEdge(i1 + 1, 1, inf);
}

void RemoveEnd(pii p) {
    F0(i, last_flow_n) {
        int e = last_flow[i].first;
        int cap = last_flow[i].second;
        ecap[e] += cap;
        ecap[e^1] -= cap;
    }
    int i1 = id[p.first][p.second];
    RemoveEdge(i1 + 1, 1);
}

void DynamicFlowInit() {
    flow_size = 0;
    t = 2;
    m = 0;
    adj[0].clear();
    adj[1].clear();
    gverts.clear();
    F0(i, n) F0(j, n) {
        id[i][j] = -1;
        if (grid[i][j] == TREE) continue;
        if (grid[i][j] == BOX) continue;
        pii q(i, j);
        gverts.push_back(q);
        if (!In2(q)) {
            id[i][j] = 0;
        } else {
            id[i][j] = t;
            adj[t].clear();
            adj[t+1].clear();
            t += 2;
        }
    }
    for (pii p1 : gverts) {
        int i1 = id[p1.first][p1.second];
        if (i1 == 1) continue;
        if (i1 > 1) {
            AddEdge(i1, i1 + 1, grid[p1.first][p1.second] != EMPTY ? inf : 1);
        }
        for (pii p2 : nei[p1.first][p1.second]) {
            int i2 = id[p2.first][p2.second];
            if (i2 <= 0) continue;
            AddEdge(i1 == 0 ? 0 : i1 + 1, i2, inf);
        }
    }
}

void CalcCut() {
    F0(i, t) fv[i] = 0;
    Cut(flow_dir);
    unordered_set<int> S;
    for (int i = 0; i < t; i++) if (fv[i]) for (int e : adj[i]) {
        int j = ey[e];
        if (!fv[j] && e%2==0) {
            S.insert(i);
        }
    }
    min_cut.clear();
    F0(i, n) {
        F0(j, n) {
            if (S.count(id[i][j]+flow_dir)) {
                min_cut.push_back(pii(i, j));
            }
        }
    }
    assert(SZ(min_cut) == flow_size);
}

void CalcFlow() {
    last_flow_n = 0;
    last_flow_cap = flow();
    flow_size += last_flow_cap;
}

vector<pii> bmin_cut;
vector<pii> best_cuts[N*N+1];
int best_counts[N*N+1];
vector<pii> presents;
int pn;

void FindBest() {
    DynamicFlowInit();
    vector<bool> added(pn, 0);
    int cnt = 0;
    F0(i, pn) {
        if (added[i]) continue;
        added[i] = true;
        pii p = presents[i];
        cnt++;
        AddEnd(p);
        CalcFlow();
        for (int j = i + 1; j < pn; j++) if (!added[j]) {
            pii p = presents[j];
            AddEnd(p);
            CalcFlow();
            if (last_flow_cap == 0) {
                added[j] = true;
                cnt++;
            } else {
                flow_size -= last_flow_cap;
                RemoveEnd(p);
            }
        }
        if (best_counts[flow_size] < cnt) {
            best_counts[flow_size] = cnt;
            CalcCut();
            best_cuts[flow_size] = min_cut;
        }
    }
    F0(i, SZ(presents)) {
        int j = my.nextInt(i + 1);
        swap(presents[i], presents[j]);
    }
}

void EstBest() {
    lft.clear();
    F1(i, n*n) {
        int bad = 0;
        F0(j, i) if (best_counts[j] >= best_counts[i]) {
            bad = 1;
            break;
        }
        if (!bad) {
            lft.push_back(i);
            ssum[i] = 0;
        }
    }

    SaveState(0);
    for (sid = 0; sid < SN; sid++) {
        //cerr << i << " " << best_counts[i] << endl;
        sims.clear();
        for (int i : lft) {
            rng[sid].reseed(sid + 1);
            min_cut = best_cuts[i];
            Simulate();
            ssum[i] += sims.back();
            LoadState(0);
        }
        if (sid + 1 == SN / 8) Elim(8);
        if (sid + 1 == SN / 4) Elim(4);
        if (sid + 1 == SN / 2) Elim(2);
    }
    Elim(1);
    int i = lft[0];
    int total = ssum[i];
    if (total > bt) {
        cerr << best_counts[i] << "/" << pn << " " << sim_total / SN << " " << sims << endl;
        bt = total;
        strat = cstrat;
        bmin_cut = best_cuts[i];
    }
}

void CalcBorder() {
    //if (1.0 / C < elfP) return;
    cstrat = 2;
    presents.clear();
    F0(i,n) F0(j,n) if (HasPresent(grid[i][j])) presents.push_back(pii(i,j));
    pn=SZ(presents);
    sort(presents.begin(), presents.end(), [&](pii p1, pii p2) {
        return dist_to_border[p1.first][p1.second] > dist_to_border[p2.first][p2.second];
    });

    F1(i, n*n) best_counts[i] = 0;
    double TL = Elapsed() + 0.8;
    while (Elapsed() < TL) {
        FindBest();
    }
    EstBest();
    min_cut = bmin_cut;
}

void PlanAll() {
    int step = n * n / (30 * 30 * 30 / n / n / n);
    if (done_sim == -1 || done_sim + step < turn) {
        bool started = false;
        F0(i, n) F0(j, n) if (grid[i][j] == ELF_BOX || grid[i][j] == ELF_PRESENT) started = true;
        if (started) {
            bt = -1;
            if (done_sim == -1) {
                SimChoose();
                CalcBorder();
            } else if (strat != 2) {
                SimChoose();
            } else if (strat == 2) {
                SaveState(0);
                bt = 0;
                sims.clear();
                cstrat = 2;
                for (sid = 0; sid < SN; sid++) {
                    rng[sid].reseed(sid + 1);
                    Simulate();
                    LoadState(0);
                    bt += sims.back();
                }
                SimChoose();
            }
            done_sim = turn;
        }
    }
}

struct StopTheElves {
void Solve() {
    Init();
    Prepare();

    SN = 8;
    if (n <= 16) SN = 32;
    else if (n <= 24) SN = 16;
    else SN = 8;

    while (turn <= n*n) {
        PlanAll();
        if (strat == 0) ChooseMove2();
        if (strat == 2) Dumb();

        game_over = true;
        F0(i, n) F0(j, n) {
            if (In2(pii(i, j))) {
                if (grid[i][j] == PRESENT || grid[i][j] == ELF_PRESENT) game_over = false;
            }
        }

        if (game_over || turn >= n*n) {
            PR(elapsedTime);
            PR(Elapsed());
        }

        if (bmoves.empty()) cout << -1 << endl;
        else {
            for (pii p : bmoves) {
                if (p != bmoves[0]) cout << " ";
                cout << p.first << " " << p.second;
            }
            cout << endl;
        }

        if (game_over) break;

        cin >> elapsedTime >> money;
        F0(i, n) F0(j, n) cin >> grid[i][j];
        saveTime = GetSeconds() - elapsedTime * 0.001;
        //if (elapsedTime > 7000) throw;
        turn++;
    }
}};

void RunVis() {
    StopTheElves obj;
    cin >> n >> C >> elfP >> money;
    F0(i, n) F0(j, n) cin >> grid[i][j];
    obj.Solve();
}

void RunFromSave() {
    ignore = freopen("1.txt", "r", stdin);
    cin >> turn;
    RunVis();
}

void RunVisSave() {
    ignore = freopen("1.txt", "w", stdout);
    cin >> n >> C >> elfP >> money;
    F0(i, n) F0(j, n) cin >> grid[i][j];
    cout << n << " " << C << " " << elfP << " " << money << endl;
    F0(i, n) {
        F0(j, n) cout << grid[i][j];
        cout << endl;
    }
}

void RunLocal(int seed) {

}

int main(int argc, char* argv[]) {
#ifdef RED
    auto x = freopen("log.txt", "w", stderr); (void)x;
#endif

    (void)argv; if (argc > 1) {
        STANDARD = 0;
        RunFromSave(); exit(0);
    }
    //RunVisSave(); exit(0);
    RunVis();

    return 0;
}
