#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define THREAD_POOL_SIZE 4
#define TASK_QUEUE_SIZE 10

typedef struct {
    void (*function)(void*);
    void *arg;
} Task;

Task taskQueue[TASK_QUEUE_SIZE];
int queueFront = 0, queueRear = 0, taskCount = 0;

pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t taskAvailable = PTHREAD_COND_INITIALIZER;

void* workerThread(void* arg) {
    while (1) {
        pthread_mutex_lock(&queueMutex);

        while (taskCount == 0) {
            pthread_cond_wait(&taskAvailable, &queueMutex);
        }

        Task task = taskQueue[queueFront];
        queueFront = (queueFront + 1) % TASK_QUEUE_SIZE;
        taskCount--;

        pthread_mutex_unlock(&queueMutex);

        task.function(task.arg);
    }
    return NULL;
}

void enqueueTask(void (*function)(void*), void *arg) {
    pthread_mutex_lock(&queueMutex);

    taskQueue[queueRear].function = function;
    taskQueue[queueRear].arg = arg;
    queueRear = (queueRear + 1) % TASK_QUEUE_SIZE;
    taskCount++;

    pthread_cond_signal(&taskAvailable);
    pthread_mutex_unlock(&queueMutex);
}

// Example task
void printNumber(void *arg) {
    int num = *(int*)arg;
    printf("Task: %d\n", num);
    sleep(1);
}

int main() {
    pthread_t threads[THREAD_POOL_SIZE];

    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_create(&threads[i], NULL, workerThread, NULL);
    }

    for (int i = 0; i < 20; i++) {
        int *num = malloc(sizeof(int));
        *num = i;
        enqueueTask(printNumber, num);
    }

    sleep(10); // Let tasks finish

    return 0;
}
