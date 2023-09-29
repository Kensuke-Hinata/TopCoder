#### sullyper
```
This was a quite interesting problem, however I think it ends up being different from what the writters was expecting:
Same as for MM131 where the goal was to game the AI, here the goal was to game the provided AI, so the option of competing againt a custom AI, while nice one paper, ended up useless.

So we were provided 5 AI to defeat, I applied a different strategy for each one of them, though some share common ideas:

Against AI 5:
This AI is quite trivial to defeat, it places box in a very predictive order, and do not attempt to block any gift, and would be lucky to manage to block one.
While we can probably do something smarter to get some extra points, a simple solution that matches a elf to its closest present and go straight to it would probably score pretty well.
I made a couple of improvement on that:
- First I don't return the last present until the end (as the AI will keep putting box and therefore spending money), it also means that I need to start picking up boxes with the extra elves.
- While targetting the present, I start by targetting the present the further away from the edes, the hope is that if I don't have time to pick up all the present, toward the end an elf spawning might still have some present close enough to go get and bring back.

Against AI4:
This AI plays defensive in a predictable way as well, it attempts to block you just before you get next to a present, however it has a few weakness:
- It places only one box at a time
- It only places a box if you threaten a single edge of the present.

This means that for some present we can go pick them up with a single elf if we arrive from the right direction (a corner). For the other present we could use two elves.

My solution actually only use the first trick, plus a strategy that I will be using for AI 2 and 3 as well:
For all my elves:
- I see if I can pick a present for free (no block will be used)
- If not I move my elves as close as possible as a present before the AI places a box and I wait

When I have enough elves waiting (enough depends on N and C), they all goes at once, as the AI place a single box at a time, a couple of elves will be blocked, but all the other will reach a present and grab it.

Before picking the last present, I place an elf in a position it will be able to pick up for free, and I keep attacking the present one elf at a time to force the AI spending more money.

Against AI3:
This is the one I struggle the most against. Mostly because I did not really look at it.
The AI basically look at every free cell, check if placing a box will increase the overall distance to reach an edge from a present. And if there is no such cell, it picks a random free cell next to a present.
This AI is annoying as it can trap your elves carrying a present.

My strategy is stupid, rush for all the present I can reach and bring them back.
If I can't find one, I just move next to a block and wait for enough elves to be ready. I once again use the fact that that AI only places a single box at a time, so if I steal enough box at once, it will create some opening for some other elves to go steal some present before the AI can close the gate again.
Once again how many elves depends on N and C (usually for big C we don't need to wait much as the AI will run out of money).

I don't even delay picking the last present because I was quite lazy to work on this AI.

Against AI2:
This AI is similar to AI3, actually until the last day I was using the exact same code for both. It also check all the free space, attempt to place a box, and see if it decrases the number of edges to a present that can be reached.

This AI has the same weakness to place a single box at a time, so when stuck, I will use the same strategy of accumulating elves before attacking all at once.

Another warkness is that for a give edge to a present, if there is no other single cell that can be blocked to cut the path to the edge, then you can use 2 elves to steal the present, the first one go pick up the box next to the present, and wait, you can bring another elf, you swap their position, and while picking up the present, you might want to bring back the previous elf to block the entrance again (in case it opened a path to other present).
This could be extended (but I did not have time) to also steal present even if some other cell can cut the path to the edge by first placing some additional elves (potentially carrying a present or box already) in those cells, then you can pic the box next to the present, and do the swap.

So at the end my strategy was:
- Run for the accessible present
- If none, see if I can steal some present using 2 elves
- If not, move next to a block, and wait for the herd to be ready to attack!

Against AI1:
There are actually 2 code being ran based on the initial grid:
- First version is simply blocking all the cell next to a present
- Second version, it pick a path that surround all the present and just attempt to block it.

While this AI was probably the smartest against MM131, it has some severe weakness (same as my solution for MM131, which while I ended n1, it would fair poorly if I had to attack it). The reason is it targetted the weakness of the AI of MM131, which is elves are independent and do not cooperate.

For the first version, we can easily steal a present using 2 elves in most cases, similar idea as AI2, just steal a box next to a present, and wait for another elf to come and swap position (not really swap, just move away while the other elf take the spot).

So my solution is just rush to the present I can reach, once they are all surrounded, use the strategy above, if it fails, pick some random boxes, in general it is enough to unblock the situation.
Of course, I don't pick the last present until the last moment.

For the second version, I run the AI code to generate the border it attempts to cover, from here I need to place one free elf on this border (sometimes I can rush one before it gets blocked), this will be an entry, and I need another elf carrying a present or a box (usually you can just steal one of the box), this is an exit.
In practice you end up having several entrance and exits, and whenever a new elf comes, it can go to an entrance, take the position of the elf waiting while this one goes inside pick up a present. It will then go to an exit and take the position of the elf there who will go to the edge.
Sometimes I change an entrance into an exit, or the opposite. TO change an exit to an entrance, you either need to exit next to each other Or an exit not on a straight line (so basically with 2 cells either inside or outside).

Overall it was fun, but the goal was really to game the AI, I suspect I do poorly on AI 2 and 3 as I spent little time on them, and I reallized in the last 30min that I have some bug in AI1 that makes me waste some exit...

Room for improvement:
- For AI 1, 2, 3:
- It is easy to predict when a box will be placed by simulating the AI, instead of rushing to the presents, I could take this information into account. Sometimes I rush to a present to be blocked in the last moment.
- For all the AI:
- I had a stupid logic to assing elves to target, sometimes even having multiple elves going to the same target (which sometimes was good because they groupped for a common attack). I could have use a Bipartite matching for the assigment.
- For AI 2, I could keep an elf next to a present instead of stealing it untill all its other neighbors are covered, it will waste more money, and also give more time for my other elves to attack.
- Some stupid thing I thought about but did not do, for elves that are trapped on the edge, we should move them on the edge as much as possible, this will reduce the probability that some other elf gets trapped, and at least provide the elf a bit earlier in average.
- Ok, the list of improvement will never ends, so many things that can be improved, the fact there were 5 AIs, means they were 5 (arguably 6) distinct problem. I doubt the top solutions will be using the same strategy (except maybe for AI 2 and 3?);

Result on first 10 seeds:
seed 1: 412
seed 2: 18750
seed 3: 9345
seed 4: 315
seed 5: 6256
seed 6: 356
seed 7: 8938
seed 8: 820
seed 9: 4478
seed 10: 1911
```

