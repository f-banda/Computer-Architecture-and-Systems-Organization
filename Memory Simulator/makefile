#
# PROGRAM: Assignment 3
# AUTHOR:  Francisco Banda
#

memsim: main.cpp hex.cpp memory.cpp hex.h memory.h
	g++ -g -ansi -pedantic -Wall -Werror -Wextra -std=c++14 -c -o main.o main.cpp
	g++ -g -ansi -pedantic -Wall -Werror -Wextra -std=c++14 -c -o memory.o memory.cpp
	g++ -g -ansi -pedantic -Wall -Werror -Wextra -std=c++14 -c -o hex.o hex.cpp
	g++ -g -ansi -pedantic -Wall -Werror -Wextra -std=c++14 -o memsim main.o memory.o hex.o

clean:
	rm -f *.o