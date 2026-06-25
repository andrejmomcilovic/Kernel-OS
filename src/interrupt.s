# 1 "src/interrupt.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/riscv64-linux-gnu/include/stdc-predef.h" 1 3
# 32 "<command-line>" 2
# 1 "src/interrupt.S"
.globl InterruptRoutine

InterruptRoutine:
    addi sp, sp, -240
    sd t0, 0(sp)
    csrr t0, sepc
    sd t0, 224(sp)
    sd ra, 8(sp)
    sd a0, 16(sp)
    sd a1, 24(sp)
    sd a2, 32(sp)
    sd a3, 40(sp)
    sd a4, 48(sp)
    sd a5, 56(sp)
    sd a6, 64(sp)
    sd a7, 72(sp)
    sd s0, 80(sp)
    sd s1, 88(sp)
    sd s2, 96(sp)
    sd s3, 104(sp)
    sd s4, 112(sp)
    sd s5, 120(sp)
    sd s6, 128(sp)
    sd s7, 136(sp)
    sd s8, 144(sp)
    sd s9, 152(sp)
    sd s10, 160(sp)
    sd s11, 168(sp)
    sd t1, 176(sp)
    sd t2, 184(sp)
    sd t3, 192(sp)
    sd t4, 200(sp)
    sd t5, 208(sp)
    sd t6, 216(sp)
    call interruptHandler
    bnez a0, label
    ld t0, 224(sp)
    addi t0, t0, 4
    sd t0, 224(sp)
    ld a0, 16(sp)
    ld a1, 24(sp)
    ld a2, 32(sp)
    ld a3, 40(sp)
    ld a4, 48(sp)
    ld a5, 56(sp)
    ld a6, 64(sp)
    ld a7, 72(sp)
    call syscall
    continue:
    ld t0, 224(sp)
    csrw sepc, t0
    ld t0, 0(sp)
    ld ra, 8(sp)
    ld a1, 24(sp)
    ld a2, 32(sp)
    ld a3, 40(sp)
    ld a4, 48(sp)
    ld a5, 56(sp)
    ld a6, 64(sp)
    ld a7, 72(sp)
    ld s0, 80(sp)
    ld s1, 88(sp)
    ld s2, 96(sp)
    ld s3, 104(sp)
    ld s4, 112(sp)
    ld s5, 120(sp)
    ld s6, 128(sp)
    ld s7, 136(sp)
    ld s8, 144(sp)
    ld s9, 152(sp)
    ld s10, 160(sp)
    ld s11, 168(sp)
    ld t1, 176(sp)
    ld t2, 184(sp)
    ld t3, 192(sp)
    ld t4, 200(sp)
    ld t5, 208(sp)
    ld t6, 216(sp)
    addi sp, sp, 240
    sret
    label:ld a0, 16(sp)
    j continue
