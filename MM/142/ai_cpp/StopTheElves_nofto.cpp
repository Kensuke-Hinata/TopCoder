#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <cmath>
#include <random>
#include <algorithm>
#include <queue>
#include <sys/time.h>

//#define NEXT(iterator, list) ( ++iterator; if (iterator == list.end()) iterator = list.begin(); ) 
#define NEXT_IL(iterator, list) (( next(iterator, 1) == list.end() ? list.begin() : next(iterator, 1) ))
//#define PREV(iterator, list) ( if (iterator == list.begin()) iterator = list.end(); --iterator; ) 
#define PREV_IL(iterator, list) (( iterator == list.begin() ? prev(list.end(), 1) : prev(iterator, 1) )) 
#define IS_NEIGH(x, y) ((abs((x) % N - (y) % N) <= 1 && abs((x) / N - (y) / N) <= 1 ))
#define IS_IN(x) (( (x) >= 0 && (x) <= N * N ))
#define IS_BORDER(x) (( (x) % N == 0 || (x) % N == N - 1 || (x) / N == 0 || (x) / N == N - 1 ))
#define IS_NEIGH_CLOG(x, y) clog << abs((x) % N - (y) % N)  << " " << abs((x) / N - (y) / N) << endl;

#define NEXT() (NEXT_IL(it, fence))
#define PREV() (PREV_IL(it, fence))
#define PRICE() (((3 * price1 + price2/(trial+1.0))))
//#define PRICE() (((3 * price1 + 2*price2)/20.0))
#define NEXT_ELF() (( grid[*it + 1] == 'e' || grid[*it + 1] == 'E' || grid[*it - 1] == 'e' || grid[*it - 1] == 'E' || grid[*it + N] == 'e' || grid[*it + N] == 'E' || grid[*it - N] == 'e' || grid[*it - N] == 'E' ))
#define NEXT_ELFF() (( gridd[*it + 1] == 'e' || gridd[*it + 1] == 'E' || gridd[*it - 1] == 'e' || gridd[*it - 1] == 'E' || gridd[*it + N] == 'e' || gridd[*it + N] == 'E' || gridd[*it - N] == 'e' || gridd[*it - N] == 'E' ))
#define MONEY_FOR_ALL() (( missingBoxes * C + C <= money ))
#define NOT_LATE() (( numTurns < N*N - 2*N ))

using namespace std;

int N;
int TRIALS = 10;
int SUBSET_TRIALS = 40;
int WITHOUT_CHANGE = 100;
int MAXSTEP = 20;
int SIM_COUNT = 10;
vector <int> DIRS;

double getTime() {
    timeval t;
    gettimeofday(&t,NULL);
    return 1e-6*t.tv_usec + t.tv_sec;
}

// random generator
unsigned long long nowRand = 1;
void seedBig(unsigned long long seed) {
	nowRand = seed;
}
unsigned long long randBig() {
	nowRand = ((nowRand * 6364136223846793005ULL + 1442695040888963407ULL) >> 1);
	return nowRand;
} 

int price(list<int>& fence, const vector<char>& grid) {
	int ans = 0;
	for (int p: fence) {
		if (grid[p] != 'T') ans++;
	}
	return ans;
}

void displayFence(list<int>& fence, vector<char> grid) {
	int fencePrice = price(fence, grid);
	for (auto it = fence.begin(); it != fence.end(); ++it) {
		grid[*it] = '*';
		clog << "["<< *it/N << "," << *it%N << "]";
	}
	clog << endl;
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			clog << grid[r * N + c];
		}
		clog << endl;
	}
	clog << "[" << fencePrice << " / " << fence.size() << "]" << endl;
}


int sideValue(int p0, int p1, int p) { // more negative => more inside
	int x0 = p0 % N; 
	int y0 = p0 / N; 
	int x1 = p1 % N; 
	int y1 = p1 / N;
	int x = p % N;
	int y = p / N;
	return x * (y1 - y0) - y * (x1 - x0) + y0 * x1 - x0 * y1;
}

void nextBy(list<int>::iterator& it, list<int>& fence, int count) {
	for (int i = 0; i < count; i++) it = NEXT();
}

