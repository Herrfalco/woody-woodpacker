					global		round_keys_asm

					extern		get_rnb_asm
					extern		shift_word_asm
					extern		sub_word_asm

					section		.text
round_keys_asm:	
					push		rbp
					mov			rbp,				rsp

					push		rdi						; key				+40
					push		rsi						; rkeys				+32
					sub			rsp,				32	; rkeys + i			+16
														; rkeys + (i - n)	+8
														; i / n				+7
														; i					+6
														; n					+5
														; rkeys_nb			+4
														; tmp				+0

					mov			rdi,				qword[key_size] 	; compute rkeys_nb (+4)
					call		get_rnb_asm								;
					xor			rdx,				rdx
					mov			rbx,				4					;
					mul			rbx										;
					mov			byte[rsp+4],		al					;

					mov			rax,				qword[key_size]		; compute n (+5)
					xor			rdx,				rdx					;
					mov			rbx,				32					;
					div			rbx										;
					mov			byte[rsp+5],		al					;

					xor			rcx,				rcx					; i = 0 (+6)
					mov			byte[rsp+6],		cl					;
	.loop:
					xor			rcx,				rcx					; cl = i
					mov			cl,					byte[rsp+6]			;

					cmp			cl,					byte[rsp+4]			; i == r_keys_nb
					je			.end									;

					mov			rbx,				qword[rsp+32]		; rkeys + i (+16)
					lea			rbx,				[rbx+rcx*4]			;
					mov			qword[rsp+16],		rbx					;

					cmp			cl,					byte[rsp+5]			; i < n
					jge			.case_2									;

					mov			rax,				qword[rsp+40]		; rkeys[i] = (32)key[i]
					mov			eax,				dword[rax+rcx*4]	;
					mov			dword[rbx],			eax					;

					jmp			.inc									;
	.case_2:
					xor			rbx,				rbx					; rbx = i - 1
					mov			bl,					cl					;
					dec			rbx										;

					mov			rax,				qword[rsp+32]		; rkeys[i - 1] (+0)
					mov			ebx,				dword[rax+rbx*4]	;
					mov			dword[rsp],			ebx					;

					xor			rdx,				rdx					; rdx = i - n
					mov			dl,					cl					;
					sub			dl,					byte[rsp+5]			;

					lea			rax,				[rax+rdx*4]			; rkeys + (i - n) (+8)
					mov			qword[rsp+8],		rax					;

					xor			rax,				rax					; i / n (+7)
					mov			al,					cl					;
					div			byte[rsp+5]								;
					mov			byte[rsp+7],		al					;

					cmp			ah,					0					; !(i % n)
					jne			.case_3									;

					mov			rdi,				rsp					; shift_word(tmp, 1)
					mov			rsi,				1					;
					call		shift_word_asm							;

					mov			rdi,				rsp					; sub_word(tmp)
					call		sub_word_asm							;

					mov			rdx,				rcon				; rcon[i/n-1]
					xor			rcx,				rcx					;
					mov			cl,					byte[rsp+7]			;
					sub			cl,					1					;
					mov			eax,				dword[rdx+rcx*4]	;

					xor			eax,				dword[rsp]			; ^ tmp

					mov			rdx,				qword[rsp+8]		; ^ rkeys[i-n]
					xor			eax,				dword[rdx]			;
					
					mov			rdx,				qword[rsp+16]		; rkeys[i] = _
					mov			dword[rdx],			eax					;

					jmp			.inc
	.case_3:
					cmp			ah,					4					; i % n == 4
					jne			.case_4									;
					cmp			byte[rsp+5],		6					;
					jle			.case_4									;

					mov			rdi,				rsp					; sub_word_asm(tmp)
					call		sub_word_asm							;
	.case_4:
					mov			eax,				dword[rsp]			; tmp

					mov			rdx,				qword[rsp+8]		; ^ rkeys[i-n]
					xor			eax,				dword[rdx]			;
					
					mov			rdx,				qword[rsp+16]		; rkeys[i] = _
					mov			dword[rdx],			eax					;
	.inc:
					inc			byte[rsp+6]								; ++i
					jmp			.loop									;
	.end:
					mov			rsp,				rbp
					pop			rbp
					ret

					section		.data
key_size			dq			256
rcon:				dd			0x1000000,0x2000000,0x4000000,0x8000000,0x10000000,0x20000000,0x40000000,0x80000000,0x1b000000,0x36000000
