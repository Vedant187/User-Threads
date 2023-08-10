echo "main program started"
gcc uthread.c queue.c main.c -o main
./main

echo "**********testlock program started**********"
gcc uthread.c queue.c testlock.c -o testlock
./testlock


echo "*******lock program started**********"
gcc uthread.c queue.c lock.c -o lock
./lock

echo "**********factorial program started***********"
gcc uthread.c queue.c factorial.c -o factorial
./factorial

echo "********** matrixmul program started ***********"
gcc uthread.c queue.c matrixmul.c -o matrixmul
./matrixmul