list<int> findBestFence(list<int>& fence, const vector<char>& grid, vector<bool>& isFence) {
	list<int> startingFence = fence;
	vector<bool> startingIsFence = isFence;
	list<int> bestFence = fence;
	int bestPrice = price(fence, grid);
	for (int trial = 0; trial < TRIALS; trial++) {
		fence = startingFence;
		isFence = startingIsFence;
		double T = 1.0 * (trial*trial+1);
		int withoutChange = 0;
		auto it = fence.begin();
		nextBy(it, fence, randBig() % fence.size());
		while (withoutChange < WITHOUT_CHANGE) {
			T = T * (1 - 0.01);
			withoutChange++;
			nextBy(it, fence, randBig() % MAXSTEP);
			
			int p0 = *PREV();
			int p1;
			
			// ADD
			p1 = *it;
			//if(true) {
			if(trial % 2 == 0 && p0 / N != p1 / N && p0 % N != p1 % N) {
				int start = randBig() % DIRS.size();
				for (int i = 0; i < DIRS.size(); i++) {
					int p = p0 + DIRS[(start + i) % DIRS.size()];
					if (IS_IN(p) && !isFence[p] && IS_NEIGH(p0, p) && IS_NEIGH(p1, p) && grid[p] != 'P' && (!IS_BORDER(p) || grid[p] == 'T')) {
						int price2 = sideValue(p0, p1, p) > 0 ? 1 : -1;
						int price1 = grid[p] == 'T' ? 0 : 1;
						if (1000000 * exp(-PRICE()/T) > randBig() % 1000000) {
							it = fence.insert(it, p);
							isFence[p] = true;
							withoutChange = 0;
							break;
						}
					}
				}
				if (withoutChange == 0) continue;
			}
			
			// ERASE
			p1 = *NEXT();
			if (IS_NEIGH(p0, p1)) {
				int price2 = sideValue(p0, p1, *it) > 0 ? -1 : 1;
				int price1 = grid[*it] == 'T' ? 0 : -1;
				if (1000000 * exp(-PRICE()/T) > randBig() % 1000000) {
					isFence[*it] = false;
					it = fence.erase(it);
					if (it == fence.end()) it = fence.begin();
					withoutChange = 0;
					continue;
				}
			}
			
			
			
			// REPLACE
			p1 = *NEXT();
			int p = *it;
			int value = sideValue(p0, p1, p);
			int start2 = randBig() % DIRS.size();
			for (int i = 0; i < DIRS.size(); i++) {
				int pNew = p + DIRS[(start2 + i) % DIRS.size()];
				//if (p0 == 3 * N + 12 && p1 == 5*N + 11 && pNew == 4*N+11 && p==4*N+12) clog << (IS_IN(pNew)) << (!isFence[pNew]) << (IS_NEIGH(p0, pNew)) << (IS_NEIGH(p1, pNew)) << (grid[pNew] != 'P') << (!IS_BORDER(pNew) || grid[pNew] == 'T') << endl;
				if (IS_IN(pNew) && !isFence[pNew] && IS_NEIGH(p0, pNew) && IS_NEIGH(p1, pNew) && grid[pNew] != 'P' && (!IS_BORDER(pNew) || grid[pNew] == 'T')) {
					int price2 = sideValue(p0, p1, pNew) < value ? -1 : 1;
					int price1 = (grid[p] == 'T' ? 0 : -1) + (grid[pNew] == 'T' ? 0 : 1);
					//clog << "[" << p0/N << "," << p0%N << "]"  << "[" << p1/N << "," << p1%N << "]"  << "[" << p/N << "," << p%N << "]" << "[" << pNew/N << "," << pNew%N << "]" << price2 << " ";
			
					if (1000000 * exp(-PRICE()/T) > randBig() % 1000000) {
						isFence[p] = false;
						isFence[pNew] = true;
						*it = pNew;
						withoutChange = 0;
						break;
					}
				}
			}
			if (withoutChange == 0) continue;

		}
		int newPrice = price(fence, grid);
		if (newPrice < bestPrice) {
			bestPrice = newPrice;
			bestFence = fence;
		}
	}
	return bestFence;
}

