.PHONY: all clean

RV_ARCH = rv32imacxw

CC = riscv-none-elf-gcc
OBJDUMP = riscv-none-elf-objdump
OBJCOPY = riscv-none-elf-objcopy
UF2CONV ?= uf2conv.py
CFLAGS = -ggdb3 -Os -march=$(RV_ARCH) -ffunction-sections -fdata-sections
LDFLAGS = -ggdb3 -march=$(RV_ARCH) -Wl,--gc-sections --specs=nosys.specs

all: main.uf2

main.elf: startup.o main.o

%.uf2: %.bin
        $(UF2CONV) -c -f 0x699b62ec -b 0x20000000 -o $@.temp $<
# This is a *terrible* hack
# (there isn't a flag for setting the "not main flash" bit)
	xxd $@.temp | sed 's/5546 320a 5751 5d9e 0020 0000/5546 320a 5751 5d9e 0120 0000/g' | xxd -r >$@
	rm $@.temp

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

%.elf: linker.lds
	$(CC) -Xlinker -Map=$(@:.elf=.map) -Wl,--script=linker.lds -nostartfiles -o $@ $(filter-out linker.lds,$+)
	$(OBJDUMP) -xdsS $@ >$(@:.elf=.dump)

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.elf *.map *.dump *.uf2
