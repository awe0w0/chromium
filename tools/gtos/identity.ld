OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(_start)
PHDRS {
  code PT_LOAD FLAGS(5);
  identity PT_NOTE FLAGS(4);
  stack PT_GNU_STACK FLAGS(6);
}
SECTIONS {
  . = 0x40000000;
  .note.gtos : { KEEP(*(.note.gtos)) } :code :identity
  .text : { *(.text .text.*) *(.rodata .rodata.*) } :code
  ASSERT(SIZEOF(.text) <= 4096, "identity probe exceeds one RX page")
  /DISCARD/ : { *(.comment) *(.note*) *(.eh_frame*) }
}
