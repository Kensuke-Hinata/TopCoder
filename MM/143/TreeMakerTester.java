import java.awt.*;
import java.awt.geom.*;
import java.util.*;
import java.io.*;
import javax.imageio.*;
import java.util.List;
import java.util.ArrayList;
import java.awt.image.BufferedImage;

import com.topcoder.marathon.*;

public class TreeMakerTester extends MarathonAnimatedVis {
    private static final Color[] pieceColors={
        Color.green, Color.blue, Color.red, Color.orange
    };
    String[] Colorations = {"\033[0;32m", "\033[0;34m", "\033[0;31m", "\033[38;5;209m"};
    // parameter ranges
    private static final int minN = 6, maxN = 30;        // grid size range
    private static final int minC = 1, maxC = 4;         // number of colors range
    private static final int minP = 1, maxP = 10;        // Penalty range
    private static final int maxTurns = 10000;           // Max number of turns

    // Inputs
    private int N;                // grid size
    private int C;                // number of colors
    private int P;                // Penalty

    // State Control
    private int[][] grid;         // Current tiles
    private int[][] colors;       // Current colors
    private int extra_piece;
    private int extra_piece_color;

    // Score computation
    private int num_moves;

    // Rendering constants
    private final double Epsilon = 0.05;
    private final double Width = 0.3;
    private final double Margin = (1.0 - Width) / 2.0;

    private boolean highlight_error;

    protected void generate() {
      N = randomInt(minN, maxN);
      C = randomInt(minC, maxC);
      P = randomInt(minP, maxP);

      //Special cases
      if (seed == 1) {
        N = minN;
        C = minC;
      } else if (seed == 2) {
        N = maxN;
        C = maxC;
      }

      //User defined parameters
      if (parameters.isDefined("N")) {
        N = randomInt(parameters.getIntRange("N"), minN, maxN);
      }
      if (parameters.isDefined("C")) {
        C = randomInt(parameters.getIntRange("C"), minC, maxC);
      }
      if (parameters.isDefined("P")) {
        P = randomInt(parameters.getIntRange("P"), minP, maxP);
      }

      highlight_error = parameters.isDefined("highlight");

      grid = new int[N][N];
      colors = new int[N][N];

      int[][] Tree = new int[N][N];
      int used = 0;
      // Place the seeds.
      while (used < C) {
        int row = randomInt(0, N-1);
        int col = randomInt(0, N-1);
        if (Tree[row][col] > 0) continue;
        colors[row][col] = used < C ? used : randomInt(0, C-1);
        Tree[row][col] = ++used;
      }
      extra_piece = randomInt(0, 15);
      extra_piece_color = randomInt(0, C-1);

      int[] drow = {-1, 0, 1, 0};
      int[] dcol = {0, 1, 0, -1};
      int[] addFrom = {1, 2, 4, 8};
      int[] addTo = {4, 8, 1, 2};

      while (used < N*N) {
        int row = randomInt(0, N-1);
        int col = randomInt(0, N-1);
        if (Tree[row][col] == 0) continue;
        int d = randomInt(0, 3);
        int nrow = row + drow[d];
        int ncol = col + dcol[d];
        if (!inGrid(nrow, ncol)) continue;
        if (Tree[nrow][ncol] > 0) continue;
        grid[row][col] += addFrom[d];
        grid[nrow][ncol] += addTo[d];
        colors[nrow][ncol] = colors[row][col];
        Tree[nrow][ncol] = Tree[row][col];
        used++;
      }

      if (debug) {
        System.out.println("Grid size, N = " + N);
        System.out.println("Number of colors, C = " + C);
        System.out.println("Penalty, P = " + P);
        System.out.println("");
        System.out.println("Board before shuffle:");
        PrintState();
      }

      if (!parameters.isDefined("noshuffle")) {
        for (int m = 0; m < 2*N*N; m++) {
          switch (randomInt(0, 1)) {
            case 0: {
              MoveRowRight(randomInt(0, N-1), extra_piece, extra_piece_color);
              break;
            }
            case 1: {
              MoveColDown(randomInt(0, N-1), extra_piece, extra_piece_color);
              break;
            }
          }
        }
        if (debug) {
          System.out.println("");
          System.out.println("Board after shuffle:");
          PrintState();
        }
      }
      num_moves = 0;
    }

