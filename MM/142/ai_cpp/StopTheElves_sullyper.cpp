// C++11
#include <bits/stdc++.h>
#include <sys/time.h>

#ifdef LOCAL
#include "../headers/logger.hpp"
#define LOG_TC(...) LOG_INFO(__VA_ARGS__);
#else
#define LOG_DEBUG(...);
#define LOG_INFO(...);
#define LOG_WARNING(...);
#define LOG_ERROR(...);
#define LOG_TC(...) {fprintf(stderr, __VA_ARGS__);fprintf(stderr, "\n"); }
#define PANIC(...);
#define ASSERT(...);
#endif

#ifdef USEPROFILER
#include "../Profiler.hpp"
#else
#define PROFILER_VAR(var, name);
#define PROFILER(name);
#endif

static inline double get_time() {
  timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

// Need calibration
static inline double get_fast_time() {
  unsigned long long timelo, timehi;
  __asm__ volatile("rdtsc" : "=a"(timelo), "=d"(timehi));
  return ((timehi << 32) + timelo) * 3.5714286e-10;
}

const double END_TIME = 9.4;
double start_time = get_time();
double end_time = start_time + END_TIME;

struct rand_gen {
  static constexpr uint32_t kMax = 2147483647;
  static constexpr double kInvMax = 1.0 / (double)kMax;
  static constexpr __uint128_t kAdd = 0x60bee2bee120fc15ull;
  static constexpr __uint128_t kMul1 = 0xa3b195354a39b70dull;
  static constexpr __uint128_t kMul2 = 0x1b03738712fad5c9ull;
  uint64_t state = 42;
  inline void seed(uint64_t seed) { state = seed; }
  inline uint32_t next_int() {
    state += kAdd; __uint128_t tmp = (__uint128_t)state * kMul1;
    uint64_t m1 = (tmp >> 64) ^ tmp; tmp = (__uint128_t)m1 * kMul2;
    uint64_t m2 = (tmp >> 64) ^ tmp; return (uint32_t)m2;
  }
  inline uint32_t next_int(uint32_t max) { return next_int()%max; }
  inline uint32_t next_int(uint32_t min, uint32_t max) { return min + (next_int()%(max-min)); }
  inline double next_float() { return next_int(kMax) * kInvMax; }
} rng;

template <class T>
class FastVector { public:
  FastVector() { size_ = 0; }
  FastVector(const FastVector& vec) {
    size_ = vec.size_;
    for (int i = 0; i < size_; i++) data_[i] = vec.data_[i];
  }
  FastVector& operator=(const FastVector& vec) {
    size_ = vec.size_;
    for (int i = 0; i < size_; i++) data_[i] = vec.data_[i];
    return *this;
  }
  void clear() {size_ = 0;}
  inline void push_back(T v) {data_[size_++] = v;}
  inline void pop_back() { --size_;}
  inline const T& back() const {return data_[size_-1];}
  inline T pop() { return data_[--size_];}
  inline T& operator[](int i) {return data_[i];}
  inline int size() {return size_;}
  inline void resize(int size) {size_ = size;}
  inline bool empty() {return size_ == 0;}
  inline T* begin() { return &data_[0]; }
  inline T* end() { return begin() + size_; }
 private: std::array<T, 1024> data_; int size_;
};

template <class T, int N = 1024>
class FastList { public:
  FastList() { beg_ = N; end_ = N; }
  void clear() {end_ = N; beg_ = N;}
  inline void push_back(const T& v) {data_[end_++] = v;}
  inline void push_front(T v) {data_[--beg_] = v;}
  inline void pop_back() { --end_;}
  inline T& front() {return data_[beg_];}
  inline T& back() {return data_[end_-1];}
  inline T& pop() { return data_[--end_];}
  inline T& pop_front() { return data_[beg_++];}
  inline T& operator[](int i) {return data_[beg_ + i];}
  inline int size() {return end_ - beg_;}
  inline void resize(int size) {end_ = beg_ + size;}
  inline bool empty() {return end_ == beg_;}
  inline T* begin() { return &data_[beg_]; }
  inline T* end() { return &data_[end_]; }
 private: std::array<T, 2*N> data_; int end_; int beg_;
};

template <int Size = 1024>
class FastMap { public:
  FastMap() {size_ = 0; std::fill(pos_.begin(), pos_.end(), -1);}
  void clear() {while (size_) pos_[taken_[--size_]] = -1;}

  inline void safeInsert(int v) {if (!has(v)) insert(v);}
  inline void insert(int v) {taken_[pos_[v] = size_++] = v;}
  inline void push_back(int v) {taken_[pos_[v] = size_++] = v;}
  inline void pop_back() {pos_[taken_[--size_]] = -1;}
  inline int back() {return taken_[size_-1];}
  inline void erase(int v) {
    const int p = pos_[v];
    const int last = taken_[--size_];
    pos_[last] = p;
    taken_[p] = last;
    pos_[v] = -1;
  }
  inline int operator[](int i) {return taken_[i];}
  inline void safeErase(int v) {if (has(v)) erase(v);}
  inline bool has(int v) {return pos_[v] >= 0;}
  std::vector<int> toVector() {
    std::vector<int> ret;
    ret.insert(ret.begin(), begin(), end());
    return ret;
  }
  inline int size() {return size_;}
  inline bool empty() {return size_ == 0;}
  inline int* begin() { return taken_.data(); }
  inline int* end() { return taken_.data() + size_; }

private:
  std::array<int, Size> pos_;
  std::array<int, Size> taken_;
  int size_;
};
template <class T>
void Shuffle(T& vec) {
  for (int i = 0; i < vec.size(); i++) {
    std::swap(vec[i], vec[rng.next_int(i, vec.size())]);
  }
}

//////////////////////////////////////////////////
int N, C, M;
double eP;

constexpr int kEmpty = 0;
constexpr int kTree = 1;
constexpr int kPresent = 2;
constexpr int kBox = 4;
constexpr int kElf = 8;
constexpr int kElfBox = 12;
constexpr int kElfPresent = 10;

int SpwanElf[1024];
bool marked[32][32];
bool outside[32][32];
int grid[32][32];
int gridInit[32][32];
int PresentDOWN[32][32];
int PresentUP[32][32];
int PresentLEFT[32][32];
int PresentRIGHT[32][32];
int CornerTL[32][32];
int CornerTR[32][32];
int CornerBL[32][32];
int CornerBR[32][32];
int elapsedTime;
int numPresents;
int numElves;
int numTrees;
int numBoxes;
int turn;
int helper;
bool let_inside;

FastMap<> blocked;
FastMap<> presents;
FastMap<> elves;

void SaveGrid() {
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      gridInit[row][col] = grid[row][col];
    }
  }
}
void ResetGrid() {
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      grid[row][col] = gridInit[row][col];
    }
  }
}

