#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include "address_map_arm.h"

#define DEVICE_NAME "gpp_data_bus"
#define CLASS_NAME "gppdatabus_class"

/*Defining memory base addreses*/
// #define LW_VIRTUAL 0xFF200000
// #define DATA_A_OFFSET 0x70
// #define DATA_B_OFFSET 0x80
// #define DATA_A_PHYS_ADDR (LW_VIRTUAL + DATA_A_OFFSET)
// #define DATA_B_PHYS_ADDR (LW_VIRTUAL + DATA_B_OFFSET)

#define SUCCESS 0

/*Global variables and structs for the functions of the module*/
static int majorNumber;
static struct class *dataBusClass = NULL;
static struct device *dataBusDevice = NULL;

void __iomem *data_a_mem;
void __iomem *data_b_mem;

/*Declaring default functions and the file operations*/
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operands device_fops = {.owner = THIS_MODULE,
                                           .read = data_bus_read,
                                           .write = data_bus_write,
                                           .open = data_bus_open,
                                           .release = data_bus_release};
}



MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Driver for the GPP Bus DATA_A and DATA_B");
MODULE_VERSION("0.1");