#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int input_counter = 0;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut_for_big = PTHREAD_MUTEX_INITIALIZER;
sem_t semaphore;
int ERROR  = 0;

typedef struct s_prod
{
    int x;
    char *text;
    struct s_prod *next;

} S_P;

S_P *list = NULL;

void *byeWorld(void *threadNumber)
{
    int *myThreadNumber = threadNumber;
    while (1)
    {
        sem_wait(&semaphore);
        pthread_mutex_lock(&mut);
        S_P new_list;
        if (input_counter != 0)
        {
            input_counter--;
            //Do text
            new_list.text = malloc(sizeof(list->text));

            for (int i = 0; i <= sizeof(new_list.text); i++)
            {
                new_list.text = list->text;
            }
            //Do x
            new_list.x = list->x;

            //new word input - do next word
            for (int i = 0; i < input_counter; i++)
            {
                list[i] = list[i + 1];
            }

            list = realloc(list, sizeof(S_P) * input_counter);

            pthread_mutex_unlock(&mut);
            pthread_mutex_lock(&mut_for_big);
            printf("Thread %d:", *myThreadNumber);
            for (int i = 0; i < new_list.x; i++)
            {
                printf(" %s", new_list.text);
            }
            printf("\n");
            pthread_mutex_unlock(&mut_for_big);
            free(new_list.text);
        }
        else
        {
            pthread_mutex_unlock(&mut);
            break;
        }
    }
    return 0;
}

void *helloWorld()
{
    int ret = 0, x = 0;
    char *text = NULL;

    while ((ret = scanf("%d %ms", &x, &text)) != EOF)
    {
        if(ret!=2){
            
            ERROR = 1;
            break;
        }
        if(x<0){
            free(text);
            ERROR = 1;
            break;
        }
        pthread_mutex_lock(&mut);

        input_counter++;
        int previous_input = input_counter - 1;
        //Do list
        list = realloc(list, sizeof(S_P) * input_counter);
        //Do next
        if (previous_input > 0)
        {
            list[previous_input - 1].next = list;
            list[previous_input - 1].next += previous_input * sizeof(S_P);
        }
        //Do text
        list[previous_input].text = malloc(sizeof(text));
        for (int i = 0; i < sizeof(list->text); i++)
        {
            list[previous_input].text = text;
        }
        //Do x
        list[previous_input].x = x;

        pthread_mutex_unlock(&mut);
        sem_post(&semaphore);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    pthread_t thread;
    sem_init(&semaphore, 0, 0);
    int status_addr = 0;
    int N = 1;
    if (argc > 1)
    {
        N = atoi(argv[1]);
    }
    if (N > sysconf(_SC_NPROCESSORS_ONLN))
    {
        exit(1);
    }
    pthread_t thread_n[N];
    int threadNumber[N];
    pthread_create(&thread, NULL, &helloWorld, NULL);

    for (int i = 0; i < N; i++)
    {
        threadNumber[i] = i + 1;
    }

    for (int i = 0; i < N; i++)
    {
        pthread_create(&thread_n[i], 0, &byeWorld, (void *)&threadNumber[i]);
    }
    pthread_join(thread, 0);

    for (int i = 0; i < N; i++)
    {
        sem_post(&semaphore);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(thread_n[i], (void **)&status_addr);
    }

    pthread_mutex_destroy(&mut);
    pthread_mutex_destroy(&mut_for_big);
    sem_destroy(&semaphore);
    free(list);
    exit(ERROR);
}