#### eulerscheZahl
```
I couldn't bring myself to handle all kinds of different AIs. I do a bit of tracking and know where to expect box placements from AI 4.

My solution generates random moves: it picks one elf at random and simulates 2 turns. If it interferes with any other elves, these also get a new action assigned to make space for the plan of the first elf. Then I score the outcome, keep the better one and try some more random plans.
The scoring function rewards being on a previous box spawn (works well for AI 1, less so for AI 2 and 3) as well as the distance to a target (present or edge of map, depending on the state of the elf). The distance doesn't 100% match the actual turns needed: some paths are considered more risky based on previous box spawns and get a penalty.

Another weakness of AI 4: it only blocks elves without a box/present. Therefore you can move towards a present, while already holding one. Then come with a 2nd elf and make room just in time.

One tiny gotcha: the game tries to spawn elves along a circle. If the initially planned cell is occupied, it continues looking for the next one until it's free. On some maps there's a section of cells that can never reach a present. Therefore it's beneficial to not only keep those elves at the edge but move it to one side of the cut-off area in order to wrap the spawn into the next region.

https://github.com/eulerscheZahl/Topcoder/tree/master/MM142
```

#### nika
```
I had similar ideas but limited time, so in the end I only detect AI5 and use special strategy (delayed last present exit)

For others, it is heuristic that does the following in order of decreasing priority

Exiting with loaded elves and moving with elves towards open path to some present
Similar 2 elf attack strategy to break perimeter, one turn first one picks the box, next turn it moves and second elf takes its place
If loaded elf is surrounded, use one elf to pick up blocking box and open exit path (none of the AI-s seem to block loaded elf intentionally)
If no progress is made pick some boxes close to presents
```

