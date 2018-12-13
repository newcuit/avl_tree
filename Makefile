avltree:avltree.c main.c
	$(CC) -g $^ -o $@
clean:
	-@rm -rf avltree
.phony: clean
