/*
MM131 submission from jediller, finished 40th
*/

package ai;

import java.io.*;
import java.util.*;

public class StopTheElves_ai4
{  
  public static boolean doomed(char[][] grid, int a, int b)
  {
     return grid[a-1][b] == 'e' || grid[a+1][b] == 'e' ||
            grid[a][b-1] == 'e' || grid[a][b+1] == 'e';
  }

  public static int wings(char[][] grid, int a, int b)
  {
     return ((grid[a-1][b-1] == 'P') ? 1 : 0) +
            ((grid[a+1][b+1] == 'P') ? 1 : 0) +
            ((grid[a-1][b-1] == 'P') ? 1 : 0) +
            ((grid[a+1][b+1] == 'P') ? 1 : 0);
  }

  public static int exposed(char[][] grid, int a, int b)
  {
     if (grid[a-1][b-1] == 'P' && grid[a-1][b+1] == 'P')
        return ((grid[a-1][b] == '.') ? 1:0) + ((grid[a][b-1] == '.') ? 1:0)+ ((grid[a][b+1] == '.') ? 1:0);
     if (grid[a-1][b-1] == 'P' && grid[a+1][b-1] == 'P')
        return ((grid[a][b-1] == '.') ? 1:0) + ((grid[a-1][b] == '.') ? 1:0)+ ((grid[a+1][b] == '.') ? 1:0);
     if (grid[a-1][b+1] == 'P' && grid[a+1][b+1] == 'P')
        return ((grid[a][b+1] == '.') ? 1:0) + ((grid[a-1][b] == '.') ? 1:0)+ ((grid[a+1][b] == '.') ? 1:0);
     if (grid[a+1][b-1] == 'P' && grid[a+1][b+1] == 'P')
        return ((grid[a+1][b] == '.') ? 1:0) + ((grid[a][b-1] == '.') ? 1:0)+ ((grid[a][b+1] == '.') ? 1:0);
     return 0; // probable coding error if this happens
  }

  public static void main(String[] args) throws Exception
  {   
    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));    
    
    int N = Integer.parseInt(br.readLine());
    int C = Integer.parseInt(br.readLine());
    float elfP = Float.parseFloat(br.readLine());
    int money = Integer.parseInt(br.readLine());

    char[][] grid=new char[N][N];
    int[][] where = new int[N*N][2];
    int p = 0;
 
    for (int y=0; y<N; y++)
    {
      for (int x=0; x<N; x++)
      {
        grid[x][y] = br.readLine().charAt(0);
        if (grid[x][y] == 'P') p++;
      }
    }
    boolean go;
      
    for (int turn=0; turn<N*N; turn++)
    {      
      int x,y,attacked,xbox,ybox;
      if (money<C)
      {
        System.out.println("-1");
      }
      else
      {
         go = true;
         for (x = 1; x < N-2 && go; x++) 
           for (y = 1; y < N-2 && go; y++)
         {
           if (grid[x][y] != 'P') continue;
           if (grid[x-1][y] == 'e' || grid[x+1][y] == 'e' || grid[x][y-1] == 'e' || grid[x][y+1] == 'e') continue;
           attacked = xbox = ybox = 0;
           if (grid[x-1][y] == '.' && x-1>0 && (grid[x-1][y-1] == 'e' || grid[x-2][y] == 'e' || grid[x-1][y+1] == 'e'))
           {
              xbox = x-1; ybox = y; attacked = 1;
           }
           if (grid[x][y+1] == '.' && y+1 < N-2 && (grid[x-1][y+1] == 'e' || grid[x][y+2] == 'e' || grid[x+1][y+1] == 'e'))
           {
              if (attacked > 0) continue;
              xbox = x; ybox = y+1; attacked = 1;
           }
           if (grid[x+1][y] == '.' && x+1 < N-2 && (grid[x+1][y-1] == 'e' || grid[x+2][y] == 'e' || grid[x+1][y+1] == 'e'))
           {
              if (attacked > 0) continue;
              xbox = x+1; ybox = y; attacked = 1;
           }
           if (grid[x][y-1] == '.' && y-1 > 0 && (grid[x-1][y-1] == 'e' || grid[x][y-2] == 'e' || grid[x+1][y-1] == 'e'))
           {
              if (attacked > 0) continue;
              xbox = x; ybox = y-1; attacked = 1;
           }
           if (attacked == 1) 
           {
              go = false;
              System.out.println(ybox + " " + xbox);
           }
         }
         if (go)
         {
            for (x = 1; x < N-2 && go; x++)   
              for (y = 1; y < N-2 && go; y++)
            {
               if (grid[x][y] != '.') continue;
               if (wings(grid,x,y) != 2) continue;
               if (doomed(grid,x,y) && exposed(grid,x,y) > 1)
               {
                  go = false;
                  System.out.println(y+" "+x);
               }
            }
         }
         if (go) System.out.println("-1");
      }
      System.out.flush();             

      //read elapsed time
      int elapsedTime = Integer.parseInt(br.readLine()); 
      //read the money
      money = Integer.parseInt(br.readLine());
      //read the updated grid
      for (y=0; y<N; y++)
      {
        for (x=0; x<N; x++)
        {
          grid[x][y] = br.readLine().charAt(0);
        }
      }
    }
  }
}
