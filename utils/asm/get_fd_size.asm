					global		get_fd_size_asm

					section		.text
get_fd_size_asm:		
					push		rbp
					mov			rbp,			rsp

					push		rdi
					sub			rsp,			8

					xor			rsi,			rsi
					mov			rdx,			2
					mov			rax,			8
					syscall

					cmp			rax,			0
					jl			.err
					
					mov			qword[rsp],		rax

					mov			rdi,			qword[rsp+8]
					xor			rsi,			rsi
					xor			rdx,			rdx
					mov			rax,			8
					syscall

					cmp			rax,			0
					jne			.err
					
					mov			rax,			qword[rsp]
					jmp			.end
	.err:
					mov			rax,			-1
	.end:
					mov			rsp,			rbp
					pop			rbp
					ret
