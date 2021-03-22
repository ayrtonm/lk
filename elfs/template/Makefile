CC=arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-a15 -mthumb -mfloat-abi=soft -std=c99 -nostartfiles -nostdlib
BUILD_PATH=build-qemu-virt-arm32-test
LDFLAGS=-Tlink.ld -Wl,-R -Wl,../../$(BUILD_PATH)/lk.elf
OBJ=template.o

template.elf: template.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm template.elf
