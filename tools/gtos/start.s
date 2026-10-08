.section .text
.global _start
.extern platform_identity_main
_start:
    call platform_identity_main
    mov %eax, %ebx
    mov $0x4704, %eax
    int $0x80
    ud2
.section .note.gtos,"a",@note
.balign 4
.long 5
.long gtos_note_end - gtos_note_desc
.long 1
.asciz "GTOS"
.balign 4
gtos_note_desc:
.asciz "ABI1;platform_identity_probe;IS_LINUX=0"
gtos_note_end:
.balign 4
.section .note.GNU-stack,"",@progbits
