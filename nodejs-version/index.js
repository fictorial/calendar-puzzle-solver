const { Worker } = require("worker_threads");
const os = require("os");

const solutions = {};
const n_solutions = parseInt(process.env.N_SOLUTIONS, 10) || 12 * 31;
let n_solved = 0;

function maybe_add_solution([key, board]) {
  console.log("got solution for ", key);

  if (!(key in solutions)) {
    solutions[key] = board;
    ++n_solved;

    const percent_done = ((100 * n_solved) / n_solutions).toFixed(2);
    console.log(`${percent_done}% done`);

    if (n_solved === n_solutions) {
      require("fs").writeFileSync("solutions.json", JSON.stringify(solutions));
      process.exit(0);
    }
  }
}

function main() {
  for (let i = 0; i < os.cpus().length; ++i) {
    const worker = new Worker("./worker.js");
    worker.on("message", maybe_add_solution);
  }
}

if (require.main === module) main();
