main: main.c queue.c linkedList.c wfdRepo.c
	gcc -g main.c queue.c linkedList.c wfdRepo.c jsdList.c -o main -lm