bool swap_board;
void SwapBoard() {
  swap_board = !swap_board;
  for (int row = 0; row < N; row++) {
    for (int col = row+1; col < N; col++) {
      std::swap(grid[row][col], grid[col][row]);
    }
  }
}

void Init() {
  PROFILER("Init");
  for (int row = 0; row < N; row++) {
    for (int col = 1; col < N; col++) {
      PresentLEFT[row][col] = PresentLEFT[row][col - 1] + (grid[row][col] == kPresent ? 1 : 0);
    }
    for (int col = N-2; col >= 0; col--) {
      PresentRIGHT[row][col] = PresentRIGHT[row][col + 1] + (grid[row][col] == kPresent ? 1 : 0);
    }
  }
}
double loading_time = 0.0;
void LoadBoard(bool gettime) {
  PROFILER("LoadBoard");
  if (gettime) {
    scanf("%d\n", &elapsedTime);
    ASSERT(elapsedTime < 9950, "Exceed time limit %d", elapsedTime);
  }
  loading_time -= get_time();
  scanf("%d\n", &M);
  int pv = numPresents;
  int pb = numBoxes;
  numPresents = 0;
  numTrees = 0;
  numElves = 0;
  numBoxes = 0;
  swap_board = false;
  presents.clear();
  elves.clear();
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      char c = getchar();getchar();
      switch(c) {
        case '.': grid[row][col] = kEmpty; break;
        case 'T': grid[row][col] = kTree; numTrees++; break;
        case 'P': grid[row][col] = kPresent; numPresents++; presents.insert(row*32+col); break;
        case 'b': grid[row][col] = kBox; numBoxes++; break;
        case 'e': grid[row][col] = kElf; numElves++; elves.insert(row*32+col); break;
        case 'B': grid[row][col] = kElfBox; break;
        case 'E': grid[row][col] = kElfPresent; break;
        default: PANIC("Invalid input: %c", c);
      }
    }
  }
  // SwapBoard();
  if (pv != numPresents || numBoxes != pb) {
    Init();
    helper++;
  }
  end_time = get_time() + END_TIME - elapsedTime / 1000.0;
  loading_time += get_time();
}

double writting_time = 0.0;
void PlaceBoxes(const std::vector<std::pair<int, int>>& boxes) {
  PROFILER("PlaceBoxes");
  writting_time -= get_time();
  if (boxes.empty()) {
    printf("-1\n");
  } else {
    numBoxes = -1;
    if (swap_board) for (const auto& box : boxes) printf("%d %d ", box.second, box.first);
    else for (const auto& box : boxes) printf("%d %d ", box.first, box.second);
    printf("\n");
  }
  fflush(stdout);
  writting_time += get_time();
}

void MarkOutside() {
  PROFILER("MarkOutside");
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      outside[row][col] = false;
    }
  }
  static std::vector<std::pair<int, int>> dd; dd.clear();
  auto add = [&](int row, int col) {
    if (outside[row][col] || grid[row][col] == kTree || marked[row][col] == kTree) return;
    outside[row][col] = true;
    dd.push_back({row, col});
  };
  for (int i = 0; i+1 < N; i++) {
    add(i, 0); add(N-1, i); add(N-i-1, N-1); add(0, N-1-i);
  }
  for (int d = 0; d < dd.size(); d++) {
    auto p = dd[d];
    if (p.first > 0) add(p.first-1, p.second);
    if (p.first+1 < N) add(p.first+1, p.second);
    if (p.second > 0) add(p.first, p.second-1);
    if (p.second+1 < N) add(p.first, p.second+1);
  }
}

bool reach_present[32][32];
void MarkPresentAvailable() {
  PROFILER("MarkPresentAvailable");
  static FastVector<std::pair<int, int>> dd; dd.clear();
  auto add = [&](int row, int col) {
    if (reach_present[row][col] || grid[row][col] != kEmpty) return;
    reach_present[row][col] = true;
    dd.push_back({row, col});
  };
  {
    // PROFILER("Init");
    for (int row = 0; row < N; row++) {
      for (int col = 0; col < N; col++) {
        reach_present[row][col] = false;
        if (grid[row][col] == kPresent) {
          reach_present[row][col] = true;
          dd.push_back({row, col});
        }
      }
    }
  }
  while (!dd.empty()) {
    const int row = dd.back().first;
    const int col = dd.back().second;
    dd.pop_back();
    // auto p = dd[d].pop();
    if (row > 0) add(row-1, col);
    if (row+1 < N) add(row+1, col);
    if (col > 0) add(row, col-1);
    if (col+1 < N) add(row, col+1);
  }
}

bool HasPresentOutside() {
  PROFILER("HasPresentOutside");
  for (int id : presents) {
    if (outside[id/32][id%32]) return true;
  }
  return false;
}


inline bool hasElf(int row, int col) {
  if (grid[row-1][col] == kElf || grid[row-1][col] == kElfPresent) return true;
  if (grid[row+1][col] == kElf || grid[row+1][col] == kElfPresent) return true;
  if (grid[row][col+1] == kElf || grid[row][col+1] == kElfPresent) return true;
  if (grid[row][col-1] == kElf || grid[row][col-1] == kElfPresent) return true;
  return false;
}

