import java.util.*;
import java.math.*;

public class Strawberry {
    final static int mod = 1000000007;

    int[][] dp;
    int[] pa, pb;

    int pow(long a, int b, int m) {
        long res = 1;
        a %= m;
        while (b > 0) {
            if ((b & 1) != 0) res = (res * a) % m;
            a = (a * a) % m;
            b >>= 1;
        }
        return (int)res;
    }

    int[] getP(int[] a) {
        int n = a.length;
        long s = 0;
        for (int i = 0; i < n; ++ i) s += a[i];
        int inv = pow(s, mod - 2, mod);
        int[] res = new int[n];
        for (int i = 0; i < n; ++ i) res[i] = (int)(((long)a[i] * inv) % mod);
        return res;
    }

    int recur(int idx, int d, int n, int k) {
        if (idx == n) return 1;
        int res = dp[idx][d];
        if (res != -1) return res;
        int m = pa.length;
        res = 0;
        if ((idx & 1) == 0) {
            for (int i = 0; i < m && d + i <= (k << 1); ++ i) {
                long ret = recur(idx + 1, d + i, n, k);
                ret = (ret * pa[i]) % mod;
                res = (res + (int)ret) % mod;
            }
        } else {
            for (int i = 0; i < m && i <= d; ++ i) {
                long ret = recur(idx + 1, d - i, n, k);
                ret = (ret * pb[i]) % mod;
                res = (res + (int)ret) % mod;
            }
        }
        dp[idx][d] = res;
        return res;
    }

    public int competitive(int n, int k, int[] a, int[] b) {
        pa = getP(a);
        pb = getP(b);
        dp = new int[n + 1][(k << 1) + 1];
        for (int i = 0; i <= n; ++ i) Arrays.fill(dp[i], -1);
        return recur(0, k, n, k);
    }

    public static void main(String[] args) {
        Strawberry obj = new Strawberry();

        int n = 1;
        int k = 3;
        int[] a = new int[]{1,1,1,1,1,1,1};
        int[] b = new int[]{1,1,1,1,1,1,1};
        System.out.println(obj.competitive(n, k, a, b));

        n = 6;
        k = 3;
        a = new int[]{4,7,0,0,0,0,0};
        b = new int[]{4,2,0,0,0,0,0};
        System.out.println(obj.competitive(n, k, a, b));

        n = 7;
        k = 3;
        a = new int[]{4,7,0,0,0,0,0};
        b = new int[]{4,2,0,0,0,0,0};
        System.out.println(obj.competitive(n, k, a, b));

        n = 50;
        k = 5;
        a = new int[]{100,101,102,103,104,105,106,107,108,109,110};
        b = new int[]{200,101,202,203,204,205,206,207,208,209,210};
        System.out.println(obj.competitive(n, k, a, b));
    }
}
