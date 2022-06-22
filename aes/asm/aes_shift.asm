					global		shift_rows_asm
					global		shift_word_asm

					section		.text
shift_rows_asm:	
					push		rbp
					mov			rbp,				rsp
					
					xor			r9,					r9
					mov			r11,				8
					cmp			rsi,				0
					jne			.enc
	.dec:
					cmp			r9,					4
					je			.end

					mov			r10d,				dword[rdi+r9*4]
					mov			ebx,				r10d

					mov			rax,				r9
					mul			r11
					mov			rcx,				rax
					shl			r10d,				cl

					push		byte 4
					sub			byte[rsp],			r9b
					pop			rax
					mul			r11
					mov			rcx,				rax
					shr			ebx,				cl

					or			r10d,				ebx
					mov			dword[rdi+r9*4],	r10d

					inc			r9
					jmp			.dec
	.enc:
					cmp			r9,					4
					je			.end

					mov			r10d,				dword[rdi+r9*4]
					mov			ebx,				r10d

					mov			rax,				r9
					mul			r11
					mov			rcx,				rax
					shr			r10d,				cl

					push		byte 4
					sub			byte[rsp],			r9b
					pop			rax
					mul			r11
					mov			rcx,				rax
					shl			ebx,				cl

					or			r10d,				ebx
					mov			dword[rdi+r9*4],	r10d

					inc			r9
					jmp			.enc
	.end:
					mov			rsp,				rbp
					pop			rbp
					ret

shift_word_asm:
					push		rbp
					mov			rbp,				rsp

					xor			r10,				r10
					mov			r10d,				dword[rdi]
					mov			rbx,				r10

					mov			rax,				rsi
					mov			rdx,				rax

					mov			r11,				8
					mul			r11
					mov			rcx,				rax
					shr			r10,				cl

					mov			rax,				4
					sub			rax,				rdx
					mul			r11
					mov			rcx,				rax
					shl			rbx,				cl

					xor			r10,				rbx

					mov			dword[rdi],			r10d

					mov			rsp,				rbp
					pop			rbp
					ret