#### Psyho 
```
Intro
The concept was really cool (especially from the design perspective), but as others have already mentioned the problem turned out to be way too complex. It also gave massive advantage to java coders, considering you had to translate AI solutions to your own language. AI #3 used rng which made it even worse. It would have been nice to get at least the AI's id as a parameter.

Even though I really like problems like this, the scope here is so large that I had trouble finding motivation to write a decent solution. In the end, I did most of the work on the very last day. I have implemented AI #2-#5 in my code, but I didn't have time to do any heavy exploitation, so I'm only using this to detect the opponent. I have a single solution that handles all of the opponents, but (as usual?) I abused testing (5K tests per 5 mins) for some parameter tuning.

My code is quite buggy and my elves are performing a ton of useless operations. I didn't have time to implement explicit exploitation of #1 (precompute boxes' placement to put elves on the path) and #4 (which IMHO was very easy to get nearly-optimal number of presents, but much harder to exploit the maximum number of wasted money).

Solution
Definitions:

reachable: means that the cell is reachable from the border
alive: means that the cell is reachable from any of the presents
guarding: special property of the elf with a box, which generally means that the elf will stand still and guard it's position
sidestep: special move for guarding elves to make space so that other elves can pass; after performing the move, it tries to go back to the original guarding position
basic elf: elf that doesn't carry anything
Order of each turn:
1. Update opponent AI prediction by running the code on the previous state
2. For all guarding elves: remove "guarding" property if the cell is not alive or all neighbors are reachable
3. For all guarding elves: if there's an elf with a present on a non-reachable cell, make a sidestep to make space for that elf
4. For all elves with presents: go along the fastest path to the border
5. For all guarding elves: if there's an elf in a non-alive cell, make a sidestep to make space for that elf
6. For all non-guarding elves with boxes: go along the fastest path to the border
7. For all basic elves that can reach any present: go along the fastest path to the present
8. For all basic elves that can reach any guarding elf that has at least 2 empty cells around: go to the closest one
9. For all basic elves that can reach an alive box: choose the one that has the most empty cells around it; in case of a tie, take the closest one
10. For all basic elves that can reach any box: go to the closest box
11. Perform all of the planned sidesteps or go to the border if unsuccessful after X turns

In each step, I'm only considering elves that have not moved yet. Except for the very few places, the logic is exactly the same for all opponents.

Apart from the above, there's also a special "waiting" logic that:

for AI #2 & #3: in tests with smaller elfP * C, disallow all moves for the first 75-80% turns where we would pick up a box
for AI #4: in tests with smaller elfP * C, don't perform any moves for the 70% turns.
Waiting is the main logic used for exploiting AIs #2-4, since they are allowed to place only a single box per turn. It was counterproductive for AI #1 as the guarding mechanic was good enough.

I also have a cheap trick of temporary marking some close-to-the-border presents as trees so that the elves prefer the presents in the center at the start of the simulation.

Example Results
{"id": 1, "AI": 1, "score": 412.0, "time": 20, "N": 10, "C": 1, "elfP": 0.1, "M": 36}
{"id": 2, "AI": 5, "score": 18750.0, "time": 354, "N": 30, "C": 10, "elfP": 0.2, "M": 9}
{"id": 3, "AI": 4, "score": 8620.0, "time": 256, "N": 29, "C": 7, "elfP": 0.111968, "M": 14}
{"id": 4, "AI": 4, "score": 221.0, "time": 14, "N": 10, "C": 3, "elfP": 0.117386, "M": 12}
{"id": 5, "AI": 1, "score": 5336.0, "time": 283, "N": 27, "C": 8, "elfP": 0.10486, "M": 8}
{"id": 6, "AI": 3, "score": 376.0, "time": 36, "N": 13, "C": 2, "elfP": 0.161918, "M": 14}
{"id": 7, "AI": 1, "score": 8053.0, "time": 302, "N": 28, "C": 3, "elfP": 0.130583, "M": 3}
{"id": 8, "AI": 4, "score": 845.0, "time": 31, "N": 14, "C": 5, "elfP": 0.150601, "M": 5}
{"id": 9, "AI": 3, "score": 5138.0, "time": 131, "N": 20, "C": 6, "elfP": 0.1832, "M": 42}
{"id": 10, "AI": 5, "score": 1911.0, "time": 22, "N": 14, "C": 1, "elfP": 0.10214, "M": 10}
My guess is that I do better on #3 than others, since I don't believe there is any clear exploitative strategy. I'd imagine the final standings can wildly differ from the provisional if the test distribution was highly skewed.

Out of curiosity, I made a run where I replaced all of the AIs with #5 to figure out how many points I'm missing vs each AI:

Tests                  5000     1021     1018     1008      981      972
Run                 Overall     AI=1     AI=2     AI=3     AI=4     AI=5
------------------  -------  -------  -------  -------  -------  -------
all_ai5             98.5909  98.4543  99.0673  98.8439  98.6716  97.8915
final_submit        85.3864  88.9982  77.8269  73.3607  89.5334  98.0376

https://github.com/FakePsyho/cpcontests/tree/master/topcoder/mm142
```

