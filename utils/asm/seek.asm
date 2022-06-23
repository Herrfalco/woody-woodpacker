					global		seek_asm

					section		.text
seek_asm:		
					push		rbp
					mov			rbp,			rsp

					push		rsi

					cmp			rdi,			0
					je			.fd_2
					
					xor			rsi,			rsi
					xor			rdx,			rdx
					mov			rax,			8
					syscall

					cmp			rax,			0
					jne			.error
	.fd_2:
					mov			rdi,			qword[rsp]
					cmp			rdi,			0
					je			.ok
					
					xor			rsi,			rsi
					xor			rdx,			rdx
					mov			rax,			8
					syscall

					cmp			rax,			0
					jne			.error
	.ok:
					xor			rax,			rax
					jmp			.end
	.error:
					mov			rax,			1
	.end:
					mov			rsp,			rbp
					pop			rbp
					ret
