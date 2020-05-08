    section .text

    global filter

filter:
    push  rbp
    mov rbp,  rsp

    mov rbx, r8

    ;rdi --- wskaznik do poczatku tablicy pixeli
    ;rsi --- szerokosc bitmapy
    ;rdx --- wysokosc bitmapy
    ;rcx --- wskaznik do poczatku 25 elementowej tablicy
    ;rbx --- tablica w ktorej przechowywane sa pixele po filtracji

    ;r8 --- X aktualnie rozwazanego pixela
    ;r9 --- Y aktualnie rozwazanego pixela
    ;r10 --- licznik iteracji w jednym wierszu
	;r11 --- licznik iteracji wierszy
    ;r12 --- numer wiersza (w iteracji)
	;r13 --- numer kolumny (w iteracji)
    ;r14 --- liczba pixeli juz wlozonych do tablicy 5x5

    ;wyzerowanie iteratorow
    xor r8, r8
    xor r9, r9
    xor r10, r10
    xor r11, r11


analizePixel:
    xor r14, r14 ; r14 = 0
    ;r12 = r8 - 2
    mov r12, r8 
    dec r12
    dec r12
    ;r13 = r9 - 2
    mov r13, r9
    dec r13
    dec r13
    xor r10, r10 ;r10 = 0
    xor r11, r11 ; r11 = 0

loopNr1:
    ;sprawdzamy czy rozwazany pixel miesci sie w bitmapie
    cmp r12, 0
    jl nextPixel
    cmp r13, 0
    jl nextPixel
    cmp r12, rdx
    jge nextPixel
    cmp r13, rsi
    jge nextPixel

    ;liczymy numer indexu w tablicy pixeli
    ;rax = r12 * rsi + r13 <=> rax = x * szerokosc + y
    mov rax, r12
    imul rax, rsi
    add rax, r13 
    
    push r9 ; zwaliniam na chwile rejestr
    mov r9, [rdi+rax];w r9 wartosc pixela z pod indexu rax
    mov [rcx+r14], r9b; zapisujemy ten pixel do tablicy 5x5
    pop r9 ; sciagam wczesniej wstawiona wartosc ze stosu

    inc r14; zwiekszamy licznik ilosci pixeli w tablicy 5x5
    jmp nextPixel

sort:
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

    cmp r10b, r11b; jesli r10 <= r11 to nic nie zmieniamy
    jle afterSwap
swap: ; zamieniamy dwa sasiednie elementy tablicy
    mov [rcx+r13], r11b
    inc r13
    mov [rcx+r13], r10b
    dec r13

afterSwap:
    inc r13
    inc r13
    cmp r13, r12
    jl sortLoop2 ; while r13 < r12 - 1
; END sortLoop2
    dec r12
    cmp r12, 0
    jg sortLoop1 ; while r12 > 1
; END sortLoop1
    xor r12, r12

endSort:
    mov r12, r14
    
    sar r12,1 ;dzielimy na 2
    mov r13, [rcx+r12] ; wyciagamy mediane

    mov rax, r8
    imul rax, rsi
    add rax, r9 ; indeks analizowanego pixela
    ;rax = r12 * rsi + r13 <=> rax = x * szerokosc + y

    mov [rbx+rax], r13b; wrzucamy mediane do drugiej tablicy na swoje miejsce

    inc r9
    cmp r9, rsi
    jl analizePixel

    ;doszlismy do konca wiersza
    xor r9, r9
    inc r8
    cmp r8, rdx
    jl analizePixel

    jmp end


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

end:

	mov rsp, rbp	; przywróć oryginalny stack pointer
	pop rbp		; przywróć wskaźnik ramki stosu
	ret