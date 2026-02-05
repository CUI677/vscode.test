#include "BlockQueue.hpp"
#include <iostream>
#include <pthread.h>
#include <ctime>
#include <unistd.h>

void* consumer(void* args)
{
    BlockQueue<int>* bq = static_cast<BlockQueue<int>*>(args);
    while(true)
    {
        int data = bq->pop();
        std::cout << "消费了一个数据: " << data << std::endl;
    }
    return nullptr;  // 错误：原来缺少 return
}

void* productor(void* args)  // 错误：原来是 void*productor() 缺少参数
{
    BlockQueue<int>* bq = static_cast<BlockQueue<int>*>(args);
    int data = 1;
    while(true)
    {
        bq->Enqueue(data);  // 错误：原来是 Equeue（拼写）
        std::cout << "生产了一个数据: " << data << std::endl;
        data++;  // 错误：原来是 int data=10; 重复定义且没有递增
        sleep(1);
    }
    return nullptr;  // 错误：原来缺少 return
}

int main()
{
    // 申请阻塞队列
    BlockQueue<int>* bq = new BlockQueue<int>();
    
    // 构建生产者和消费者
    pthread_t c, p;
    pthread_create(&c, nullptr, consumer, bq);  // 错误：pthread_creat → pthread_create
    pthread_create(&p, nullptr, productor, bq); // 错误：pthread_creat → pthread_create

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);
    
    delete bq;  // 错误：原来缺少释放内存
    return 0;
}
