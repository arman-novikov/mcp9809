#include <linux/kernel.h> 	// printk()
#include <linux/module.h> 	// get module's metainfo
#include <linux/init.h>		// 
#include <linux/fs.h>		// struct inode, struct file
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <asm/uaccess.h>	// put_user (contains several includes and defines)

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "armnov" );
MODULE_DESCRIPTION( "test module for MCP9809" );
MODULE_SUPPORTED_DEVICE( "MCP9809" );

#define SUCCESS (0)
#define DEVICE_NAME "MCP9809"

// sort of a device's descriptor 
static int major_number;	// value is to be requested from kernel
static int MCP9809_open_counter = 0;
static const char text[5] = "text\n"; // can be read with cmd "cat /dev/..."
static const char* text_ptr = text;

static int MCP9809_open(struct inode*, struct file*);
static int MCP9809_release(struct inode*, struct file*);
static ssize_t MCP9809_read(struct file*, char*, size_t, loff_t*);
static ssize_t MCP9809_write(struct file*, const char*, size_t, loff_t*);
static struct file_operations fops = {
	.open = MCP9809_open,
	.release = MCP9809_release,
	.read = MCP9809_read,
	.write = MCP9809_write,	
};

// i2c
struct i2c_adapter* mcp9809_adap;

/*
<name>.ko
insmod – добавить модуль в ядро
rmmod – соответственно, удалить
lsmod – вывести список текущих модулей
modinfo – вывести информацию о модуле
apt-get install linux-headers-2.6.26-2-686
*/
static int __init MCP9809_init(void)
{
	printk(KERN_ALERT "MCP9809 driver loaded\n");
	major_number = register_chrdev(0/*as we have no major_number*/, DEVICE_NAME, &fops);

	if (major_number < 0) {
		printk("register_chrdev of MCP9809 device failed with %d\n", major_number);
		return major_number;
	}
	
	//12c
	mcp9809_adap = i2c_get_adapter(1); // 1 means i2c-1 bus	

	printk("MCP9809 module loaded\n");
	printk("create a dev file with \'mknod /dev/MCP9809 c %d 0\'\n", major_number);
	return SUCCESS;

}

static void __exit MCP9809_exit(void)
{
	unregister_chrdev(major_number, DEVICE_NAME);
	//i2c_del_adapter(mcp9809_adap);
	printk(KERN_ALERT "MCP9809 module is unloaded");
}

module_init(MCP9809_init);
module_exit(MCP9809_exit);

/* 
 * called when a process tries to access to open device's file
 * for example with cmd" cat /dev/chardev"
 */
static int MCP9809_open(struct inode* inode, struct file* file)
{	
	text_ptr = text;
	if (MCP9809_open_counter)
		return -EBUSY;

	++MCP9809_open_counter;
	return SUCCESS;
}

/*
* called when a proccess closes the file
*/
static int MCP9809_release(struct inode* inode, struct file* file)
{
	--MCP9809_open_counter;
	return SUCCESS;
}

/*
* called when a process tries to read an already opened device's file
*/
static ssize_t MCP9809_read(struct file* filp, char* buffer, size_t length, loff_t* loff) 
// loff_t is a pointer to long long "long offset"
{
	int byte_read = 0;
	uint16_t i2c_read_res;
	struct i2c_client* mcp9809_client = NULL;
	
	if (*text_ptr == '\0')
		return 0;

	while ( length && *text_ptr != '\0') {
		put_user(*(text_ptr++), buffer++);
		--length;
		++byte_read;
	}
	// i2c
	mcp9809_client = i2c_new_dummy(mcp9809_adap, 0x18); // 0x18 - slave address on i2c bus
	i2c_read_res = i2c_smbus_read_word_data(mcp9809_client, 0x05); // 0x05 reg addr to read from
	i2c_unregister_device(mcp9809_client);
printk("i2c_read_res: %u\n", i2c_read_res);
	return byte_read;
}

/*
* called when a process tries to write into device's file
* like: echo "msg" > /dev/...
*/
static ssize_t MCP9809_write(struct file* filp, const char* msg, size_t length, loff_t* loff)
{
	printk("msg: %s\n", msg);
	return -EINVAL;
}

