					global		get_rnb_asm

					section		.text
get_rnb_asm:	
					push		rbp
					mov			rbp,				rsp

					mov			rax,				11
					cmp			rdi,				128
					je			.end

					mov			rax,				13
					cmp			rdi,				192
					je			.end

					mov			rax,				15
	.end:
					mov			rsp,				rbp
					pop			rbp
					ret