void optimizeFence(list<int>& fence, vector<char>& grid, vector<bool>& isFence) {
	int withoutChange = 0;
	auto it = fence.begin();
	while (withoutChange < fence.size()) {
		//cerr << *it/N << " " << *it%N << endl;
		//IS_NEIGH_CLOG(*PREV(), *NEXT())
		if (IS_NEIGH(*PREV(), *NEXT())) {
			isFence[*it] = false;
			it = fence.erase(it);
			if (it == fence.end()) it = fence.begin();
			withoutChange = 0;
		} else {
			int p0 = *PREV();
			int p1 = *NEXT();
			int p = *it;
			int value = sideValue(p0, p1, p);
			bool found = false;
			for (int dir: {1, -1, N, -N, 1 + N, 1 - N, -1 + N, -1 - N}) {
				if (IS_NEIGH(p0, p + dir) && IS_NEIGH(p1, p + dir) && grid[p + dir] != 'P' && !isFence[p + dir]) {
					if (sideValue(p0, p1, p + dir) < value) {
						isFence[p] = false;
						isFence[p + dir] = true;
						*it = p + dir;
						found = true;
						break;
					}
				}
			}
			if(found) withoutChange = 0;
			else withoutChange++;
			it = NEXT();
		}
	}
}

void reduceFence(list<int>& fence, const vector<char>& grid) {
	int withoutChange = 0;
	auto it = fence.begin();
	while (withoutChange < fence.size()) {
		//IS_NEIGH_CLOG(*PREV(), *NEXT())
		if (IS_NEIGH(*PREV(), *NEXT())) {
			it = fence.erase(it);
			if (it == fence.end()) it = fence.begin();
			withoutChange = 0;
		} else {
			withoutChange++;
			it = NEXT();
		}
	}
}

int firstElf;
vector <int> spawnPoint;

int dr[4] = {0,  -1,  0,  1};
int dc[4] = {-1,  0,  1,  0};
int numTurns;
int numPresents;
int boxesPlaced;
int score;
queue<int> qr;
queue<int> qc;
queue<int> qd;
vector <char> gridd;

void moveElf(int r, int c, int d) {
	char e = gridd[r*N+c];
	gridd[r*N+c] = '.';
	r += dr[d];
	c += dc[d];
	if (r<0 || r>=N || c<0 || c>=N) {
		if (e=='E') numPresents--; // Stole the present
		return;
	}
	if (gridd[r*N+c]=='b') {// Pick up box 
		gridd[r*N+c] = 'B';
	} else if (gridd[r*N+c]=='P') { // Pick up present
		gridd[r*N+c] = 'E';
	} else {
		gridd[r*N+c] = e;
	}
}
void findMove(int y, int x) {
	qd = queue<int>();
	qr = queue<int>();
	qc = queue<int>();
	qd.push(-1);
	qr.push(y);
	qc.push(x);
	char e = gridd[y*N+x];
	vector<int> order = {0,1,2,3};
    vector<int> visited(N*N, 0);
	int dirToBox = -1; // direction to nearest box
	while (qd.size()!=0) {
		int d = qd.front(); qd.pop();
        int r = qr.front(); qr.pop();
        int c = qc.front(); qc.pop();
        if (e=='e' && gridd[r*N+c]=='P') {
			moveElf(y, x, d);
			return;
		}
        if (visited[r+c*N]!=1) {
			visited[r+c*N] = 1;
			random_shuffle(order.begin(), order.end());
			for (int dd=0;dd<4;dd++) {
				int dir = order[dd];
				int nr = r + dr[dir];
				int nc = c + dc[dir];
				if ((nc>=0 && nc<N && nr>=0 && nr<N) || e=='E' || e=='B') {
					bool canMove = false;
					if (e=='e') {
						if (gridd[nr * N + nc]=='.' || gridd[nr * N + nc]=='P') canMove = true;
					} else {
						if (nc<0 || nc>=N || nr<0 || nr>=N) {
							// found a path to the border
							moveElf(y, x, d==-1 ? dir : d);
							return;
						}
						if (gridd[nr * N+ nc]=='.') canMove = true;
					}
					if (gridd[nr * N + nc]=='b' && dirToBox==-1) {// Collect info about nearest box
						dirToBox = (d==-1 ? dir : d);
					}
					if (canMove) {
						if (d==-1) qd.push(dir);
						else qd.push(d);
						qr.push(nr);
						qc.push(nc);
					}
				}
			}
		}
	}
	// can't find a path to a present or border when carrying a present, go and pick up a box
	if (dirToBox!=-1 && e=='e') {
		moveElf(y, x, dirToBox);
	}
	// otherwise remain stationary
}

