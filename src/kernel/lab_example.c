#include <asm/io.h>
#include <linux/fs.h>          // struct file, struct file_operations
#include <linux/miscdevice.h>  // for misc_device_register and struct miscdev
#include <linux/module.h>      // for module init and exit macros

/* Kernel character device driver /dev/chardev. */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations chardev_fops = {
    .owner = THIS_MODULE, .read = device_read, .write = device_write, .open = device_open, .release = device_release};

#define SUCCESS 0
#define DEV_NAME "chardev"

static struct miscdevice chardev = {.minor = MISC_DYNAMIC_MINOR, .name = DEV_NAME, .fops = &chardev_fops, .mode = 0666};

static int chardev_registered = 0;

#define MAX_SIZE 256                // assume that no message longer than this will be used
static char chardev_msg[MAX_SIZE];  // the string that can be read or written

static int __init start_chardev(void) {
  int err = misc_register(&chardev);
  if (err < 0) {
    printk(KERN_ERR "/dev/%s: misc_register() failed\n", DEV_NAME);
  } else {
    printk(KERN_INFO "/dev/%s driver registered\n", DEV_NAME);
    chardev_registered = 1;
  }
  strcpy(chardev_msg, "Hello from chardev\n");

  return err;
}

static void __exit stop_chardev(void) {
  if (chardev_registered) {
    misc_deregister(&chardev);
    printk(KERN_INFO "/dev/%s driver de-registered\n", DEV_NAME);
  }
}

/* Called when a process opens chardev */
static int device_open(struct inode *inode, struct file *file) {
  return SUCCESS;
}

/* Called when a process closes chardev */
static int device_release(struct inode *inode, struct file *file) {
  return 0;
}

/* Called when a process reads from chardev. Provides character data from
 * chardev_msg. Returns, and sets *offset to, the number of bytes read. */
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset) {
  size_t bytes;
  bytes = strlen(chardev_msg) - (*offset);  // how many bytes not yet sent?
  bytes = bytes > length ? length : bytes;  // too much to send at once?

  if (bytes) (void)copy_to_user(buffer, &chardev_msg[*offset], bytes);
  *offset = bytes;  // keep track of number of bytes sent to the user
  return bytes;
}

/* Called when a process writes to chardev. Stores the data received into
 * chardev_msg, and returns the number of bytes stored. */
static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset) {
  size_t bytes;
  bytes = length;

  if (bytes > MAX_SIZE - 1)  // can copy all at once, or not?
    bytes = MAX_SIZE - 1;
  (void)copy_from_user(chardev_msg, buffer, bytes);
  chardev_msg[bytes] = ’\0’;  // NULL terminate
  // Note: we do NOT update *offset; we keep the last MAX_SIZE or fewer bytes
  return bytes;
}

MODULE_LICENSE("GPL");
module_init(start_chardev);
module_exit(stop_chardev);