#### mugurelionut
```
I barely had time to finish a working solution, let alone read the AIs' code and try to understand it :)

It was clear to me that understanding in more depth how the 5 AIs work and devising strategies specific to countering each one would be better than a single common strategy, but... that would be too much work and too little time. In fact, could someone briefly summarize how the 5 AIs work?

In my opinion it would have been cooler to compete against the top 5 AIs from MM131 - the source code for them was provided anyway.

As I said, my solution uses a single strategy and doesn't try to understand anything about the opponent. At each step I recompute an assignment. I use Greedy here. For each present I compute the shortest path to each elf, minimizing the number of obstacles first, then distance (obstacle = present or box; I assume that other elves can be passed through when computing the paths). Then for each present I consider the closest unassigned elf (according to the same metric). If there's more than 2 obstacles along the path, I ignore this. If there's 1 obstacle along the path and it's not paired, I identify the obstacle and I find the closest unassigned elf to the obstacle (this time I don't allow other intermediate obstacles along the path).

I compute a metric based on distances between present, intermediate obstacle and elf/elves and repeatedly pick the best tuple (shorter distances are better). For the best tuple I then assign / (I called it pair) the 1 or 2 elves to the 1 or 2 destinations (present and intermediate obstacle), preferring the elf closer to the intermediate obstacle to be paired with it.

If I still have unpaired elves, I pair them with close unpaired boxes.

Then I compute paths for each elf to their destinations (box, present, out) and I perform moves for each elf, in some order. I prefer elves with presents first, then free elves. Elves with boxes generally stay on the map. I use the distance to the target destination as a tie breaker. If the cell to which an elf moves is already occupied, I try to push the occupying elf away, if it hasn't moved, yet. The pushing away is recursive and may devolve into backtracking - I try all the possible neighboring cells (preferring the moved elf's preferred move cell); if needed, this may mean another elf is pushed further, and so on.

I initially didn't keep the elves with boxes inside. When I decided to keep them, I got a large score boost. The last large score boost (literally the difference betwen my last 2 submissions) happened with less than 1 hour from the contest to go. If an elf with a box was moved, it would try to move back to the cell from which they picked up the box.

Example scores:
1: 412
2: 18620
3: 8286
4: 318
5: 5080
6: 568
7: 8244
8: 875
9: 4238
10: 1909
```
