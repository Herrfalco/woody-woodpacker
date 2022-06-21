					global		add_rkeys_asm

					section		.text
add_rkeys_asm:	
					push		rbp
					mov			rbp,				rsp
					
					imul		rdx,				4
					xor			rcx,				rcx
	.loop:
					cmp			rcx,				4
					je			.end

					mov			r10d,				dword[rdi+rcx*4]
					mov			r11d,				dword[rsi+rdx*4]
					xor			r10d,				r11d
					mov			dword[rdi+rcx*4],	r10d

					inc			rcx
					inc			rdx
					jmp			.loop

	.end:
					mov			rsp,				rbp
					pop			rbp
					ret
