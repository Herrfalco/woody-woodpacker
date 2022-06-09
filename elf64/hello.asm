; ----------------------------------------------------------------------------------------
; Writes "Hello, World" to the console using only system calls. Runs on 64-bit Linux only.
; To assemble and run:
;
;	 nasm -felf64 hello.asm && ld hello.o && ./a.out
; ----------------------------------------------------------------------------------------

			global	_start

			section	 .text
_start:	 
			mov		 rax, 1
			mov		 rdi, 1
			mov		 rsi, 0x401000	; address of string to output
			mov		 rdx, 14	    ; number of bytes
			syscall				    ; invoke operating system to do the write
			mov		 rax, 60		; system call for exit
			xor		 rdi, rdi		; exit code 0
			syscall				    ; invoke operating system to exit
