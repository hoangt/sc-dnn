.code

mulsum3_opt1_75_25 PROC
    xorps xmm3, xmm3

    movss dword ptr[rsp-10h], xmm2
    movss dword ptr[rsp-0ch], xmm2
    movss dword ptr[rsp-08h], xmm2
    movss dword ptr[rsp-04h], xmm2
    movups xmm2, dword ptr[rsp-10h]

    mov   r8, r9
    shr   r9, 2
    test  r9, r9
    jz    loop_11_end
loop_11:
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_11_end
    movups xmm1, xmmword ptr [rdx]
    nop
    nop
    nop
    nop
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_11_end
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_11_end
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jne   loop_11
loop_11_end:
    ret   0
mulsum3_opt1_75_25 ENDP

mulsum3_opt1_50_50 PROC
    xorps xmm3, xmm3

    movss dword ptr[rsp-10h], xmm2
    movss dword ptr[rsp-0ch], xmm2
    movss dword ptr[rsp-08h], xmm2
    movss dword ptr[rsp-04h], xmm2
    movups xmm2, dword ptr[rsp-10h]

    mov   r8, r9
    shr   r9, 2
    test  r9, r9
    jz    loop_12_end
loop_12:
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_12_end
    movups xmm1, xmmword ptr [rdx]
    nop
    nop
    nop
    nop
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_12_end
    movups xmm1, xmmword ptr [rdx]
    nop
    nop
    nop
    nop
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_12_end
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jne   loop_12
loop_12_end:
    ret   0
mulsum3_opt1_50_50 ENDP

mulsum3_opt1_25_75 PROC
    xorps xmm3, xmm3

    movss dword ptr[rsp-10h], xmm2
    movss dword ptr[rsp-0ch], xmm2
    movss dword ptr[rsp-08h], xmm2
    movss dword ptr[rsp-04h], xmm2
    movups xmm2, dword ptr[rsp-10h]

    mov   r8, r9
    shr   r9, 2
    test  r9, r9
    jz    loop_13_end
loop_13:
    movups xmm1, xmmword ptr [rdx]
    nop
    nop
    nop
    nop
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_13_end
    movups xmm1, xmmword ptr [rdx]
    nop
    nop
    nop
    nop
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_13_end
    movups xmm1, xmmword ptr [rdx]
    nop
    nop
    nop
    nop
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_13_end
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jne   loop_13
loop_13_end:
    ret   0
mulsum3_opt1_25_75 ENDP


mulsum3_opt1_0_100 PROC
    xorps xmm3, xmm3

    movss dword ptr[rsp-10h], xmm2
    movss dword ptr[rsp-0ch], xmm2
    movss dword ptr[rsp-08h], xmm2
    movss dword ptr[rsp-04h], xmm2
    movups xmm2, dword ptr[rsp-10h]

    mov   r8, r9
    shr   r9, 2
    test  r9, r9
    jz    loop_14_end
loop_14:
    movups xmm1, xmmword ptr [rdx]
    nop
    nop
    nop
    nop
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_14_end
    movups xmm1, xmmword ptr [rdx]
    nop
    nop
    nop
    nop
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_14_end
    movups xmm1, xmmword ptr [rdx]
    nop
    nop
    nop
    nop
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_14_end
    movups xmm1, xmmword ptr [rdx]
    nop
    nop
    nop
    nop
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jne   loop_14
loop_14_end:
    ret   0
mulsum3_opt1_0_100 ENDP

END
