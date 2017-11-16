BUILD_DIR=build
BOOTLOADER=$(BUILD_DIR)/bootloader/bootloader.o
KERNEL=$(BUILD_DIR)/kernel/kernel
FLOPPY_IMG=disk.img
DISK_IMG=disk.iso

all: bootdisk cdrom

.PHONY: bootdisk bootloader kernel

bootloader:
	make -C bootloader

kernel:
	make -C kernel

bootdisk: bootloader kernel
	dd if=/dev/zero of=$(FLOPPY_IMG) bs=512 count=2880
	dd conv=notrunc if=$(BOOTLOADER) of=$(FLOPPY_IMG) bs=512 count=1 seek=0
	dd conv=notrunc if=$(KERNEL) of=$(FLOPPY_IMG) bs=512 count=`du -k $(KERNEL)|cut -f1` seek=1

cdrom:
	cp $(FLOPPY_IMG) $(BUILD_DIR)/tmp
	mkisofs -o $(DISK_IMG) -V KERNEL -b $(FLOPPY_IMG) $(BUILD_DIR)/tmp

qemu-gdb:
	qemu-system-x86_64 -machine q35 -cdrom $(DISK_IMG) -gdb tcp::26000 -S
qemu:
	qemu-system-x86_64 -machine q35 -cdrom $(DISK_IMG)

clean:
	make -C bootloader clean
	make -C kernel clean
	rm $(BUILD_DIR)/tmp/*
