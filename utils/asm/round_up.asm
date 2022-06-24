					global		round_up_asm

					section		.text
round_up_asm:		
					push		rbp
					mov			rbp,			rsp

					xor			rdx,			rdx
					mov			rax,			rdi
					div			rsi

					cmp			rdx,			0
					jne			.rem
					
					mov			rax,			rdi
					jmp			.end
	.rem:
					mul			rsi
					add			rax,			rsi
	.end:
					mov			rsp,			rbp
					pop			rbp
					ret
