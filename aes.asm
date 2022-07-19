							global		aes_encoding
							global		aes_decoding

aes_round_keys:
		movdqu				xmm11,		xmm0
		aeskeygenassist		xmm12,		xmm11,		0x1
		call				aes_round_key_exp
		movdqu				xmm1,		xmm11
		aeskeygenassist		xmm12,		xmm11,		0x2
		call				aes_round_key_exp
		movdqu				xmm2,		xmm11
		aeskeygenassist		xmm12,		xmm11,		0x4
		call				aes_round_key_exp
		movdqu				xmm3,		xmm11
		aeskeygenassist		xmm12,		xmm11,		0x8
		call				aes_round_key_exp
		movdqu				xmm4,		xmm11
		aeskeygenassist		xmm12,		xmm11,		0x10
		call				aes_round_key_exp
		movdqu				xmm5,		xmm11
		aeskeygenassist		xmm12,		xmm11,		0x20
		call				aes_round_key_exp
		movdqu				xmm6,		xmm11
		aeskeygenassist		xmm12,		xmm11,		0x40
		call				aes_round_key_exp
		movdqu				xmm7,		xmm11
		aeskeygenassist		xmm12,		xmm11,		0x80
		call				aes_round_key_exp
		movdqu				xmm8,		xmm11
		aeskeygenassist		xmm12,		xmm11,		0x1b
		call				aes_round_key_exp
		movdqu				xmm9,		xmm11
		aeskeygenassist		xmm12,		xmm11,		0x36
		call				aes_round_key_exp 
		movdqu				xmm10,		xmm11
		ret

aes_round_key_exp:
		pshufd		xmm12,		xmm12,		0xff		; Copy W[4] in each W[i]

		vpslldq		xmm13,		xmm11,		0x4			; Compute W[i-1] ^ W[i-4]
		pxor		xmm11,		xmm13					;
		vpslldq		xmm13,		xmm11,		0x4			;
		pxor		xmm11,		xmm13					;
		vpslldq		xmm13,		xmm11,		0x4			;
		pxor		xmm11,		xmm13					;
		pxor		xmm11,		xmm12					;
		ret 

aes_round_keys_rev:
		aesimc				xmm1,		xmm1
		aesimc				xmm2,		xmm2
		aesimc				xmm3,		xmm3
		aesimc				xmm4,		xmm4
		aesimc				xmm5,		xmm5
		aesimc				xmm6,		xmm6
		aesimc				xmm7,		xmm7
		aesimc				xmm8,		xmm8
		aesimc				xmm9,		xmm9
		ret

aes_decoding:
		movdqu		xmm14,		oword[rdi]	; data
		movdqu		xmm0,		oword[rsi]	; key

		call		aes_round_keys
		call		aes_round_keys_rev

		pxor		xmm14,		xmm10		; First xor
		aesdec		xmm14,		xmm9		; Round 1
		aesdec		xmm14,		xmm8		; Round 2
		aesdec		xmm14,		xmm7		; Round 3
		aesdec		xmm14,		xmm6		; Round 4
		aesdec		xmm14,		xmm5		; Round 5
		aesdec		xmm14,		xmm4		; Round 6
		aesdec		xmm14,		xmm3		; Round 7
		aesdec		xmm14,		xmm2		; Round 8
		aesdec		xmm14,		xmm1		; Round 9
		aesdeclast	xmm14,		xmm0		; Round 10

		movdqu		oword[rdi],	xmm14
		ret

aes_encoding:
		movdqu		xmm14,		oword[rdi]	; data
		movdqu		xmm0,		oword[rsi]	; key

		call		aes_round_keys

		pxor		xmm14,		xmm0		; Whitening step (Round 0)
		aesenc		xmm14,		xmm1		; Round 1
		aesenc		xmm14,		xmm2		; Round 2
		aesenc		xmm14,		xmm3		; Round 3
		aesenc		xmm14,		xmm4		; Round 4
		aesenc		xmm14,		xmm5		; Round 5
		aesenc		xmm14,		xmm6		; Round 6
		aesenc		xmm14,		xmm7		; Round 7
		aesenc		xmm14,		xmm8		; Round 8
		aesenc		xmm14,		xmm9		; Round 9
		aesenclast	xmm14,		xmm10		; Round 10

		movdqu		oword[rdi],	xmm14
		ret
