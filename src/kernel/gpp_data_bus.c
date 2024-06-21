```c
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define DEVICE_NAME "gpp_data_bus"    /**< Name of the device */
#define CLASS_NAME "gppdatabus_class" /**< Name of the device class */

/*Defining memory base addresses*/
#define LW_VIRTUAL 0xFF200000 /**< Base address of virtual memory */
#define DATA_A_OFFSET 0x70    /**< Offset for Data A */
#define DATA_B_OFFSET 0x80    /**< Offset for Data B */
#define WRREG_OFFSET 0xc0     /**< Offset for Write Register */
#define WRFULL_OFFSET 0xb0    /**< Offset for Write Full */

#define DATA_A_PHYS_ADDR (LW_VIRTUAL + DATA_A_OFFSET) /**< Physical address of Data A */
#define DATA_B_PHYS_ADDR (LW_VIRTUAL + DATA_B_OFFSET) /**< Physical address of Data B */
#define WREG_PHYS_ADDR (LW_VIRTUAL + WRREG_OFFSET)    /**< Physical address of Write Register */
#define WRFULL_PHYS_ADDR (LW_VIRTUAL + WRFULL_OFFSET) /**< Physical address of Write Full */

#define SUCCESS 0 /**< Success return value */

    /* Global variables and structs for the functions of the module */
    static int majorNumber;                 /**< Major number of the device */
static struct class *dataBusClass = NULL;   /**< Device class struct */
static struct device *dataBusDevice = NULL; /**< Device struct */

void __iomem *data_a_mem; /**< Memory mapped pointer for Data A */
void __iomem *data_b_mem; /**< Memory mapped pointer for Data B */
void __iomem *wreg_mem;   /**< Memory mapped pointer for Write Register */
void __iomem *wrfull_mem; /**< Memory mapped pointer for Write Full */

/* Declaring default functions and the file operations */
static ssize_t data_bus_read(struct file *, char *, size_t, loff_t *);
static ssize_t data_bus_write(struct file *, const char *, size_t, loff_t *);
static int data_bus_open(struct inode *, struct file *);
static int data_bus_release(struct inode *, struct file *);

static struct file_operations fops = {.owner = THIS_MODULE,
                                      .read = data_bus_read,
                                      .write = data_bus_write,
                                      .open = data_bus_open,
                                      .release = data_bus_release};

/**
 * @brief Initializes the DataBus module.
 *
 * This function registers the character device, creates a device class, and initializes memory mapping.
 *
 * @return 0 on success, negative errno on failure.
 */
static int __init data_bus_init(void) {
  printk(KERN_INFO "DataBus: Initializing the DataBus\n");

  majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
  if (majorNumber < 0) {
    printk(KERN_ALERT "DataBus failed to register a major number\n");
    return majorNumber;
  }
  printk(KERN_INFO "DataBus: registered correctly with major number %d\n", majorNumber);

  dataBusClass = class_create(THIS_MODULE, CLASS_NAME);
  if (IS_ERR(dataBusClass)) {
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_ALERT "Failed to register device class\n");
    return PTR_ERR(dataBusClass);
  }
  printk(KERN_INFO "DataBus: device class registered correctly\n");

  dataBusDevice = device_create(dataBusClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
  if (IS_ERR(dataBusDevice)) {
    class_destroy(dataBusClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_ALERT "Failed to create the device\n");
    return PTR_ERR(dataBusDevice);
  }
  printk(KERN_INFO "DataBus: device class created correctly\n");

  /* Mapping memory address for Data A, Data B, Write Register, and Write Full */
  data_a_mem = ioremap(DATA_A_PHYS_ADDR, sizeof(uint32_t));
  if (!data_a_mem) {
    printk(KERN_ALERT "Failed to map memory for DATA_A\n");
    iounmap(data_a_mem);
    device_destroy(dataBusClass, MKDEV(majorNumber, 0));
    class_destroy(dataBusClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    return -EIO;
  }

  data_b_mem = ioremap(DATA_B_PHYS_ADDR, sizeof(uint32_t));
  if (!data_b_mem) {
    printk(KERN_ALERT "Failed to map memory for DATA_B\n");
    iounmap(data_b_mem);
    device_destroy(dataBusClass, MKDEV(majorNumber, 0));
    class_destroy(dataBusClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    return -EIO;
  }

  wreg_mem = ioremap(WREG_PHYS_ADDR, sizeof(uint32_t));
  if (!wreg_mem) {
    printk(KERN_ALERT "Failed to map memory for WREG\n");
    iounmap(wreg_mem);
    device_destroy(dataBusClass, MKDEV(majorNumber, 0));
    class_destroy(dataBusClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    return -EIO;
  }

  wrfull_mem = ioremap(WRFULL_PHYS_ADDR, sizeof(uint32_t));
  if (!wrfull_mem) {
    printk(KERN_ALERT "Failed to map memory for WRFULL\n");
    iounmap(wrfull_mem);
    device_destroy(dataBusClass, MKDEV(majorNumber, 0));
    class_destroy(dataBusClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    return -EIO;
  }

  return 0;
}

/**
 * @brief Cleans up resources used by the DataBus module.
 *
 * This function unmaps memory and destroys the device and device class.
 */
static void __exit data_bus_exit(void) {
  iounmap(data_a_mem);
  iounmap(data_b_mem);
  iounmap(wreg_mem);
  iounmap(wrfull_mem);
  device_destroy(dataBusClass, MKDEV(majorNumber, 0));
  class_destroy(dataBusClass);
  unregister_chrdev(majorNumber, DEVICE_NAME);
  printk(KERN_INFO "DataBus Finalized!\n");
}

/**
 * @brief Reads data from the device.
 *
 * Reads data from Data A and Data B registers, combines them, and copies to user space.
 *
 * @param filep Pointer to the file structure.
 * @param buffer User space buffer to store data.
 * @param len Length of the buffer.
 * @param offset Offset in the file.
 * @return Number of bytes read on success, negative errno on failure.
 */
static ssize_t data_bus_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
  uint32_t data_a;
  uint32_t data_b;
  uint64_t data;

  data_a = ioread32(data_a_mem);
  data_b = ioread32(data_b_mem);

  data = ((uint64_t)data_b << 32) | data_a;

  if (copy_to_user(buffer, &data, sizeof(data))) {
    return -EFAULT;
  }

  return sizeof(data);
}

/**
 * @brief Writes data to the device.
 *
 * Writes data to Data A and Data B registers from user space buffer.
 *
 * @param filep Pointer to the file structure.
 * @param buffer User space buffer containing data to write.
 * @param len Length of the buffer.
 * @param offset Offset in the file.
 * @return Number of bytes written on success, negative errno on failure.
 */
static ssize_t data_bus_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
  uint64_t data;
  uint32_t data_a;
  uint32_t data_b;

  uint32_t instruction_buffer;
  instruction_buffer = ioread32(wrfull_mem);

  while (instruction_buffer) {
    msleep(10);
    instruction_buffer = ioread32(wrfull_mem);
    if (instruction_buffer == 0) {
      break;
    }
  }

  uint32_t start = 0x00000000;
  iowrite32(start, wreg_mem);

  if (copy_from_user(&data, buffer, sizeof(data))) {
    return -EFAULT;
  }

  data_a = (uint32_t)(data & 0xFFFFFFFF);
  data_b = (uint32_t)(data >> 32);

  iowrite32(data_a, data_a_mem);
  iowrite32(data_b, data_b_mem);

  start = 0x00000001;
  iowrite32(start, wreg_mem);

  return sizeof(data);
}

/**
 * @brief Opens the device file.
 *
 * Logs when the device file is opened.
 *
 * @param inode Pointer to the inode structure.
 * @param file Pointer to the file structure.
 * @return SUCCESS (0).
 */
static int data_bus_open(struct inode *inode, struct file *file) {
  printk(KERN_INFO "File opened!\n");
  return SUCCESS;
}

/**
 * @brief Releases the device file.
 *
 * @param inode Pointer to the inode structure.
 * @param file Pointer to the file structure.
 * @return SUCCESS (0).
 */
static int data_bus_release(struct inode *inode, struct file *file) {
  return SUCCESS;
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Driver for the GPP Bus DATA_A and DATA_B");