void PrepareBFS() {
  PROFILER("PrepareBFS");
  for (int row = 0; row < N; row++) {
    for (int col = 1; col < N; col++) {
      PresentLEFT[row][col] = PresentLEFT[row][col - 1] + (grid[row][col] == kPresent ? 1 : 0);
    }
    for (int col = N-2; col >= 0; col--) {
      PresentRIGHT[row][col] = PresentRIGHT[row][col + 1] + (grid[row][col] == kPresent ? 1 : 0);
    }
  }
  for (int col = 0; col < N; col++) {
    for (int row = 1; row < N; row++) {
      PresentUP[row][col] = PresentUP[row - 1][col] + (grid[row][col] == kPresent ? 1 : 0);
    }
    for (int row = N-2; row >= 0; row--) {
      PresentDOWN[row][col] = PresentDOWN[row + 1][col] + (grid[row][col] == kPresent ? 1 : 0);
    }
  }
  for (int row = 1; row < N; row++) {
    for (int col = 1; col < N; col++) {
      CornerTL[row][col] = CornerTL[row-1][col] + PresentLEFT[row][col-1];
      CornerBL[N-row-1][col] = CornerBL[N-row][col] + PresentLEFT[N-row][col];
      CornerBR[N-row-1][N-col-1] = CornerBR[N-row][N-col-1] + PresentRIGHT[N-row-1][N-col];
      CornerTR[row][N-col-1] = CornerTR[row-1][N-col-1] + PresentRIGHT[row-1][N-col-1];
    }
  }
}

void MakeSafer(std::vector<std::pair<int, int>>& vec) {
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      marked[row][col] = false;
    }
  }
  for (auto& p : vec) {
    marked[p.first][p.second] = true;
  }
  while (true) {
    bool changed = false;
    for (auto& b : vec) {
      int row = b.first;
      int col = b.second;
      if (grid[row][col] == kTree) continue;
      if (row == 1) {
        if ((grid[2][col] & kPresent) == 0 && (marked[2][col-1] || marked[1][col-1]) && (marked[2][col+1] || marked[1][col+1])) {
          changed = true;
          marked[row][col] = false;
          marked[row+1][col] = true;
          b.first++;
        }
      } else if (col == 1) {
        if ((grid[row][2] & kPresent) == 0 && (marked[row-1][2] || marked[row-1][1]) && (marked[row+1][2] || marked[row+1][1])) {
          changed = true;
          marked[row][col] = false;
          marked[row][col+1] = true;
          b.second++;
        }
      }
    }
    if (!changed) break;
  }
}

int BFSDist[32][32][4][300];
int BFSv[32][32][4][300];
struct NodeBFS {
  int row, col;
  int mode;
  int skipped;
  int dist;
};
NodeBFS BFSf[32][32][4][300];
std::deque<NodeBFS> qq;
int helper_bfs;
void BFS(int start) {
  PROFILER("BFS");
  helper_bfs++;
  qq.clear();

  auto add = [&](int row, int col, int mode, int skipped, int dist, const NodeBFS& node) {
    if (row < 0 || col < 0 || row == N || col == N || skipped > numPresents) return;
    if ((row == 0 || col == 0 || row == N-1 || col == N-1) && grid[row][col] != kTree) return;
    if (BFSv[row][col][mode][skipped] != helper_bfs ||
        BFSDist[row][col][mode][skipped] > dist &&
        (skipped == 0 || BFSv[row][col][mode][skipped-1] != helper_bfs ||
         BFSDist[row][col][mode][skipped-1] >= dist)) {
      if (dist == node.dist) qq.push_front({row, col, mode, skipped, dist});
      else qq.push_back({row, col, mode, skipped, dist});
      BFSf[row][col][mode][skipped] = node;
      BFSv[row][col][mode][skipped] = helper_bfs;
      BFSDist[row][col][mode][skipped] = dist;
    }
  };
  add(start, start, 0, CornerTL[start][start], 0, {0, 0, 0, 0, -1});

  while (!qq.empty()) {
    auto node = qq.front();
    const int row = node.row;
    const int col = node.col;
    const int mode = node.mode;
    const int skipped = node.skipped;
    int dist = node.dist;
    // if (skipped == 0) LOG_DEBUG("%d %d %d %d %d", qq.front().row, qq.front().col, qq.front().mode, qq.front().skipped, qq.front().dist);
    qq.pop_front();
    if (BFSDist[row][col][mode][skipped] > dist) continue;
    if (node.row == start && node.col == start && node.mode == 3) continue;
    if (node.mode == 0) add(row, col, mode+1, skipped + CornerBL[row][col], dist, node);
    if (node.mode == 1) add(row, col, mode+1, skipped + CornerBR[row][col], dist, node);
    if (node.mode == 2) add(row, col, mode+1, skipped + CornerTR[row][col], dist, node);
    if (grid[row][col] != kTree) dist++;
    if (mode == 0) {
      add(row, col-1, 0, skipped, dist, node);
      add(row, col+1, 0, skipped + (grid[row][col+1] != kTree), dist, node);
      add(row+1, col-1, 0, skipped + PresentLEFT[row+1][col-1], dist, node);
      add(row+1, col, 0, skipped + PresentLEFT[row+1][col], dist, node);
      add(row+1, col+1, 0, skipped + PresentLEFT[row+1][col+1], dist, node);
    } else if (mode == 2) {
      add(row, col+1, 2, skipped, dist, node);
      add(row, col-1, 2, skipped + (grid[row][col-1] != kTree), dist, node);
      add(row-1, col-1, 2, skipped + PresentRIGHT[row-1][col-1], dist, node);
      add(row-1, col, 2, skipped + PresentRIGHT[row-1][col], dist, node);
      add(row-1, col+1, 2, skipped + PresentRIGHT[row-1][col+1], dist, node);
    } else if (mode == 1) {
      add(row+1, col, 1, skipped, dist, node);
      add(row-1, col, 1, skipped + (grid[row-1][col] != kTree), dist, node);
      add(row-1, col+1, 1, skipped + PresentDOWN[row-1][col+1], dist, node);
      add(row, col+1, 1, skipped + PresentDOWN[row][col+1], dist, node);
      add(row+1, col+1, 1, skipped + PresentDOWN[row+1][col+1], dist, node);
    } else if (mode == 3) {
      add(row-1, col, 3, skipped, dist, node);
      add(row+1, col, 3, skipped + (grid[row+1][col] != kTree), dist, node);
      add(row-1, col-1, 3, skipped + PresentUP[row-1][col-1], dist, node);
      add(row, col-1, 3, skipped + PresentUP[row][col-1], dist, node);
      add(row+1, col-1, 3, skipped + PresentUP[row+1][col-1], dist, node);
    }
  }
}

