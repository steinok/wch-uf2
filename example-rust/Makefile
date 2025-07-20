.PHONY: all clean

RV_ARCH = rv32imacxw
TARGET = riscv32imac-unknown-none-elf

CC = riscv-none-elf-gcc
OBJDUMP = riscv-none-elf-objdump
OBJCOPY = riscv-none-elf-objcopy
UF2CONV ?= uf2conv.py

all: rust-blinky.uf2

rust-blinky.elf: startup.o Cargo.toml src/main.rs linker.lds
	cargo build --release --target $(TARGET)
	cp target/$(TARGET)/release/rust-blinky $@
	$(OBJDUMP) -xdsS $@ >$(@:.elf=.dump)

startup.o: startup.S
	$(CC) -march=$(RV_ARCH) -c -o $@ $<

rust-blinky.bin: rust-blinky.elf
	$(OBJCOPY) -O binary $< $@

rust-blinky.uf2: rust-blinky.bin
	$(UF2CONV) -c -f 0x699b62ec -b 0x08001000 -o $@ $<

clean:
	rm -rf target *.o *.elf *.bin *.uf2 *.dump
