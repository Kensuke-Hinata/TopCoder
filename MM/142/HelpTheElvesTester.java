import java.awt.*;
import java.awt.geom.*;
import java.util.*;
import java.io.*;
import javax.imageio.*;
import java.util.List;
import java.util.ArrayList;

import com.topcoder.marathon.*;
import ai.*;

public class HelpTheElvesTester extends MarathonAnimatedVis {
    //parameter ranges
    private static final int minN = 10, maxN = 30; // grid size range
    private static final int minC = 1, maxC = 10; // cost of box range
    private static final double minE = 0.1, maxE = 0.2; // elf spawning probability range
    private static final double minP = 0.1, maxP = 0.3; // present probability range
    private static final double minT = 0.1, maxT = 0.2; // tree probability range

    //Inputs
    private int N;            //grid size
    private int C;            //cost of a box
    private double elfP;      //elf spawn rate
    private double presentP;  //present probability  
    private double treeP;     //present probability
    private int[] spawnPoint; //starting locations for elves
    
    //Constants
    private static final char Tree = 'T';
    private static final char Box = 'b';
    private static final char Elf = 'e';
    private static final char ElfPresent = 'E';
    private static final char ElfBox = 'B';
    private static final char Present = 'P';
    private static final char Empty = '.';
    private static final int[] dr = {0,  -1,  0,  1};
    private static final int[] dc = {-1,  0,  1,  0};
    private static final int PresentValue = 100;
          
    //Ai comms
    private BufferedWriter aiInputWriter;
    private BufferedWriter aiErrorWriter;
    private ErrorReader aiErrorReader;
    private BufferedReader aiOutputReader;
    private Process aiProcess;
    private String aiCmd = ""; // ai command
    private int aiIndex;

    // make sure we compile all the java ai.
    private ai.StopTheElves_ai1 aiG1; // author: gardn999
    private ai.StopTheElves_ai2 aiG2; // author: ATowerOfHanoi
    private ai.StopTheElves_ai3 aiG3; // author: dimkadimon
    private ai.StopTheElves_ai4 aiG4; // author: jediller
    private ai.StopTheElves_ai5 aiG5; // MM131 sample solution

    //Derived values 
    
    //Graphics
    private Image presentPic;
    private Image treePic;
    private Image boxPic;
    private Image elfPic;
    private Image elfPresentPic;
    private Image elfBoxPic;

    //State Control
    private char[][] grid; 
    private int numTurns;
    private int presents;
    private int presentsStolen;
    private int boxesPlaced;
    private int score;
    private int money;
    private int moneySpent;
    private int[] lastBoxes;
    private int numLastBoxes;

