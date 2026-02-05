#pragma once
#include <vector>
#include <semaphore.h>

template<typename T>
class RingQueue {
private:
    std::vector<T> _ring;      // 环形队列（用vector模拟）
    int _cap;                  // 队列容量
    sem_t _blank_sem;          // 空位信号量（初始值=容量）
    sem_t _data_sem;           // 数据信号量（初始值=0）
    int _p_step;               // 生产者位置（写入位置）
    int _c_step;               // 消费者位置（读取位置）

public:
    RingQueue(int cap = 5) 
        : _ring(cap), _cap(cap), _p_step(0), _c_step(0) 
    {
        // 初始化信号量
        sem_init(&_blank_sem, 0, cap);  // 空位数量 = 容量
        sem_init(&_data_sem, 0, 0);     // 数据数量 = 0
    }
    
    ~RingQueue() {
        sem_destroy(&_blank_sem);
        sem_destroy(&_data_sem);
    }
    
    // 生产者：入队
    void Enqueue(const T& in) {
        // 1. 申请空位信号量（P操作）
        //    如果没有空位（blank_sem=0），生产者阻塞等待
        sem_wait(&_blank_sem);  // 空位 -1
        
        // 2. 写入数据到环形队列
        _ring[_p_step] = in;
        
        // 3. 移动生产者位置（环形）
        _p_step = (_p_step + 1) % _cap;
        
        // 4. 发布数据信号量（V操作）
        //    通知消费者：有新数据了
        sem_post(&_data_sem);   // 数据 +1
    }
    
    // 消费者：出队
    void Dequeue(T* out) {
        // 1. 申请数据信号量（P操作）
        //    如果没有数据（data_sem=0），消费者阻塞等待
        sem_wait(&_data_sem);   // 数据 -1
        
        // 2. 从环形队列读取数据
        *out = _ring[_c_step];
        
        // 3. 移动消费者位置（环形）
        _c_step = (_c_step + 1) % _cap;
        
        // 4. 发布空位信号量（V操作）
        //    通知生产者：有空位了
        sem_post(&_blank_sem);  // 空位 +1
    }
};
