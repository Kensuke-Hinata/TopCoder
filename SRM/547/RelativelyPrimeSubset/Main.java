import java.util.*;
import java.math.*;

public class Main {
    List<Integer> sieve(int m) {
        var res = new ArrayList<Integer>();
        var f = new boolean[m + 1];
        for (int i = 2; i <= m; ++ i) if (!f[i]) {
            res.add(i);
            for (int j = i * i; j <= m; j += i) f[j] = true;
        }
        return res;
    }

    int recur(int mask, int idx, int[] s, boolean[] f, int[] b, int[][] dp) {
        if (idx == s.length) return 0;
        var res = dp[mask][idx];
        if (res != -1) return res;
        res = recur(mask, idx + 1, s, f, b, dp);
        if (!f[s[idx]] && (mask & b[idx]) == 0) {
            var ret = recur(mask | b[idx], idx + 1, s, f, b, dp);
            res = Math.max(res, ret + 1);
        }
        dp[mask][idx] = res;
        return res;
    }

    public int findSize(int[] s) {
        final int m = 100;
        var p = sieve(m);
        var n = s.length;
        Arrays.sort(s);
        var b = new int[n];
        for (int i = 0; i < n; ++ i) for (int j = 0; j < p.size(); ++ j) {
            if (s[i] % p.get(j) == 0) b[i] |= 1 << j;
        }
        var f = new boolean[m + 1];
        var res = 0;
        for (int i = 0; i < n; ++ i) if (i == 0 || s[i] != s[i - 1]) {
            for (int j = 15; j < p.size(); ++ j) if (s[i] % p.get(j) == 0) {
                f[s[i]] = true;
                ++ res;
                break;
            }
        }
        var dp = new int[1 << 15][n + 1];
        for (int i = 0; i < dp.length; ++ i) Arrays.fill(dp[i], -1);
        var ret = recur(0, 0, s, f, b, dp);
        res += ret;
        return res;
    }

    public static void main(String[] args) {
        var main = new Main();

        var s = new int[]{2, 3, 7, 11, 4};
        var ret = main.findSize(s);
        System.out.println(ret);

        s = new int[]{4, 8, 12, 16};
        ret = main.findSize(s);
        System.out.println(ret);

        s = new int[]{100, 17, 81, 82};
        ret = main.findSize(s);
        System.out.println(ret);

        s = new int[]{2, 3, 4, 5, 6};
        ret = main.findSize(s);
        System.out.println(ret);
    }
}
