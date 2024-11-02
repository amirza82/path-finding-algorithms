OBJS = main.c graph.c algorithm.c
all:
	cc $(OBJS) -lm && ./a.out

db:
	cc -g3 $(OBJS) -lm && ./a.out

clean:
	rm a.out 
