import java.util.*;

public class Main {
    static final int mod = 44444444;

    int[][] calcComb(int n) {
        int[][] res = new int[n + 1][n + 1];
        for (int i = 0; i <= n; ++ i) res[i][0] = res[i][i] = 1;
        for (int i = 1; i <= n; ++ i) {
            for (int j = 1; j < i; ++ j) {
                res[i][j] = res[i - 1][j - 1] + res[i - 1][j];
                res[i][j] %= mod;
            }
        }
        return res;
    }

    int recur(int n, int c, int f, int k, int[][][] dp, int[][] comb) {
        if (n == 0) return (f == 1 ? 1 : 0);
        if (c == 0) return 0;
        if (dp[n][c][f] != -1) return dp[n][c][f];
        int res = 0;
        if (c == 10) {
            int ret = recur(n - 1, c, f, k, dp, comb);
            res = (res + ret) % mod;
        }
        for (int i = 1; i <= Math.min(k, n); ++ i) {
            int nf = (i == k ? 1 : f);
            long ret = recur(n - i, c - 1, nf, k, dp, comb);
            ret *= (c == 10 ? 9 : c);
            //ret *= (c == 10 ? comb[n - 1][i - 1] : comb[n][i]);
            res = (res + (int)(ret % mod)) % mod;
        }
        dp[n][c][f] = res;
        return res;
    }

    public int howMany(int n, int k) {
        int[][] comb = calcComb(n);
        int[][][] dp = new int[n + 1][11][2];
        for (int i = 0; i <= n; ++ i) {
            for (int j = 0; j <= 10; ++ j) Arrays.fill(dp[i][j], -1);
        }
        return recur(n, 10, 0, k, dp, comb);
    }

    public static void main(String[] args) {
        Main obj = new Main();

        int n = 2;
        int k = 2;
        System.out.println(obj.howMany(n, k));

        n = 2;
        k = 1;
        System.out.println(obj.howMany(n, k));

        n = 3;
        k = 2;
        System.out.println(obj.howMany(n, k));

        n = 723;
        k = 38;
        System.out.println(obj.howMany(n, k));
    }
}