std::vector<std::vector<std::pair<int, int>>> blocked_list;
std::vector<int> blocked_dist;
void RunAllBFS() {
  PROFILER("RunAllBFS");
  double stime = get_time();
  blocked_list.resize(numPresents+1);
  blocked_dist.resize(numPresents+1, 1000);
  PrepareBFS();
  for (int i = 1; i < N/2; i++) {
    BFS(i);
    for (int sk = 0; sk+1 < numPresents; sk++) {
      if (BFSv[i][i][3][sk] != helper_bfs) continue;
      if (blocked_dist[sk] > BFSDist[i][i][3][sk]) {
        blocked_dist[sk] = BFSDist[i][i][3][sk];
        blocked_list[sk].clear();
        auto node = BFSf[i][i][3][sk];
        while (node.dist != -1) {
          // if (sk == 0) LOG_DEBUG("%d %d - %d - %d %d", node.row, node.col, node.mode, node.skipped, node.dist);
          blocked_list[sk].push_back({node.row, node.col});
          node = BFSf[node.row][node.col][node.mode][node.skipped];
        }
      }
    }
  }
  LOG_TC("RunAllBFS in %lf", get_time() - stime);
}

// Row, C1, d < C2, d
int DP[32][32][2][32][2];
int DPv[32][32][2][32][2];
std::pair<int, std::pair<std::pair<int, int>, std::pair<int, int>>> DPf[32][32][2][32][2];

constexpr int kInfinity = 100000000;
int fct(int row, int c1, int d1, int c2, int d2) {
  if (row == N-1) {
    if (c1 == 0 && c2 == 0) return 0;
    return kInfinity;
  }
  if (c1 > c2) return kInfinity;
  if (PresentLEFT[row][c1] > 0) return kInfinity;
  if (PresentRIGHT[row][c2] > 0) return kInfinity;

  int& best = DP[row][c1][d1][c2][d2];
  auto& link = DPf[row][c1][d1][c2][d2];
  if (DPv[row][c1][d1][c2][d2] == helper) return best;
  DPv[row][c1][d1][c2][d2] = helper;
  best = kInfinity;

  auto update = [&](int row, int c1, int d1, int c2, int d2, int add) {
    int cost = fct(row, c1, d1, c2, d2) + add;
    if (best > cost) {
      best = cost;
      link = {row, {{c1, d1}, {c2, d2}}};
    }
  };

  if (c1 == 0 && c2 == 0) {
    if (PresentRIGHT[row][0] == 0) {
      best = fct(row+1, 0, 0, 0, 0);
      link = {row+1, {{0, 0}, {0, 0}}};
    }
    for (int c = 1; c+1 < N; c++) {
      update(row, c, 0, c, 1, 0);
    }
    return best;
  }

  if (c1 == 0) return kInfinity;
  if (c2 == N-1) return kInfinity;

  const int cc1 = (grid[row][c1] == kTree || grid[row][c1] == kBox) ? 0 : 1000;
  const int cc2 = (c1 == c2 || grid[row][c2] == kTree || grid[row][c2] == kBox) ? 0 : 1000;

  if (c1 == c2 && d1 == 1 && d2 == 0) {
    link = {row+1, {{0, 0}, {0, 0}}};
    return best = cc1 + fct(row+1, 0, 0, 0, 0);
  }

  if (row < N-2) {
    for (int nd1 = 0; nd1 < 2; nd1++) {
      for (int d1 = -1; d1 < 2; d1++) {
        if (c1+d1 == 0) continue;
        for (int nd2 = 0; nd2 < 2; nd2++) {
          for (int d2 = -1; d2 < 2; d2++) {
            if (c2+d2 == 0) continue;
            update(row+1, c1 + d1, nd1, c2 + d2, nd2, cc1+cc2);
          }
        }
      }
    }
  }
  update(row, c1+2*d1-1, d1, c2, d2, cc1);
  update(row, c1, d1, c2+2*d2-1, d2, cc2);
  // LOG_DEBUG("%d %d %d %d %d -> %d", row, c1, d1, c2, d2, best);
  return best;
}

//*
int DP2[32][32][2][32][2][328];
struct Node {
  Node() {}
  Node(int k) { key = k; }
  Node(int row, int c1, int d1, int c2, int d2, int p) {
    // ASSERT(row >= 0 && c1 >= 0 && d1 >= 0 && d2 >= 0 && c2 >= 0 && p >= 0, "");
    // ASSERT(row < 32 && c1 < 32 && d1 < 2 && d2 < 2 && c2 < 32 && p < 512,
    //        "%d %d %d %d %d %d", row, c1, d1, c2, d2, p);
    key = (row<<24)|(c1<<18)|(d1<<16)|(c2<<10)|(d2<<9)|p;
  }
  inline int row() const { return key >> 24;}
  inline int c1() const { return (key >> 18)&31;}
  inline int d1() const { return (key >> 16)&1;}
  inline int c2() const { return (key >> 10)&31;}
  inline int d2() const { return (key >> 9)&1;}
  inline int p() const { return (key)&511;}
  uint32_t key;
};
Node DPf2[32][32][2][32][2][328];
inline void fct2(int row, int c1, int d1, int c2, int d2, int p) {
  if (c1 == 0 && grid[row][c1] != kTree) return;
  if (c1 == N-1 && grid[row][c1] != kTree) return;
  if (c2 == 0 && grid[row][c2] != kTree) return;
  if (c2 == N-1 && grid[row][c2] != kTree) return;
  auto& best = DP2[row][c1][d1][c2][d2][p];
  auto& link = DPf2[row][c1][d1][c2][d2][p];

  // auto update = [&](int row, int c1, int d1, int c2, int d2, int p, int add, bool update) {
  #define update(row, c1, d1, c2, d2, p, add) { \
    int cost = DP2[row][c1][d1][c2][d2][p] + add;       \
    if (best > cost) {                                  \
      best = cost;                                      \
      link = {row, c1, d1, c2, d2, p};                  \
    }                                                   \
  };

  if (c1 == 30) {
    update(row+1, 30, 0, 30, 0, p, 0);
    for (int c = 1; c+1 < N; c++) {
      update(row, c, 0, c, 1, p, 0);
    }
    return;
  }

  const int cc1 = (grid[row][c1] == kTree || grid[row][c1] == kBox) ? 0 : 1;
  const int cc2 = (c1 == c2 || grid[row][c2] == kTree || grid[row][c2] == kBox) ? 0 : 1;
  if (grid[row][c1] == kPresent && d1 == 1) p++;
  if (grid[row][c2] == kPresent && (d1 == 0 || c1 != c2) && d2 == 0) p++;
  if (p > 28 + numPresents) return;

  if (c1 == c2 && d1 == 1 && d2 == 0) {
    update(row+1, 30, 0, 30, 0, p, cc1);
  }

  if (row < N-2) {
    for (int nd1 = 0; nd1 < 2; nd1++) {
      for (int d1 = -1; d1 < 2; d1++) {
        if (c1+d1 == -1) continue;
        for (int nd2 = 0; nd2 < 2; nd2++) {
          for (int d2 = -1; d2 < 2; d2++) {
            if (c2+d2 + nd2 - 1 < 0) continue;
            const int up = PresentRIGHT[row+1][c1+d1 + 1 - nd1] -
                           PresentRIGHT[row+1][c2+d2 + nd2 - 1];
            if (p < up) continue;
            update(row+1, c1 + d1, nd1, c2 + d2, nd2, p - up, cc1+cc2);
          }
        }
      }
    }
  }
  update(row, c1+2*d1-1, d1, c2, d2, p, cc1);
  update(row, c1, d1, c2+2*d2-1, d2, p, cc2);
  #undef update
}

