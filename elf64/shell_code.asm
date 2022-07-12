				global shellcode
				global shellcode_end

				.text:
shellcode:
				push	rax
				push	rbx
				push	rcx
				push	rdx
				push	rdi
				push	rsi

   				sub		rsp,			16

   				mov		rbx,			0x444f4f572e2e2e2e
   				mov		qword[rsp],		rbx
   				mov		rbx,			0xa2e2e2e2e59
   				mov		qword[rsp+8],	rbx

   				mov		rdi,			1
   				mov		rsi,			rsp
   				mov		rdx,			14
   				mov		rax,			1
   				syscall

   				add		rsp,			16

				pop		rsi
				pop		rdi
				pop		rdx
				pop		rcx
				pop		rbx
				pop		rax

				jmp		0x11223344
shellcode_end:
