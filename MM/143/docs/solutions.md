#### Psyho
```
My solution uses exactly N*N moves and pushes all of the tiles from the same side. Every column (or row if pushes are sideways) will receive exactly N tiles.

Whole solution is constructed via a single run of beam search that takes all of the available time. For the biggest test cases (N=30), my beam width is around 2500-3000. At every state I evaluate all of the possible N moves (unless column is completely full). Score evals are nearly constant time (with a very large constant) and I believe most of the execution time goes to state copying. I'm using union-find for calculating components.

Complexity is hidden within the beam search evaluation function. I'm using scoring function from the problem statement only at the very end, when selecting the final solution.

Eval Function
My eval function is a linear combination of all parts mentioned in the first list.

Component = set of connected tiles of a single color

Things that worked:

Number of cells that are impossible to satisfy * 35; empty cells that have either two incoming edges from the same component or two incoming edges from different colors
Broken connections * (20 + P); In order to minimize the number of components you have to minimize the number of broken connections, but broken connections have more immediate feedback
Number of components with zero edges open * 17; Those are "dead" components that we can't extend anymore
Number of components with a single edge open * 3;
Number of components * [0, 4, 8, 13] (based on C); Component weight is mostly to discourage placing tiles far apart
Relative height difference: 1.2 * MSE (Mean Squared Difference); "Spiky" shape limits our options as cells in "concave" spots are going to be highly restricted; Note that in my final submission it's a little bit more complex, but 1.2 * MSE is almost as good as the final one
Number of times when a tile is adjacent to an empty tile or tile of another color * 3; My intuition is that this helps avoiding the problem of creating "spiky" shapes, while also trying to keep colors close to each other
Things that didn't work:

Ten different ways of trying to add penalty for spreading out tiles of different colors. Things like: pre-assigning columns for specific colors and giving penalties for placing tiles outside of those, counting inversions in each row, distance to the closest tile of the same color, forcing tiles into columns for the first X rows, etc. All of those changes drastically reduced my scores.
Bonus/Penalty for clearing out complete columns. My intuition was that clearing out complete column is bad because it limits our options. At the same clearing out complete column is good because it's hard to do that properly without incurring heavy penalty somewhere. Alternative that could've worked: giving a bonus for clearing out "outer" columns.
Adding a temporary bonus based on the number of possible placements for the next tile.
Customizing weights based on C/P/N. I found it surprising that my "locally optimal" set of weights was essentially independant of C/P/N parameters (with some very rare exceptions).
Promising things that I didn't try:

Scoring how well the current board can accomodate all of the potential tiles and penalize situations when a lot of tiles cannot be put safely without immediately creating a bad situation.
Scoring "effectiveness" of each component. While my eval tries to avoid creating situations where the cycle is forced, it has no way of penalizing cycles. Each cycle indirectly increases the number of components by 1.
Insert Facepalm Emoji
For nearly the whole duration of the contest, I have assumed that there's a hard limit on the number of moves and it's N*N instead of 10K. I was very confused about the results, because I thought that with such limit, approach that I was using is probably the only viable one. And yet, for some reason it was clear (after making a submit for only C=1) that my solution has most of the bests of C=1, while it struggles with everything else. I just assumed that I'm missing some simple formula for C>1 tests that everyone else had.

I had "discovered" my mistake few hours before the submission deadline and in that short timespan I came up with 3 ideas that would utilize additional moves:

Perform some additional setup before the main beam search starts. I'm not sure what's the best strategy here, but the only one that worked for me was when I tried minimizing the number of outside edges. It gave a small ~.5% improvement. Unfortunately I wasn't convinced about adding 200-300 lines of code 20 minutes before the deadline, for a mere half a point gain.

Perform post-optimization after the main beam search is done. The last row in my solution is usually very bad / nearly random. If you perform UP c1 UP c2 DOWN c1 DOWN c2, you're essentially rotating the set of 3 tiles: [tile in hand, N-1:c1, N-1:c2]. This looked promising but it had 2 problems: Calculating components from scratch is very slow and while my solution could be improved by a lot, I'd probably need a proper beam search to find those improvements. Simple greedy was probably an .5% improvement again (but again, wasn't included in the final submit). With full beam search (or very clever heuristics) my guess is that it could be a further 2-3% gain.

During beam search, add an option to swap the current tile by performing perpendicular push that doesn't affect any formed rows. While this doesn't help with the poor "end game" of my solution, this should solve nearly all of the problems while the last row is still available. Sadly, I haven't tried this one, as it felt the most complex to implement. That being said, I wouldn't be surprised if that's a 5-15% improvement if properly implemented.

Ablation Study
Results based on the seeds 2001-3000. Output from psytester. For this ablation study, I have used beam search with fixed width in order to get comparable results among different runs.

The impact on C=1 is generally very small, because my score was already very close to N*N on many of the tests.

Tests                 1000      275      237      243      245      195      171      223      194      217      497      503
Run                Overall      C=1      C=2      C=3      C=4   N=6-10  N=11-15  N=16-20  N=21-25  N=26-30    P=1-5   P=6-10  Bests  Uniques    Gain
-----------------  -------  -------  -------  -------  -------  -------  -------  -------  -------  -------  -------  -------  -----  -------  ------
final              93.6761  97.0913  92.8212  92.3060  92.0288  97.6062  94.0410  93.3293  92.0858  91.6351  94.0328  93.3237    256       75  0.2339
tlx0.1             82.4624  92.5623  80.0655  77.9337  77.9363  91.8811  85.1869  81.4593  78.0274  76.8476  83.2144  81.7195     45        1  0.0016
tlx0.5             90.9802  96.0663  89.5963  89.1053  88.4695  96.0267  91.8297  90.7453  88.9259  87.8536  91.5111  90.4556    147       29  0.0936
tlx1.2             94.7232  97.3884  93.9998  93.3462  93.7973  97.8423  95.3251  94.0402  93.9212  92.8651  95.1149  94.3363    286      106  0.3647
tlx2.0             96.5419  97.6742  96.1257  95.7783  96.4311  98.6600  96.5323  96.3953  96.2189  95.0857  96.7582  96.3282    448      231  1.0717
no_rotations       91.7919  95.2874  91.6842  89.3951  90.3497  91.7062  91.9655  92.8617  90.4755  91.8095  92.3220  91.2680    220      147  0.5937
no_hashing         88.6922  93.7506  86.1037  86.3542  87.8373  95.7272  90.0515  87.9828  85.6664  84.7334  89.1001  88.2891    123       20  0.0480
eval_og            47.4467  47.8335  39.2915  48.9033  53.4568  88.0282  64.0578  42.6530  28.7192  19.5584  45.6781  49.1942     28        0  0.0000
eval_no_relh       74.2523  95.4830  68.7305  64.9614  64.9785  94.5778  82.7033  72.7578  65.1190  59.0289  74.6082  73.9006    153       30  0.0764
eval_no_bad        78.1544  91.3060  71.9235  72.3227  75.2039  96.0966  88.1871  80.0144  69.0815  60.3250  77.7735  78.5307     92       14  0.0488
eval_no_01edge     86.5862  87.2276  83.6503  85.8217  89.4647  94.9255  89.6712  86.1723  82.9962  80.2962  87.2835  85.8973     90       24  0.0620
eval_no_adjacency  90.5303  94.7830  88.2132  89.3401  89.1787  97.5549  93.6518  89.4356  87.7074  85.4066  91.4758  89.5960    176       50  0.1677
final: final submit
tl.1x: 10% of final's time limit / beam width
tl.5x: 50% of final's time limit / beam width
tl1.2x: 120% of final's time limit / beam width; this is a maximum realistic improvement from code optimization alone
tl2x: 200% of final's time limit / beam width
no_rotations: beam search considers only a single direction of pushes instead of all 4
no_hashing: no hashing for removal duplicate states
eval_og: eval: default scoring function from problem statement
eval_no_relh: eval: no relative height
eval_no_bad: eval: no penalty for empty cells where it's impossible to place tiles without breaking connections
eval_no_01edge: eval: no penalty based on compenents' open edges
eval_no_adjacency: eval: removed penalty based on adjacency
Example Results
{"id": 1, "score": 48.0, "time": 7286, "N": 6, "C": 1, "P": 1}
{"id": 2, "score": 2052.0, "time": 9737, "N": 30, "C": 4, "P": 2}
{"id": 3, "score": 727.0, "time": 9770, "N": 24, "C": 2, "P": 3}
{"id": 4, "score": 252.0, "time": 9684, "N": 11, "C": 2, "P": 10}
{"id": 5, "score": 57.0, "time": 9339, "N": 7, "C": 1, "P": 2}
{"id": 6, "score": 148.0, "time": 9725, "N": 12, "C": 1, "P": 3}
{"id": 7, "score": 498.0, "time": 9735, "N": 15, "C": 4, "P": 2}
{"id": 8, "score": 1228.0, "time": 9755, "N": 26, "C": 3, "P": 8}
{"id": 9, "score": 794.0, "time": 9740, "N": 28, "C": 1, "P": 2}
{"id": 10, "score": 682.0, "time": 9756, "N": 26, "C": 1, "P": 1}

https://github.com/FakePsyho/cpcontests/blob/master/topcoder/mm143/TreeMaker.cpp
```

