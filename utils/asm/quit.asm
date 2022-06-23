					global		quit_4_fd_asm
					global		quit_3_fd_asm
					global		quit_2_fd_asm
					global		quit_fd_asm
					global		quit_asm

					extern		str_len_asm

					section		.text
quit_4_fd_asm:
					push		rbp
					mov			rbp,			rsp

					push		rdi				; fd_1	+32
					push		rsi				; fd_2	+24
					push		rdx				; fd_3	+16
					push		rcx				; fd_4	+8
					push		r8				; msg	+0


					mov			r9,				1
	.loop
					cmp			r9,				5
					je			.msg

					mov			rdi,			qword[rsp+r9*8]
					cmp			rdi,			0
					je			.inc

					mov			rax,			3
					syscall
	.inc
					inc			r9
					jmp			.loop
	.msg:
					mov			rdi,			qword[rsp]
					cmp			rdi,			0
					je			.exit

					mov			rax,			1
					mov			rdi,			2
					mov			rsi,			err_msg
					mov			rdx,			qword[err_msg_sz]
					syscall

					mov			rdi,			qword[rsp]
					call		str_len_asm

					mov			rdx,			rax
					mov			rsi,			qword[rsp]
					mov			rdi,			2
					mov			rax,			1
					syscall

					mov			rax,			1
					mov			rdi,			2
					mov			rsi,			err_msg_ret
					mov			rdx,			1
					syscall
	.exit:
					mov			rax,			60
					mov			rdi,			qword[err_stat]
					syscall

					mov			rsp,			rbp
					pop			rbp
					ret

quit_3_fd_asm:
					push		rbp
					mov			rbp,			rsp

					mov			r8,				rcx
					xor			rcx,			rcx
					call		quit_4_fd_asm

					mov			rsp,			rbp
					pop			rbp
					ret

quit_2_fd_asm:
					push		rbp
					mov			rbp,			rsp

					mov			rcx,			rdx
					xor			rdx,			rdx
					call		quit_3_fd_asm

					mov			rsp,			rbp
					pop			rbp
					ret

quit_fd_asm:
					push		rbp
					mov			rbp,			rsp

					mov			rdx,			rsi
					xor			rsi,			rsi
					call		quit_2_fd_asm

					mov			rsp,			rbp
					pop			rbp
					ret

quit_asm:
					push		rbp
					mov			rbp,			rsp

					mov			rsi,			rdi
					xor			rdi,			rdi
					call		quit_fd_asm

					mov			rsp,			rbp
					pop			rbp
					ret

					section		.data
err_msg:			db			"Error: "
err_msg_sz:			dq			7
err_msg_ret:		db			10
err_stat:			dq			42