int simulateGame(list<int>& fence, const vector<char>& grid, int C, int money, int elfPosition, int nTurns) {
	
    boxesPlaced = 0;
    gridd = grid;
    numPresents = 0;
    for (char ch: grid) if(ch == 'P') numPresents++;
    
	vector<int> elves(N*N);
	for (numTurns=nTurns; numTurns <= N*N; numTurns++) {
		
		int missingBoxes = 0;
		for (auto it = fence.begin(); it != fence.end(); ++it) if (gridd[*it] == '.') missingBoxes++;
		bool moneyForAllBoxes = MONEY_FOR_ALL();
		
		
		for (auto it = fence.begin(); it != fence.end(); ++it) {
			if (gridd[*it] == '.' && money >= C  && (NEXT_ELFF() || (moneyForAllBoxes && NOT_LATE()) )) {
				gridd[*it] = 'b';
				money -= C;
				boxesPlaced++;
			};
		}
		
		// Move the elves
		int numElves = 0;
		for (int r=0; r<N; r++) {
			for (int c=0; c<N; c++) if (gridd[r * N + c]=='e' || gridd[r * N + c]=='E' || gridd[r * N + c]=='B') {
				elves[numElves++] = r+c*N;
			}
		}
		for (int i=0;i<numElves;i++) {
			findMove(elves[i]%N, elves[i]/N);
		}

		// Spawn the elves
		int p = spawnPoint[numTurns];
		if (p>=0) {
			// spawn one
			int pr,pc;
			int check = 0;
			while (true) {
				check++;
				if (check>N*4) break; // no space to spawn, then don't spawn
				if (p<N) { pr = 0; pc = p; }
				else if (p<N*2-1) { pr = p+1-N; pc = N-1; }
				else if (p<N*3-2) { pr = N-1; pc = N-1-(p-N*2+2); }
				else { pr = N-1-(p-N*3+3); pc = 0; }
				if (gridd[pr * N + pc]=='.') {
					gridd[pr * N + pc] = 'e';
					break;
				}
				p = (1+p) % (N*4-4);
			}
		} else if (p == -2) {
			gridd[firstElf] = 'e';
		}

		// Increase money
		money++;

		// Update score
		score = 100*numPresents + money;

		if (numPresents==0) break; // end simulation when all the presents were stolen.
	}
	return score;
}

int generateElves(double elfP, int seed, uniform_real_distribution<double>& distributionA, uniform_int_distribution<int>& distributionP, int elfPosition, int nTurns) {
	default_random_engine rng(seed);
	spawnPoint = vector<int>(N * N + 1, -1);
	int elfCount = 0;
	spawnPoint[nTurns - 1] = -2;
	firstElf = elfPosition;
	for (int i = nTurns; i <= N * N; i++) {
		double a = distributionA(rng);
		if (a < elfP) {
			int p = distributionP(rng);
			spawnPoint[i] = p;
			elfCount++;
		} else {
			spawnPoint[i] = -1; // nothing will spawn at turn i
		}
	}
	return elfCount;
}

