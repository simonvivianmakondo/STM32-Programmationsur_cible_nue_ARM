OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
/* éviter d'inverser les éléments 2 et 3 pour pas inverser little endian et big endian :) */

OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY {
	RAM (rw) : ORIGIN = 0x20000000, LENGTH = 128K
 	FLASH (rx) : ORIGIN = 0x08000000 , LENGTH = 2M
}

SECTIONS {
    .text : {    
    	*(vectors)
    	*(boot)
        *(.text)
        . = ALIGN(4);
    } > FLASH

    .data : {
        *(.data)
        . = ALIGN(4);
    } > RAM AT> FLASH

	_sdata = ADDR(.data);
	_ldata = LOADADDR(.data);
	_lendata = SIZEOF(.data);

	bss_start = .;
    .bss : {
        *(.bss)
        . = ALIGN(4);
    } > RAM
    bss_end = .;

    .rodata : {
        *(.rodata)
        . = ALIGN(4);
    } > FLASH
}