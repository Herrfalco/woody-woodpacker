					global		shift_rows_asm
					global		shift_word_asm

					section		.text
shift_rows_asm:	
					push		rbp
					mov			rbp,				rsp
					
					xor			r9,					r9
					cmp			rsi,				0
					jne			.enc
	.dec:
					cmp			r9,					4
					je			.end

					mov			eax,				dword[rdi+r9*4]
					mov			ebx,				eax

					mov			rcx,				r9
					imul		rcx,				8
					shl			eax,				cl

					push		byte 4
					sub			byte[rsp],			r9b
					pop			rcx
					imul		rcx,				8
					shr			ebx,				cl

					or			eax,				ebx
					mov			dword[rdi+r9*4],	eax

					inc			r9
					jmp			.dec
	.enc:
					cmp			r9,					4
					je			.end

					mov			eax,				dword[rdi+r9*4]
					mov			ebx,				eax

					mov			rcx,				r9
					imul		rcx,				8
					shr			eax,				cl

					push		byte 4
					sub			byte[rsp],			r9b
					pop			rcx
					imul		rcx,				8
					shl			ebx,				cl

					or			eax,				ebx
					mov			dword[rdi+r9*4],	eax

					inc			r9
					jmp			.enc
	.end:
					mov			rsp,				rbp
					pop			rbp
					ret

shift_word_asm:
					push		rbp
					mov			rbp,				rsp

					xor			rax,				rax
					mov			eax,				dword[rdi]
					mov			rbx,				rax

					mov			rcx,				rsi
					mov			rdx,				rcx

					imul		rcx,				8
					shr			rax,				cl

					mov			rcx,				4
					sub			rcx,				rdx
					imul		rcx,				8
					shl			rbx,				cl

					xor			rax,				rbx

					mov			dword[rdi],			eax

					mov			rsp,				rbp
					pop			rbp
					ret
