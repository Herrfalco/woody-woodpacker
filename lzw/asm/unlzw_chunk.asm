					global		unlzw_chunk_asm
					extern		value_reader
					extern		file_writer

					section		.text

unlzw_chunk_asm:
					push		rbp
					mov			rbp,				rsp

					push		rdi							; dst			+15400
					push		rsi							; src			+15392
					push		rdx							; *r_buff		+15384
					push		rcx							; *w_buff		+15376
															; first			+15363
															; init			+15361
															; last_value	+15359
															; value			+15357
															; dico.bits		+15356
															; dico.entry	+8
					sub			rsp,				15376	; dico.size		+0

					mov			rdi,				rsp
					mov			rsi,				15376
					call		b_zero_asm

					mov			byte[rsp+15356],	9

	.loop:
					mov			rdi,				qword[rsp+15400]
					lea			rsi,				[rsp+15357]
					xor			rdx,				rdx
					mov			dl,					byte[rsp+15356]
					mov			rcx,				qword[rsp+15384]
					call		value_reader

					cmp			rax,				-1
					je			.end_of_loop
