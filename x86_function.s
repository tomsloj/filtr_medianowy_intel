    section .text

    global x86_function

section .data
    width DD 0
    height DD 0
    ;myArray DB 25 dup (0)
    index DW 0
    currentPixelX DW -1
    currentPixely DW -1
    zero DB 0

section .text

; linker puts the entry point here:
x86_function:
    push  rbp
    mov rbp,  rsp

    mov [width], rsi
    mov [height], rdx
    mov rdx, [rbp+8] ;poczatek tablicy

    ;wyzerowanie iteratorow
    mov r8, 0
    mov r9, 0
    mov r10, 0
    mov r11, 0

analizePixel:
    mov r14, 0
    mov r12, r8
    sub r12, 2
    mov r13, r9
    sub r13, 2
    mov r10, 0
    mov r11, 0

loopNr1:
    cmp r12, 0
    jl nextPixel
    cmp r13, 0
    jl nextPixel
    cmp r12, height
    jg nextPixel
    cmp r13, width
    jg nextPixel

    mov rax, r12
    mul rax, width
    add rax, r13

    mov rbx, [rdx+rax];w rbx wartosc pixela
    mov [myArray+r14], rbx;

    inc r14
    jmp nextPixel

sort:
    mov r12, r14 ; r12 - licznik zewnetrznej petli
                 ; r13 - licznik wewnetrznej petli
 
sortLoop1:
    move r13, 1
sortLoop2:
    ; wrzucamy do r10 i r11 elementow tablicy o indeksach r13 i r13+1
    sub r13, 1
    move r10, [myArray+r13]
    add r13, 1
    mov r11, [myArray+r13]
    sub r13, 1

    



afterSwap:








nextPixel:
    inc r10
    inc r13
    cmp r10, 5
    jne loopNr1
    mov r10, 0
    mov r13, r9
    sub r13, 2
    inc r12
    inc r11
    
    cmp r11, 5
    jne loopNr1

    jmp sort







    
    ;mov rax, 0

    ;mov rsi, rdi

;---------find size of array
    ;mov dl, [rdi]	; load byte
	;cmp dl, 0	; cmp will set ZERO flag if dl is zero
	;jz end		; jump to end because the string is 
			;	empty (first byte is zero!)
	;inc rdi		; move to the next byte
	;mov dl, [rdi]
	;cmp dl, 0
	;jnz loop_1	; now RDI points to the terminating byte of the string 
			;	- one byte beyond the last 'visible' character
	
	
	;dec rsi

end:

;------------------------------------------------------------------------------

	mov rsp, rbp	; restore original stack pointer
	pop rbp		; restore "calling procedure" frame pointer
	ret