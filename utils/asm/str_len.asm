					global		str_len_asm

					section		.text
str_len_asm:		
					push		rbp
					mov			rbp,			rsp

					xor			rax,			rax
	.loop:
					cmp			byte[rdi+rax],	0	
					je			.end

					inc			rax
					jmp			.loop
	.end:
					mov			rsp,			rbp
					pop			rbp
					ret
