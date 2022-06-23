					global		quit_2_fd_asm
					global		quit_fd_asm
					global		quit_asm

					extern		str_len_asm

					section		.text
quit_2_fd_asm:
					push		rbp
					mov			rbp,			rsp

					push		rsi
					push		rdx

					cmp			rdi,			0
					je			.fd2

					mov			rax,			3
					syscall
	.fd2:
					mov			rdi,			qword[rsp+8]
					cmp			rdi,			0
					je			.error

					mov			rax,			3
					syscall
	.error:
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

quit_fd_asm:
					push		rbp
					mov			rbp,			rsp

					mov			rdx,			rsi
					mov			rsi,			0
					call		quit_2_fd_asm

					mov			rsp,			rbp
					pop			rbp
					ret

quit_asm:
					push		rbp
					mov			rbp,			rsp

					mov			rsi,			rdi
					mov			rdi,			0
					call		quit_fd_asm

					mov			rsp,			rbp
					pop			rbp
					ret

					section		.data
err_msg:			db			"Error: "
err_msg_sz:			dq			7
err_msg_ret:		db			10
err_stat:			dq			42