void ComputeFct2() {
  PROFILER("ComputeFct2");
  {
    PROFILER("fill");
    std::fill(&DP2[0][0][0][0][0][0], &DP2[30][0][0][0][0][0], kInfinity);
  }
  DP2[N-1][30][0][30][0][28] = 0;
  for (int row = N-2; row > 0; row--) {
    for (int c1 = 0; c1 < N; c1++) {
      for (int c2 = c1+1; c2 < N; c2++) {
        for (int p = 28 + numPresents; p > 0; p--) {
          fct2(row, c1, 0, c2, 0, p);
        }
      }
    }
    for (int c1 = 0; c1 < N; c1++) {
      for (int c2 = N-1; c2 >= c1; c2--) {
        for (int p = 28 + numPresents; p > 0; p--) {
          fct2(row, c1, 0, c2, 1, p);
        }
      }
    }
    for (int c1 = N-1; c1 >= 0; c1--) {
      for (int c2 = c1; c2 < N; c2++) {
        for (int p = 28 + numPresents; p > 0; p--) {
          fct2(row, c1, 1, c2, 0, p);
        }
      }
    }
    for (int c1 = N-1; c1 >= 0; c1--) {
      for (int c2 = N-1; c2 > c1; c2--) {
        for (int p = 28 + numPresents; p > 24; p--) {
          fct2(row, c1, 1, c2, 1, p);
        }
      }
    }
    for (int p = 28 + numPresents; p > 24; p--) {
      fct2(row, 30, 0, 30, 0, p);
    }
  }
}

int Simulate();

void Solve2() {
  PROFILER("Solve2");
  helper++;
  auto extract = [&](int bb) {
    PROFILER("extract");
    blocked.clear();
    for (int row = 0; row < N; row++) {
      for (int col = 0; col < N; col++) {
        marked[row][col] = false;
      }
    }
    for (auto& p : blocked_list[numPresents - bb]) {
      marked[p.first][p.second] = true;
      blocked.safeInsert(p.first * 32 + p.second);
    }
    MarkOutside();
  };
  int bb = 0;
  double stime_fct2 = get_time();
  {
    PROFILER("fct");
    ComputeFct2();
    for (int i = 0; i <= numPresents; i++) {
      int needed = DP2[1][30][0][30][0][28+i];
      int sc = 0;
      // for (int s = 0; s < 10; s++) sc += Simulate();
      LOG_INFO("Present: %d -> %d (%d) + %d - %d vs %d -> %d",
                i, needed, blocked_dist[numPresents - i], (int)(N * N * eP), numPresents - i, (M + N * N) / C, sc/10);
      if (std::min(needed, blocked_dist[numPresents - i]) + (int)(N * N * eP) - numPresents + i <= (M + N * N) / C) {
        bb = i;
      }
      if (DP2[1][30][0][30][0][28+bb] < blocked_dist[numPresents - bb]) {
        blocked_dist[numPresents - bb] = DP2[1][30][0][30][0][28+bb];
        blocked_list[numPresents - bb].clear();
        int row = 1;
        int d1 = 0, c1 = 30, d2 = 0, c2 = 30, p = bb + 28;
        while (row+1 < N) {
          if (c1 != 30) blocked_list[numPresents - bb].push_back({row, c1});
          if (c2 != c1) blocked_list[numPresents - bb].push_back({row, c2});
          auto& next = DPf2[row][c1][d1][c2][d2][p];
          // LOG_DEBUG("%d %d %d %d %d %d -> %d", row, c1, d1, c2, d2, p-28, DP2[row][c1][d1][c2][d2][p]);
          row = next.row();
          c1 = next.c1();
          d1 = next.d1();
          c2 = next.c2();
          d2 = next.d2();
          p = next.p();
        }
      }
      MakeSafer(blocked_list[numPresents - bb]);
    }
  }
  LOG_TC("FCT2 in %lf", get_time() - stime_fct2);
  LOG_DEBUG("Initial pick: %d", bb);
  std::map<int, int> last[2];
  std::map<int, double> rel[2];
  std::map<int, double> totals[2];
  std::map<int, int> counts[2];
  //*
  double stime = get_time();
  int num_sims = 0;
  int bbs[2] = {bb, bb};
  int bswitch = 0;
  while (counts[0][bbs[0]] < 1000) {
    int bs = 0;
    for (int i = 0; i <= N*N; i++) {
      SpwanElf[i] = rng.next_float() < eP ? rng.next_int(0, 4*N-4) : -1;
    }
    const int dec = 100 / N;
    for (int ii = 0; ii < 2; ii++) {
      let_inside = ii;
      for (int i = bbs[ii] - dec; i <= bbs[ii]+dec; i++) {
        if (i < 0 || i > numPresents) continue;
        extract(i);
        int sc = Simulate();
        num_sims++;
        last[ii][i] = sc;
        bs = std::max(bs, sc);
        if (get_time() > end_time) goto end_loop;
      }
    }
    double sss[2];
    for (int ii = 0; ii < 2; ii++) {
      int nb = bb;
      double ss = 0.0;
      for (int i = bbs[ii] - dec; i <= bbs[ii]+dec; i++) {
        if (i < 0 || i > numPresents) continue;
        totals[ii][i] += last[ii][i];
        counts[ii][i]++;
        rel[ii][i] += last[ii][i] * 100.0 / bs;
        double cs = rel[ii][i] / counts[ii][i];
        if (cs > ss) {
          ss = cs;
          nb = i;
        }
      }
      bbs[ii] = nb;
      sss[ii] = ss;
    }
    if (sss[0] > sss[1]) { bb = bbs[0]; bswitch = false; }
    else  { bb = bbs[1]; bswitch = true; }
  }
  end_loop:;
  let_inside = bswitch;

  for (int ii = 0; ii < 2; ii++) {
    for (auto& it : totals[ii]) {
      LOG_DEBUG("%d -> %.2lf (Avg: %.1lf %d)", it.first, rel[ii][it.first]/counts[ii][it.first],
                it.second / counts[ii][it.first], counts[ii][it.first]);
      // if (it.second * counts[bb] > counts[it.first] * totals[bb]) bb = it.first;
    }
  }
  /**/
  LOG_TC("Pick: %d (%d) (%d simulation in %lf)", bb, bswitch, num_sims, get_time() - stime);
  extract(bb);
  // for (int b : blocked) LOG_INFO("%d %d", b/32, b&31);
  /*
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      if (marked[row][col]) fprintf(stderr, "X");
      else if (outside[row][col]) fprintf(stderr, ".");
      else fprintf(stderr, "#");
    }
    fprintf(stderr, "\n");
  }
  fprintf(stderr, "\n");
  */
}
/**/

