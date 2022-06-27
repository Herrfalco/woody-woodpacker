					global		new_entry_asm
					section		.text

new_entry_asm:
					push		rbp
					mov			rbp,				rsp

					mov			r8,					qword[rdx]

					mov			word[rdx+r8*4+8],	di
					mov			word[rdx+r8*4+10],	si
					inc			qword[rdx]

					mov			rsp,				rbp
					pop			rbp
					ret
