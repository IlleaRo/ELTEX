/*
 * Function my_timer_func blinks the keyboard LEDs periodically by invoking
 * command KDSETLED of ioctl() on the keyboard driver. So you can control
 * mode of blink with working with file test (sys/kernel/sysdevcontrol/test)
 *
 * The argument to KDSETLED is alternatively set to 7 (thus causing the led
 * mode to be set to LED_SHOW_IOCTL, and all the leds are lit) and to 0xFF
 * (any value above 7 switches back the led mode to LED_SHOW_FLAGS, thus
 * the LEDs reflect the actual keyboard status). To learn more on this,
 * please see file:
 *     /usr/src/linux/drivers/char/keyboard.c, function setledstate().
 */


#include <linux/module.h>
#include <linux/configfs.h>
#include <linux/init.h>
#include <linux/tty.h>          /* For fg_console, MAX_NR_CONSOLES */
#include <linux/kd.h>           /* For KDSETLED */
#include <linux/vt.h>
#include <linux/console_struct.h>       /* For vc_cons */
#include <linux/vt_kern.h>
#include <linux/timer.h>
#include <linux/fs.h>
 
MODULE_AUTHOR("IlleaRo");
MODULE_DESCRIPTION("Example module illustrating the use of Keyboard LEDs.");
MODULE_LICENSE("GPL");

#define BLINK_DELAY   HZ/5
#define ALL_LEDS_ON   0x07
#define RESTORE_LEDS  0xFF

// static variables
static struct timer_list my_timer;
static struct tty_driver *my_driver;
static int _kbledstatus = 0;
static int test = 3; // cod that will be use to start
static struct kobject *kobject; // object for save start value

// Functions
static ssize_t foo_show(struct kobject*, struct kobj_attribute*,
						char*);
static ssize_t foo_store(struct kobject*, struct kobj_attribute*,
                      const char*,size_t);
static void my_timer_func(struct timer_list*);
static int __init kbleds_init(void);
static void __exit kbleds_cleanup(void);



static struct kobj_attribute foo_attribute =__ATTR(test, 0660, foo_show,
                                                   foo_store); // ATTR for file


static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
        return sprintf(buf, "%d\n", test);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
                      const char *buf, size_t count)
{
        sscanf(buf, "%du", &test);
        return count;
}

// attribute for file "test"


static void my_timer_func(struct timer_list *ptr)
{
        int *pstatus = &_kbledstatus;
        if (*pstatus == test)
                *pstatus = RESTORE_LEDS;
        else
                *pstatus = test;
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,
                            *pstatus);
        my_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&my_timer);
}

static int __init kbleds_init(void)
{
        int i;
        printk(KERN_INFO "kbleds: loading\n");
        printk(KERN_INFO "kbleds: fgconsole is %x\n", fg_console);

		kobject = kobject_create_and_add("sysdevcontrol",
                                                 kernel_kobj);

        for (i = 0; i < MAX_NR_CONSOLES; i++) {
                if (!vc_cons[i].d)
                        break;
                printk(KERN_INFO "poet_atkm: console[%i/%i] #%i, tty %lx\n", i,
                       MAX_NR_CONSOLES, vc_cons[i].d->vc_num,
                       (unsigned long)vc_cons[i].d->port.tty);
        }
        printk(KERN_INFO "kbleds: finished scanning consoles\n");
        my_driver = vc_cons[fg_console].d->port.tty->driver;

		int error;
		error = sysfs_create_file(kobject, &foo_attribute.attr);

        timer_setup(&my_timer, my_timer_func, 0);
        my_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&my_timer);
        return 0;
}

static void __exit kbleds_cleanup(void)
{
        printk(KERN_INFO "kbleds: unloading...\n");
        del_timer(&my_timer);
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,
                            RESTORE_LEDS);
		kobject_put(kobject);
}

module_init(kbleds_init);
module_exit(kbleds_cleanup);