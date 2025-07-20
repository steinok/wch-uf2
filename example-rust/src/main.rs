#![no_std]
#![no_main]

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

const RCC_APB2PCENR: *mut u32 = 0x40021018 as *mut u32;
const GPIOA_CFGLR: *mut u32 = 0x40010800 as *mut u32;
const GPIOA_BSHR: *mut u32 = 0x40010810 as *mut u32;

#[export_name = "main"]
pub extern "C" fn main() -> ! {
    unsafe {
        RCC_APB2PCENR.write_volatile(RCC_APB2PCENR.read_volatile() | 1 << 2);
        GPIOA_CFGLR.write_volatile((GPIOA_CFGLR.read_volatile() & !0xf) | 0b0010);
        loop {
            GPIOA_BSHR.write_volatile(1 << 0);
            delay();
            GPIOA_BSHR.write_volatile(1 << 16);
            delay();
        }
    }
}

#[inline(never)]
fn delay() {
    for _ in 0..250_000 {
        unsafe { core::arch::asm!("", options(nomem, nostack)) };
    }
}
