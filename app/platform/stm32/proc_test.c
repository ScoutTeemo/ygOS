
#include <rtos.h>
#include "systick.h"
#include "ramlog.h"
#include <ygos/libc/stdio.h>
#include <ygos/fs/fs.h>
#include "cmd.h"

#define TASK1_STATCK_SIZE  128 

//任务1
uint32_t  task1_stack[TASK1_STATCK_SIZE];

//任务
int task1_flag = 0;
sem_t sem_mavlink;
char uptime_buf[24] = {0x00};
char uptime_buf_len = 0x00;

//任务1入口函数
void task1_entry(void *arg)
{   
	int ret = 0;
	int fd = 0;
   
    //挂载proc文件系统
	ret = mount(NULL, "/proc", "procfs", 0, NULL);
	if (ret == 0) {
		//获取系统时钟
		cat("/proc/uptime");
	}
	
	echo("log", "/dev/ramlog");
	cat("/dev/ramlog");

	while(1){
		task1_flag =1 ;
		//等待时间是1000ms
		ygos_sleep_tick(100);
		task1_flag =0 ;
	}
}

int main(int argc, char **argv)
{   
	//系统时钟初始化，在系统中断服务程序中任务调度    
	systick_init();
   
    //注册驱动	
	ygos_inode_list_init();
	ramlog_register(); 
	
	//操作系统初始化，为TCB分配内存空间，同时启动空闲任务
	ygos_init();

    //创建用户任务
	ygos_tcb_create(0, task1_entry, (void*)0, &task1_stack[TASK1_STATCK_SIZE]);
	
	//操作系统启动，会触发任务调度到最高优先级的任务
	ygos_start();
	
	//ygos启动失败,会执行下面
	while(1) {
		//return -1;
	}
}