#### msmits
```
My approach was similar to that of many others in that I split the problem in two. I first made a layout, then solved the layout with moves.

The layout

I used simulated annealing to come up with a good layout. It completely solved smaller N and did pretty well for large N (maybe 50-100 score penalty for large N). All I did was swap two random tiles. With the following rules:

Don't swap if the tiles are identical
Don't swap if an edge tile has an edge leading off the board
Don't swap if both tiles reduce their number of connections (Thanks Siman. Got this from your hatenablog, atcoder PM)
I used the TC scoring formula as eval, nothing else. Linear temperature reduction starting at T = 3. At first this was slow, because I was recalculating the entire formula for every swap, but later on I changed this to an update. It is a bit complicated to deal with the partial floodfills (for connected components), but I managed it and got a massive improvement.

The solver

Even though I knew this was a good idea, I had no idea how to solve. Simans (and Eulers) approach with vertical and horizontal alignment did not occur to me, mostly from lack of knowledge of these types of approaches. I did think about beamsearch the way yowa did, but I was afraid it would block some needed tiles and it would probably be bad. So instead I went with an approach that was worse than either of the above. I was somewhat inspired by the torus solver method explained here:

https://www.reddit.com/r/ExponentialIdle/comments/k9pcfc/how_to_solve_hard_torus_puzzle_quickly/

I loosely based my approach on this video even though the rules for our game are different. I left the rightmost column and bottom row as working space and solved from top to bottom. Each time I add a tile I find the fastest way to get the tile to its required position (using all possible candidates). If there are multiple ways to do it, I random them. The right column is solved next and lastly the bottom row. Sometimes this leaves a parity problem (the free tile cannot be swapped with the last tile). Because my solver was very fast (can do 50 times/s for N=30, 150k times/s for N =6) I would not have the parity problem. I just selected the solution with the lowest number of moves. EDIT: I also alternated between startingcorners by rotating the board before solving. That was a fun exercise, but only helped a little bit due to random variation.

I made some improvements later, such allowing for a different order of the middle rows and selecting the best one. In the end my approach used about 60-70% (I estimate) or so more moves than yowa's approach and maybe around 30-40% more moves than that of Siman (also used by Euler). The score was completely dominated by movecount. The other score-penalties were negligible.

I'm happy with my result as it is my first topcoder marathon!
```

