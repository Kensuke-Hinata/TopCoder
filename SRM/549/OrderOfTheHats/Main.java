import java.util.*;
import java.math.*;

public class Main {
    int recur(int mask, int[] bc, int[] bm, int[] dp) {
        var n = bm.length;
        if (mask + 1 == 1 << n) return 0;
        var res = dp[mask];
        if (res != -1) return res;
        res = 1 << 30;
        for (int i = 0; i < n; ++ i) if ((mask & (1 << i)) == 0) {
            var ret = recur(mask | (1 << i), bc, bm, dp);
            var tm = bm[i] & (mask ^ ((1 << n) - 1));
            res = Math.min(res, ret + bc[tm]);
        }
        dp[mask] = res;
        return res;
    }

    public int minChanged(String[] sc) {
        var n = sc.length;
        var bc = new int[1 << n];
        for (int i = 0; i < bc.length; ++ i) {
            for (int j = 0; j < n; ++ j) if ((i & (1 << j)) != 0) ++ bc[i];
        }
        var bm = new int[n];
        for (int i = 0; i < n; ++ i) {
            for (int j = 0; j < n; ++ j) if (sc[j].charAt(i) == 'Y') bm[i] |= (1 << j);
        }
        var dp = new int[1 << n];
        Arrays.fill(dp, -1);
        var ret = recur(0, bc, bm, dp);
        return ret;
    }
    
    public static void main(String[] args) {
        var obj = new Main();

        var sc = new String[]{"Y"};
        var ret = obj.minChanged(sc);
        System.out.println(ret);

        sc = new String[]{"NYN", "NNY", "NNN"};
        ret = obj.minChanged(sc);
        System.out.println(ret);

        sc = new String[]{"NYN", "NNY", "YNN"};
        ret = obj.minChanged(sc);
        System.out.println(ret);

        sc = new String[]{"NYYYYYY", "YNYYYYY", "YYNYYYY", "YYYNYYY", "YYYYNYY", "YYYYYNY", "YYYYYYN"};
        ret = obj.minChanged(sc);
        System.out.println(ret);

        sc = new String[]{"N"};
        ret = obj.minChanged(sc);
        System.out.println(ret);

        sc = new String[]{"YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY", "YYYYYYYYYYYYYYYYYYYY"};
        ret = obj.minChanged(sc);
        System.out.println(ret);

        sc = new String[]{"NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN", "NNNNNNNNNNNNNNNNNNNN"};
        ret = obj.minChanged(sc);
        System.out.println(ret);
    }
}