    void PrintState() {
      char[] cc = { ' ', '╵', '╶', '└',
                    '╷', '│', '┌', '├',
                    '╴', '┘', '─', '┴',
                    '┐', '┤', '┬', '┼'};
      boolean color = !parameters.isDefined("nocolor");
      for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
          if (color) {
            System.out.print(Colorations[colors[row][col]] + cc[grid[row][col]] + "\033[0m");
          } else {
            System.out.print(cc[grid[row][col]]);
          }
        }
        System.out.println();
      }
      if (color) {
        System.out.println(Colorations[extra_piece_color] + cc[extra_piece] + "\033[0m");
      } else {
        System.out.println(cc[extra_piece]);
      }
    }

    void MoveRowLeft(int row, int val, int color) {
      extra_piece = grid[row][0];
      extra_piece_color = colors[row][0];
      for (int col = 1; col < N; col++) {
        grid[row][col-1] = grid[row][col];
        colors[row][col-1] = colors[row][col];
      }
      grid[row][N-1] = val;
      colors[row][N-1] = color;
      num_moves++;
    }

    void MoveRowRight(int row, int val, int color) {
      extra_piece = grid[row][N-1];
      extra_piece_color = colors[row][N-1];
      for (int col = N-1; col > 0; col--) {
        grid[row][col] = grid[row][col-1];
        colors[row][col] = colors[row][col-1];
      }
      grid[row][0] = val;
      colors[row][0] = color;
      num_moves++;
    }

    void MoveColUp(int col, int val, int color) {
      extra_piece = grid[0][col];
      extra_piece_color = colors[0][col];
      for (int row = 1; row < N; row++) {
        grid[row-1][col] = grid[row][col];
        colors[row-1][col] = colors[row][col];
      }
      grid[N-1][col] = val;
      colors[N-1][col] = color;
      num_moves++;
    }

    void MoveColDown(int col, int val, int color) {
      extra_piece = grid[N-1][col];
      extra_piece_color = colors[N-1][col];
      for (int row = N-1; row > 0; row--) {
        grid[row][col] = grid[row-1][col];
        colors[row][col] = colors[row-1][col];
      }
      grid[0][col] = val;
      colors[0][col] = color;
      num_moves++;
    }

    protected boolean up(int row, int col) {
      return (grid[row][col]&1) > 0;
    }
    protected boolean right(int row, int col) {
      return (grid[row][col]&2) > 0;
    }
    protected boolean down(int row, int col) {
      return (grid[row][col]&4) > 0;
    }
    protected boolean left(int row, int col) {
      return (grid[row][col]&8) > 0;
    }

    protected boolean isMaximize() {
        return false;
    }

    protected double run() throws Exception {
      init();
      if (parameters.isDefined("manual")) {
        setDefaultDelay(0);
        return 0;
      } else {
        return runAuto();
      }
    }

    protected double runAuto() throws Exception {
      double score = callSolution();
      if (score < 0) {
        if (!isReadActive()) return getErrorScore();
        return fatalError();
      }
      return score;
    }

    protected void timeout() {
      addInfo("Time", getRunTime());
      update();
    }

    private double callSolution() throws Exception {
      writeLine(N);
      writeLine(C);
      writeLine(P);
      // print grid
      for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
          writeLine("" + grid[row][col] + " " + colors[row][col]);
        }
      }
      writeLine("" + extra_piece + " " + extra_piece_color);
      flush();
      if (!isReadActive()) return -1;

      boolean animate = !parameters.isDefined("noanimate");

      try {
        // Get the solution
        startTime();
        int numMoves = Integer.parseInt(readLine());
        if (numMoves > maxTurns) {
          return fatalError("Number of moves must not exceed " + maxTurns);
        }
        String[] moves = new String[numMoves];
        for (int id = 0; id < numMoves; id++) {
          moves[id] = readLine();
        }
        stopTime();

        for (int id = 0; id < numMoves; id++) {
          String[] move = moves[id].split(" ");
          if (move.length != 2) {
            return fatalError("Cannot parse your move: " + moves[id]);
          }
          String dir = move[0];
          int index = Integer.parseInt(move[1]);
          if (index < 0 || index >= N) {
            return fatalError("Invalid index: " + moves[id]);
          }
          if (dir.charAt(0) == 'D') {
            prev_col = index;
            prev_row = -1;
            if (animate) updateDelay();
            MoveColDown(index, extra_piece, extra_piece_color);
            prev_col = index;
            prev_row = N;
            if (animate) updateDelay();
          } else if (dir.charAt(0) == 'U') {
            prev_col = index;
            prev_row = N;
            if (animate) updateDelay();
            MoveColUp(index, extra_piece, extra_piece_color);
            prev_col = index;
            prev_row = -1;
            if (animate) updateDelay();
          } else if (dir.charAt(0) == 'R') {
            prev_row = index;
            prev_col = -1;
            if (animate) updateDelay();
            MoveRowRight(index, extra_piece, extra_piece_color);
            prev_row = index;
            prev_col = N;
            if (animate) updateDelay();
          } else if (dir.charAt(0) == 'L') {
            prev_row = index;
            prev_col = N;
            if (animate) updateDelay();
            MoveRowLeft(index, extra_piece, extra_piece_color);
            prev_row = index;
            prev_col = -1;
            if (animate) updateDelay();
          } else {
            return fatalError("Invalid move: " + moves[id]);
          }
          if (animate) score();
        }

        updateDelay();
        return score();
      } catch (Exception e) {
        if (debug) System.out.println(e.toString());
        return fatalError("Cannot parse your output");
      }
    }

    private boolean inGrid(int row, int col) {
      return row >= 0 && row < N && col >= 0 && col < N;
    }

    protected double score() {
      // Compute the number of wrong edges
      int wrong_edge = 0;
      for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
          if (row > 0) {
            if (((grid[row][col]&1) > 0) != ((grid[row-1][col]&4) > 0)) {
              wrong_edge++;
            } else if (((grid[row][col]&1) > 0) && ((grid[row-1][col]&4) > 0) &&
                       colors[row][col] != colors[row-1][col]) {
              wrong_edge++;
            }
          }
          if (col > 0) {
            if (((grid[row][col-1]&2) > 0) != ((grid[row][col]&8) > 0)) {
              wrong_edge++;
            } else if (((grid[row][col-1]&2) > 0) && ((grid[row][col]&8) > 0) &&
                       colors[row][col-1] != colors[row][col]) {
              wrong_edge++;
            }
          }
        }
      }

      for (int i = 0; i < N; i++) {
        if ((grid[0][i]&1) > 0) wrong_edge++;
        if ((grid[i][N-1]&2) > 0) wrong_edge++;
        if ((grid[N-1][i]&4) > 0) wrong_edge++;
        if ((grid[i][0]&8) > 0) wrong_edge++;
      }

      // Compute the number of connected components
      int Comps[] = new int[C];
      boolean seen[][] = new boolean[N][N];
      for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
          if (seen[row][col]) continue;
          dfs(seen, row, col);
          Comps[colors[row][col]]++;
        }
      }

      int sc = num_moves;
      for (int c = 0; c < C; c++) {
        addInfo(pieceColors[c], Comps[c]);
        sc += Comps[c] * Comps[c];
      }
      sc += P * wrong_edge;

      addInfo("moves", num_moves);
      addInfo("missmatches", wrong_edge);
      addInfo("Score", sc);
      addInfo("Time", getRunTime());

      return sc;
    }

    void dfs(boolean[][] seen, int row, int col) {
      if (seen[row][col]) return;
      seen[row][col] = true;
      if (row > 0 && colors[row][col] == colors[row-1][col] && up(row, col) && down(row-1, col)) dfs(seen, row-1, col);
      if (col+1 < N && colors[row][col] == colors[row][col+1] && right(row, col) && left(row, col+1)) dfs(seen, row, col+1);
      if (row+1 < N && colors[row][col] == colors[row+1][col] && down(row, col) && up(row+1, col)) dfs(seen, row+1, col);
      if (col > 0 && colors[row][col] == colors[row][col-1] && left(row, col) && right(row, col-1)) dfs(seen, row, col-1);
    }

    protected void drawPiece(Graphics2D g, int row, int col, int val, Color color) {
      boolean wrong = false;
      if (highlight_error && inGrid(row, col)) {
        if (up(row, col) && (row == 0 || !down(row-1, col) || colors[row][col] != colors[row-1][col])) wrong = true;
        if (left(row, col) && (col == 0 || !right(row, col-1) || colors[row][col] != colors[row][col-1])) wrong = true;
        if (down(row, col) && (row == N-1 || !up(row+1, col) || colors[row][col] != colors[row+1][col])) wrong = true;
        if (right(row, col) && (col == N-1 || !left(row, col+1) || colors[row][col] != colors[row][col+1])) wrong = true;
      }
      g.setColor(wrong ? new Color(255, 196, 196) : Color.white);

      g.fillRect(col, row, 1, 1);
      g.setColor(Color.gray);
      g.drawRect(col, row, 1, 1);

      g.setColor(color);
      Ellipse2D t = new Ellipse2D.Double(col + Margin, row + Margin, Width, Width);
      g.fill(t);

      if ((val&1) > 0) {
        g.fill(new Rectangle2D.Double(col + Margin, row, Width, 0.5));
      }
      if ((val&2) > 0) {
        g.fill(new Rectangle2D.Double(col + 0.5, row + Margin, 0.5, Width));
      }
      if ((val&4) > 0) {
        g.fill(new Rectangle2D.Double(col + Margin, row + 0.5, Width, 0.5));
      }
      if ((val&8) > 0) {
        g.fill(new Rectangle2D.Double(col, row + Margin, 0.5, Width));
      }
    }

    protected void paintContent(Graphics2D g) {
      adjustFont(g, Font.SANS_SERIF, Font.PLAIN, String.valueOf("1"), new Rectangle2D.Double(0, 0, 0.5, 0.5));
      g.setStroke(new BasicStroke(0.005f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));

      // draw grid
      for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
          drawPiece(g, row, col, grid[row][col], pieceColors[colors[row][col]]);
        }
      }
      g.setColor(new Color(0, 0, 0));
      for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
          if (row > 0) {
            if ((up(row, col) != down(row-1, col)) ||
                (up(row, col) && down(row-1, col) && colors[row][col] != colors[row-1][col])) {
              g.fill(new Rectangle2D.Double(col + Margin, row - Epsilon * 0.5, Width, Epsilon));
            }
          }
          if (col > 0) {
            if ((left(row, col) != right(row, col-1)) ||
                (left(row, col) && right(row, col-1) && colors[row][col-1] != colors[row][col])) {
              g.fill(new Rectangle2D.Double(col - Epsilon * 0.5, row + Margin, Epsilon, Width));
            }
          }
        }
      }
      for (int i = 0; i < N; i++) {
        if (up(0, i)) g.fill(new Rectangle2D.Double(i + Margin, - Epsilon * 0.5, Width, Epsilon));
        if (right(i, N-1)) g.fill(new Rectangle2D.Double(N - Epsilon * 0.5, i + Margin, Epsilon, Width));
        if (down(N-1, i)) g.fill(new Rectangle2D.Double(i + Margin, N - Epsilon * 0.5, Width, Epsilon));
        if (left(i, 0)) g.fill(new Rectangle2D.Double(-Epsilon * 0.5, i + Margin, Epsilon, Width));
      }

      Color fade = new Color(pieceColors[extra_piece_color].getRed() / 2+128,
                             pieceColors[extra_piece_color].getGreen() / 2+128,
                             pieceColors[extra_piece_color].getBlue() / 2+128);
      if (prev_col == -2) drawPiece(g, -1, -1, extra_piece, fade);
      else drawPiece(g, prev_row, prev_col, extra_piece, fade);
    }

    private void init() {
      if (hasVis()) {
        setDefaultDelay(500);
        setContentRect(-1, -1, N+1, N+1);
        setInfoMaxDimension(20, 13);
        addInfo("Seed", seed);
        addInfoBreak();
        addInfo("N", N);
        addInfo("C", C);
        addInfo("P", P);
        addInfoBreak();
        addInfo("Components");
        for (int c = 0; c < C; c++) {
          addInfo(pieceColors[c], 0);
        }
        addInfoBreak();
        addInfo("moves", 0);
        addInfo("missmatches", 0);
        addInfoBreak();
        addInfo("Score", score());
        update();
      }
    }

    protected void contentClicked(double x, double y, int mouseButton, int clickCount) {
      if (!parameters.isDefined("manual")) return;

      int row = (int)Math.floor(y);
      int col = (int)Math.floor(x);
      if (mouseButton == java.awt.event.MouseEvent.BUTTON1) {
        if (row == -1 && col >= 0 && col < N) MoveColDown(col, extra_piece, extra_piece_color);
        if (row == N && col >= 0 && col < N) MoveColUp(col, extra_piece, extra_piece_color);
        if (col == -1 && row >= 0 && row < N) MoveRowRight(row, extra_piece, extra_piece_color);
        if (col == N && row >= 0 && row < N) MoveRowLeft(row, extra_piece, extra_piece_color);
        addInfo("Score", score());
        updateDelay();
      }
    }

    private int prev_row = -2;
    private int prev_col = -2;
    protected void contenMouseMoved(double x, double y) {
      if (!parameters.isDefined("manual")) return;

      int row = (int)Math.floor(y);
      int col = (int)Math.floor(x);
      if (row == prev_row && col == prev_col) return;
      boolean valid = prev_col != -2;
      prev_col = -2;
      prev_row = -2;
      if (row == -1 && col >= 0 && col < N) { prev_row = row; prev_col = col; }
      if (row == N && col >= 0 && col < N) { prev_row = row; prev_col = col; }
      if (col == -1 && row >= 0 && row < N) { prev_row = row; prev_col = col; }
      if (col == N && row >= 0 && row < N) { prev_row = row; prev_col = col; }
      if (valid || prev_col != -2) updateDelay();
    }

    public static void main(String[] args) {
        new MarathonController().run(args);
    }
}