#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <linux/mutex.h>
#include <mach/hardware.h>
#include <mach/platform.h>
#include <mach/irqs.h>

#define DEVICE_NAME "mymodule"
#define NUMCHANNELS 3

static ssize_t device_read(struct file *file, char __user *buf, size_t length, loff_t *f_pos)
{
    int channel = (int)file->private_data;
    int outbuflen = 64;
    char outbuf[outbuflen];

    if (channel < 0 || channel >= NUMCHANNELS)
    {
        return -EFAULT;
    }


    if (*f_pos == 0)
    {
        unlock_mutex();

        int len;
        int conversionValue = 1010;
        len = snprintf(outbuf, outbuflen, "%d", conversionValue);
        if (copy_to_user(buf, outbuf, len))
        {
            return -EFAULT;
        }
        *f_pos += len;
        return len;
    }
    return 0;
}

static int device_open(struct inode *inode, struct file *file)
{
    // get channel from 'inode'
    file->private_data = (void *)MINOR(inode->i_rdev);
    try_module_get(THIS_MODULE);
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    //printk(KERN_WARNING DEVICE_NAME ": device_release()\n");
    module_put(THIS_MODULE);
    return 0;
}

static struct file_operations fops =
    {
        .owner = THIS_MODULE,
        .read = device_read,
        .open = device_open,
        .release = device_release};

static struct chardev
{
    dev_t dev;
    struct cdev cdev;
} mymoduledev;

int mymodule_init(void)
{
    // On embedded devices, consider using a predefined major nr
    // Try to get a dynamically allocated major number
    int error = alloc_chrdev_region(&mymoduledev.dev, 0, NUMCHANNELS, DEVICE_NAME);

    if (error < 0)
    {
        // failed to get major number for our device.
        printk(KERN_WARNING DEVICE_NAME ": dynamic allocation of major number failed, error=%d\n", error);
        return error;
    }

    //printk(KERN_INFO DEVICE_NAME ": major number=%d\n", MAJOR(mymoduledev.dev));

    cdev_init(&mymoduledev.cdev, &fops);
    mymoduledev.cdev.owner = THIS_MODULE;
    mymoduledev.cdev.ops = &fops;

    error = cdev_add(&mymoduledev.cdev, mymoduledev.dev, NUMCHANNELS);
    if (error < 0)
    {
        // failed to add our character device to the system
        printk(KERN_WARNING DEVICE_NAME ": unable to add device, error=%d\n", error);
        return error;
    }

    return 0;
}

void cleanup_module()
{
    cdev_del(&mymoduledev.cdev);
    unregister_chrdev_region(mymoduledev.dev, NUMCHANNELS);
}

module_init(mymodule_init);

MODULE_LICENSE("");
MODULE_AUTHOR("");
MODULE_DESCRIPTION("mymodule");
