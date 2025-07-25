# WCH RISC-V UF2 Bootloader

This code implements a [uf2](https://github.com/microsoft/uf2) bootloader for RISC-V microcontrollers from [WCH](https://www.wch-ic.com). This allows user-friendly uploading of code by dragging files onto a virtual USB Mass Storage device (i.e. a pretend thumb drive). This means that, unlike the vendor ISP bootloader, no special software is required.

## Usage notes

- Triggered by a double reset within 0.5 seconds (like Adafruit bootloaders)
- Needs usage of `R16_BKP_DATAR10`
    - Write 0x4170 ('Ap') to boot into the application immediately without delay
    - Write 0x624c ('bL') to unconditionally enter the bootloader
- Fits in $\leq$ 4096 bytes
- Tested and built with MounRiver GCC V1.91
    - Code size is improved through use of "XW" instructions, which are not upstream
- Supports USBD peripheral *only* (i.e. not USBFS)
    - USBD and USBFS are completely different, and the QFN28 package (which is available in largest quantities on LCSC) only bonds out USBD
    - Note that USBD requires a USB A-A cable if using the official devkit
- Runs off of internal 8 MHz oscillator (despite stability concerns)
    - The HSI oscillator's tolerance (-1.0% to +1.6%) is well outside of the range specified for USB full speed ($\pm$ 0.25%), but it seems to work in practice n=1 ¯\\\_(ツ)\_/¯
    - The vendor's ISP ROM bootloader also does this
- Should work across CH32V2xx family with appropriate changes to hardcoded constants. Does *not* support CH32V3xx (which doesn't have USBD).
- Allows both download to flash and to SRAM with the "not main flash" flag (this is how the RP2040 bootrom works)
    - Flash download address must be 08xxxxxx (i.e. not starting at 0)
    - SRAM download address must be 20xxxxxx
    - The entire size of the SRAM can be used, as USBD contains its own buffer memory independent of the main SRAM
- Auto-reboot on complete download, _but_ **only** works if the download is sufficiently small. Larger downloads will of course still flash, but they will not trigger auto-reboot and a manual reboot will be required.
- Lots of nasty code golfing tricks -- see comments in [bootloader.c](https://github.com/ArcaneNibble/wch-uf2/blob/main/bootloader.c)

## Examples

There are two included examples which build "blinky" applications (that blink pin PA0). The "RAM" example blinks at a different speed than the "flash" example so that you can tell that you've successfully loaded it.

The examples do *not* use a verbatim copy of the vendor startup bits -- some elements which we (IANAL) consider to be purely functional have been borrowed, but other things have been changed substantially:
- Vector table isn't hardcoded in the startup assembly file (as it doesn't need to be located at the beginning of flash, this isn't necessary)
- Preinit, init, and fini functions (`.init_array`, e.g. `__attribute__((constructor))` and (untested) C++ static constructors) are properly called
    - Legacy `.init` is *not* called
- Various initialization for CSRs, interrupt handling, etc. are not hardcoded in the startup assembly code. The intention is that this can be done with `__attribute__((constructor))`.

The example Makefiles now use the `UF2CONV` variable to locate `uf2conv.py`.
Set `UF2CONV` if the script is not in your `PATH`.