void Solve() {
  PROFILER("Solve");
  int needed = fct(1, 0, 0, 0, 0) / 1000;
  int rem = N*N-turn;

  // if (needed > M / C) {
  if (needed + numElves + (int)(rem * eP) > 3 * N * eP +(M + rem) / C) {//} || needed > (M+needed) / C) {
    LOG_INFO("%d + (%d + %d) vs %d", needed, numElves, (int)(rem * eP), (M + rem) / C);
  } else {
    for (int row = 0; row < N; row++) {
      for (int col = 0; col < N; col++) {
        marked[row][col] = false;
      }
    }
    int row = 1;
    int d1 = 0, c1 = 0, d2 = 0, c2 = 0;
    while (row+1 < N) {
      if (c1 > 0) marked[row][c1] = true;
      if (c2 > 0) marked[row][c2] = true;
      auto& next = DPf[row][c1][d1][c2][d2];
      // LOG_DEBUG("%d %d %d %d %d -> %d", row, c1, d1, c2, d2, DP[row][c1][d1][c2][d2]);
      row = next.first;
      c1 = next.second.first.first;
      d1 = next.second.first.second;
      c2 = next.second.second.first;
      d2 = next.second.second.second;
    }
    LOG_WARNING("%d + (%d + %d) vs %d", needed, numElves, (int)(rem * eP), (M + rem) / C);
    MarkOutside();
  }

}

std::vector<std::pair<int, int>> GetBoxes() {
  std::vector<std::pair<int, int>> boxes;
  for (int row = 2; row + 2 < N; row++) {
    for (int col = 2; col + 2 < N; col++) {
      if (grid[row][col] & kPresent) {
        const bool elf = false; // grid[row][col] & kElf;
        if (grid[row-1][col] == kEmpty && C <= M && (elf || hasElf(row-1, col))) {
          grid[row-1][col] = kBox; M -= C;
          boxes.push_back({row-1, col});
        }
        if (grid[row+1][col] == kEmpty && C <= M && (elf ||  hasElf(row+1, col))) {
          grid[row+1][col] = kBox; M -= C;
          boxes.push_back({row+1, col});
        }
        if (grid[row][col-1] == kEmpty && C <= M && (elf ||  hasElf(row, col-1))) {
          grid[row][col-1] = kBox; M -= C;
          boxes.push_back({row, col-1});
        }
        if (grid[row][col+1] == kEmpty && C <= M && (elf ||  hasElf(row, col+1))) {
          grid[row][col+1] = kBox; M -= C;
          boxes.push_back({row, col+1});
        }
      }
    }
  }
  return boxes;
}

bool NoTrivialPath() {
  PROFILER("NoTrivialPath");
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      if (grid[row][col] == kPresent && outside[row][col]) {
        return false;
      }
    }
  }
  for (int b : blocked) {
    if (grid[b/32][b&31] != kTree && grid[b/32][b&31] != kBox) {
      return false;
    }
  }
  return true;
}

int DistanceToEdge[32][32];