#### nika
```
My solution was similar to this one, will list the major differences only:

For layout SA was done by adding random edges in the grid (connecting two cells) and if cycles are created (always the case for C=1) remove edge from it. For C>1 two components may touch and in this case try converting this cell to another color. Evaluation is just sum of absolute differences between tile frequencies used vs given. When it reaches 0 or 1 we have perfect grid.
For solving I only keep last row for working space and do it last. There are 5 kinds of moves to get next cell to free tile and then its destination: LEFT/RIGHT (row is free), LEFT/RIGHT and reverse with extra steps (row is busy), DOWN and reverse, UP and reverse, DOWN with extra steps. There are tiebreakers.
```

#### eulerscheZahl
```
I was just searching for motivation to draw some pictures and describe my approach, then you shared this.
Our ideas are extremely similar. I also construct trees by adding one cell at a time, using pre-determined locations for each color (but I limit myself to the tiles that actually exist and don't transform trees - instead I just generate a few thousand of them and take the best).
For the movement phase I also split it into horizontal actions to put the correct tiles in arbitrary order in a row and then solve each row as a single small problem (using beam search instead of detecting cycles).
Looks like you found a slightly better implementation.

For those who haven't seen it: yowa's strategy:

https://twitter.com/yowa/status/1618328487869440002?cxt=HHwWhIDTle_LuvUsAAAA

Looks so simple in hind-sight, well done.
```

