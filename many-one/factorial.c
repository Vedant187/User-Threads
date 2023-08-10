#include <stdio.h>
#include <stdlib.h>
#include "uthread.h"

struct limits {
    int l;
    int h;
};

void* f(void* arg) {
    int res = 1;
    struct limits* lim = (struct limits*) arg;
    for(int i=lim->l; i<=lim->h; i++) {
        res *= i;
    }
    int* ret = (int*) malloc(sizeof(int));
    *ret = res;
    printf("function f exited with return value=%d\n",*ret);
    return ret;
}

int main() {
    int n;
    printf("Enter a number: ");
    scanf("%d", &n);
    int tid[2];
    struct limits lim[2];
    lim[0].l = 1;
    lim[0].h = n/2;
    lim[1].l = n/2 + 1;
    lim[1].h = n;
    thread_create(&tid[0], f, &lim[0]);
    thread_create(&tid[1], f, &lim[1]);
    int* ret1;
    int* ret2;
    uthread_join(tid[0], (void**) &ret1);
    uthread_join(tid[1], (void**) &ret2);
    printf("Factorial of %d is %d\n", n, (*ret1) * (*ret2));
    free(ret1);
    free(ret2);
    return 0;
}
