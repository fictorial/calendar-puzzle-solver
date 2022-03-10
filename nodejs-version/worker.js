const { parentPort } = require("worker_threads");
const assert = require("assert");

const PcgRandom = require("pcg-random");
const random = new PcgRandom();

const board = [
  [0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0],
];

const labels = [
  ["Jan", "Feb", "Mar", "Apr", "May", "Jun"],
  ["Jul", "Aug", "Sep", "Oct", "Nov", "Dec"],
  ["1", "2", "3", "4", "5", "6", "7"],
  ["8", "9", "10", "11", "12", "13", "14"],
  ["15", "16", "17", "18", "19", "20", "21"],
  ["22", "23", "24", "25", "26", "27", "28"],
  ["29", "30", "31"],
];

const pieces = [
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
];

function place_form_at(board, form, bx, by) {
  let to_set = [];
  let piece_no;

  for (let fy = 0; fy < 4; ++fy) {
    let cy = by + fy;
    if (cy > 6) return false;

    let row = form[fy];

    for (let fx = 0; fx < 4; ++fx) {
      const occupied_by = row[fx];

      if (occupied_by !== 0) {
        piece_no = occupied_by;

        let cx = bx + fx;
        if (
          board[cy][cx] ||
          cx > 6 ||
          (cy < 2 && cx > 5) ||
          (cy === 6 && cx > 2)
        )
          return false;
        to_set.push(cx, cy);
      }
    }
  }

  for (let i = 0, n = to_set.length; i < n; i += 2)
    board[to_set[i + 1]][to_set[i]] = piece_no;

  return true;
}

function place_form(board, form) {
  for (let by = 0, n = board.length; by < n; ++by) {
    let row = board[by];
    for (let bx = 0, m = row.length; bx < m; ++bx)
      if (place_form_at(board, form, bx, by)) return true;
  }

  return false;
}

function place_piece(board, forms) {
  for (let form of forms) if (place_form(board, form)) return true;
}

function solution_labels(board, strict = true) {
  let n_months = 0;
  let n_days = 0;
  const open_coords = [];

  for (let [by, row] of board.entries()) {
    let is_month = by < 2;

    for (let [bx, occupied_by] of row.entries()) {
      if (!occupied_by) {
        open_coords.push(bx, by);
        if (is_month) ++n_months;
        else ++n_days;
      }
    }
  }

  if (open_coords.length !== 4) return null;
  if (strict && (n_months != 1 || n_days != 1)) return null;

  return [
    labels[open_coords[1]][open_coords[0]],
    labels[open_coords[3]][open_coords[2]],
  ];
}

function clear_board(board) {
  board.forEach((row) => row.fill(0));
}

function shuffle(a) {
  let i, j, t;
  for (i = a.length - 1; i > 0; i--) {
    j = Math.floor(random.number() * (i + 1));
    t = a[i];
    a[i] = a[j];
    a[j] = t;
  }
}

function generate_solutions() {
  while (true) {
    shuffle(pieces);
    clear_board(board);

    let placed_all = true;
    for (let piece of pieces) {
      shuffle(piece);
      if (!place_piece(board, piece)) {
        placed_all = false;
        break;
      }
    }

    if (placed_all) {
      const res = solution_labels(board);
      if (res) {
        const [month, day] = res;
        parentPort.postMessage([`${month}-${day}`, board]);
      }
    }
  }
}

function test_check_solution() {
  const example_solutions = {
    "Jan-10": [
      [0, 2, 8, 8, 8, 3],
      [2, 2, 2, 2, 8, 3],
      [7, 7, 7, 5, 8, 3, 3],
      [7, 7, 0, 6, 6, 6, 3],
      [1, 1, 1, 1, 5, 6, 5],
      [4, 4, 4, 1, 5, 5, 5],
      [4, 4, 4],
    ],
    "Sep-27": [
      [8, 5, 5, 5, 4, 4],
      [8, 5, 0, 5, 4, 4],
      [8, 8, 8, 7, 4, 4, 2],
      [1, 3, 7, 7, 6, 6, 2],
      [1, 3, 7, 7, 6, 2, 2],
      [1, 3, 3, 6, 6, 0, 2],
      [1, 1, 3],
    ],
    "Nov-25": [
      [8, 8, 8, 6, 5, 5],
      [8, 6, 6, 6, 0, 5],
      [8, 6, 4, 4, 5, 5, 2],
      [1, 3, 4, 4, 7, 2, 2],
      [1, 3, 4, 4, 7, 7, 2],
      [1, 3, 3, 0, 7, 7, 2],
      [1, 1, 3],
    ],
    "Nov-31": [
      [4, 4, 2, 5, 5, 5],
      [4, 4, 2, 5, 0, 5],
      [4, 4, 2, 2, 3, 3, 3],
      [1, 7, 2, 3, 3, 6, 8],
      [1, 7, 7, 6, 6, 6, 8],
      [1, 7, 7, 6, 8, 8, 8],
      [1, 1, 0],
    ],
  };

  for (let [k, v] of Object.entries(example_solutions)) {
    assert.deepEqual(solution_labels(v), k.split("-"));
  }
}

test_check_solution();
generate_solutions();
