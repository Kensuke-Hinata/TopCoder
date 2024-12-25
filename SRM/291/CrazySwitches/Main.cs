using System;
using System.Diagnostics;
using System.Collections.Generic;

public class CrazySwitches
{
    int minimumActions(int[] s)
    {
        var n = s.Length;
        var dp = new int[1 << n, n];
        for (int i = 0; i < (1 << n); ++ i) for (int j = 0; j < n; ++ j) dp[i, j] = -1;
        dp[1, 0] = 0;
        var q = new Queue<(int, int)>();
        q.Enqueue((1, 0));
        if (s[0] != 0)
        {
            dp[1 | (1 << s[0]), 0] = 0;
            q.Enqueue((1 | (1 << s[0]), 0));
        }
        while (q.Count > 0)
        {
            var e = q.Dequeue();
            int mask = e.Item1, idx = e.Item2;
            if (mask == (1 << (n - 1)) && idx == n - 1) return dp[mask, idx];
            for (int i = 0; i < n; ++ i) if (i != idx && (mask & (1 << i)) != 0 && dp[mask, i] == -1)
            {
                dp[mask, i] = dp[mask, idx] + 1;
                q.Enqueue((mask, i));
                if (s[i] != i && dp[mask ^ (1 << s[i]), i] == -1)
                {
                    dp[mask ^ (1 << s[i]), i] = dp[mask, i];
                    q.Enqueue((mask ^ (1 << s[i]), i));
                }
            }
        }
        return -1;
    }
    
    static void Main()
    {
        var obj = new CrazySwitches();

        var s = new int[]{1, 0};
        var res = obj.minimumActions(s);
        Console.WriteLine(res);

        s = new int[]{0, 1};
        res = obj.minimumActions(s);
        Console.WriteLine(res);

        s = new int[]{1, 2, 0};
        res = obj.minimumActions(s);
        Console.WriteLine(res);
        
        s = new int[]{4, 4, 3, 0, 5, 2};
        res = obj.minimumActions(s);
        Console.WriteLine(res);

        s = new int[]{7, 11, 1, 12, 6, 3, 0, 2, 6, 0, 0, 5, 9};
        res = obj.minimumActions(s);
        Console.WriteLine(res);

        s = new int[]{15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
        res = obj.minimumActions(s);
        Console.WriteLine(res);

        s = new int[]{15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
        res = obj.minimumActions(s);
        Console.WriteLine(res);
    }
}
