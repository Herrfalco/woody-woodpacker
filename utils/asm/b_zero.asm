					global		b_zero_asm

					section		.text
b_zero_asm:
					push		rbp
					mov			rbp,			rsp
	.loop:
					cmp			rsi,			0
					je			.end
					dec			rsi

					mov			byte[rdi+rsi],	0
					jmp			.loop
	.end:
					mov			rsp,			rbp
					pop			rbp
					ret

