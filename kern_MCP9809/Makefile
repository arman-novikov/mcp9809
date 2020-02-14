# это список модулей ядра для сборки. .o будет автоматически
# собран из соответствующего .c файла (нет необходимости указывать
# список исходных файлов).
obj-m += mcp9809.o # the same named C-file is expected

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
