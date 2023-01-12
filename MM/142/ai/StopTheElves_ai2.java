/*
MM131 submission from aTowerOfHanoi, finished 32th
*/

package ai;

import java.io.*;

public class StopTheElves_ai2
{
  static int[] DR = {0, -1, 0, 1};
  static int[] DC = {1, 0, -1, 0};
  static void reachable(boolean[][] visited, char[][] grid, int r, int c) {
    if (grid[r][c] == 'T' || grid[r][c] == 'P' || grid[r][c] == 'b' || visited[r][c]) {
      return;
    }
    int N = grid.length;
    visited[r][c] = true;
    for (int i = 0; i < 4; i++) {
      int nr = r + DR[i];
      int nc = c + DC[i];
      if (invalid(nr, nc, N)) {
        continue;
      }
      reachable(visited, grid, nr, nc);
    }
  }

  static boolean[][] findOuterReachable(char[][] grid) {
    int N = grid.length;
    boolean[][] res = new boolean[N][N];
    for (int i = 0; i < N; i++) {
      reachable(res, grid, 0, i);
      reachable(res, grid, N - 1, i);
      reachable(res, grid, i, 0);
      reachable(res, grid, i, N - 1);
    }
    return res;
  }

  static int countOuterReachablePresentEdges(boolean[][] reachable, char[][] grid) {
    int N = grid.length;
    int outerReachablePresentEdges = 0;
    for (int r = 0; r < N; r++) {
      for (int c = 0; c < N; c++) {
        if (grid[r][c] != 'P') {
          continue;
        }
        for (int i = 0; i < 4; i++) {
          int nr = r + DR[i];
          int nc = c + DC[i];

          if (invalid(nr, nc, N) || !reachable[nr][nc]) {
            continue;
          }
          outerReachablePresentEdges++;
        }
      }
    }
    return outerReachablePresentEdges;
  }

  static boolean invalid(int r, int c, int N) {
    return r < 0 || N <= r || c < 0 || N <= c;
  }

  static int[] findBestBoxPlace(int money, int C, char[][] grid) {
    if (money < C) {
      return null;
    }
    int N = grid.length;

    int[] res = null;
    int minReachablePresentEdges = countOuterReachablePresentEdges(findOuterReachable(grid), grid);
    for (int r = 1; r < N - 1; r++) {
      for (int c = 1; c < N - 1; c++) {
        if (grid[r][c] != '.') {
          continue;
        }
        grid[r][c] = 'b';
        boolean[][] reachable = findOuterReachable(grid);
        int reachableEdges = countOuterReachablePresentEdges(reachable, grid);
//        System.err.println(r+" "+c+" "+reachableEdges);
        if (reachableEdges < minReachablePresentEdges) {
          minReachablePresentEdges = reachableEdges;
          res = new int[]{r, c};
        }
        grid[r][c] = '.';
      }
    }
    return res;
  }

  public static void main(String[] args) throws Exception
  {   
    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));    
    
    int N = Integer.parseInt(br.readLine());
    int C = Integer.parseInt(br.readLine());
    float elfP = Float.parseFloat(br.readLine());
    int money = Integer.parseInt(br.readLine());

    char[][] grid=new char[N][N];
    for (int r=0; r<N; r++) {
      for (int c = 0; c < N; c++) {
        grid[r][c] = br.readLine().charAt(0);
      }
    }

    for (int turn=0; turn<N*N; turn++)
    {
      int[] best = findBestBoxPlace(money, C, grid);

      int or = -1, oc = -1;
      if (best != null) {
        or = best[0];
        oc = best[1];
      }
//      System.err.println(or + " " + oc);

      if (or != -1 && money>=C && grid[or][oc]=='.')
      {
        System.out.println(or + " " + oc);
      } else
      {
        System.out.println("-1");
      }
      System.out.flush();             

      //read elapsed time
      int elapsedTime = Integer.parseInt(br.readLine()); 
      //read the money
      money = Integer.parseInt(br.readLine());
      //read the updated grid
      for (int r=0; r<N; r++) {
        for (int c = 0; c < N; c++) {
          grid[r][c] = br.readLine().charAt(0);
        }
      }
    }
  }
}
