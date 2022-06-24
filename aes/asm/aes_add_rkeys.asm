					global		add_rkeys_asm

					section		.text
add_rkeys_asm:	
					push		rbp
					mov			rbp,				rsp
					mov			rax,				rdx
					xor			rdx,				rdx
					mov			rbx,				4
					
					mul			rbx
					xor			rcx,				rcx
	.loop:
					cmp			rcx,				4
					je			.end

					mov			r10d,				dword[rdi+rcx*4]
					xor			r10d,				dword[rsi+rax*4]
					mov			dword[rdi+rcx*4],	r10d

					inc			rcx
					inc			rax
					jmp			.loop

	.end:
					mov			rsp,				rbp
					pop			rbp
					ret
