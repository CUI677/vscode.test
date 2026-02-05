//阻塞队列的实现
#pragma once  // 错误：#pragam once（拼写错误）
#include <iostream>
#include <string>
#include <queue>  // 错误：缺少这个头文件，queue需要
#include <pthread.h>

template<typename T>
class BlockQueue
{
private:
    std::queue<T> _q;           // 临界资源
    int _cap;                   // 容量大小
    pthread_mutex_t _mutex;
    pthread_cond_t _full_cond;  // 满了就放在条件队列
    pthread_cond_t _empty_cond; // 消费完就在这里等
    
    bool IsFull()
    {
        return _q.size() >= _cap;
    }
    
    bool IsEmpty()  // 错误：原来没有实现函数体
    {
        return _q.empty();
    }
    
public:
    BlockQueue(int cap = 5) : _cap(cap)  // 错误：const int defaltcap=5 不能这样定义
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_full_cond, nullptr);
        pthread_cond_init(&_empty_cond, nullptr);
    }
    
    void Enqueue(const T &in)  // 原来是 Equeue（拼写）
    {
        // 生产者
        pthread_mutex_lock(&_mutex);
        while(IsFull())
        {
            // 等待
            pthread_cond_wait(&_full_cond, &_mutex);  // 错误：原来缺少分号
            // 重点2：线程被唤醒时，默认就在临界区唤醒，就要当前进程重新申请锁；
            // 这些过程都在pthread_cond_wait函数内部自己完成
            // 重点3：如果我被唤醒了，但是抢不到锁了，在锁上阻塞等待
        }
        _q.push(in);
        pthread_cond_signal(&_empty_cond);  // 错误：原来缺少这行！生产后要唤醒消费者
        pthread_mutex_unlock(&_mutex);
    }
    
    T pop()
    {
        // 消费者
        pthread_mutex_lock(&_mutex);
        while(IsEmpty())
        {
            pthread_cond_wait(&_empty_cond, &_mutex);
        }
        T data = _q.front();
        _q.pop();
        pthread_cond_signal(&_full_cond);  // 错误：原来缺少这行！消费后要唤醒生产者
        pthread_mutex_unlock(&_mutex);
        return data;
    }
    
    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);      // 错误：destory → destroy
        pthread_cond_destroy(&_empty_cond);  // 错误：destory → destroy
        pthread_cond_destroy(&_full_cond);   // 错误：destory → destroy
    }
};  // 错误：原来缺少分号
