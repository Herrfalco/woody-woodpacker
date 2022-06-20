					global		aes_data_dec_asm

					extern		decode_block_asm

					section		.text
aes_data_dec_asm:	
					push		rbp
					mov			rbp,			rsp

					push		rdi
					push		rsi
					push		rdx
					push		rcx
					xor			r8,				r8
					push		r8
	.for:
					mov			r8,				qword[rsp]
					cmp			r8,				qword[rsp+24]
					je			.end

					mov			rdi,			qword[rsp+32]
					lea			rdi,			[rdi+r8]
					mov			rsi,			qword[rsp+16]
					mov			rdx,			qword[rsp+8]
					call		decode_block_asm	
						
					add			qword[rsp],		16
					jmp			.for
	.end:	
					mov			rsp,			rbp
					pop			rbp
					ret
