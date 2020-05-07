    section .text

    global x86_function

section .data
    width DD 0
    height DD 0
    index DW 0
    currentPixelX DW -1
    currentPixely DW -1
    zero DB 0


    msg db  'Hello, world!',0xa ;our dear string
    len equ $ - msg         ;length of our dear string

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

    mov rbx, r8

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



loopNr1:
    cmp r12, 0
    jl nextPixel
    cmp r13, 0
    jl nextPixel
    cmp r12, rdx
    jge nextPixel
    cmp r13, rsi
    jge nextPixel

    ;liczymy numer indexu w tablicy pixeli
    mov rax, r12
    imul rax, rsi
    add rax, r13 
    
    push r9 ; potrzebuje rejestr
    mov r9, [rdi+rax];w rbx wartosc pixela
    mov [rcx+r14], r9b;
    pop r9 ; sciagam wczesniej wstawiona wartosc ze stosu

    inc r14
    jmp nextPixel

sort:
    ;mov rax, r8
    ;imul rax, rsi
    ;add rax, r9
    ;cmp rax, 36
    ;je end
    ;mov [rbx+rax], r14


    mov r12, r14 ; r12 - licznik zewnetrznej petli
                 ; r13 - licznik wewnetrznej petli
sortLoop1:
    xor r13, r13
    inc r13
sortLoop2:
    ; wrzucamy do r10 i r11 elementow tablicy o indeksach r13 i r13+1
    dec r13
    mov r10, [rcx+r13]
    inc r13
    mov r11, [rcx+r13]
    dec r13

    ;mov rax, r8
    ;imul rax, rsi
    ;add rax, r9
    ;cmp rax, 36
    ;jne jump

    ;mov [rbx], r10b
    ;inc rbx
    ;mov [rbx], r11b
    ;inc rbx

    ;cmp r12, r14
    ;jne end
    ;jmp end
jump:

    cmp r10b, r11b
    jle afterSwap
swap:
    mov [rcx+r13], r11b
    inc r13
    mov [rcx+r13], r10b
    dec r13

    ;mov rax, r8
    ;imul rax, rsi
    ;add rax, r9
    ;cmp rax, 36
    ;je end
afterSwap:
    inc r13
    inc r13
    cmp r13, r12
    jl sortLoop2 ; while r13 < r12 - 1
; END sortLoop2
    dec r12
    cmp r12, 0
    jg sortLoop1
; END sortLoop1
    xor r12, r12


endSort:
    mov r12, r14
    
    sar r12,1 ;dzielimy na 2
    mov r13, [rcx+r12] ; wyciagamy mediane

    mov rax, r8
    imul rax, rsi
    add rax, r9 ; indeks analizowanego pixela

    ;inc rax
    ;mov [rdi], rax
    ;jmp end


    mov [rbx+rax], r13b; wrzucamy mediane do drugiej tablicy na swoje miejsce
    
    ;jmp end
    ;cmp rax, 36
    ;je end

    inc r9
    cmp r9, rsi;;;;;;;;;;;;;;;;;;;;;;;;;;
    jl analizePixel
    ;jmp end

    ;doszlismy do konca wiersza
    xor r9, r9
    inc r8
    cmp r8, rdx
    jl analizePixel


    jmp swapPixels


nextPixel:
    inc r10
    inc r13
    cmp r10, 5
    jne loopNr1
    xor r10, r10
    mov r13, r9
    dec r13
    dec r13
    inc r12
    inc r11
    
    cmp r11, 5
    jne loopNr1

    jmp sort
    




swapPixels:

    
end:

;------------------------------------------------------------------------------

	mov rsp, rbp	; restore original stack pointer
	pop rbp		; restore "calling procedure" frame pointer
	ret