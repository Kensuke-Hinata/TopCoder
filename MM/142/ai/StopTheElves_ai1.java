/*
MM131 submission from gardn999, finished 20th
*/

package ai;

import java.io.*;
import java.util.*;

public class StopTheElves_ai1{
  static final int IMAX = Integer.MAX_VALUE;
  static int N, N2, C, elapsedTime, money;
  static float elfP;
  static char[] grid;
  static int[] dGrid, pGrid;
  static String line;
  
  static boolean isEmptyOrElf(char c){
    return c == '.' || c == 'e' || c == 'E' || c == 'B';
  }
  
  static void addToLine(int i){
    if (line == null) line = i/N + " " + i%N; else line+= " " + i/N + " " + i%N;
    grid[i] = 'b';
    money-= C; 
  }
  
  static void doDGrid(){
    Arrays.fill(dGrid, -1);
    for (int i = 0; i < N; i++){
      if (isEmptyOrElf(grid[i])) dGrid[i] = 0;
      if (isEmptyOrElf(grid[i + N2-N])) dGrid[i + N2-N] = 0;
      if (isEmptyOrElf(grid[i*N])) dGrid[i*N] = 0;
      if (isEmptyOrElf(grid[i*N + N-1])) dGrid[i*N + N-1] = 0;
    }
    int d = 0;
    boolean found = true;
    while (found){
      found = false;
      for (int y = 1; y <= N-2; y++)
        for (int x = 1; x <= N-2; x++){
          int i = x + y*N;
          if (dGrid[i] == -1 && isEmptyOrElf(grid[i]) &&
              (dGrid[i-1] == d || dGrid[i+1] == d || dGrid[i-N] == d || dGrid[i+N] == d)){
            dGrid[i] = d+1;
            found = true;
          }
        }
      d++;
    }
  }
  
  static void addSeed(int oldSeed, int newSeed, List<Integer> seedList){
    if (pGrid[newSeed] == -1){
      pGrid[newSeed] = pGrid[oldSeed] + (isEmptyOrElf(grid[newSeed]) ? 100 : 1);
      seedList.add(newSeed);
    }
  }
  
  static void doPGrid(int start){
    for (int i = 0; i < N2; i++) 
      pGrid[i] = grid[i] == 'P' ? IMAX : -1;
    for (int i = 0; i < N; i++){
      if (grid[i] != 'T') pGrid[i] = IMAX;
      if (grid[i + N2-N] != 'T') pGrid[i + N2-N] = IMAX;
      if (grid[i*N] != 'T') pGrid[i*N] = IMAX;
      if (grid[N-1 + i*N] != 'T') pGrid[N-1 + i*N] = IMAX;
    }
    
    int nPresents = 0, xSum = 0, ySum = 0;
    for (int y = 3; y < N-3; y++)
      for (int x = 3; x < N-3; x++)
        if (grid[x + y*N] == 'P'){ nPresents++; xSum+= x; ySum+= y; }
    if (nPresents == 0) return;
    
    int xAvg = xSum/nPresents, yAvg = ySum/nPresents;
    pGrid[xAvg + yAvg*N] = IMAX;
    for (int y = 3; y < N-3; y++)
      for (int x = 3; x < N-3; x++)
        if (grid[x + y*N] == 'P'){
          int xx = x, yy = y;
          while (xx != xAvg || yy != yAvg){
            int ii = xx + yy*N;
            pGrid[ii] = IMAX;
            int dx = xAvg - xx, dy = yAvg - yy;
            boolean dxt = (dx > 0 && grid[ii+1] != 'T') || (dx < 0 && grid[ii-1] != 'T'),
                    dyt = (dy > 0 && grid[ii+N] != 'T') || (dy < 0 && grid[ii-N] != 'T');
            if ((dxt && dyt) || (!dxt && !dyt)){
              if (Math.abs(dx) > Math.abs(dy)){
                if (dx > 0) xx++; else xx--;
              }else{
                if (dy > 0) yy++; else yy--;
              }
            }else if (dxt || dy == 0){
              if (dx > 0) xx++; else xx--;
            }else{
              if (dy > 0) yy++; else yy--;
            }
          }
        }
    
    if (pGrid[start] == IMAX) return;
    
    int sx = start%N, sy = start/N;
    pGrid[sx+1] = IMAX;
    for (int y = 1; pGrid[sx+1 + N*y] != IMAX; y++) pGrid[sx+1 + N*y] = IMAX;      
    
    List<Integer> seedList = new ArrayList<>();
    seedList.add(start);
    pGrid[start] = 0;
    while (seedList.size() > 0){
      int iSeed = -1;
      for (int i = 0; i < seedList.size(); i++)
        if (iSeed == -1 || pGrid[seedList.get(i)] < pGrid[seedList.get(iSeed)])
          iSeed = i;
        
      int seed = seedList.get(iSeed);
      seedList.remove(iSeed);
      int x = seed%N, y = seed/N;
      if (x > 0) addSeed(seed, seed-1, seedList);
      if (y > 0) addSeed(seed, seed-N, seedList);
      if (x < N-1) addSeed(seed, seed+1, seedList);
      if (y < N-1) addSeed(seed, seed+N, seedList);
      if (x > 0 && y > 0) addSeed(seed, seed-1-N, seedList);
      if (x < N-1 && y > 0) addSeed(seed, seed+1-N, seedList);
      if (x > 0 && y < N-1) addSeed(seed, seed-1+N, seedList);
      if (x < N-1 && y < N-1) addSeed(seed, seed+1+N, seedList);
    }
  }
  
