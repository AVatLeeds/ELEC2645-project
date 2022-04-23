ENTRY(reset_handler) /* populates the ELF file header with the entry point information. Not strictly necessary but required for debugging with GDB */

MEMORY
{
    FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 1024K
    SRAM1(rwx) : ORIGIN = 0x20000000, LENGTH = 96K
    SRAM2(rwx) : ORIGIN = 0x10000000, LENGTH = 32K   /* SRAM2 not presently used */
}

SECTIONS
{
    .text :
    {
        start_of_text_section = .;  /* start of text section symbol with current address of location counter */
        startup_stm32l476rg.o(.vector_table)
        *(.text)                    /* combine text sections from all object files */
        *(.rodata)                  /* combine read only sections from all object files */
        . = ALIGN(4);               /* ensure the location counter is aligned to a word boundary at end of the section */
        end_of_text_section = .;    /* end of text section symbol with current address of location counter */
    } > FLASH                       /* VMA and LMA are both relative to FLASH origin */

    .init_array :
    {
        start_of_init_array = .;
        *(.init_array)
        . = ALIGN(4);
        end_of_init_array = .;
    } > FLASH

    .data :
    {
        start_of_data_section = .;  /* start of data section symbol with current address of location counter */
        *(.data)                    /* combine data sections from all object files */
        . = ALIGN(4);               /* ensure the location counter is aligned to a word boundary at end of the section */
        end_of_data_section = .;    /* end of data section symbol with current address of location counter */
    } > SRAM1 AT > FLASH            /* LMA is flash. VMA are relative to SRAM1 origin */

    .bss :
    {
        start_of_bss_section = .;   /* start of bss section symbol with current address of location counter */
        *(.bss)                     /* combine bss sections from all object files */
        *(.COMMON)
        . = ALIGN(4);               /* ensure the location counter is aligned to a word boundary at end of the section */
        end_of_bss_section = .;     /* end of bss section symbol with current address of location counter */
    } > SRAM1                       /* VMA are relative to SRAM1 origin. no LMA as not output in final binary */
}