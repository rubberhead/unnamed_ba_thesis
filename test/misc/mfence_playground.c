#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

// Shared variable
int a = 1;

void foo(void) {
    while (a)
        asm volatile ("":::"memory");
    printf("Exiting from foo...\n");
    exit(EXIT_SUCCESS);
}

void bar(void) {
    a = 0;
    printf("Exiting from bar...\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
    const size_t NR_THREADS = 2;
    pthread_t *threads = calloc(NR_THREADS, sizeof(pthread_t));

    int ret_create_thread_1 = pthread_create(
        &threads[0], NULL, (void *(*)(void *))foo, NULL);
    if (ret_create_thread_1) {
        perror("Failed to create pthread for `foo`.");
        exit(EXIT_FAILURE);
    }

    int ret_create_thread_2 = pthread_create(
        &threads[1], NULL, (void *(*)(void *))bar, NULL);
    if (ret_create_thread_2) {
        perror("Failed to create pthread for `bar`.");
        pthread_cancel(threads[0]);
        exit(EXIT_FAILURE);
    }

    for (size_t thread_idx = 0; thread_idx < NR_THREADS; thread_idx++) {
        pthread_join(threads[thread_idx], NULL);
    }

    exit(EXIT_SUCCESS);
}