import java.util.*;
import java.math.*;

public class MultiplesWithLimit {
    public String minMultiples(int n, int[] fd) {
        var f = new boolean[10];
        for (var v : fd) f[v] = true;
        var dp = new int[n];
        Arrays.fill(dp, -1);
        var p = new int[n][2];
        var q = new ArrayDeque<Integer>();
        for (int i = 1; i < 10; ++ i) if (!f[i]) {
            var r = i % n;
            q.add(r);
            dp[r] = 1;
            p[r][0] = -1;
            p[r][1] = i;
        }
        while (q.size() > 0) {
            var e = q.poll();
            if (e == 0) {
                var r = e;
                var d = new ArrayList<Integer>();
                while (r != -1) {
                    d.add(p[r][1]);
                    r = p[r][0];
                }
                var res = new StringBuilder();
                for (int i = d.size() - 1; i >= 0; -- i) res.append((char)('0' + d.get(i)));
                return res.toString();
            }
            for (int i = 0; i < 10; ++ i) if (!f[i]) {
                var r = (e * 10 + i) % n;
                if (dp[r] == -1) {
                    dp[r] = dp[e] + 1;
                    p[r][0] = e;
                    p[r][1] = i;
                    q.add(r);
                }
            }
        }
        return "IMPOSSIBLE";
    }

    public static void main(String[] args) {
        var obj = new MultiplesWithLimit();

        var n = 8;
        var fd = new int[]{2,3,4,5,6,7,8,9};
        var ret = obj.minMultiples(n, fd);
        System.out.println(ret);

        n = 9;
        fd = new int[]{1,3,4,5,6,7,8,9};
        ret = obj.minMultiples(n, fd);
        System.out.println(ret);

        n = 524;
        fd = new int[]{5,2,4};
        ret = obj.minMultiples(n, fd);
        System.out.println(ret);

        n = 10000;
        fd = new int[]{0};
        ret = obj.minMultiples(n, fd);
        System.out.println(ret);
    }
}
