					global		sat_sub_asm

					section		.text
sat_sub_asm:		
					push		rbp
					mov			rbp,			rsp

					xor			rax,			rax
					cmp			rdi,			rsi
					jb			.end

					sub			rdi,			rsi
					mov			rax,			rdi
	.end:
					mov			rsp,			rbp
					pop			rbp
					ret