std::vector<std::pair<int, int>> interesting_elves;
std::vector<std::pair<int, int>> GetBoxesForMarked() {
  PROFILER("GetBoxesForMarked");
  std::vector<std::pair<int, int>> boxes;
  auto update = [&](int row, int col, bool bypass = false) {
    if (row <= 0 || col <= 0 || row >= N-1 || col >= N-1) return;
    if ((bypass || marked[row][col]) && grid[row][col] == kEmpty && M >= C) {
      grid[row][col] = kBox;
      boxes.push_back({row, col});
      M -= C;
    }
  };
  interesting_elves.clear();
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      if (grid[row][col] == kElfPresent && DistanceToEdge[row][col] < N*N-turn && !outside[row][col]) {
        update(row-1, col); update(row+1, col); update(row, col-1); update(row, col+1);
      }
      if (grid[row][col] == kElf && DistanceToEdge[row][col] < N*N-turn) {
        if (!let_inside) {update(row-1, col); update(row+1, col); update(row, col-1); update(row, col+1);}
        if (row > 0 && (grid[row-1][col] == kBox)) continue;
        if (grid[row+1][col] == kBox) continue;
        if (col > 0 && (grid[row][col-1] == kBox)) continue;
        if (grid[row][col+1] == kBox) continue;
        interesting_elves.push_back({row, col});
      }
    }
  }

  if (!interesting_elves.empty()) {
    MarkPresentAvailable();
    // LOG_INFO("Activated: %d", interesting_elves.size());
    for (auto& e : interesting_elves) {
      if (M < C) break;
      const int row = e.first;
      const int col = e.second;
      if (row > 0 && (grid[row-1][col] == kBox || reach_present[row-1][col])) continue;
      if (grid[row+1][col] == kBox || reach_present[row+1][col]) continue;
      if (col > 0 && (grid[row][col-1] == kBox || reach_present[row][col-1])) continue;
      if (grid[row][col+1] == kBox || reach_present[row][col+1]) continue;

      /* .....
         .E...
         ..#p.
         ..p..
         .....
      */
      if (grid[row+1][col+1] == kBox && marked[row+1][col+1] &&
          (grid[row+2][col+1] == kElfPresent || grid[row+1][col+2] == kElfPresent)) {
        if (grid[row+1][col] == kEmpty) {
          update(row+1, col, true);
          continue;
        }
        if (grid[row][col+1] == kEmpty) {
          update(row, col+1, true);
          continue;
        }
      }

      /* .....
         ...E.
         ..#..
         ..p..
         .....
      */
      if (grid[row+1][col-1] == kBox && marked[row+1][col-1] && grid[row+2][col-1] == kElfPresent) {
        if (grid[row+1][col] == kEmpty) {
          update(row+1, col, true);
          continue;
        }
        if (grid[row][col-1] == kEmpty) {
          update(row, col-1, true);
          continue;
        }
      }

      /* .....
         .....
         ..#p.
         .E...
         .....
      */
      if (grid[row-1][col+1] == kBox && marked[row-1][col+1] && grid[row-1][col+2] == kElfPresent) {
        if (grid[row-1][col] == kEmpty) {
          update(row-1, col, true);
          continue;
        }
        if (grid[row][col+1] == kEmpty) {
          update(row, col+1, true);
          continue;
        }
      }

      /* ..E..
         .....
         ..#p.
         ..p..
         .....
      */
      if (grid[row+2][col] == kBox && marked[row+2][col] &&
          (grid[row+3][col] == kElfPresent || grid[row+2][col+1] == kElfPresent)) {
        if (grid[row+1][col] == kEmpty) {
          update(row+1, col, true);
          continue;
        }
      }
      /* .....
         .....
         ..#p.
         .....
         ..E..
      */
      if (row > 3 && grid[row-2][col] == kBox && marked[row-2][col] && grid[row-2][col+1] == kElfPresent) {
        if (grid[row-1][col] == kEmpty) {
          update(row-1, col, true);
          continue;
        }
      }
      /* .....
         .....
         ..#.E
         ..p..
         .....
      */
      if (col > 3 && grid[row][col-2] == kBox && marked[row][col-2] && grid[row+1][col-2] == kElfPresent) {
        if (grid[row][col-1] == kEmpty) {
          update(row, col-1, true);
          continue;
        }
      }

      /* .....
         .....
         E.#p.
         ..p..
         .....
      */
      if (grid[row][col+2] == kBox && marked[row][col+2] &&
          (grid[row][col+3] == kElfPresent || grid[row+1][col+2] == kElfPresent)) {
        if (grid[row][col+1] == kEmpty) {
          update(row, col+1, true);
          continue;
        }
      }
    }
  }

  return boxes;
}

void InitDistanceToEnd() {
  std::vector<std::pair<int, int>> dd[128];
  auto add = [&](int row, int col, int d) {
    if (DistanceToEdge[row][col] || grid[row][col] == kTree) return;
    DistanceToEdge[row][col] = d;
    dd[d].push_back({row, col});
  };
  for (int i = 0; i+1 < N; i++) {
    add(i, 0, 1); add(N-1, i, 1); add(N-i-1, N-1, 1); add(0, N-1-i, 1);
  }
  for (int d = 1; d < 128; d++) {
    for (auto& p : dd[d]) {
      if (p.first > 0) add(p.first-1, p.second, d+1);
      if (p.first+1 < N) add(p.first+1, p.second, d+1);
      if (p.second > 0) add(p.first, p.second-1, d+1);
      if (p.second+1 < N) add(p.first, p.second+1, d+1);
    }
    dd[d].clear();
  }
}

int Helper[32 * 32];
int Dir[32 * 32];
int hh;
int gg[1024];