  static List<Integer> getBestPath(int dest){
    List<Integer> path = new ArrayList<>();
    path.add(dest);
    int i = dest;
    while (pGrid[i] > 0){
      int min = i, x = i%N, y = i/N;
      if (x > 0 && pGrid[i-1] < pGrid[min]) min = i-1;
      if (y > 0 && pGrid[i-N] < pGrid[min]) min = i-N;
      if (x < N-1 && pGrid[i+1] < pGrid[min]) min = i+1;
      if (y < N-1 && pGrid[i+N] < pGrid[min]) min = i+N;
      if (x > 0 && y > 0 && pGrid[i-1-N] < pGrid[min]) min = i-1-N;
      if (x < N-1 && y > 0 && pGrid[i+1-N] < pGrid[min]) min = i+1-N;
      if (x > 0 && y < N-1 && pGrid[i-1+N] < pGrid[min]) min = i-1+N;
      if (x < N-1 && y < N-1 && pGrid[i+1+N] < pGrid[min]) min = i+1+N;
      path.add(min);
      i = min;
    }
    return path;
  }
  
  static void doSurroundPresents(){
    doDGrid();
    for (int y = 1; y <= N-2; y++){
      if (money < C) break;
      for (int x = 1; x <= N-2; x++){
        if (money < C) break;
        int i = x + y*N;
        if (grid[i] == 'P'){
          int[] d = { -1, 1, -N, N };
          int n = 0;
          for (int di : d)
            if (isEmptyOrElf(grid[i+di]) && dGrid[i+di] >= 0) n++;
          if (money >= n*C){
            int min;
            do{
              min = -1;
              for (int di : d) 
                if (grid[i+di] == '.' && dGrid[i+di] != -1 && (min == -1 || dGrid[i+di] < min)) 
                  min = dGrid[i+di];
              for (int di : d){
                if (money < C) break;
                if (grid[i+di] == '.' && min != -1 && dGrid[i+di] == min){ 
                  addToLine(i+di); doDGrid(); 
                }
              }
            }while (min != -1 && money >= C);
          }
        }
      }
    }
  }
  
  static void doWithPath(){
    int nPresents = 0, tlPresent = -1;
    for (int i = 0; i < N2; i++) 
      if (grid[i] == 'P'){ 
        nPresents++;
        if (nPresents == 1) tlPresent = i;
      }
      
    if (money >= C && nPresents > 0){
      List<Integer> bestPath = null;
      int minCost = -1;
        
      for (int y = 1; y <= tlPresent/N; y++){
        int start = (tlPresent%N)-1 + N*y;
        if (grid[start+1-N] == 'P' || grid[start+2-N] == 'P') break;
        doPGrid(start);
        if (pGrid[start] == IMAX) break;
          
        List<Integer> path = getBestPath((y==1 && grid[start+2-N] != 'T') || 
             (grid[start+2] != 'P' && pGrid[start+2] < pGrid[start+2-N]) ? start+2 : start+2-N);
               
        if (grid[start+1] == 'T' || (y==1 && grid[start+1-N] != 'T')) path.add(start+1); 
        else path.add(start+1-N);
          
        int cost = 0;
        for (int p : path) if (isEmptyOrElf(grid[p])) cost+= C;
        if (bestPath == null || cost < minCost){ bestPath = path; minCost = cost; }
      }
      
      int minI = 0, runSize = Math.min(bestPath.size(), 10), minSize = runSize;
      for (int i = 0; i < bestPath.size(); i++){
        int n = 0;
        for (int j = i; j < i+runSize; j++){
          int p = bestPath.get(j%bestPath.size());
          if (grid[p] == '.') n++;
        }
        if (n > 0 && n < minSize){ minSize = n; minI = i; }
      }
      
      for (int i = minI; i < minI + bestPath.size(); i++){
        int p = bestPath.get(i%bestPath.size());
        if (money < C) break;
        if (grid[p] == '.') addToLine(p);
      }
    }
  }
  
  public static void main(String[] args) throws Exception{
    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
    
    N = Integer.parseInt(br.readLine());
    N2 = N*N;
    C = Integer.parseInt(br.readLine());
    elfP = Float.parseFloat(br.readLine());
    elapsedTime = 0;
    money = Integer.parseInt(br.readLine());
    
    grid = new char[N2];
    for (int i = 0; i < N2; i++) grid[i] = br.readLine().charAt(0);
    dGrid = new int[N2]; 
    pGrid = new int[N2];
     
    int nPresents = 0;
    for (char c : grid) if (c == 'P') nPresents++;
    
    double nLose = (N2-N)*(elfP - 1.0/C) - (double)money/C + 3,
           cut1 = (nPresents-nLose)/nPresents,
           cut2 = nPresents/((N2-money/C)*elfP*C);
    
    for (int turn = 0; turn < N2; turn++){
      line = null;
      
      if (cut1 > 0 && turn < N2-6){
        if (cut2 > 0.08) doWithPath(); else doSurroundPresents();
      }
      
      System.out.println(line == null ? "-1" : line);
      System.out.flush();
      
      elapsedTime = Integer.parseInt(br.readLine());
      money = Integer.parseInt(br.readLine());
      for (int i = 0; i < N2; i++) grid[i] = br.readLine().charAt(0);
    }
    
  }
}