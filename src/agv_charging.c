#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM 5                // 哲学家数量
#define THINKING_TIME 1      // 思考时长
#define EATING_TIME 2        // 吃饭时长
#define MAX_EAT 10           // 每个哲学家最多吃几次就退出

long long total_wait[NUM] = {0};  // 总等待时间（微秒）
int eat_count[NUM] = {0};         // 吃饭次数

pthread_mutex_t ch[NUM];    // 筷子互斥锁数组
sem_t dining_room;          // 方案0：房间限制信号量
int scheme = 0;             // 选择死锁解决方案

// 哲学家线程执行函数
void *philosopher(void *arg)
{
    int id = *(int *)arg;
    int left = id;
    int right = (id + 1) % NUM;

    struct timespec ts_start, ts_end;

    while(1)
    {
        // 达到最大吃饭次数，退出循环
        if (eat_count[id] >= MAX_EAT)
            break;

        printf("Philosopher %d is thinking.\n", id);
        sleep(rand() % THINKING_TIME + 1);

        // 开始计时
        clock_gettime(CLOCK_MONOTONIC, &ts_start);

        // 方案0：限制就餐人数（最多4人同时申请筷子，破坏死锁循环等待）
        if(scheme == 0)
        {
            sem_wait(&dining_room);
            pthread_mutex_lock(&ch[left]);
            pthread_mutex_lock(&ch[right]);
        }
        // 方案1：尝试拿筷子，拿不到就全部释放重试（破坏占有等待）
        else if(scheme == 1)
        {
            pthread_mutex_lock(&ch[left]);
            if(pthread_mutex_trylock(&ch[right]) != 0)
            {
                pthread_mutex_unlock(&ch[left]);
                continue;
            }
        }
        // 方案2：奇偶哲学家不对称拿筷子（破坏循环等待）
        else if(scheme == 2)
        {
            if(id % 2 == 0)
            {
                pthread_mutex_lock(&ch[right]);
                pthread_mutex_lock(&ch[left]);
            }
            else
            {
                pthread_mutex_lock(&ch[left]);
                pthread_mutex_lock(&ch[right]);
            }
        }

        // 结束计时，计算等待时间
        clock_gettime(CLOCK_MONOTONIC, &ts_end);
        long long sec  = ts_end.tv_sec - ts_start.tv_sec;
        long long nsec = ts_end.tv_nsec - ts_start.tv_nsec;
        long long us   = sec * 1000000 + nsec / 1000;

        total_wait[id] += us;    // 累加总等待时间
        eat_count[id]++;         // 吃饭次数+1

        // 哲学家吃饭
        printf("Philosopher %d is eating. (eat times: %d)\n", id, eat_count[id]);
        sleep(rand() % EATING_TIME + 1);

        // 释放筷子资源
        pthread_mutex_unlock(&ch[left]);
        pthread_mutex_unlock(&ch[right]);

        // 方案0释放房间名额
        if(scheme == 0)
        {
            sem_post(&dining_room);
        }
    }

    // 线程结束，输出统计结果
    printf("\n[Philosopher %d Exit] Total wait: %lld us, Total eat: %d\n",
           id, total_wait[id], eat_count[id]);

    return NULL;
}

int main(int argc, char *argv[])
{
    // 参数合法性检查
    if(argc != 2)
    {
        printf("Usage:%s <scheme>\n", argv[0]);
        printf("Scheme: 0-Limit dining room, 1-Take both chopsticks, 2-Asymmetric\n");
        return 1;
    }

    srand((unsigned)time(NULL)); // 初始化随机数种子
    scheme = atoi(argv[1]);

    // 初始化房间信号量：最多 NUM-1 个哲学家同时进入
    sem_init(&dining_room, 0, NUM-1);

    // 初始化所有筷子互斥锁
    for(int i=0; i<NUM; i++)
    {
        pthread_mutex_init(&ch[i], NULL);
    }

    pthread_t philosophers[NUM];
    int ids[NUM];

    // 创建5个哲学家线程
    for(int i=0; i<NUM; i++)
    {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // 等待所有线程结束
    for(int i=0; i<NUM; i++)
    {
        pthread_join(philosophers[i], NULL);
    }

    // 主函数最后输出汇总数据
    printf("\n==================== Scheme %d Summary ====================\n", scheme);
    long long avg_wait_all = 0;
    int total_eat_all = 0;
    for(int i=0; i<NUM; i++){
        avg_wait_all += total_wait[i];
        total_eat_all += eat_count[i];
        printf("Philosopher %d | total_wait = %10lld us | eat_count = %2d\n",
               i, total_wait[i], eat_count[i]);
    }
    printf("-----------------------------------------------------------\n");
    printf("Total eat times: %d | Average wait per person: %.2f us\n",
            total_eat_all, (double)avg_wait_all / NUM);
    printf("===========================================================\n");

    // 销毁互斥锁资源
    for(int i=0; i<NUM; i++)
    {
        pthread_mutex_destroy(&ch[i]);
    }

    // 销毁信号量
    if(scheme == 0)
    {
        sem_destroy(&dining_room);
    }

    return 0;
}























