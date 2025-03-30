#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_STUDENTS 5
#define NUM_CHAIRS 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_students;
sem_t sem_ta;
int waiting_students = 0;
int all_students_finished = 0;

void *student(void *student_id) 
{
    int id = *((int *)student_id);

    while (1) 
    {
        printf("Student %d is programming.\n", id);
        sleep(rand() % 5 + 1); // 学生随机进行编程（1-5秒）

        pthread_mutex_lock(&mutex);

        if (waiting_students < NUM_CHAIRS) 
        {
            waiting_students++;
            printf("Student %d takes a seat. Waiting students: %d\n", id, waiting_students);
            pthread_mutex_unlock(&mutex);
            sem_post(&sem_students); // 通知TA有学生需要帮助
            sem_wait(&sem_ta); // 等待TA的帮助
        }
        else 
        {
            printf("Student %d cannot find a seat. Leaving...\n", id);
            pthread_mutex_unlock(&mutex);
            break; // 如果没有座位可用，学生离开
        }
    }

    // 学生随机一段时间后再回来
    sleep(rand() % 10 + 1); // 学生过一段时间后回来（1-10秒）
    pthread_mutex_lock(&mutex);

    if (waiting_students < NUM_CHAIRS) 
    {
        waiting_students++;
        printf("Student %d returns and takes a seat again. Waiting students: %d\n", id, waiting_students);
        pthread_mutex_unlock(&mutex);
        sem_post(&sem_students); // 通知TA有学生需要帮助
        sem_wait(&sem_ta); // 等待TA的帮助
    } 
    else 
    {
        printf("Student %d returns but cannot find a seat again. Leaving...\n", id);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    all_students_finished++;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *ta(void *arg) 
{
    while (1) 
    {
        sem_wait(&sem_students); // 检查是否有等待帮助的学生
        pthread_mutex_lock(&mutex);

        if (waiting_students > 0) 
        {
            printf("TA is helping a student. Waiting students: %d\n", waiting_students);
            waiting_students--;
            sem_post(&sem_ta); // 通知学生TA可以帮助
            pthread_mutex_unlock(&mutex);
            usleep((rand() % 5 + 1) * 1000000); // TA帮助学生一段随机时间（1-5秒）
        } 
        else
        {
            printf("No students waiting. TA goes to sleep.\n");
            pthread_mutex_unlock(&mutex);
            if (all_students_finished == NUM_STUDENTS) 
            {
                printf("All students finished. TA goes home.\n");
                break; // 如果所有学生都完成了，TA回家
            }
        }
    }

    return NULL;
}

int main()
{
    pthread_t students[NUM_STUDENTS];
    pthread_t ta_thread;

    srand(time(NULL));

    sem_init(&sem_students, 0, 0);
    sem_init(&sem_ta, 0, 0);

    // 创建TA线程
    pthread_create(&ta_thread, NULL, ta, NULL);

    // 创建学生线程
    int student_ids[NUM_STUDENTS];
    for (int i = 0; i < NUM_STUDENTS; i++) 
    {
        student_ids[i] = i + 1;
        pthread_create(&students[i], NULL, student, (void *)&student_ids[i]);
    }

    // 等待线程结束
    pthread_join(ta_thread, NULL);
    for (int i = 0; i < NUM_STUDENTS; i++) 
    {
        pthread_join(students[i], NULL);
    }

    sem_destroy(&sem_students);
    sem_destroy(&sem_ta);

    pthread_mutex_destroy(&mutex);

    return 0;
}

