#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

void* producer(void* arg) {
	int item = 0;
	while (1) {
		pthread_mutex_lock(&mutex);
		while (count == BUFFER_SIZE) {
			pthread_cond_wait(&full, &mutex);
		}
		buffer[count++] = item;
		printf("Produced: %d\n", item++);
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	return NULL;
}

void* consumer(void* arg) {
	int item;
	while (1) {
		pthread_mutex_lock(&mutex);
		while (count == 0) {
			pthread_cond_wait(&empty, &mutex);
		}
		item = buffer[--count];
		printf("Consumed: %d\n", item);
		pthread_cond_signal(&full);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	return NULL;
}

int main() {
	pthread_t producer_thread, consumer_thread;

	pthread_create(&producer_thread, NULL, producer, NULL);
	pthread_create(&consumer_thread, NULL, consumer, NULL);

	pthread_join(producer_thread, NULL);
	pthread_join(consumer_thread, NULL);

	return 0;
}

