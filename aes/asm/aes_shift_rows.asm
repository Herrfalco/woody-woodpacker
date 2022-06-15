					global		shift_rows_asm

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
					pop			rbp
					ret
