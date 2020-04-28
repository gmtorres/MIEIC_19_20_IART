PROG = Eximo

CC = g++
CPPFLAGS = -Wall 

SRCS = main.cpp game.cpp

OBJS = $(SRCS:.cpp=.o)


all: $(PROG)

$(PROG) : $(OBJS)
	$(CC) $(FLAGS) -o $(PROG) $(OBJS)

clean:
	rm -f *.o *~ $(MAIN)