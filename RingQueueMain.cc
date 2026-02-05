#include "RingQueue.hpp"
#include <iostream>
#include <pthread.h>
#include <unistd.h>

RingQueue<int>* rq;

// 生产者线程
void* producer(void* arg) {
    int data = 1;
    while (true) {
        rq->Enqueue(data);
        std::cout << "生产: " << data << std::endl;
        data++;
        sleep(1);  // 模拟生产耗时
    }
    return nullptr;
}

// 消费者线程
void* consumer(void* arg) {
    while (true) {
        int data;
        rq->Dequeue(&data);
        std::cout << "消费: " << data << std::endl;
        sleep(2);  // 模拟消费耗时（比生产慢）
    }
    return nullptr;
}

int main() {
    // 创建容量为5的环形队列
    rq = new RingQueue<int>(5);
    
    // 创建生产者和消费者线程
    pthread_t p, c;
    pthread_create(&p, nullptr, producer, nullptr);
    pthread_create(&c, nullptr, consumer, nullptr);
    
    // 等待线程结束
    pthread_join(p, nullptr);
    pthread_join(c, nullptr);
    
    delete rq;
    return 0;
}
