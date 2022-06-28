					global		value_reader_asm

					extern		file_reader

					section		.text
value_reader_asm:
					push		rbp
					mov			rbp,				rsp

					push		rdi							; fd		+40
					push		rsi							; *value	+32
					push		rdx							; size		+24
					push		rcx							; *buff		+16
					sub			rsp,				16		; byte		+2
															; mask		+0
					mov			rcx,				16
					sub			rcx,				qword[rsp+24]
					mov			word[rsp],			0xffff
					shr			word[rsp],			cl
	.loop:
					mov			r8,					qword[rsp+16]

					cmp			byte[r8+1052],		0
					jne			.get_value

					cmp			qword[r8+1044],		24
					ja			.get_value

					mov			rdi,				qword[rsp+40]
					lea			rsi,				[rsp+2]
					mov			rdx,				qword[rsp+16]
					call		file_reader

					cmp			rax,				-1
					je			.set_eof

					mov			r8,					qword[rsp+16]

					shl			dword[r8+1040],		8
					xor			rax,				rax
					mov			al,					byte[rsp+2]
					or			dword[r8+1040],		eax
					add			qword[r8+1044],		8

					jmp			.loop
	.set_eof:
					mov			r8,					qword[rsp+16]
					mov			byte[r8+1052],		1
	.get_value:
					mov			r9,					qword[rsp+24]
					cmp			qword[r8+1044],		r9
					jb			.err

					sub			qword[r8+1044],		r9
					mov			rcx,				qword[r8+1044]
					mov			eax,				dword[r8+1040]
					shr			eax,				cl
					mov			bx,					word[r8]
					and			ax,					bx
					mov			r9,					qword[rsp+32]
					mov			word[r9],			ax

					xor			rax,				rax
					jmp			.end
	.err:
					mov			rax,				-1
	.end:
					mov			rsp,				rbp
					pop			rbp
					ret
