using System;

public class RandomConcat
{
    int[] getNext(String n)
    {
        int nlen = n.Length;
        int[,] lcp = new int[nlen, nlen];
        for (int i = 0; i < nlen; ++ i)
        {
            lcp[i, nlen - 1] = lcp[nlen - 1, i] = (n[nlen - 1] == n[i] ? 1 : 0);
        }
        for (int i = 0; i < nlen - 1; ++ i) for (int j = 0; j < nlen - 1; ++ j)
        {
            if (n[i] != n[j]) lcp[i, j] = 0;
            else lcp[i, j] = lcp[i + 1, j + 1] + 1;
        }
        int[] next = new int[nlen + 1];
        for (int i = 0; i < nlen; ++ i)
        {
            next[i] = 0;
            for (int j = 1; j <= i; ++ j) if (lcp[0, j] >= i - j + 1)
            {
                next[i] = i - j + 1;
                break;
            }
        }
        next[nlen] = 0;
        for (int i = 1; i < nlen; ++ i) if (lcp[0, i] >= nlen - i)
        {
            next[nlen] = nlen - i;
            break;
        }
        for (int i = 0; i <= nlen; ++ i) Console.WriteLine(next[i]);
        return next;
    }

    public double expectation(String[] ps, String n)
    {
        int plen = ps.Length, nlen = n.Length;
        int[] next = getNext(n);
        int[,] s = new int[plen, nlen];
        int[,] c = new int[plen, nlen];
        for (int i = 0; i < plen; ++ i) for (int j = 0; j < nlen; ++ j)
        {
            c[i, j] = 0;
            int pidx, nidx = j;
            for (pidx = 0; pidx < ps[i].Length; ++ pidx)
            {
                while (nidx != 0 && ps[i][pidx] != n[nidx]) nidx = next[nidx];
                if (ps[i][pidx] != n[nidx]) continue;
                ++ nidx;
                if (nidx == nlen)
                {
                    ++ c[i, j];
                    nidx = next[nidx];
                }
            }
            s[i, j] = nidx;
        }
        double[,] dp = new double[1 << plen, nlen];
        for (int i = 0; i < (1 << plen); ++ i) for (int j = 0; j < nlen; ++ j) dp[i, j] = -1;
        return Recur(0, 0, c, s, dp);
    }

    double Recur(int mask, int idx, int[,] c, int[,] s, double[,] dp)
    {
        if (mask + 1 == (1 << c.GetLength(0))) return 0;
        if (dp[mask, idx] != -1) return dp[mask, idx];
        double res = 0;
        int cnt = 0;
        for (int i = 0; i < c.GetLength(0); ++ i) if ((mask & (1 << i)) == 0)
        {
            double ret = Recur(mask | (1 << i), s[i, idx], c, s, dp);
            res += (ret + c[i, idx]);
            ++ cnt;
        }
        res /= cnt;
        dp[mask, idx] = res;
        return res;
    }

    static void Main()
    {
        RandomConcat obj = new RandomConcat();

        /*String[] ps = new String[1];*/
        //ps[0] = "hahaha";
        //String n = "aha";
        //double ret = obj.expectation(ps, n);
        //Console.WriteLine(ret);

        //ps = new String[2];
        //ps[0] = "hah"; ps[1] = "ah";
        //n = "aha";
        //ret = obj.expectation(ps, n);
        //Console.WriteLine(ret);

        //ps = new String[8];
        //ps[0] = "t"; ps[1] = "o"; ps[2] = "p"; ps[3] = "c"; ps[4] = "o"; ps[5] = "d";
        //ps[6] = "e"; ps[7] = "r";
        //n = "topcoder";
        //ret = obj.expectation(ps, n);
        //Console.WriteLine(ret);

        //ps = new String[3];
        //ps[0] = "hellotopc"; ps[1] = "oderhellotop"; ps[2] = "coderbye";
        //n = "topcoder";
        //ret = obj.expectation(ps, n);
        //Console.WriteLine(ret);

        String[] ps = new String[5];
        ps[0] = "aabaa"; ps[1] = "aabaaaaa"; ps[2] = "aba"; ps[3] = "goodluck"; ps[4] = "havefun";
        String n = "aaaa";
        double ret = obj.expectation(ps, n);
        Console.WriteLine(ret);

        //ps = new String[4];
        //ps[0] = "a"; ps[1] = "a"; ps[2] = "aa"; ps[3] = "b";
        //n = "aba";
        //ret = obj.expectation(ps, n);
        //Console.WriteLine(ret);
    }
}
