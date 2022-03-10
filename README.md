# Calendar Puzzle Solver

![puzzle photo](photo.jpg)

The goal of this puzzle is to place all 8 pieces together on the board such that
all squares of the board are covered except a single month and a single day
thereof.  Repeat daily.

I became curious about how one might automatically solve for all dates and,
well, here we are. A simple algorithm finds all solutions (there are 1000s) in
less than 10 minutes. One solution per date is here included.

## HTML5 Visualization

Included is a HTML5 visualization of the solutions.

[Online version](https://fictorial.github.io/calendar-puzzle-solver/)

## Definitions

### Board

There are 43 board coordinates.

- origin in top-left; row-major ordering
- (0,0) is "Jan"
- (6,0) (right of "Jun") and (6,1) (right of "Dec") are invalid coordinates
- (6,2..6) (right of "31") are invalid coordinates

There are 12x31=372 month-day combinations that we wish to solve.
There are choose(43,2)=903 if all combinations including month-month and day-day are included.

### Pieces

- 8 *logical* pieces.  Every solution contains all logical pieces.
- There are a total of 46 unique *piece forms* across all logical pieces.
- Rotate logical piece by 0/90/180/270 degrees about Z (out of board).
- Each Z-rotated piece form can further be rotated 180 degrees about X as well as Y
  (which amounts to flips vertically and horizontally) to create more piece forms.

![piece forms](piece_forms.png)

## Exhaustive Search Approach (not used)

Enumerate all combinations of piece forms. For each piece form, enumerate all
possible placements of the piece form on an empty board. Merge the piece form
boards of each piece form of each piece form combination. Check if valid
solution and record if so. Stop when all solutions have been found or all piece
form combinations have been enumerated.  There are 524288 piece form
combinations, 372 dates to search, and a search space of 13 quintillion 421
quadrillion 772 trillion 800 billion (mostly invalid) options.  A
straightforward solution running in a single thread of a single process will
take about *20000 years* to complete.

    C(8,1) * C(8,1) * C(8,1) * C(2,1) * C(4,1) * C(4,1) * C(8,1) * C(4,1)
    = 524288

    C(20,1) * C(20,1) * C(20,1) * C(20,1) * C(20,1) * C(20,1) * C(20,1) * C(20,1)
    = 20^8 = 25.6e12

    524288 * 25.6e12
    = 13421772800000000000

## Stochastic Approach (used)

Shuffle piece order and piece form order. Place first piece form of each logical
piece on board. If a piece form from each logical piece fits together, check if
it's a valid solution and record it if so.  Repeat until all solutions are
found.

> Since solutions are randomly discovered, many previously solved dates are
> repeatedly solved. Thus, as progress gets closer to 100%, it becomes much more
> difficult to find a solution for any of the few remaining dates to be solved.

## Implementations

There are several implementations included.

## Development machine

All development was performed on a 2018 Mac Mini running OS X Monterey.

- C
  - Apple clang version 13.0.0 (clang-1300.0.29.30)
  - Target: x86_64-apple-darwin21.3.0
- Python
  - Python 3.9.10 (8276b505180f70c5784a698a510f0a17317a85c3, Feb 19 2022, 16:51:03)
  - [PyPy 7.3.8 with GCC Apple LLVM 13.0.0 (clang-1300.0.29.30)]
- Node.js
  - v16.11.1

### C (default)

This version launches N threads which run in parallel, locking only when a
potential solution is found.  It runs in about a minute (or 10 minutes if you
mad the RNG mad).

#### Running

This version automatically updates the HTML5 visualization files as needed. It's the fastest option and thus the default. The other versions dump a `solutions.json` file which you can (if you want) simply copy into the index.html file after running this C version at least once.

```bash
make clean
make
open index.html
```

### Node.js

This version uses worker threads to generate potentially unique solutions.

#### Running

```bash
cd nodejs-version
time make
time make cpu-profile
time make profile
```

#### Conclusions

V8 is "weird" in that it only uses half the number of cores which apparently
actually makes sense technically (see README for links) but isn't all that
useful. See [1], [2].

[1]: https://github.com/nodejs/help/issues/2316
[2]: https://github.com/denoland/deno/issues/10592#issuecomment-838982656

### Python

- CPython reached 90% solved in 40 minutes.
- CPython with multiprocessing reached 90% solved in 10 minutes.
- PyPy reached 90% solved in 3 minutes.
- PyPy with multiprocessing reached 90% solved in 3 minutes.

Why 90%? To reach 100%, there are fewer and fewer solutions for
dates that have yet to be discovered so the time between new solutions
increases dramatically.

#### Running

```bash
cd python-version

echo "pypy no multiprocessing"
time N_SOLUTIONS=334 pypy3 solver.py

echo "pypy with multiprocessing"
time N_SOLUTIONS=334 MP=1 pypy3 solver.py

echo "cpython no multiprocessing"
time N_SOLUTIONS=334 python3 solver.py

echo "cpython with multiprocessing"
time N_SOLUTIONS=334 MP=1 python3 solver.py
```

#### Conclusions

- PyPy is a nice speedup over CPython (... in this case)
- Multiprocessing doesn't add much for PyPy...
  - The 6 cores on the dev machine were steady at ~45% utilization
    for some reason (communication overhead?).

#### Other things to consider

- NumPy instead of vanilla Python lists for board and piece form representations
- Better piece representation (indexes where there's a value and nothing more)
- PCG RNG instead of MT which is what Python uses under the covers

---

## Future

- Add other implementations
  - Julia
  - Go
  - Rust
  - Janet

---

Please let me know if you have a better algorithm or other optimizations for one
or more implementations.
