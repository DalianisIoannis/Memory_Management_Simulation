comp:
	@echo " Compile main ...";
	gcc main.c virtual_memory.c replacement_algorithms.c ws.c -ggdb3 -o  runner