				global main

main:
				push	rbp
				mov		rbp,			rsp

				and		rsp,			0xfffffffffffffff0

				sub		rsp,			16

				mov		rax,			0x444f4f572e2e2e2e
				mov		qword[rsp],		rax
				mov		rax,			0xa2e2e2e2e59
				mov		qword[rsp+8],	rax

				mov		rdi,			1
				mov		rsi,			rsp
				mov		rdx,			14
				mov		rax,			1
				syscall

				mov		rsp,			rbp
				pop		rbp

				mov		rax,			0x1122334455667788
				jmp		rax
				
