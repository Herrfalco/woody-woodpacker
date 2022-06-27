					global		entry_writer_asm
					extern		file_writer
					section		.text

entry_writer_asm:
					push		rbp,
					mov			rbp,				rsp
					
					xor			r8,					r8
					mov			r8w,				si

					push		rdi								; fd		+40
					push		r8								; value		+32
					push		rdx								; *dico		+24
					push		rcx								; *buff		+16
																; *entry	+8
					sub			rsp,				16			; ret		+0

					cmp			r8,					qword[dico_start]
					jb			.out_dico

					sub			r8,					qword[dico_start]
					lea			r9,					[rdx+r8*4+8]
					mov			qword[rsp+8],		r9
					xor			rsi,				rsi
					mov			si,					word[r9]
					call		entry_writer_asm

					mov			qword[rsp],			rax
					mov			rdi,				qword[rsp+40]
					mov			r8,					qword[rsp+8]
					xor			rsi,				rsi
					mov			sil,				byte[r8+2]
					xor			rdx,				rdx
					mov			rcx,				qword[rsp+16]
					call		file_writer

					cmp			rax,				-1
					je			.end

					mov			rax,				qword[rsp]
					jmp			.end

	.out_dico:
					xor			rdx,				rdx
					call		file_writer

					cmp			rax,				-1
					je			.end

					mov			rax,				qword[rsp+32]

	.end:
					mov			rsp,				rbp
					pop			rbp
					ret

					section		.data
dico_start:			dq			259
