ttree:avltree.c main.c
	$(CC) -g $^ -o $@
clean:
	-@rm -rf ttree
.phony: clean
