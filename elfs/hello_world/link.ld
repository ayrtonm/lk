OUTPUT_FORMAT("elf32-littlearm")
ENTRY(main)

MEMORY {
    USER_SPACE : ORIGIN = 16M, LENGTH = 1024M - 16M
}

SECTIONS {
    .text : {
        *(.text*)
    } > USER_SPACE
}
