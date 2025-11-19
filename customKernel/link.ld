OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

SECTIONS
{
    /* Load kernel at 1MB */
    . = 0x100000;

    .text : 
    { 
        *(.text) 
    }

    .data : 
    { 
        *(.data) 
    }

    .bss : 
    { 
        *(.bss)
        *(COMMON)
    }

    /* Symbol that marks the end of the entire kernel */
    _kernel_end = .;
}

