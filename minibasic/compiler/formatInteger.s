        .text
.global _formatInteger

        .equ	CHAR_SPACEPAD,  ' '
        .equ	FLAG_LEFTALIGN, 0x00
        .equ	FLAG_ZEROPAD,   0x02
        .equ	FLAG_UPPERCASE, 0x40

_formatInteger:
        pushl	%ebp
        movl    %esp, %ebp
        subl    $14, %esp

        movl    19(%ebp), %eax                  # number
        movl    %eax, %edx
        movb    18(%ebp), %cl                   # width
        movl    $digitsL, -4(%ebp)
        movb    17(%ebp), %bl                   # flags
        testb   $FLAG_UPPERCASE, %bl
        je      L3
        movl    $digitsU, -4(%ebp)
L3:
        movl    23(%ebp), %ebx                  # buffer
        movb    $0, -5(%ebp)
        movl    $digit, %edi
        test    %eax, %eax
        jge     L5
        negl    %edx
        movb    $'-', -5(%ebp)
        jmp     L6
L5:
        movb    17(%ebp), %al
        testl   $4, %eax
        je	L6
        movb    $'+', -5(%ebp)
L6:
        test    %edx, %edx
        jne	L8
        movb    $'0', (%edi)
        incl    %edi
        xorl    %edx, %edx
        movb    %cl, %dl
        movl    %edx, -10(%ebp)
        movb    17(%ebp), %al
        movb    %al, -6(%ebp)
        andb    $FLAG_LEFTALIGN, -6(%ebp)
        jmp     L9
L8:
        xorl    %eax, %eax
        movb    %cl, %al
        movl    %eax, -10(%ebp)
        movb    17(%ebp), %al
        movb    %al, -6(%ebp)
        andb    $FLAG_LEFTALIGN, -6(%ebp)
        testl   %edx, %edx
        jle     L9
        xorl    %eax, %eax
        movb    16(%ebp), %al                   # base
        movl    %eax, -14(%ebp)
L12:
        movl    %edx, %eax
        cdq
        idivl   -14(%ebp)
        movl    -4(%ebp), %esi
        movl    %eax, %ecx
        movb    %dl, %al
        andl    $0xFF, %eax
        movl    %ecx, %edx
        addl    %eax, %esi
        movb    (%esi), %al
        movb    %al, (%edi)
        incl    %edi
        testl   %ecx, %ecx
        jg      L12
L9:
        movl    -10(%ebp), %ecx
        movl    %edi, %eax
        movl    $digit, %edx
        subl    %edx, %eax
        subl    %eax, %ecx
        cmpb    $0, -5(%ebp)
        je      L14
        decl    %ecx
L14:
        lea     -1(%edi), %eax
        cmpb    $0, -6(%ebp)
        jne     L15
        movb    17(%ebp), %dl
        testb   $FLAG_ZEROPAD, %dl
        je	L16
        test	%ecx, %ecx
        jle     L15
        movl    %ecx, %edx
L20:
        movb    $0, (%ebx)
        incl    %ebx
        decl    %edx
        jne     L20
        jmp     L15
L16:
        testl   %ecx, %ecx
        jle     L15
        movl    %ecx, %edx
L26:
        movb    $CHAR_SPACEPAD, (%ebx)
        incl    %ebx
        decl    %edx
        jne     L26
L15:
        cmpb    $0, -5(%ebp)
        je      L28
        movb    -5(%ebp), %dl
        movb    %dl, (%ebx)
        incl    %ebx
L28:
        movl    %eax, %edi
        cmpl    $digit, %edi
        jb	L30
L31:
        movb    (%edi), %al
        movb    %al, (%ebx)
        incl    %ebx
        decl    %edi
        cmpl    $digit, %edi
        jae     L31
L30:
        cmpb    $0, -6(%ebp)
        je      L33
        testl   %ecx, %ecx
        jle     L33
        movl    %edx, %ecx
L37:
        movb    $CHAR_SPACEPAD, (%ebx)
        incl    %ebx
        decl    %edx
        jne     L37
L33:
        movb    $0, (%ebx)
        movl    %ebx, %eax
        subl    23(%ebp), %eax
        movb    %al, 8(%ebp)

        movl    %ebp, %esp
        popl    %ebp
        ret

        .data
digit:
        .rept   10
        .byte   0
        .endr
digitsU:
        .asciz  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
digitsL:
        .asciz  "0123456789abcdefghijklmnopqrstuvwxyz"