list<int> generateFence(int numP, vector<char>& grid, vector<int>& presents, vector <int> &presentsRand, double start) {
	list<int> bestFence;
	if (numP > 0) {
		int bestPrice = N * N;
		for (int trial = 0; trial < SUBSET_TRIALS; trial++) {
			list<int> fence;
			vector<bool> isFence(N * N, false);
			int pm = presentsRand[trial % presents.size()];
			int xm = pm % N; int ym = pm / N;
			sort(presents.begin(), presents.end(), [&](int a, int b) -> bool { 
				int xa = a % N; int ya = a / N;
				int xb = b % N; int yb = b / N;
				return (xa - xm) * (xa - xm) + (ya - ym) * (ya - ym) < (xb - xm) * (xb - xm) + (yb - ym) * (yb - ym);
			});
			int minR = N - 2; int maxR = 1;
			int minC = N - 2; int maxC = 1;
			for (int i = 0; i < numP; i++) {
				minR = min(minR, presents[i] / N);
				maxR = max(maxR, presents[i] / N);
				minC = min(minC, presents[i] % N);
				maxC = max(maxC, presents[i] % N);
			}
			
			
			for (int r = minR; r <= maxR; r++) {
				fence.insert(fence.begin(), r * N + minC - 1);
			}
			for (int c = minC; c <= maxC; c++) {
				fence.insert(fence.begin(), (maxR + 1) * N + c);
			}
			for (int r = maxR; r >= minR; r--) {
				fence.insert(fence.begin(), r * N + maxC + 1);
			}
			for (int c = maxC; c >= minC; c--) {
				fence.insert(fence.begin(), (minR - 1) * N + c);
			}
	
			/*for (int r = 1; r < N - 1; r++) {
				fence.insert(fence.begin(), r * N + 1);
			}
			for (int c = 2; c < N - 1; c++) {
				fence.insert(fence.begin(), (N - 2) * N + c);
			}
			for (int r = N - 3; r >= 1; r--) {
				fence.insert(fence.begin(), r * N + N - 2);
			}
			for (int c = N - 3; c >= 2; c--) {
				fence.insert(fence.begin(), N + c);
			}*/
	
			for (int p: fence) isFence[p] = true;
			
			for (int i = numP; i < presents.size(); i++) {
				grid[presents[i]] = '.';
			}
			fence = findBestFence(fence, grid, isFence);
			reduceFence(fence, grid);
			int fencePrice = price(fence, grid);
			if (fencePrice < bestPrice) {
				bestPrice = fencePrice;
				bestFence.swap(fence);
			}
			for (int i = numP; i < presents.size(); i++) {
				grid[presents[i]] = 'P';
			}
			if (getTime() - start > 8.5) break;
		}
	}
	return bestFence;
}

double calculateExpectedScore(int numP, vector<char>& grid, double elfP, int C, int money, uniform_real_distribution<double>& distributionA, uniform_int_distribution<int>& distributionP, vector<int>& presents, vector<int>& presentsRand, list<int>& fence, int elfPosition, int nTurns, double start) {
	fence = generateFence(numP, grid, presents, presentsRand, start);
	
	double scoreSum = 0.0;
	int scoreMin = 10000000;
	//clog << numP << ": ";
	for(int i = 0; i < SIM_COUNT; i++) {
		if (getTime() - start > 8) return 0;
		int elfCount = generateElves(elfP, i + 1, distributionA, distributionP, elfPosition, nTurns);
		int score = simulateGame(fence, grid, C, money, elfPosition, nTurns);
		//clog << score << ", ";
		scoreSum += score;
		scoreMin = min(scoreMin, score);
		//clog << elfCount << " " << simScore << endl;
	}
	//clog << " avg " << scoreSum / SIM_COUNT << endl;
	return scoreSum / SIM_COUNT;
	//return scoreMin;
}

