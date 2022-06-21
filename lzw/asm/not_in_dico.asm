						global		not_in_dico_asm
	
						extern		new_entry
						section		.text

not_in_dico_asm:
						push		rbp
						mov			rbp,				rsp
	
						cmp			rdi,				qword[max_byte]
						jg			.find_first_pattern
						mov			rdx,				rsi
						mov			rsi,				rdi
						call		new_entry
						jmp			.end

	.find_first_pattern:
						sub			rdi,				qword[dico_start]
						xor			r9,					r9
						mov			r9w,				word[rsi+rdi*4+8]
						cmp			r9,					qword[dico_start]
						jl			.else
						mov			rdi,				r9
						jmp			.find_first_pattern
	
	.else:				
						mov			rdx,				rsi
						mov			rsi,				r9
						add			rdi,				qword[dico_start]
						call		new_entry
						jmp			.end

	.end:
						mov			rsp,				rbp
						pop			rbp
						ret

						section		.data
max_byte:				dq			255
dico_start:				dq			259
