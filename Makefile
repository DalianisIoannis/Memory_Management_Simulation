comp:
	@echo " Compile main ...";
	gcc main.c virtual_memory.c replacement_algorithms.c -ggdb3 -o  runner