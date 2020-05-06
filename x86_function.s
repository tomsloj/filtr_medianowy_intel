    section .text

    global x86_function

section .data
    width DD 0
    height DD 0
    myArray DB 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    index DW 0
    currentPixelX DW -1
    currentPixely DW -1
    zero DB 0

section .text

; linker puts the entry point here:
x86_function:
    push  rbp
    mov rbp,  rsp

    ;mov [width], rsi
    ;mov [height], rdx
    ;mov rdx, [rbp+8] ;poczatek tablicy
    ;mov [rdi], dx
    ;add [rdi], [width]

    ;wyzerowanie iteratorow
    xor r8, r8
    xor r9, r9
    xor r10, r10
    xor r11, r11

analizePixel:
    xor r14, r14
    mov r12, r8
    dec r12
    dec r12
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;mov [rdi], r12b
    mov r13, r9
    dec r13
    dec r13
    xor r10, r10
    xor r11, r11


    
end:

;------------------------------------------------------------------------------

	mov rsp, rbp	; restore original stack pointer
	pop rbp		; restore "calling procedure" frame pointer
	ret