/*
MM131 tester submission from dimkadimon
*/

package ai;

import java.io.*;
import java.util.*;

public class StopTheElves_ai3
{  
  static int N;
  static int C;
  static int Money;
  static double elfP;
  static char[][] Grid;
  static int[][] Dist;
  static int Turn;
  static long elapsedTime;
  
  static int MinR;
  static int MinC;
  static int MaxR;
  static int MaxC;
  
  //Constants
  static final char Tree = 'T';
  static final char Box = 'b';
  static final char Elf = 'e';
  static final char ElfPresent = 'E';
  static final char ElfBox = 'B';
  static final char Present = 'P';
  static final char Empty = '.';
  static final int[] dr = {0,  -1,  0,  1};
  static final int[] dc = {-1,  0,  1,  0};
  static final int PresentValue = 100;  
  static final int MaxDist = 1000;

  static Random random = new Random(76152376);
  
  public static void main(String[] args) throws Exception
  { 
    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));        
        
    N = Integer.parseInt(br.readLine());
    C = Integer.parseInt(br.readLine());
    elfP = Double.parseDouble(br.readLine());
    Money = Integer.parseInt(br.readLine());

    
    Grid=new char[N][N];
    for (int r=0; r<N; r++)
      for (int c=0; c<N; c++)
        Grid[r][c] = br.readLine().charAt(0);
        
    init();
       
        
    for (Turn=1; Turn<=N*N; Turn++)
    {      
      String move=placeSmart();
      
      System.out.println(move);
      System.out.flush();     
      
      elapsedTime = Long.parseLong(br.readLine());
      Money = Integer.parseInt(br.readLine());
      
      //read the updated grid
      for (int r=0; r<N; r++)
        for (int c=0; c<N; c++)
          Grid[r][c] = br.readLine().charAt(0);      

      // System.err.println("after turn "+Turn);
      // for (int r=0; r<N; r++)
      // {
      //   for (int c=0; c<N; c++) System.err.print(Grid[r][c]);
      //   System.err.println();
      // }
    }
  }
  
  public static void init()
  {
    MinR=100;
    MinC=100;
    MaxR=-1;
    MaxC=-1;
    
    for (int r=0; r<N; r++)
      for (int c=0; c<N; c++)
        if (Grid[r][c]==Present)
        {
          MinR=Math.min(MinR,r);
          MinC=Math.min(MinC,c);
          MaxR=Math.max(MaxR,r);
          MaxC=Math.max(MaxC,c);
        }

    Dist=new int[N][N];
  }
  
  
  public static String placeGreedy()
  {
    //if (Turn<=N*N/4) return "-1";     //don't build walls initially
    if (Money<C) return "-1";         //not enough money to build
    
    int minOpen=100;
    int bestR=-1;
    int bestC=-1;
    
    for (int r=0; r<N; r++)
      for (int c=0; c<N; c++)
        if (Grid[r][c]==Present)
        {
          List<Integer> open=new ArrayList<Integer>();
          for (int m=0; m<dr.length; m++)
          {
            int r2=r+dr[m];
            int c2=c+dc[m];
            if (inGrid(r2,c2) && Grid[r2][c2]==Empty) open.add(r2*N+c2);
          }
          
          if (open.size()>0 && open.size()<minOpen)
          {
            minOpen=open.size();
            int id=(int)(random.nextDouble()*open.size());
            int move=open.get(id);
            bestR=move/N;
            bestC=move%N;
          }
        }
        
    if (bestR!=-1) return (bestR+" "+bestC);
    
    return "-1";
  }


  public static String placeSmart()
  {
    if (Money<C) return "-1";         //not enough money to build   
    
    int bestScore=score();
    int initial=bestScore;
    int bestR=-1;
    int bestC=-1;

    int[][] startDist=new int[N][N];
    for (int r=0; r<N; r++)
      for (int c=0; c<N; c++)
        startDist[r][c]=Dist[r][c];


    boolean allSaved=true;
    for (int r=0; r<N; r++)
      for (int c=0; c<N; c++)
        if (Grid[r][c]==Present && Dist[r][c]!=MaxDist)
        {
          allSaved=false;
          break;
        }    

    if (allSaved) return "-1";      //all saved so nothing to do, nice optimization

    
    //try all single box placements and pick the one that maximizes the score
    for (int r=0; r<N; r++)
      for (int c=0; c<N; c++)
      {
        if (r==0 || r==N-1 || c==0 || c==N-1) continue;

        if (Grid[r][c]==Empty && startDist[r][c]<MaxDist)
        {
          Grid[r][c]=Box;
          int score=score();
          Grid[r][c]=Empty;

          if (score>bestScore)
          {
            bestScore=score;
            bestR=r;
            bestC=c;
          }
        }
      }
        
    if (bestR!=-1)
    {
      System.err.println("before "+initial+" after "+bestScore+" loc "+bestR+" "+bestC);
      return (bestR+" "+bestC);
    }

    return placeGreedy();     //default to greedy
  }


  //min turns to get all boxes
  public static int score()
  {
    findDist();

    int score=0;

    for (int r=0; r<N; r++)
      for (int c=0; c<N; c++)
        if (Grid[r][c]==Present)
          score+=Dist[r][c];

    return score;
  }


  public static void findDist()
  {
    List<Loc> Q=new ArrayList<Loc>();

    for (int r=0; r<N; r++)
      for (int c=0; c<N; c++)
      {
        Dist[r][c]=MaxDist;
        if (Grid[r][c]==Empty && (r==0 || r==N-1 || c==0 || c==N-1))
          Q.add(new Loc(r,c,0));
      }

    while(Q.size()>0)
    {
      Loc L=Q.remove(0);      //BFS
      int r=L.r;
      int c=L.c;

      if (!inGrid(r,c)) continue;   //out of bounds
      if (Grid[r][c]==Tree || Grid[r][c]==Box) continue;    //blocked
      if (L.dist>=Dist[r][c]) continue;     //no improvement in dist

      Dist[r][c]=L.dist;

      if (Grid[r][c]==Present) continue;    //don't propagate through presents      

      for (int m=0; m<dr.length; m++)
        Q.add(new Loc(r+dr[m],c+dc[m],L.dist+1));
    }

    // for (int r=0; r<N; r++)
    // {
    //   for (int c=0; c<N; c++) System.err.print(Dist[r][c]+" ");
    //   System.err.println();
    // }
  }
  
  public static boolean inGrid(int r, int c)
  {
    return r>=0 && r<N && c>=0 && c<N;
  }


  static class Loc
  {
    int r;
    int c;
    int dist;

    public Loc(int r2, int c2, int d)
    {
      r=r2;
      c=c2;
      dist=d;
    }
  }
}