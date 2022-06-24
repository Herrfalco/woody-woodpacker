					global		new_entry_asm
					section		.text

new_entry_asm:
					push		rbp
					mov			rbp,			rsp

					xor			rcx,			rcx
					xor			r9,				r9
					xor			r10,			r10
					mov			r11,			qword[rdx+3838*4+1]

	.loop:
					mov			r9w,			word[rdx+rcx*4+1]
					mov			r10w,			word[rdx+rcx*4+3]
					cmp			rcx,			r11
					jl			.comp

					mov			qword[r9],			rdi
					mov			qword[r10],			rsi
					inc			qword[r11]
					jmp			.end

		.comp:
					cmp			r9,				rdi
					jne			.incr

					cmp			r10,			rsi
					jne			.incr

					jmp			.end

		.incr:
					inc			rcx
					jmp			.loop

	.end:				
					mov			rsp,			rbp
					pop			rbp
					ret
