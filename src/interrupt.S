.globl InterruptRoutine

InterruptRoutine:
    addi sp, sp, -80
    sd t0, 0(sp)
    sd ra, 8(sp)
    sd a0, 16(sp)
    sd a1, 24(sp)
    sd a2, 32(sp)
    sd a3, 40(sp)
    sd a4, 48(sp)
    sd a5, 56(sp)
    sd a6, 64(sp)
    sd a7, 72(sp)
    csrr t0, sepc
    addi t0, t0, 4
    csrw sepc, t0
    call syscall
    ld t0, 0(sp)
    ld ra, 8(sp)
    ld a1, 24(sp)
    ld a2, 32(sp)
    ld a3, 40(sp)
    ld a4, 48(sp)
    ld a5, 56(sp)
    ld a6, 64(sp)
    ld a7, 72(sp)
    addi sp, sp, 80
    sret