    protected void generate()
    {
      N = randomInt(minN, maxN);
      C = randomInt(minC, maxC);
      elfP = randomDouble(minE, maxE);
      presentP = randomDouble(minP, maxP);
      treeP = randomDouble(minT, maxT);
      // Starting money
      money = C*randomInt(minC, maxC);

      //Special cases
      if (seed == 1)
      {
        N = minN;
        C = minC;
        elfP = minE;
        presentP = minP;
        treeP = minT;
      }
      else if (seed == 2)
      {
        N = maxN;
        C = maxC;
        elfP = maxE;
        presentP = maxP;
        treeP = maxT;
      }    
      
      //User defined parameters   
      if (parameters.isDefined("N"))
      {
        N = randomInt(parameters.getIntRange("N"), minN, maxN);
      }
      if (parameters.isDefined("C"))
      {
        C = randomInt(parameters.getIntRange("C"), minC, maxC);
      }   
      if (parameters.isDefined("money"))
      {
        money = randomInt(parameters.getIntRange("money"), 0, maxC*maxC);
      }    
      if (parameters.isDefined("elfP"))
      {
        elfP = randomDouble(parameters.getDoubleRange("elfP"), 0, maxE);
      }
      if (parameters.isDefined("presentP"))
      {
        presentP = randomDouble(parameters.getDoubleRange("presentP"), 0, maxP);
      }
      if (parameters.isDefined("treeP"))
      {
        treeP = randomDouble(parameters.getDoubleRange("treeP"), 0, maxT);
      }
      
      if (parameters.isDefined("ai"))
      {
        // custom ai
        aiIndex = -1;
        aiCmd = parameters.getString("ai");
      } else
      {
        List<String> aiCmds = new ArrayList<String>();
        // Add our default java ai
        // Check if we are running inside a jar file
        String sTesterPath = getClass().getProtectionDomain().getCodeSource().getLocation().getPath();
        if (sTesterPath.endsWith(".jar"))
        {
          aiCmds.add("java -cp "+sTesterPath+" ai.StopTheElves_ai1");
          aiCmds.add("java -cp "+sTesterPath+" ai.StopTheElves_ai2");
          aiCmds.add("java -cp "+sTesterPath+" ai.StopTheElves_ai3");
          aiCmds.add("java -cp "+sTesterPath+" ai.StopTheElves_ai4");
          aiCmds.add("java -cp "+sTesterPath+" ai.StopTheElves_ai5");
        } else
        {
          aiCmds.add("java ai.StopTheElves_ai1");
          aiCmds.add("java ai.StopTheElves_ai2");
          aiCmds.add("java ai.StopTheElves_ai3");
          aiCmds.add("java ai.StopTheElves_ai4");
          aiCmds.add("java ai.StopTheElves_ai5");
        }

        aiIndex = randomInt(0, aiCmds.size()-1);
        aiCmd = aiCmds.get(aiIndex);
      }

      //starting locations
      spawnPoint = new int[N*N+1];
      for (int i=1;i<=N*N;i++)
      {
        double a = randomDouble(0,1);
        if (a<elfP)
        {
          int p = randomInt(0,N*4-4);
          spawnPoint[i] = p;
        } else
        {
          spawnPoint[i] = -1; // nothing will spawn at turn i
        }
      }
   
      //storage for boxes placed, used for visualization
      numLastBoxes = 0;
      lastBoxes = new int[N*N];

      //initialize grid
      while (true)
      {
        grid = new char[N][N];
        presents = 0;
      
        for (int i=0; i<N; i++)
          for (int k=0; k<N; k++)
          {
            int distFromBorder = Math.min(Math.min(Math.min(i, k), N-k-1), N-i-1);
            double a = randomDouble(0,1);
            if (a<treeP)
            {
              grid[i][k]=Tree;
            }
            else if (a<treeP+presentP && distFromBorder>2)
            {
              grid[i][k]=Present;
              presents++;
            }
            else 
            {
              grid[i][k]=Empty;
            }            
          }
        if (presents>0) break;      
      }

      presentsStolen = 0;
      moneySpent = 0;
      boxesPlaced = 0;

      // Connect the ai
      try {
        aiProcess = Runtime.getRuntime().exec(aiCmd);
        aiErrorReader = new ErrorReader(aiProcess.getErrorStream(), false, aiErrorWriter);
        aiErrorReader.start();
        aiOutputReader = new BufferedReader(new InputStreamReader(aiProcess.getInputStream()));
        aiInputWriter = new BufferedWriter(new OutputStreamWriter(aiProcess.getOutputStream()));
      } catch (Exception e) {
          e.printStackTrace();
          System.exit(-1);
      }

      if (debug) 
      {
        System.out.println("Grid size, N = " + N);
        System.out.println("Cost of box, C = " + C);
        System.out.println("Elf spawn probability, elfP = " + elfP);
        System.out.println("Present probability, presentP = " + presentP);
        System.out.println("Tree probability, treeP = " + treeP);
        System.out.println("Ai index = " + (aiIndex+1) );
        System.out.println("Ai command = " + aiCmd);
        System.out.println();
        System.out.println("Grid:");          
        for (int r=0; r<N; r++)
        {
          for (int c=0; c<N; c++)        
            System.out.print(grid[r][c]+" ");
          System.out.println();
        }         
      }
    }
    
    protected boolean isMaximize() {
        return true;
    }
    
