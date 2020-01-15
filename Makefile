comp:
	@echo " Compile main ...";
	gcc main.c virtual_memory.c LRU.c WS.c -ggdb3 -o  runner