						global		decode_block_asm

						extern		add_rkeys_asm
						extern		mix_columns
						extern		shift_rows_asm
						extern		sub_bytes_asm
						extern		get_rnb

						section		.text
decode_block_asm:	
						push		rbp
						mov			rbp,			rsp

						push		rdi					; block @ 24
						push		rsi					; rkeys @ 16
						sub			rsp,			16	; round @ 1
														; r_nb @ 0
						
						call		get_rnb
						mov			byte[rsp],		al
						mov			byte[rsp+1],	al
	.loop:					
						mov			al,				byte[rsp+1]
						cmp			al,				0
						je			.end

						dec			al
						mov			byte[rsp+1],	al

						mov			rdi,			[rsp+24]
						mov			rsi,			[rsp+16]
						xor			rdx,			rdx
						mov			dl,				byte[rsp+1]
						call		add_rkeys_asm

						mov			al,				byte[rsp+1]
						cmp			al,				0
						je			.end

						inc			al
						cmp			al,				byte[rsp]
						je			.no_mix
						
						mov			rdi,			[rsp+24]
						mov			rsi,			1
						call		mix_columns
	.no_mix:
						mov			rdi,			[rsp+24]
						mov			rsi,			1
						call		shift_rows_asm

						mov			rdi,			[rsp+24]
						mov			rsi,			1
						call		sub_bytes_asm

						jmp			.loop
	.end:				
						add			rsp,		32
						pop			rbp
						ret
