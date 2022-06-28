					global		file_reader_asm

					section		.text
file_reader_asm:
					push		rbp
					mov			rbp,				rsp

					push		rsi,				; *byte		+8
					push		rdx,				; *buff		+0

					mov			rax,				qword[rdx+1032]
					cmp			rax,				qword[rdx+1024]
					jne			.get_byte
					
					mov			r8,					qword[rsp]
					mov			qword[r8+1032],		0

					mov			rsi,				r8
					mov			rdx,				1024
					xor			rax,				rax
					syscall

					cmp			rax,				0
					jle			.err

					mov			r8,					qword[rsp]
					mov			qword[r8+1024],		rax
	.get_byte:
					mov			r8,					qword[rsp]

					mov			r9,					qword[r8+1032]
					mov			r9b,				byte[r8+r9]
					mov			r10,				qword[rsp+8]
					mov			byte[r10],			r9b

					inc			byte[r8+1032]

					xor			rax,				rax
					jmp			.end
	.err:
					mov			rax,				-1
	.end:
					mov			rsp,				rbp
					pop			rbp
					ret
