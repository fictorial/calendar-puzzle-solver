demo.js: header.js.in footer.js.in solutions.js
	cat header.js.in solutions.js footer.js.in >demo.js

solutions.js: solver
	time ./solver $(shell sysctl -n machdep.cpu.core_count) 1>solutions.js

solver: solver.c pcg_basic.c
	clang -o solver solver.c pcg_basic.c

clean:
	rm -f solutions.js demo.js

