solutions.js:
	node .

profile: clean-profile
	N_SOLUTIONS=5 node --prof .
	node --prof-process isolate*.log > prof.txt
	@echo Look for Summary in prof.txt

cpu-profile: clean-cpu-profile
	N_SOLUTIONS=5 node --cpu-prof .
	@echo "Open CPU.xxx in Chrome Dev Tools (Performance, Import)"
	@ls CPU.*

clean-profile:
	rm -f isolate* prof.txt

clean-cpu-profile:
	rm -f CPU.*

clean: clean-profile clean-cpu-profile
	rm -f solutions.js

.PHONY: clean clean-profile profile