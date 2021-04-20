main: main.c queue.c wfdList.c wfdRepo.c jsdList.c
	gcc -w main.c queue.c wfdList.c wfdRepo.c jsdList.c -o main -lm
