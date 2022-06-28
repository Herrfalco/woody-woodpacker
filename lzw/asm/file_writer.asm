					global		file_writer_asm
					section		.text

file_writer_asm:
					push		rbp
					mov			rbp,				rsp
					
					push		rcx										; *buff	+8
					sub			rsp,				8

					cmp			rdx,				2
					je			.comp
					
					mov			r8,					qword[rcx+1024]
					mov			byte[rcx+r8],		sil
					inc			qword[rcx+1024]
					
	.comp:
					cmp			rdx,				0
					jne			.write

					mov			r8,					qword[rcx+1024]	
					cmp			r8,					qword[buff_sz]
					je			.write

					mov			rax,				0
					jmp			.end

	.write:
					mov			rax,				1
					lea			rsi,				[rcx]
					mov			rdx,				qword[rcx+1024]
					syscall

					cmp			rax,				0
					jl			.error

					mov			r8,					qword[rsp+8]
					cmp			rax,				qword[r8+1024]
					jne			.error

					mov			qword[r8+1024],		0
					mov			rax,				0
					jmp			.end

	.error:
					mov			rax,				-1

	.end:
					mov			rsp,				rbp
					pop			rbp
					ret



					section		.data
buff_sz:			dq			1024
