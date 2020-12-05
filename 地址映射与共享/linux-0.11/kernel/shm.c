#include<linux/mm.h>
#include <linux/sched.h>

#define MAX_SHM_TABLE 50

struct shm_record{
    int key; //共享内存的标识符
    long address; //get_free_page()返回的物理地址
    int opened; //标记是否正在使用
};

struct shm_record shm_table[MAX_SHM_TABLE];
int shm_top = 0;

int sys_shmget(int key){
    int i,empty = -1;

    for(i=0;i<shm_top;i++){
        if(shm_table[i].key == key){
            return i;
        }
        if(shm_table[i].opened == 0){ //寻找shm_top前的空位
            empty = i;
        }
    }
    if(empty == -1){ //shm_top前没有空位，用shm_top
        empty = shm_top;
    }
    shm_top++;
    shm_table[empty].key = key;
    shm_table[empty].address = get_free_page();
    shm_table[empty].opened = 1;
    return empty;
}

void * sys_shmat(int shm_id){
    if(shm_table[shm_id].opened != 1){
        return NULL;
    }
    put_page(shm_table[shm_id].address,current->brk + current->start_code); //共享内存线性地址=代码段位置+(代码段长度+数据段长度+bss段长度)
    return (void *)current->brk;
    return NULL;
}

int sys_shmdt(int shm_id){
    if(shm_table[shm_id].opened != 1){
        return 0;
    }
    shm_table[shm_id].opened = 0;
    return 1;
}