#### sullyper
```
I did not compete obviously as I wrote the problem, but I gave it a try on my own, of course, to make sure there were enough approaches and depth to the problem.

First for context, this problem was inspired from an old board game that I used to play as a kid: "Labyrinth", this is where the extra piece comes from. Afterwards I realized we could have done without by just rotating a row/column, it might have been interesting as well.

When setting the problem, I thought about @Psyho's approach, however I did not think it would lead to good result when using several colors. I guess I was somehow wrong, it does seem that it's not as good for big boards of several colors, but it is quite close, and too be fair I did not expect the final submissions to be as good either. At the beginning the penalty for missmatch was always P=N, which, for big board might have been too much for @Psyho's solution (or probably he will have tuned a magic parameter in his scoring function to drastically reduce the number of missmatches =))

What I thought people would do is to find a target, and then reconstruct, so I am going to details those parts:

1- Find a target:
- One solution could be to run a simple HC or SA by swapping tiles, something as simple might work for C=1, but is unlikely to succeed for C > 1 as it would go in a local minimum with components of various color isolated.
- Beam Search to find a target might also have worked, but I think it would require some clever scoring function or it would have the same issue as a simple SA.
The approach I ended up using did not succeed all the time to generate a target, but was generating decent candidates (low number of connected components, usually 1 or 2 per color, rarely 3, as well as few missmatches: usually none, sometimes a couple.:

First thing I did is to detect which color has an extra tile, it's only possible if the extra tile generated did not have exactly 2 pipes going out, so we can detect it in 10/16 scenario. When we are unlucky, a solution could be to remove a tile from the color having the most of the tiles, or simply the one having the most tile. In addition to be the best option in term of probability (I think), it will give the most chance to still be able to reconstruct a tree by having more tiles.
Then I split the board in area of different colors, there are different solutions for that, the simple one is to split by row, the first color would take the top rows, then the second color the following rows, and so on... We could also expand from the corner for each color (as @simanman did, however this approach might not work well for all the reconstruction methods).
FInally I work on each color separately where I run 2 HC:
First HC starts from an empty grid, and add/remove edges between tiles trying to minimize the sum of square error between the number of tiles of each type we are using vs the target (similar to @nika). This really quickly converges to a solution or to an almost solution where one or two tiles might be wrong (in which case I replace them by the available tiles).
Second HC is here to take care of the connected components, what I did is find a loop running a dfs, select 2 adjacent tiles in that loop, and swap them with another 2 tiles somewhere else such as it does not create any missmatch. This also very quickly converge with in general very few connected component (which was a bit surprising to me).
I had now a way to generate decent targets in a negligeable amount of time (< 100ms, probably much faster in general).
There has been other approaches mentioned in this post, but the idea was to allow several solutions, what I wanted was to not force people in finding a valid a tree (because it was hard, I am sure the top competitors would have been able to do that if it was a requirement, but I want to keep it interesting for everyone). I wanted a penalty that was punishing enough, as I was hoping the top competitor would have very few missmatches and connected components, but still not too punishing to not discourage anyone. I am not sure I succeeded here, any feedback is welcome, but I think maybe the penalty on the connected components was too high, and might have been a simple P2 * number_components, while maybe increasing the range of P for the missmatches?

2- Reconstruction:
I thought about various solutions that would work, some better than others:
- reconstruct row by row, to move a tile at position (i, j) to position (r, c), assuming all the row before r are done, and all the tiles on row r before c are done, you can simply do:
- If r == i, first do: D j, L/R i+1, U j, then move to the next case
- Do: (i-r) time: D c, then (j-c) time R/L i, finally (i-r) time U c.
- This was the simplest approach to me, but I would expect it to take quite a lot of moves O(N^3) but with a constant quite high compare to some other methods.
- Second approach would also reconstruct row by row, this time by inserting, let's say from left to right. So at first you insert on the left the tile that will end up at the right most position, then the tile just before and so on. Now you needed to get this tile out, there are different moves that can be made, for example if it's on a row not used yet, you can simply rotate that row until the tile get's out, you could also push it out (let's say using Up motion), store it in a safe position,, then restore the grid in the other direction (Down motion), and get the tile out again.
- Both those approaches however does not resolve the issue of the last row, but we can see that using U a U b D a D b, we do a 3-permutation, this can be used to sort the final row. If unlucky, the last tile and the extra tile might be swapped. We could either accept this (the penalty would not be too high in general), or we could perform some other swaps as long as on the edge we have two identical tiles.

The second method can be improved by reconstructing in parallel several rows, in general this requires fewer move to get on of the piece we need out, and it can be further improved using a beam search, this is what I ended up doing as well as other competitors.
When picking this approach however, we need to pick a good target, if all the tiles are inserted from left to right, we need to make sure that we have piece of every colors on each column (roughly, of course if a color has very few pieces, this might not happen). If we were to have one color per corner, it means that at the beginning we would have only half of the pieces that are useful, and each time we get a tile of the wrong color, we need to discard it. So I was expecting that the target generation might also be important.

At the end, I generated several target (4/5 for the big grids, 250 for the smaller grids) and ran a BS to solve it. I completely missed the approach used by @RafbillFr and @simanman, I only started to see how I might have thought about it when looking how to fix the last row in a more optimized way, but by then I had already seen the solution from @RafbillFr, so I am not sure if I would have been to figure it out myself or not. Anyways, it was quite clever, and I did not think it would be possible to reconstruct that efficiently any target.

Final words:
I was aware of the AtCoder competition when I wrote this problem, I actually competed (more causually for lack of time), I had the idea before it came out, so I tried to keep it different enough:
- For the reconstruction part, I thought, and still think it was sufficiently different, first AC was much easier given the small size and having one color, actually during the competition I solved it using a simple randomized bruteforce with a few clever check to detect loops and isolated components. Here this approach would most likely not work for big board, and required some special handling of the colors. For those that used HC/SA, I still think they needed to handle the colors in some way, adding some novelty from the AC problem, and also, I was hoping (but I am not sure how much it happened) that same as AC, you will need to design a target "close" to the original board. And here "close" is based on the reconstruction method which was completely different.
- I think the reconstruction method was completely different. Of course some used rules based + greedy, but the rules are different, some use BS, but the scoring function would be different, the state would be different etc.

So at the end, it shared a two steps process (though @Psyho actually managed to skip it), and then common method to a lot of problem HC/SA/BS, but that's just too generic, we can't prevent all the problems from using those methods.

I hope I succeeded in making it different enough. I will be happy to get any feedback about the problem in general.
```
