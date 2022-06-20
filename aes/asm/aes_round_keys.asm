					global		round_keys_asm

					section		.text
round_keys_asm:	
					push		rbp
					mov			rbp,				rsp

					push		rdi						; key @ 24
					push		rsi						; rkeys @ 16
					sub			rsp,				16	; i @ 8
														; tmp @ 0

					xor			rcx,				rcx
					mov			[rsp+8],			rcx
	.loop
					cmp			
					
					mov			rsp,				rbp
					pop			rbp
					ret

					section		.data
key_sz:				db			8
rcon:				ddw			0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,0x40000000,0x80000000,0x1b000000,0x36000000

