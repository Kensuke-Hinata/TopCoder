import java.util.*;
import java.math.*;

class XD implements Comparable<XD> {
    int mask; 
    int bc;

    public XD(int mask) {
        this.mask = mask;
        this.bc = 0;
        for (int i = 0; (1 << i) <= mask; ++ i) {
            if ((mask & (1 << i)) != 0) ++ this.bc;
        }
    }

    public boolean include(XD xd) {
        for (int i = 0; (1 << i) <= xd.mask; ++ i) if ((xd.mask & (1 << i)) != 0) {
            if ((this.mask & (1 << i)) == 0) return false;
        }
        return true;
    }

    @Override
    public int compareTo(XD xd) {
        return xd.bc - this.bc;
    }
}

public class Main {
    int recur(int n, int cnt, int mask, Integer best, List<Map<Integer, Integer>> dp, List<XD> xds) {
        if (n < 0) return 0;
        if (n + 1 + cnt <= best) return -1;
        if (dp.get(n).containsKey(mask)) return dp.get(n).get(mask);
        int res = recur(n - 1, cnt, mask, best, dp, xds);
        if ((xds.get(n).mask & mask) == 0) {
            int ret = recur(n - 1, cnt + 1, mask | xds.get(n).mask, best, dp, xds);
            if (ret != -1) res = Math.max(res, ret + 1);
        }
        best = Math.max(best, res);
        dp.get(n).put(mask, res);
        return res;
    }

    public int findSize(int[] s) {
        int n = s.length;
        Arrays.sort(s);
        final int b = 100;
        List<Integer> p = new ArrayList<Integer>();
        boolean[] f = new boolean[b + 1];
        for (int i = 2; i <= b; ++ i) if (!f[i]) {
            boolean ok = false;
            for (int j = 0; j < n; ++ j) if (s[j] % i == 0) {
                ok = true;
                break;
            }
            if (ok) p.add(i);
            for (int j = i * i; j <= b; j += i) f[j] = true;
        }
        XD[] xds = new XD[n];
        for (int i = 0; i < n; ++ i) {
            xds[i] = new XD(0);
            for (int j = 0; j < p.size(); ++ j) if (s[i] % p.get(j) == 0) {
                xds[i].mask |= (1 << j);
            }
        }
        Arrays.sort(xds);
        List<XD> nxds = new ArrayList<XD>();
        f = new boolean[n];
        for (int i = n - 1; i >= 0; -- i) {
            if (!f[i]) {
                nxds.add(xds[i]);
                continue;
            }
            for (int j = i - 1; j >= 0; -- j) {
                if (xds[j].include(xds[i])) f[j] = true;
            }
        }
        n = nxds.size();
        List<Map<Integer, Integer>> dp = new ArrayList<Map<Integer, Integer>>();
        for (int i = 0; i < n; ++ i) dp.add(new TreeMap<Integer, Integer>());
        Integer best = 0;
        return recur(n - 1, 0, 0, best, dp, nxds);
    }

    public static void main(String[] args) {
        Main main = new Main(); 

        int[] s = new int[]{2, 3, 7, 11, 4};
        int ret = main.findSize(s);
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
