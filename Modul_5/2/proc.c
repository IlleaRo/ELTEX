#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define MSG_SIZE 10

static int len;
static int temp;
 
static char *msg;

static int proc_show(struct seq_file *m, void *v) {
  seq_printf(m, "Hello proc!\n");
  return 0;
}

static int proc_open(struct inode *inode, struct  file *file) {
  return single_open(file, proc_show, NULL);
}

ssize_t read_proc(struct file *filp,char *buf,size_t count,loff_t *offp )
{
    if(count>temp)
    {
        count=temp;
    }
    temp=temp-count;
    copy_to_user(buf,msg, count);
    if(count==0)
        temp=len;
 
    return count;
}

ssize_t write_proc(struct file *filp,const char *buf,size_t count,loff_t *offp)
{
    copy_from_user(msg,buf,count);
    len=count;
    temp=len;
    return count;
}

static const struct proc_ops proc_fops = {
  .proc_open = proc_open,
  .proc_read = read_proc,
  .proc_write = write_proc,
  .proc_lseek = seq_lseek,
  .proc_release = single_release
};

static int __init proc_init(void) {
  proc_create("hello", 0, NULL, &proc_fops);
  msg=kmalloc(MSG_SIZE*sizeof(char), GFP_KERNEL);
  return 0;
}

static void __exit proc_exit(void) {
  remove_proc_entry("hello", NULL);
  kfree(msg);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("IlleaRo");
MODULE_DESCRIPTION("A simple module for wirite and read via proc");

module_init(proc_init);
module_exit(proc_exit);