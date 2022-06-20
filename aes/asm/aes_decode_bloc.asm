						global		decode_block_asm

						extern		add_rkeys_asm
						extern		mix_columns_asm
						extern		shift_rows_asm
						extern		sub_bytes_asm

						section		.text
decode_block_asm:	
						push		rbp
						mov			rbp,			rsp

						push		rdi					; block @ 24
						push		rsi					; rkeys @ 16
						push		rdx					; r_nb @ 8
						sub			rsp,			8	; round @ 0
						
						mov			byte[rsp],		dl
	.loop:					
						mov			al,				byte[rsp+8]
						cmp			al,				0
						je			.end

						dec			al
						mov			byte[rsp+8],	al

						mov			rdi,			[rsp+24]
						mov			rsi,			[rsp+16]
						xor			rdx,			rdx
						mov			dl,				byte[rsp+8]
						call		add_rkeys_asm

						mov			al,				byte[rsp+8]
						cmp			al,				0
						je			.end

						inc			al
						cmp			al,				byte[rsp]
						je			.no_mix
						
						mov			rdi,			[rsp+24]
						mov			rsi,			1
						call		mix_columns_asm
	.no_mix:
						mov			rdi,			[rsp+24]
						mov			rsi,			1
						call		shift_rows_asm

						mov			rdi,			[rsp+24]
						mov			rsi,			1
						call		sub_bytes_asm

						jmp			.loop
	.end:				
						mov			rsp,		rbp
						pop			rbp
						ret
