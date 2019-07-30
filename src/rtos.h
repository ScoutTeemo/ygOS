#ifndef RTOS_H
#define RTOS_H

#include <stdint.h>

//配置任务最大数量
#define TASK_NUM_MAX                      5

//空闲任务分配最低优先级
#define IDLE_TASK_PRIO                    31

//任务最高数量是32个
#define TASK_NUM_TOTAL_NUM                32

struct tcb_s
{  
	//栈地址
	uint32_t 		*stack_ptr;

    //指向任务列表的下一个
	struct tcb_s    *next;
	//执行任务列表的上一个
	struct tcb_s    *prev;  
    
	//任务优先级, 0是最高优先级
	int8_t 			prio;
    
	//任务休眠ticks
	uint32_t        sleep_tick;                 
};

//当前执行的任务
extern struct tcb_s *ygos_tcb_current;   

//最高优先级的就绪任务
extern struct tcb_s *ygos_tcb_high_ready;

//空闲的任务链表
extern struct tcb_s *ygos_tcb_free_list; 

//就就绪表的最高优先级
extern uint8_t ygos_prio_hig_ready;

//当前执行的就绪任务的优先级
extern uint8_t ygos_prio_current; 

//中断嵌套级别
extern volatile int ygos_interrupt_nest;

//指示os是否启动,启动后ysos_os_runing=1
extern uint8_t ygos_os_runing ;

//ygos_init调用后调用ygos_start启动
void ygos_start (void);

//ygos初始化
void ygos_init(void);

//创建任务
void ygos_tcb_create (int prio, void (*task)(void *p_arg), void *p_arg, uint32_t  *ptos);

//从就绪列表中找到最高优先级的就绪任务,在退出所有嵌套中断后就行任务切换
void ygos_sche (void);

//刷新就绪列表中最高优先级的任务
void ygos_sche_new (void);

//定时器周期查询就绪任务队列中的最高优先级是否发生变化
void ygos_timer_sche(void);

//当前任务从未就绪状态变为就绪状态，prio表示任务的优先级
void ygos_task_ready_add(int prio);

//当前任务从就绪状态变为未就绪状态，prio表示任务的优先级
void ygos_task_ready_delete(int prio);

//进入中断服务程序，此函数被调用，更新中断嵌套的级别
void ygos_interrupt_enter(void);

//中断退出，任务切换一次，和ygos_interrupt_enter进行配对使用
void ygos_interrupt_leave(void);

//全局中断关闭
void ygos_interrupt_disable(void);

//全局中断打开
void ygos_interrupt_enable(int level);

//当前任务休眠ticks个系统滴答
void ygos_sleep_tick (int32_t ticks);

//ygos获取系统tick
uint32_t ygos_get_tick(void);

#endif 