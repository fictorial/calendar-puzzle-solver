import json
import sys
import random
import copy
import multiprocessing as mp
import os

USE_MULTIPROCESSING = os.environ.get("MP") == "1"
N_SOLUTIONS = int(os.environ.get("N_SOLUTIONS", 12 * 31))

labels = [
    ["Jan", "Feb", "Mar", "Apr", "May", "Jun"],
    ["Jul", "Aug", "Sep", "Oct", "Nov", "Dec"],
    ["1", "2", "3", "4", "5", "6", "7"],
    ["8", "9", "10", "11", "12", "13", "14"],
    ["15", "16", "17", "18", "19", "20", "21"],
    ["22", "23", "24", "25", "26", "27", "28"],
    ["29", "30", "31"],
]

pieces = [
    [
        [
            [1, 1, 1, 1],
            [0, 0, 0, 1],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [1, 1, 1, 1],
            [1, 0, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [1, 1, 0, 0],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
        ],
        [
            [1, 1, 0, 0],
            [0, 1, 0, 0],
            [0, 1, 0, 0],
            [0, 1, 0, 0],
        ],
        [
            [1, 0, 0, 0],
            [1, 1, 1, 1],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [0, 0, 0, 1],
            [1, 1, 1, 1],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [0, 1, 0, 0],
            [0, 1, 0, 0],
            [0, 1, 0, 0],
            [1, 1, 0, 0],
        ],
        [
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 1, 0, 0],
        ],
    ],
    [
        [
            [2, 2, 2, 2],
            [0, 2, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [0, 2, 0, 0],
            [2, 2, 2, 2],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [2, 0, 0, 0],
            [2, 0, 0, 0],
            [2, 2, 0, 0],
            [2, 0, 0, 0],
        ],
        [
            [0, 2, 0, 0],
            [2, 2, 0, 0],
            [0, 2, 0, 0],
            [0, 2, 0, 0],
        ],
        [
            [2, 2, 2, 2],
            [0, 0, 2, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [0, 0, 2, 0],
            [2, 2, 2, 2],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [0, 2, 0, 0],
            [0, 2, 0, 0],
            [2, 2, 0, 0],
            [0, 2, 0, 0],
        ],
        [
            [2, 0, 0, 0],
            [2, 2, 0, 0],
            [2, 0, 0, 0],
            [2, 0, 0, 0],
        ],
    ],
    [
        [
            [3, 3, 3, 0],
            [0, 0, 3, 3],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [3, 3, 0, 0],
            [0, 3, 3, 3],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [0, 3, 0, 0],
            [3, 3, 0, 0],
            [3, 0, 0, 0],
            [3, 0, 0, 0],
        ],
        [
            [0, 3, 0, 0],
            [0, 3, 0, 0],
            [3, 3, 0, 0],
            [3, 0, 0, 0],
        ],
        [
            [0, 3, 3, 3],
            [3, 3, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [0, 0, 3, 3],
            [3, 3, 3, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [3, 0, 0, 0],
            [3, 3, 0, 0],
            [0, 3, 0, 0],
            [0, 3, 0, 0],
        ],
        [
            [3, 0, 0, 0],
            [3, 0, 0, 0],
            [3, 3, 0, 0],
            [0, 3, 0, 0],
        ],
    ],
    [
        [
            [4, 4, 4, 0],
            [4, 4, 4, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [4, 4, 0, 0],
            [4, 4, 0, 0],
            [4, 4, 0, 0],
            [0, 0, 0, 0],
        ],
    ],
    [
        [
            [5, 0, 5, 0],
            [5, 5, 5, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [5, 5, 5, 0],
            [5, 0, 5, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [5, 5, 0, 0],
            [0, 5, 0, 0],
            [5, 5, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [5, 5, 0, 0],
            [5, 0, 0, 0],
            [5, 5, 0, 0],
            [0, 0, 0, 0],
        ],
    ],
    [
        [
            [6, 0, 0, 0],
            [6, 6, 6, 0],
            [0, 0, 6, 0],
            [0, 0, 0, 0],
        ],
        [
            [0, 0, 6, 0],
            [6, 6, 6, 0],
            [6, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [0, 6, 6, 0],
            [0, 6, 0, 0],
            [6, 6, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [6, 6, 0, 0],
            [0, 6, 0, 0],
            [0, 6, 6, 0],
            [0, 0, 0, 0],
        ],
    ],
    [
        [
            [7, 0, 0, 0],
            [7, 7, 0, 0],
            [7, 7, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [0, 7, 0, 0],
            [7, 7, 0, 0],
            [7, 7, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [7, 7, 0, 0],
            [7, 7, 0, 0],
            [7, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [7, 7, 0, 0],
            [7, 7, 0, 0],
            [0, 7, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [0, 7, 7, 0],
            [7, 7, 7, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [7, 7, 7, 0],
            [7, 7, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [7, 7, 0, 0],
            [7, 7, 7, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [7, 7, 7, 0],
            [0, 7, 7, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
        ],
    ],
    [
        [
            [8, 8, 8, 0],
            [0, 0, 8, 0],
            [0, 0, 8, 0],
            [0, 0, 0, 0],
        ],
        [
            [8, 8, 8, 0],
            [8, 0, 0, 0],
            [8, 0, 0, 0],
            [0, 0, 0, 0],
        ],
        [
            [8, 0, 0, 0],
            [8, 0, 0, 0],
            [8, 8, 8, 0],
            [0, 0, 0, 0],
        ],
        [
            [0, 0, 8, 0],
            [0, 0, 8, 0],
            [8, 8, 8, 0],
            [0, 0, 0, 0],
        ],
    ],
]


def place_form(board, form, bx=None, by=None):
    if bx is None:
        for (by, row) in enumerate(board):
            for bx in range(len(row)):
                if place_form(board, form, bx, by):
                    return True
        return False

    to_set = []
    piece_no = None

    for (fy, row) in enumerate(form):
        for (fx, occupied_by) in enumerate(row):
            if occupied_by != 0:
                piece_no = occupied_by
                cx, cy = bx + fx, by + fy
                if (cy < 2 and cx >= 6) or (cy == 6 and cx > 2) or cy > 6 or cx > 6:
                    return False
                if board[cy][cx] != 0:
                    return False
                to_set.append((cx, cy))

    for (cx, cy) in to_set:
        board[cy][cx] = piece_no

    return True


def place_piece(board, forms):
    for form in forms:
        if place_form(board, form):
            return True
    return False


class InvalidSolutionError(Exception):
    pass


def solution_labels(board, strict=True):
    """Returns 2 labels for board representing *some* solution consisting of 2
    open squares on the board. If no solution is present, raises InvalidSolutionError.

    If `strict`, only a single month and single day are allowed to be open; 372
    solutions.  Else 2 open months or 2 open days are allowed; 903 solutions."""

    n_months, n_days = 0, 0
    open_coords = []

    for (y, row) in enumerate(board):
        is_month = y < 2
        for (x, value) in enumerate(row):
            if value == 0:
                open_coords.append((x, y))
                if is_month:
                    n_months += 1
                else:
                    n_days += 1

    if len(open_coords) != 2:
        raise InvalidSolutionError()

    if strict:
        if n_months != 1 or n_days != 1:
            raise InvalidSolutionError()

    a, b = open_coords[0], open_coords[1]

    label_a = labels[a[1]][a[0]]
    label_b = labels[b[1]][b[0]]

    return label_a, label_b


def clear_board(board):
    for row in board:
        for i in range(len(row)):
            row[i] = 0


def find_solutions(solutions, lock=None):
    "Finds one solution per 372 month-day combination."
    board = [
        [0 for x in range(6)],
        [0 for x in range(6)],
        [0 for x in range(7)],
        [0 for x in range(7)],
        [0 for x in range(7)],
        [0 for x in range(7)],
        [0 for x in range(3)],
    ]

    n_total = N_SOLUTIONS
    while len(solutions) < n_total:
        random.shuffle(pieces)
        clear_board(board)
        for forms in pieces:
            random.shuffle(forms)
            if not place_piece(board, forms):
                break
        else:
            try:
                month, day = solution_labels(board)
                key = f"{month}-{day}"

                if key not in solutions:
                    if lock:
                        lock.acquire()

                    solutions[key] = copy.deepcopy(board)
                    n_solutions = len(solutions)
                    done = 100 * n_solutions / n_total
                    print(
                        f"{done:.3}% done ({n_solutions}/{n_total})",
                        file=sys.stderr,
                    )

                    if lock:
                        lock.release()

            except InvalidSolutionError:
                pass


def save_solutions(solutions):
    with open("solutions.json", "w") as fp:
        json.dump(solutions, fp=fp)


def test_check_solution():
    board = [
        [0, 2, 8, 8, 8, 3],
        [2, 2, 2, 2, 8, 3],
        [7, 7, 7, 5, 8, 3, 3],
        [7, 7, 0, 6, 6, 6, 3],
        [1, 1, 1, 1, 5, 6, 5],
        [4, 4, 4, 1, 5, 5, 5],
        [4, 4, 4],
    ]

    assert solution_labels(board) == ("Jan", "10")


if __name__ == "__main__":
    test_check_solution()

    solutions: dict[str, list[int]] = {}

    if USE_MULTIPROCESSING:
        mgr = mp.Manager()
        solutions = mgr.dict()
        lock = mgr.Lock()

        n_procs = os.cpu_count()
        assert n_procs

        procs = [
            mp.Process(target=find_solutions, args=(solutions, lock), name=str(i))
            for i in range(n_procs)
        ]
        for proc in procs:
            proc.start()
        for proc in procs:
            proc.join()
    else:
        find_solutions(solutions)

    save_solutions(solutions)