    protected double run() throws Exception
    {     
      init();
    
      return runAuto();
    }
    
    
    protected double runAuto() throws Exception
    {   
      double score = callSolution();

      // terminate ai
      if (aiInputWriter != null) {
        try {
          aiInputWriter.close();
        } catch (Exception e) {
        }
      }
      if (aiErrorWriter != null) {
          try {
            aiErrorWriter.close();
          } catch (Exception e) {
          }
      }
      if (aiErrorReader != null) {
          try {
            aiErrorReader.close();
          } catch (Exception e) {
          }
      }
      if (aiOutputReader != null) {
          try {
            aiOutputReader.close();
          } catch (Exception e) {
          }
      }
      if (aiProcess != null) {
          try {
            aiProcess.destroy();
          } catch (Exception e) {
          }
      }

      if (score < 0) {
        if (!isReadActive()) return getErrorScore();
        return fatalError();
      }
      return score;    
    }    

    protected void updateState()
    {
      if (hasVis())
      {      
        synchronized (updateLock) {
          addInfo("Time",  getRunTime());      
          addInfo("Presents stolen", presentsStolen);
          addInfo("Money spent", moneySpent);
          addInfo("Money", money);
          addInfo("Turns", numTurns);
          addInfo("Score", score);         
        }
        updateDelay();
      }
    } 
    
    protected void timeout() {
      addInfo("Time", getRunTime());
      update();
    }    
      
    private void moveElf(int r, int c, int nr, int nc)
    {
      char e = grid[r][c];
      grid[r][c] = Empty;
      if (nr<0 || nr>=N || nc<0 || nc>=N)
      {
        if (e==ElfPresent) 
        {
          presents--; // Stole the present
          presentsStolen++;
        }
        return;
      }
      if (grid[nr][nc]==Box) // Pick up box 
      {
        grid[nr][nc] = ElfBox;
      }
      else if (grid[nr][nc]==Present) // Pick up present
      {
        grid[nr][nc] = ElfPresent;
      } else
      {
        grid[nr][nc] = e;
      }
    }
    //shuffle the array randomly
    private void shuffle(int[] a)
    {
      for (int i=0; i<a.length; i++)
      {
        int k=randomInt(i, a.length-1);
        int temp=a[i];  
        a[i]=a[k];
        a[k]=temp;  
      }   
    } 

    protected final void aiWriteLine(int v) throws Exception 
    {
      aiWriteLine(String.valueOf(v));
    }

    protected final void aiWriteLine(String str) throws Exception 
    {
      aiInputWriter.write(str);
      aiInputWriter.newLine();
    }

    protected final void aiFlush() throws Exception 
    {
      aiInputWriter.flush();
    }

    private boolean moveTheAi(String line) throws Exception
    {
      numLastBoxes = 0;
      String[] temp = line.trim().split(" ");
      if (temp.length==1)
      {
        // do nothing
      } else if (temp.length%2==1)
      {
        System.out.println("AI: Cannot parse your output. Uneven number of elements. It should contain coordinates.");
        return false;
      } else
      {
        // Plant the boxes
        for (int i=0; i<temp.length; i+=2)
        {
          try
          {
            int row = Integer.parseInt(temp[i]);
            int col = Integer.parseInt(temp[i+1]);
            if (col<=0 || col>=N-1 || row<=0 || row>=N-1)
            {
              System.out.println("AI: You can not place on the border or outside the grid.");
              return false;
            }
            if (grid[row][col]!=Empty)
            {
              System.out.println("AI: Trying to place a box on a non empty cell.");
              return false;
            }
            if (money<C)
            {
              System.out.println("AI: Not enough money.");
              return false;
            }
            grid[row][col] = Box;
            money -= C;
            boxesPlaced++;
            moneySpent += C;
            lastBoxes[numLastBoxes++] = row+col*N;
          }
          catch (Exception e)
          {
            if (debug) System.out.println(e.toString());
            System.out.println("AI: Cannot parse your output");      
            return false;
          }
        }
      }
      return true;
    }