int main(int argc, char* argv[]) {
		
	int C, money;
	double elfP;
	cin >> N >> C >> elfP >> money;
	vector <char> grid(N * N);
	vector <int> presents;
	for (int r = 0; r < N; r++) {
	    for (int c = 0; c < N; c++) {
	    	cin >> grid[r * N + c];
	    	if (grid[r * N + c] == 'P') presents.push_back(r * N + c);
		}
	}
	
	vector <int> presentsRand = presents;
	//random_shuffle(presentsRand.begin(), presentsRand.end());
	sort(presentsRand.begin(), presentsRand.end(), [&](int a, int b) -> bool { 
				int xa = a % N; int ya = a / N;
				int xb = b % N; int yb = b / N;
				double xm = (N + 1.0) / 2.0; double ym = (N + 1.0) / 2.0;
				return (xa - xm) * (xa - xm) + (ya - ym) * (ya - ym) < (xb - xm) * (xb - xm) + (yb - ym) * (yb - ym);
			});

	int numTurns = 1;
	int elfPosition = -1;
    while (elfPosition < 0 && numTurns <= N*N) {
		cout << "-1";
		cout << endl;
		cout.flush();
		int elapsedTime;
		//read elapsed time
		cin >> elapsedTime;
		//read the money
		cin >> money;
		//read the updated grid
		for (int r = 0; r < N; r++) {
			for (int c = 0; c < N; c++) {
				cin >> grid[r * N + c];
				if (grid[r * N + c] == 'e') elfPosition = r * N + c;
			}
		}
		numTurns++;
	}
	double start = getTime();
	
	uniform_real_distribution<double> distributionA(0.0, 1.0);
	uniform_int_distribution<int> distributionP(0, N * 4 - 3);
	
	
	
	DIRS = {1, -1, N, -N, 1 + N, 1 - N, -1 + N, -1 - N};
	
	int P = presents.size();
	vector<double> expectedScore(P + 1, -1);
	vector<list<int>> fences(P+1);
	
	expectedScore[0] = calculateExpectedScore(0, grid, elfP, C, money, distributionA, distributionP, presents, presentsRand, fences[0], elfPosition, numTurns, start);
	expectedScore[P] = calculateExpectedScore(P, grid, elfP, C, money, distributionA, distributionP, presents, presentsRand, fences[P], elfPosition, numTurns, start);
	int pBest = expectedScore[0] > expectedScore[P] ? 0 : P;
	int pMin = 0;
	int pMax = P;
	int pMid = P / 2;
	if (expectedScore[pMid] == -1) expectedScore[pMid] = calculateExpectedScore(pMid, grid, elfP, C, money, distributionA, distributionP, presents, presentsRand, fences[pMid], elfPosition, numTurns, start);
	if (expectedScore[pMid] >= expectedScore[pBest]) pBest = pMid;
	
	while (pMax - pMin > 2 && getTime() - start < 7) {
		int pIn[5];
		pIn[0] = pMin;
		pIn[4] = pMax;
		pIn[2] = pMid;
		pIn[1] = (pMin + pMid) / 2;
		pIn[3] = (pMid + pMax) / 2;
		int bestI = 4;
		for (int i = 0; i < 5; i++) {
			if (expectedScore[pIn[i]] == -1) expectedScore[pIn[i]] = calculateExpectedScore(pIn[i], grid, elfP, C, money, distributionA, distributionP, presents, presentsRand, fences[pIn[i]], elfPosition, numTurns, start);
			if (expectedScore[pIn[i]] >= expectedScore[pBest]) {
				pBest = pIn[i];
				bestI = i;
			}
		}
		if (bestI <= 1) {
			pMax = pMid;
			pMid = pIn[1];
		} else if (bestI >= 3) {
			pMin = pMid;
			pMid = pIn[3];
		} else {
			pMin = pIn[1];
			pMax = pIn[3];
		}
	}
	
	//clog << pBest << " / " << P << endl;
	list<int> fence = fences[pBest];//generateFence(pBest, grid, presents);
	//displayFence(fence, grid);
	
	for (; numTurns <= N*N; numTurns++) {
		bool boxPlaced = false;
		
		int missingBoxes = 0;
		for (auto it = fence.begin(); it != fence.end(); ++it) if (grid[*it] == '.') missingBoxes++;
		bool moneyForAllBoxes = MONEY_FOR_ALL();
		
		for (auto it = fence.begin(); it != fence.end(); ++it) {
			if (grid[*it] == '.' && money >= C && (NEXT_ELF() || (moneyForAllBoxes && NOT_LATE()) )) {
				if (boxPlaced) cout << " ";
				else boxPlaced = true;
				cout << *it / N << " " << *it % N;
				money -= C;
			};
		}	
		if (!boxPlaced) cout << "-1";
		cout << endl;
		cout.flush();
		int elapsedTime;
		//read elapsed time
		cin >> elapsedTime;
		//read the money
		cin >> money;
		//read the updated grid
		bool isPresent = false;
		for (int r = 0; r < N; r++) {
			for (int c = 0; c < N; c++) {
				cin >> grid[r * N + c];
				if (grid[r * N + c] == 'P' || grid[r * N + c] == 'E') isPresent = true;				
			}
		}
		if (!isPresent) break;
	}
	return 0;
}
