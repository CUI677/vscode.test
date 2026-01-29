#include <iostream>
#include <cstring>
#include <unistd.h>
#include "shm_common.h"

int main() {
    // 1. 生成 key（两个进程必须用相同的 key）
    key_t key = GetShmKey();
    if (key == -1) {
        return 1;
    }
    std::cout << "Server: key = " << key << std::endl;
    
    // 2. 创建共享内存
    // IPC_CREAT: 如果不存在则创建
    // 0666: 权限（rw-rw-rw-）
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        return 1;
    }
    std::cout << "Server: shmid = " << shmid << std::endl;
    
    // 3. 附加共享内存到进程地址空间
    // NULL: 让系统自动选择地址
    // 0: 读写模式
    char* addr = (char*)shmat(shmid, NULL, 0);
    if (addr == (char*)-1) {
        perror("shmat failed");
        return 1;
    }
    std::cout << "Server: attached at " << (void*)addr << std::endl;
    
    // 4. 写入数据
    const char* msg = "Hello from server!";
    strcpy(addr, msg);
    std::cout << "Server: wrote: " << msg << std::endl;
    
    // 5. 等待 client 读取（实际应用中用信号量同步）
    std::cout << "Server: waiting for client to read..." << std::endl;
    sleep(5);
    
    // 6. 分离共享内存
    if (shmdt(addr) < 0) {
        perror("shmdt failed");
    }
    
    // 7. 删除共享内存
    // IPC_RMID: 标记删除（当所有进程都 detach 后真正删除）
    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        perror("shmctl failed");
    }
    std::cout << "Server: shared memory deleted" << std::endl;
    
    return 0;
}
