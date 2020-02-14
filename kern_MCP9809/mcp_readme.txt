apt-get install linux-headers-$(uname -r)
(for raspbian:
	apt-get install raspberrypi-kernel-headers)

1) make all
2) modinfo mcp9809.ko
3) insmod mcp9809.ko
4) dmesg | tail
5) 
	{c: character device (b: block,p: FIFO pipe)}
	mknod /dev/mcp9809 c <major> <minor>
6) cat /dev/mcp9809
7) dmesg | tail
8) rmmod mcp9809
9) rm /dev/MCP9809