FastList<int> open;
template<bool BORDER>
void MoveElf(int row, int col, bool no_path = true) {
  PROFILER(BORDER ? "MoveElfToBorder" : "MoveElfToTarget");
  hh++;
  std::vector<std::array<int, 4>> dirs =
    {{-1, 1, 32, -32}, {-1, 1, -32, 32}, {-1, 32, 1, -32}, {-1, 32, -32, 1}, {-1, -32, 1, 32}, {-1, -32, 32, 1},
     {1, -1, 32, -32}, {1, -1, -32, 32}, {1, 32, -1, -32}, {1, 32, -32, -1}, {1, -32, -1, 32}, {1, -32, 32, -1},
     {32, 1, -1, -32}, {32, 1, -32, -1}, {32, -1, 1, -32}, {32, -1, -32, 1}, {32, -32, 1, -1}, {32, -32, -1, 1},
     {-32, 1, 32, -1}, {-32, 1, -1, 32}, {-32, 32, 1, -1}, {-32, 32, -1, 1}, {-32, -1, 1, 32}, {-32, -1, 32, 1}};
  const int sidx = (1+row) * 32 + col+1;
  open.clear();
  open.push_back(sidx);
  Helper[sidx] = hh;
  Dir[sidx] = 0;
  int box_d = 0;
  while (!open.empty()) {
    int idx = open.pop_front();
    int d = Dir[idx];
    for (int i : dirs[rng.next_int(24)]) {
      int dd = d==0 ? i : d;
      const int nidx = idx + i;
      if (Helper[nidx] == hh) continue;

      if (BORDER) {
        if (gg[nidx] == -1) {
          const int ssidx = sidx + dd;
          gg[ssidx] = gg[sidx];
          gg[sidx] = kEmpty;
          grid[(ssidx)/32 - 1][(ssidx & 31) - 1] = gg[ssidx];
          grid[sidx/32 - 1][(sidx&31)-1] = kEmpty;
          return;
        }
      }

      if (gg[nidx] == kEmpty) {
        Helper[nidx] = hh;
        Dir[nidx] = dd;
        open.push_back(nidx);
      } else if (!BORDER) {
        if (gg[nidx] == kPresent) {
          const int ssidx = sidx + dd;
          gg[ssidx] |= kElf;
          gg[sidx] = kEmpty;

          grid[(ssidx)/32 - 1][(ssidx & 31) - 1] = gg[ssidx];
          grid[sidx/32 - 1][(sidx&31)-1] = kEmpty;
          return;
        }
        if (box_d == 0 && gg[nidx] == kBox) {
          box_d = dd;
          if (no_path) {
            const int ssidx = sidx + dd;
            gg[ssidx] |= kElf;
            gg[sidx] = kEmpty;

            grid[(ssidx)/32 - 1][(ssidx & 31) - 1] = gg[ssidx];
            grid[sidx/32 - 1][(sidx&31)-1] = kEmpty;
            return;
          }
        }
      }
    }
  }
  if (box_d != 0) {
    const int ssidx = sidx + box_d;
    gg[ssidx] |= kElf;
    gg[sidx] = kEmpty;

    grid[(ssidx)/32 - 1][(ssidx & 31) - 1] = gg[ssidx];
    grid[sidx/32 - 1][(sidx&31)-1] = kEmpty;
  }
}

int Simulate() {
  PROFILER("Simulate");
  // SaveState();
  int money = M;
  int sturn = turn;
  int score = numPresents * 100;
    std::vector<std::pair<int, int>> elves;
  for (int row = 0; row < 1024; row++) {
    gg[row] = -1;
  }
  for (; turn < N*N && score; turn++) {
    GetBoxesForMarked();
    elves.clear();
    bool no_path_trivial = NoTrivialPath();
    for (int row = 0; row < N; row++) {
      for (int col = 0; col < N; col++) {
        if (grid[row][col] & kElf) elves.push_back({row, col});
        gg[(row+1)*32 + col + 1] = grid[row][col];
      }
    }
    for (auto& p : elves) {
      if (grid[p.first][p.second] == kElf) {
        // Need to find a present/box
        MoveElf<false>(p.first, p.second, no_path_trivial);
      } else {
        // Need to find an edge
        bool edge = false;
        if (p.first < p.second) {
          if (p.first == 0 || p.second == N-1) edge = true;
        } else {
          if (p.second == 0 || p.first == N-1) edge = true;
        }
        if (edge) {
          if (grid[p.first][p.second] == kElfPresent) score -= 100;
          grid[p.first][p.second] = kEmpty;
          gg[(p.first+1)*32 + p.second + 1] = kEmpty;
          continue;
        }
        if (no_path_trivial && grid[p.first][p.second] == kElfPresent && !outside[p.first][p.second]) continue;
        MoveElf<true>(p.first, p.second);
      }
    }
    if (SpwanElf[turn] >= 0) {
      int p = SpwanElf[turn];
      for (int check = 0; check < 4 * N; check++) {
        int pr, pc;
        if (p < N) { pr = 0; pc = p; }
        else if (p<N*2-1) { pr = p+1-N; pc = N-1; }
        else if (p<N*3-2) { pr = N-1; pc = N-1-(p-N*2+2); }
        else { pr = N-1-(p-N*3+3); pc = 0; }
        if (grid[pr][pc] == kEmpty) {
          grid[pr][pc] = kElf;
          gg[(pr+1)*32 + pc + 1] = kElf;
          break;
        }
        p = (1+p) % (N*4-4);
      }
    }
    M++;
  }
  ResetGrid();
  turn = 0;
  int ret = score + M;
  M = money;
  turn = sturn;
  return ret;
}

int main() {
  std::cin >> N >> C >> eP;
  // let_inside = C * eP > 1.0;
  let_inside = true;
  LoadBoard(false);
  InitDistanceToEnd();

  LOG_TC("N: %d, C: %d, M: %d, P: %d (%.1lf%%), T: %d (%.1lf%%), E: %.1lf%%)", N, C, M,
         numPresents, 100.0 * numPresents / ((N-4.0) * (N-4.0)),
         numTrees, 100.0 * numTrees / (N * N), 100.0 * eP);

  bool init = false;

  RunAllBFS();

  // bool use1 = false;

  double test = 0.0;
  for (; turn < N*N; turn++) {
    PROFILER("loop");
    if (!init) {
      for (int i = 1; i+1 < N; i++) {
        if (grid[i][0] == kElf && grid[i][1] == kEmpty) init = true;
        if (grid[0][i] == kElf && grid[1][i] == kEmpty) init = true;
        if (grid[i][N-1] == kElf && grid[i][N-2] == kEmpty) init = true;
        if (grid[N-1][i] == kElf && grid[N-2][i] == kEmpty) init = true;
      }
      if (init) {
        LOG_INFO("Start initialization (%d)", elapsedTime);
        SaveGrid();
        Solve2();
        LOG_INFO("End at %lf vs %lf", get_time()-start_time, end_time-start_time);
      } else {
        PlaceBoxes({});
        LoadBoard(true);
        continue;
      }
    }
    // if (!use1) {
    //   if (!HasPresentOutside()) {
    //     use1 = true;
    //     LOG_INFO("Switch to method 1");
    //   }
    // }
    // if (use1 && elapsedTime < 9800) Solve();
    test -= get_time();
    PlaceBoxes(GetBoxesForMarked());
    test += get_time();
    // LOG_TC("Turn: %d, time: %d", turn+1, elapsedTime);
    LoadBoard(true);
  }
  LOG_TC("Time: %d (%lf + %lf)", elapsedTime, loading_time, writting_time);
  LOG_TC("test %lf", test);

  return 0;
}
