OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
/* éviter d'inverser les éléments 2 et 3 pour pas inverser little endian et big endian :) */

OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS {
    . = 0x20000000;
    
    .text : {    	
    	*(boot)
        *(.text)
        . = ALIGN(4);
    }

    .data : {
        *(.data)
        . = ALIGN(4);
    }

	bss_start = .;
    .bss : {
        *(.bss)
        . = ALIGN(4);
    }
    bss_end = .;

    .rodata : {
        *(.rodata)
        . = ALIGN(4);
    }
}