		global				sc
		global				sc_end
		global				sc_data
		global				sc_data_end
		global				sc_key

		global				sc_entry
		global				sc_old_entry

		global				sc_text_addr
		global				sc_text_sz
		global				sc_p_text_addr
		global				sc_p_text_sz

		global				aes_encoding

sc:
		push				rdi
		push				rsi
		push				rdx

		mov					rdi,				1
		lea					rsi,				[rel sc_msg]
		mov					rdx,				sc_msg_end - sc_msg
		mov					rax,				1
		syscall

		lea					rax,				[rel sc]
		sub					rax,				qword[rel sc_entry]
		push				rax

		mov					rdi,				qword[rel sc_text_addr]
		add					rdi,				qword[rsp]
		and					rdi,				0xfffffffffffff000
		mov					rsi,				qword[rel sc_text_sz]
		add					rsi,				0x1000
		mov					rdx,				0x7
		mov					rax,				10
		syscall

		xor					rcx,				rcx
	.loop:
		cmp					rcx,				qword[rel sc_text_sz]
		je					.end

		mov					rdi,				qword[rel sc_text_addr]
		add					rdi,				qword[rsp]
		add					rdi,				rcx
		lea					rsi,				[rel sc_key]
		call				aes_decoding

		add					rcx,				16
		jmp					.loop
	.end:
		pop					rax

		pop					rdx
		pop					rsi
		pop					rdi

		add					rax,				qword[rel sc_old_entry]
		jmp					rax

aes_round_keys:
		movdqu				xmm11,				xmm0
		aeskeygenassist		xmm12,				xmm11,		0x1
		call				aes_round_key_exp
		movdqu				xmm1,				xmm11
		aeskeygenassist		xmm12,				xmm11,		0x2
		call				aes_round_key_exp
		movdqu				xmm2,				xmm11
		aeskeygenassist		xmm12,				xmm11,		0x4
		call				aes_round_key_exp
		movdqu				xmm3,				xmm11
		aeskeygenassist		xmm12,				xmm11,		0x8
		call				aes_round_key_exp
		movdqu				xmm4,				xmm11
		aeskeygenassist		xmm12,				xmm11,		0x10
		call				aes_round_key_exp
		movdqu				xmm5,				xmm11
		aeskeygenassist		xmm12,				xmm11,		0x20
		call				aes_round_key_exp
		movdqu				xmm6,				xmm11
		aeskeygenassist		xmm12,				xmm11,		0x40
		call				aes_round_key_exp
		movdqu				xmm7,				xmm11
		aeskeygenassist		xmm12,				xmm11,		0x80
		call				aes_round_key_exp
		movdqu				xmm8,				xmm11
		aeskeygenassist		xmm12,				xmm11,		0x1b
		call				aes_round_key_exp
		movdqu				xmm9,				xmm11
		aeskeygenassist		xmm12,				xmm11,		0x36
		call				aes_round_key_exp 
		movdqu				xmm10,				xmm11
		ret

aes_round_key_exp:
		pshufd				xmm12,				xmm12,		0xff		; Copy W[4] in each W[i]

		vpslldq				xmm13,				xmm11,		0x4			; Compute W[i-1] ^ W[i-4]
		pxor				xmm11,				xmm13					;
		vpslldq				xmm13,				xmm11,		0x4			;
		pxor				xmm11,				xmm13					;
		vpslldq				xmm13,				xmm11,		0x4			;
		pxor				xmm11,				xmm13					;
		pxor				xmm11,				xmm12					;
		ret 

aes_round_keys_rev:
		aesimc				xmm1,				xmm1
		aesimc				xmm2,				xmm2
		aesimc				xmm3,				xmm3
		aesimc				xmm4,				xmm4
		aesimc				xmm5,				xmm5
		aesimc				xmm6,				xmm6
		aesimc				xmm7,				xmm7
		aesimc				xmm8,				xmm8
		aesimc				xmm9,				xmm9
		ret

aes_decoding:
		movdqu				xmm14,				oword[rdi]				; data
		movdqu				xmm0,				oword[rsi]				; key

		call				aes_round_keys
		call				aes_round_keys_rev

		pxor				xmm14,				xmm10					; First xor
		aesdec				xmm14,				xmm9					; Round 1
		aesdec				xmm14,				xmm8					; Round 2
		aesdec				xmm14,				xmm7					; Round 3
		aesdec				xmm14,				xmm6					; Round 4
		aesdec				xmm14,				xmm5					; Round 5
		aesdec				xmm14,				xmm4					; Round 6
		aesdec				xmm14,				xmm3					; Round 7
		aesdec				xmm14,				xmm2					; Round 8
		aesdec				xmm14,				xmm1					; Round 9
		aesdeclast			xmm14,				xmm0					; Round 10

		movdqu				oword[rdi],			xmm14
		ret
sc_end:

sc_data:
sc_entry:
				dq		0
sc_old_entry:
				dq		0
sc_text_addr:
				dq		0
sc_text_sz:
				dq		0
sc_key:
				dq		0, 0
sc_msg:
				db		"....WOODY....", 10
sc_msg_end:
sc_data_end:

aes_encoding:
		movdqu				xmm14,				oword[rdi]				; data
		movdqu				xmm0,				oword[rsi]				; key

		call				aes_round_keys

		pxor				xmm14,				xmm0					; Whitening step (Round 0)
		aesenc				xmm14,				xmm1					; Round 1
		aesenc				xmm14,				xmm2					; Round 2
		aesenc				xmm14,				xmm3					; Round 3
		aesenc				xmm14,				xmm4					; Round 4
		aesenc				xmm14,				xmm5					; Round 5
		aesenc				xmm14,				xmm6					; Round 6
		aesenc				xmm14,				xmm7					; Round 7
		aesenc				xmm14,				xmm8					; Round 8
		aesenc				xmm14,				xmm9					; Round 9
		aesenclast			xmm14,				xmm10					; Round 10

		movdqu				oword[rdi],			xmm14
		ret
