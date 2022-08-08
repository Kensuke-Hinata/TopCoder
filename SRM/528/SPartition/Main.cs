using System;
using System.Collections.Generic;

class SPartition
{
    long Calc(char c)
    {
        return (c == 'o' ? 1L : 2L);
    }

    void IncV(Dictionary<long, int> d, long k)
    {
        if (!d.ContainsKey(k)) d.Add(k, 0);
        ++ d[k];
    }

    long GetV(Dictionary<long, int> d, long k)
    {
        if (!d.ContainsKey(k)) return 0;
        return Convert.ToInt64(d[k]);
    }

    Dictionary<long, int>[] Count(String s, int idx)
    {
        var n = s.Length;
        var fp = new int[n];
        var sp = new int[n];
        var res = new Dictionary<long, int>[3];
        for (int i = 0; i < 3; ++ i) res[i] = new Dictionary<long, int>();
        for (int i = 0; i < (1 << n); ++ i)
        {
            int fn = 0, sn = 0;
            for (int j = 0; j < n; ++ j)
            {
                if ((i & (1 << j)) != 0) fp[fn ++] = j;
                else sp[sn ++] = j;
            }
            var ok = true;
            if (idx == 0)
            {
                for (int j = 0; j < Math.Min(fn, sn); ++ j) if (s[fp[j]] != s[sp[j]])
                {
                    ok = false;
                    break;
                }
                if (!ok) continue;
                if (fn == sn) IncV(res[2], 0);
                long mask = (1L << (n << 1)) - 1;
                if (fn > sn)
                {
                    for (int j = sn; j < fn; ++ j) mask ^= Calc(s[fp[j]]) << ((j - sn) << 1);
                    IncV(res[0], mask);
                }
                else if (fn < sn)
                {
                    for (int j = fn; j < sn; ++ j) mask ^= Calc(s[sp[j]]) << ((j - fn) << 1);
                    IncV(res[1], mask);
                }
            }
            else
            {
                for (int j = 0; j < Math.Min(fn, sn); ++ j) if (s[fp[fn - 1 - j]] != s[sp[sn - 1 - j]])
                {
                    ok = false;
                    break;
                }
                if (!ok) continue;
                if (fn == sn) IncV(res[2], 0);
                long mask = (1L << (n << 1)) - 1;
                if (fn > sn)
                {
                    for (int j = 0; j < fn - sn; ++ j) mask ^= Calc(s[fp[j]]) << (j << 1);
                    IncV(res[0], mask);
                }
                else if (fn < sn)
                {
                    for (int j = 0; j < sn - fn; ++ j) mask ^= Calc(s[sp[j]]) << (j << 1);
                    IncV(res[1], mask);
                }
            }
        }
        return res;
    }

    public long getCount(String s)
    {
        var fn = s.Length >> 1;
        var fc = Count(s.Substring(0, fn), 0);
        var sc = Count(s.Substring(fn, s.Length - fn), 1);
        long res = GetV(fc[2], 0) * GetV(sc[2], 0);
        foreach (KeyValuePair<long, int> kvp in fc[0]) res += kvp.Value * GetV(sc[1], kvp.Key);
        foreach (KeyValuePair<long, int> kvp in fc[1]) res += kvp.Value * GetV(sc[0], kvp.Key);
        return res;
    }

    static void Main(String[] args)
    {
        var obj = new SPartition();
        Console.WriteLine(obj.getCount("oxox"));
        Console.WriteLine(obj.getCount("oooxxx"));
        Console.WriteLine(obj.getCount("xoxxox"));
        Console.WriteLine(obj.getCount("xo"));
        Console.WriteLine(obj.getCount("ooooxoox"));
        Console.WriteLine(obj.getCount("ooxxoxox"));
        Console.WriteLine(obj.getCount("oooooooooooooooooooooooooooooooooooooooo"));
        Console.WriteLine(obj.getCount("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"));
        Console.WriteLine(obj.getCount("ooxxooxxooxxooxxooxxooxxooxxooxxooxxoo"));
    }
}
