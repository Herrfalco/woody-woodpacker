				global main

main:
				push	rax
				push	rdi
				push	rsi
				push	rdx

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

				pop		rdx
				pop		rsi
				pop		rdi
				pop		rax

				jmp		0x11223344