    private double callSolution() throws Exception
    { 
      // Call Ai --------------------------
      aiWriteLine(N);
      aiWriteLine(C);
      aiWriteLine(""+elfP);
      aiWriteLine(money);
      //print grid
      for (int r=0; r<N; r++)
        for (int c=0; c<N; c++)        
        aiWriteLine(""+grid[r][c]);
      aiFlush();
      // -----------------------------------

      // Call solution
      writeLine(N);
      writeLine(C);
      writeLine(""+elfP);
      writeLine(money);
      flush();

      if (!isReadActive()) return -1;
      
      if (hasVis() && hasDelay()) {   
          synchronized (updateLock) {        
          }
          updateDelay();
      }         
      
      int[] elves = new int[N*N];
      boolean aiHealthy = true;
      for (numTurns=1; numTurns <= N*N; numTurns++)
      {            
        // ai move
        if (aiHealthy)
        try
        {
          if (aiOutputReader!=null)
          {
            String aiLine = aiOutputReader.readLine();
            aiHealthy &= moveTheAi(aiLine);
          }
        } catch (Exception e)
        {
          // Suppress any exception caused by the ai, and just let the ai make no moves for the rest of the game
        }

        //output elapsed time, money and the grid
        writeLine(""+getRunTime());
        writeLine(""+money);
        //print grid
        for (int r=0; r<N; r++)
          for (int c=0; c<N; c++)        
            writeLine(""+grid[r][c]);
        flush(); 

        // solution move
        startTime();
        String line = readLine();
        stopTime();

        String[] temp = line.trim().split(" ");
        if (!(temp.length==1 && Integer.parseInt(temp[0])==-1))
        if (temp.length%3!=0)
        {
          return fatalError("Cannot parse your output. Should be a multiple of 3 in the format R C M R C M ...");
        } else
        {
          // Move the elves
          boolean[][] moved = new boolean[N][N];
          for (int i=0; i<temp.length; i+=3)
          {
            try
            {
              int row = Integer.parseInt(temp[i]);
              int col = Integer.parseInt(temp[i+1]);
              if (col<0 || col>=N || row<0 || row>=N) return fatalError("Coordinates outside the grid.");
              if (!(grid[row][col]==ElfPresent || grid[row][col]==ElfBox || grid[row][col]==Elf))
              {
                return fatalError("Trying to move a non elf cell. Row="+row+" Column="+col+" Cell="+grid[row][col]);
              }
              if (temp[i+2].length()!=1)
              {
                return fatalError("Invalid move, should be a single character U,D,L or R.");
              }
              if (moved[row][col])
              {
                return fatalError("Elf already moved, can't move an elf multiple times in the same turn.");
              }
              char dir = temp[i+2].charAt(0);
              int nrow = row;
              int ncol = col;
              if (dir=='U') nrow = row-1;
              else if (dir=='D') nrow = row+1;
              else if (dir=='L') ncol = col-1;
              else if (dir=='R') ncol = col+1;
              else 
              {
                return fatalError("Invalid move, should be U,D,L or R.");
              }
              moved[row][col] = true;
              if (ncol<0 || ncol>=N || nrow<0 || nrow>=N)
              {
                // moving outside
                moveElf(row, col, nrow, ncol);
              } else
              {
                if (grid[row][col]==Elf)
                {
                  // hands empty, can carry something
                  if (grid[nrow][ncol]==Empty || grid[nrow][ncol]==Present || grid[nrow][ncol]==Box)
                  {
                    moveElf(row, col, nrow, ncol);
                  } else 
                  {
                    return fatalError("Invalid move, elf walking into an object.");
                  }
                } else
                {
                  // already carrying something
                  if (grid[nrow][ncol]==Empty)
                  {
                    moveElf(row, col, nrow, ncol);
                  } else
                  {
                    return fatalError("Invalid move, elf walking into an object while carrying something.");
                  }
                }
              }
               
            }
            catch (Exception e)
            {
              if (debug) System.out.println(e.toString());
              return fatalError("Cannot parse your output");      
            }
          }
        }
        // Spawn new elves
        int p = spawnPoint[numTurns];
        if (p>=0)
        {
          // spawn one
          int pr,pc;
          int check = 0;
          while (true)
          {
            check++;
            if (check>N*4) break; // no space to spawn, then don't spawn
            if (p<N) { pr = 0; pc = p; }
            else if (p<N*2-1) { pr = p+1-N; pc = N-1; }
            else if (p<N*3-2) { pr = N-1; pc = N-1-(p-N*2+2); }
            else { pr = N-1-(p-N*3+3); pc = 0; }
            if (grid[pr][pc]==Empty)
            {
              grid[pr][pc] = Elf;
              break;
            }
            p = (1+p) % (N*4-4);
          }
        }

        // Increase money
        money++;

        // Update score
        score = PresentValue*presentsStolen + moneySpent;

        updateState();    //state 

        if (presents==0) break; // end simulation when all the presents were stolen.

        //output elapsed time, money and the grid for the ai to make a move
        aiWriteLine("5000");
        aiWriteLine(""+money);
        //print grid
        for (int r=0; r<N; r++)
          for (int c=0; c<N; c++)        
            aiWriteLine(""+grid[r][c]);
        aiFlush(); 
      }
           
      return score;
    }

     
    protected void paintContent(Graphics2D g)
    {    
      g.setStroke(new BasicStroke(0.005f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));                          

      //draw grid      
      for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
        {
          g.setColor(Color.white);
          g.fillRect(c, r, 1, 1);
          g.setColor(Color.gray);       
          g.drawRect(c, r, 1, 1);      
        }  
      //draw objects
      for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
        {            
          if (parameters.isDefined("noImages"))
          {
            if (grid[r][c]!=Empty)
            {
              if (grid[r][c]==Elf || grid[r][c]==ElfPresent || grid[r][c]==ElfBox)
              {
                if (grid[r][c]==Elf) g.setColor(Color.blue);
                else if (grid[r][c]==ElfBox) g.setColor(new Color(153,102,0));
                else g.setColor(Color.red);
                Ellipse2D t = new Ellipse2D.Double(c + 0.15, r + 0.15, 0.7, 0.7);
                g.fill(t);
                continue;
              }

              if (grid[r][c]==Tree) g.setColor(Color.green);
              else if (grid[r][c]==Present) g.setColor(Color.red);
              else if (grid[r][c]==Box) g.setColor(new Color(153,102,0));
              g.fillRect(c, r, 1, 1);
            }
          }
          else
          {
            if (grid[r][c]==Tree) g.drawImage(treePic,c,r,1,1,null);
            else if (grid[r][c]==Box) g.drawImage(boxPic,c,r,1,1,null);
            else if (grid[r][c]==Present) g.drawImage(presentPic,c,r,1,1,null);
            else if (grid[r][c]==Elf) g.drawImage(elfPic,c,r,1,1,null);
            else if (grid[r][c]==ElfPresent) g.drawImage(elfPresentPic,c,r,1,1,null);
            else if (grid[r][c]==ElfBox) g.drawImage(elfBoxPic,c,r,1,1,null);
          }
        }
      //highlight the last placed boxes
      g.setColor(Color.red);     
      g.setStroke(new BasicStroke(0.05f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));  
      for (int i=0;i<numLastBoxes;i++)
      {
        g.drawRect(lastBoxes[i]/N, lastBoxes[i]%N, 1, 1);  
      }
        
    }


    private void init()
    {
      numTurns = 0;
      score = PresentValue*presentsStolen + moneySpent;
      if (hasVis())
      {
        setDefaultDelay(100);    //this needs to be first

        if (!parameters.isDefined("noImages"))
        {
          presentPic = loadImage("images/present.png");  
          treePic = loadImage("images/tree.png");  
          boxPic = loadImage("images/box.png");  
          elfPic = loadImage("images/elf.png");  
          elfPresentPic = loadImage("images/elfPresent.png");  
          elfBoxPic = loadImage("images/elfBox.png"); 
        }        
        
        setContentRect(0, 0, N, N);
        setInfoMaxDimension(17, 13);

        addInfo("Seed", seed);
        addInfo("N", N);
        addInfo("C", C);
        addInfo("elfP", String.format("%.5f",elfP));
        addInfo("presentP", String.format("%.5f",presentP));
        addInfo("treeP", String.format("%.5f",treeP));
        if (aiIndex<0)
          addInfo("ai", aiCmd );
        else
          addInfo("ai", (aiIndex+1) );
       
        addInfoBreak();
        addInfo("Presents stolen", presentsStolen);
        addInfo("Money spent", moneySpent);
        addInfo("Money", money);
        addInfo("Turns", numTurns);
        addInfo("Score", score);
        
        addInfoBreak();
        addInfo("Time", "-");
        update();
      }
    }

    Image loadImage(String name) {
      try{
        Image im=ImageIO.read(getClass().getResourceAsStream(name));
        return im;
      } catch (Exception e) { 
        return null;  
      }             
    }     

    public static void main(String[] args) {
        new MarathonController().run(args);
    }
}