# Arcanoid : main.o my_getch.o
# 	gcc main.o my_getch.o -o Arcanoid

# main.o : main.c
# 	gcc -c main.c -o main.o

# my_getch : my_getch.c
# 	gcc -c my_getch.c -o my_getch.o

all:
	gcc main.c my_getch.c -o Arcanoid -Wall -Wextra -Werror -lncurses