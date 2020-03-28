.text
.global _writeString

_writeString:
        pushl   %ebp
        movl    %esp, %ebp

        movl    16(%ebp), %esi          # 1st parameter, the string
        movl    %esi, %ecx
        xorl    %edx, %edx              # counter = 0
next:
        movb    (%esi), %al             # Load next character
        andb    %al, %al
        jz      ok                      # if 0, then ok
        incl    %edx                    # counter = counter + 1
        incl    %esi
        jmp     next
ok:
                                        # %ecx/%edx, the string and #chars
        movl    $1, %ebx                # fd for stdout in %ebx
	      movl    $4, %eax                # system call number (sys_write)
	      int     $0x80                   # call kernel

        movl    %ebp, %esp
        popl    %ebp
        ret
