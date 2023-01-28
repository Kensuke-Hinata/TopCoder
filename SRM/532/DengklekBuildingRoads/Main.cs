using System;

public class DengklekBuildingRoads
{
    int Recur(int n, int m, int mask, int steps, int k, int[,,,] dp)
    {
        var res = dp[n, m, mask, steps];
        if (res != -1) return res;
        if (m == 0)
        {
            dp[n, m, mask, steps] = (mask == 0 ? 1 : 0);
            return dp[n, m, mask, steps];
        }
        if (n <= 1)
        {
            dp[n, m, mask, steps] = 0;
            return 0;
        }
        const int mod = 1000000007;
        res = 0;
        if (steps >= k || n - (steps + 1) <= 0)
        {
            if ((mask & 1) == 0)
            {
                var ret = Recur(n - 1, m, mask >> 1, 0, k, dp);
                res = (res + ret) % mod;
            }
        }
        else
        {
            for (int i = 0; i <= m; ++ i)
            {
                var nmask = mask;
                if ((i & 1) != 0) nmask ^= (1 | (1 << (steps + 1)));
                var ret = Recur(n, m - i, nmask, steps + 1, k, dp);
                res = (res + ret) % mod;
            }
        }
        dp[n, m, mask, steps] = res;
        return res;
    }

    int numWays(int n, int m, int k)
    {
        var dp = new int[n + 1, m + 1, 1 << (k + 1), k + 1];
        for (int i = 0; i <= n; ++ i) for (int j = 0; j <= m; ++ j)
        {
            for (int mask = 0; mask < (1 << (k + 1)); ++ mask)
            {
                for (int s = 0; s <= k; ++ s) dp[i, j, mask, s] = -1;
            }
        }
        return Recur(n, m, 0, 0, k, dp);
    }

    static void Main()
    {
        var obj = new DengklekBuildingRoads();
        
        Console.WriteLine(obj.numWays(3, 4, 1));
        Console.WriteLine(obj.numWays(4, 3, 3));
        Console.WriteLine(obj.numWays(2, 1, 1));
        Console.WriteLine(obj.numWays(5, 0, 3));
        Console.WriteLine(obj.numWays(10, 20, 5));
        Console.WriteLine(obj.numWays(30, 30, 8));
        Console.WriteLine(obj.numWays(4, 0, 7));
    }
}
