#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <linux/stddef.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

#include "ascii.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mansurov Vladislav");

typedef struct
{
    struct work_struct work;
    int code;
} my_work_struct_t;

static struct workqueue_struct *my_wq;

static my_work_struct_t *work1;
static struct work_struct *work2;

int keyboard_irq = 1;

void work1_func(struct work_struct *work)
{
    my_work_struct_t *my_work = (my_work_struct_t *)work;
    int code = my_work->code;

    printk(KERN_INFO "MyWorkQueue: work1 begin");

    if (code < 84)
        printk(KERN_INFO "MyWorkQueue: the key is %s", ascii[code]);

    printk(KERN_INFO "MyWorkQueue: work1 end");
}

void work2_func(struct work_struct *work)
{
    printk(KERN_INFO "MyWorkQueue: work2 sleep begin");
    msleep(10);
    printk(KERN_INFO "MyWorkQueue: work2 sleep end");
}

irqreturn_t my_irq_handler(int irq, void *dev)
{
    int code;
    printk(KERN_INFO "MyWorkQueue: my_irq_handler");

    if (irq == keyboard_irq)
    {
        printk(KERN_INFO "MyWorkQueue: called by keyboard_irq");

        code = inb(0x60);
        work1->code = code;
        printk(KERN_INFO "MyWorkQueue: key code is %d", code);

        queue_work(my_wq, (struct work_struct *)work1);
        queue_work(my_wq, work2);

        return IRQ_HANDLED;
    }

    printk(KERN_INFO "MyWorkQueue: called not by keyboard_irq");

    return IRQ_NONE;
}

static int __init my_workqueue_init(void)
{
    int ret;
    

    ret = request_irq(keyboard_irq, my_irq_handler, IRQF_SHARED,
                      "test_my_irq_handler", (void *) my_irq_handler);

    printk(KERN_INFO "MyWorkQueue: init");
    if (ret)
    {
        printk(KERN_ERR "MyWorkQueue: request_irq error");
        return ret;
    }
    else
    {
        my_wq = alloc_workqueue("%s", __WQ_LEGACY | WQ_MEM_RECLAIM, 1, "my_wq");

        if (my_wq == NULL)
        {
            printk(KERN_ERR "MyWorkQueue: create queue error");
            ret = GFP_NOIO;
            return ret;
        }

        work1 = kmalloc(sizeof(my_work_struct_t), GFP_KERNEL);
        if (work1 == NULL)
        {
            printk(KERN_ERR "MyWorkQueue: work1 alloc error");
            destroy_workqueue(my_wq);
            ret = GFP_NOIO;
            return ret;
        }

        work2 = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
        if (work2 == NULL)
        {
            printk(KERN_ERR "MyWorkQueue: work2 alloc error");
            destroy_workqueue(my_wq);
            kfree(work1);
            ret = GFP_NOIO;
            return ret;
        }

        INIT_WORK((struct work_struct *)work1, work1_func);
        INIT_WORK(work2, work2_func);
        printk(KERN_ERR "MyWorkQueue: loaded");
    }
    return ret;
}

static void __exit my_workqueue_exit(void)
{
    printk(KERN_INFO "MyWorkQueue: exit");

    synchronize_irq(keyboard_irq); // ожидание завершения обработчика
    free_irq(keyboard_irq, my_irq_handler); // освобождение линни от обработчика

    flush_workqueue(my_wq);
    destroy_workqueue(my_wq);
    kfree(work1);
    kfree(work2);
    
    printk(KERN_INFO "MyWorkQueue: unloaded");
}

module_init(my_workqueue_init);
module_exit(my_workqueue_exit);

//IRQ - interrupt request
//PIC - programable interrupe controller
//Какие действия и в каких местах кода, нужно выполнить, чтобы запланировать выполнение ворков
//1. зарегистрировать прерывание с функцией request_irq
//2. создать очередь, тип work_queue_struct
//3. инициализировать работы-ворки, динамически, то есть во время выполнения программы
//4. В хендлере получаем код клавиши и с помощью queue_work 
//5. Их выполнит поток ядра, системный поток kworker
// Какая система была на лекции?
// Сетевая

// Покажите, что ваш ворк кью имеет очередь, где она?
// Загружаем модуль с очередью, далее
// $ ps -jax | grep kworker 

// Что делаете в работах work queue?
// В одной - блокируем.
// В другой - выводим имя клавиши

// Кто выполняет softirq?
// Eго выполняет в контексте прерывания демон ksoftirqd

// Какие действия и в каких местах кода, нужно выполнить, чтобы запланировать выполнение ворков? 
// Аллоцируется очередь работ, структура workqueue_struct
// Инициализируются ворки - используется макрос INIT_WORK(work_struct)
// Ворки добавляются в очередь с помощью функции queue_work(workqueue_struct, work_struct)

// Что за 1 в /proc/interrupts?
// Это номер линии в обработчике прерываний. 1 соответствует клавиатуре PS/2.

// Зачем нужен dev_id в request_irq?
// данные по указателю dev требуются для удаления устройства

// Как мы регистрируем наши тасклеты и ворки? Статически или динамически? В чем разница статической и динамической регистрации?
// Динамически.
// статически - во время инициализации
// динамически - во время выполнения программы.

// Где посмотреть системные интеррапты с proc?
// cat /proc/interrupts

// В какую очередь добавляем ворк?
// В свою, которую аллоцировали
