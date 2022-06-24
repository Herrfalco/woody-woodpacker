					global		seek_4_fd_asm
					global		seek_3_fd_asm
					global		seek_2_fd_asm
					global		seek_fd_asm

					section		.text
seek_4_fd_asm:		
					push		rbp
					mov			rbp,			rsp

					push		rdi				; fd_1	+24
					push		rsi				; fd_2	+16
					push		rdx				; fd_3	+8
					push		rcx				; fd_4	+0

					xor			r9,				r9
	.loop:
					cmp			r9,				4
					je			.ok

					mov			rdi,			qword[rsp+r9*8]
					cmp			rdi,			0
					jl			.inc

					xor			rsi,			rsi
					xor			rdx,			rdx
					mov			rax,			8
					syscall

					cmp			rax,			0
					jne			.error
	.inc:
					inc			r9
					jmp			.loop
	.ok:
					xor			rax,			rax
					jmp			.end
	.error:
					mov			rax,			-1
	.end:
					mov			rsp,			rbp
					pop			rbp
					ret

seek_3_fd_asm:
					push		rbp
					mov			rbp,			rsp

					mov			rcx,			-1
					call		seek_4_fd_asm

					mov			rsp,			rbp
					pop			rbp
					ret

seek_2_fd_asm:
					push		rbp
					mov			rbp,			rsp

					mov			rdx,			-1
					call		seek_3_fd_asm

					mov			rsp,			rbp
					pop			rbp
					ret

seek_fd_asm:
					push		rbp
					mov			rbp,			rsp

					mov			rsi,			-1
					call		seek_2_fd_asm

					mov			rsp,			rbp
					pop			rbp
					ret

seek_asm:
					push		rbp
					mov			rbp,			rsp

					mov			rdi,			-1
					call		seek_fd_asm

					mov			rsp,			rbp
					pop			rbp
					ret
