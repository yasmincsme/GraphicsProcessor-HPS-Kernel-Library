#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include "address_map_arm.h"

#define DATA_B 0x70
#define DATA_A 0x80

#define DEVICE_NAME "gpp_data_bus"
#define CLASS_NAME "gppdatabus_class"

/*Struct for the devices DATA A and DATA B*/
struct data_bus {
}

static int
device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operands device_fops = {
    .owner = THIS_MODULE, .read = device_read, .write = device_write, .open = device_open, .release = device_release};

void *LW_virtual;                      /*Lightweight bridge base address*/
volatile int *DATA_A_ptr, *DATA_B_ptr; /*Start pulse, Data A bus and Data B bus base address*/

/*Default function for initialization of the module*/
static int __init initialize_module(void) {
  printk(KERN_ALERT "Module Initialized with Success!");
  /*generate a virtual address for the FPGA lightweight bridge*/
  LW_virtual = ioremap_nocache(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);

  WRREG_ptr = LW_virtual + WRREG;   /*Virtual address for Start pulse port*/
  DATA_A_ptr = LW_virtual + DATA_A; /*Virtual address for Data A bus port*/
  DATA_B_ptr = LW_virtual + DATA_B; /*Virtual address for Data B bus port*/

  return 0;
}

/*Default function for cleanup and unload the module */
void __exit cleanup_module(void) {
  iounmap(LW_virtual);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Driver for the GPP Bus DATA_A and DATA_B");
MODULE_VERSION("0.1");