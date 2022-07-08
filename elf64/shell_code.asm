				global main

main:
				push	rbp
				mov		rbp,			rsp

				and		rsp,			0xfffffffffffffff0

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

				mov		rbx,			0x1122334455667788
				sub		rax,			rbx
				mov		rbx,			0x1122334455667788
				add		rax,			rbx
				call	rax

				mov		rsp,			rbp
				pop		rbp
