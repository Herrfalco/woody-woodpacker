						global		unlzw_chunk_asm
						extern		value_reader
						extern		file_writer
						extern		quit_2_fd_asm
						extern		b_zero_asm
						extern		entry_writer_asm
						extern		new_entry_asm
						extern		find_first_pattern_asm

						section		.text

unlzw_chunk_asm:
						push		rbp
						mov			rbp,				rsp

						push		rdi							; dst			+15400
						push		rsi							; src			+15392
						push		rdx							; *r_buff		+15384
						push		rcx							; *w_buff		+15376
																;
																;
																; init			+15361
																; last_value	+15359
																; value			+15357
																; dico.bits		+15356
																; dico.entry	+8
						sub			rsp,				15376	; dico.size		+0

						mov			rdi,				rsp
						mov			rsi,				15376
						call		b_zero_asm

						mov			byte[rsp+15356],	9

	.loop:
						mov			rdi,				qword[rsp+15392]
						lea			rsi,				[rsp+15357]
						xor			rdx,				rdx
						mov			dl,					byte[rsp+15356]
						mov			rcx,				qword[rsp+15384]
						call		value_reader

						cmp			rax,				-1
						je			.read_err

						cmp			word[rsp+15361],	0
						je			.init

						xor			r8,					r8
						mov			r8w,				word[rsp+15357]
						cmp			r8,			 		qword[incr_code]
						je			.incr

						cmp			r8,					qword[reset_code]
						je			.flush

						cmp			r8,					qword[stop_code]
						je			.flush

						xor			r8,					r8
						mov			r8w,				word[rsp+15357]
						mov			r9,					qword[dico_start]
						add			r9,					qword[rsp]
						cmp			r8,					r9
						je			.in_dico
						ja			.corrupted
						
						mov			rdi,				qword[rsp+15400]
						xor			rsi,				rsi
						mov			si,					word[rsp+15357]
						lea			rdx,				[rsp]
						mov			rcx,				qword[rsp+15376]
						call		entry_writer_asm

						cmp			rax,				0
						jl			.write_err

						xor			rdi,				rdi
						mov			di,					word[rsp+15359]
						mov			rsi,				rax
						lea			rdx,				[rsp]
						call		new_entry_asm
						jmp			.end_of_loop

		.init:
						mov			rdi,				qword[rsp+15400]
						xor			rsi,				rsi
						mov			si,					word[rsp+15357]
						lea			rdx,				[rsp]
						mov			rcx,				qword[rsp+15376]
						call		entry_writer_asm

						cmp			rax,				0
						jl			.write_err
	
						mov			word[rsp+15361],	1
						jmp			.end_of_loop

		.incr:
						inc			byte[rsp+15356]
						xor			r8,					r8
						mov			r8w,				word[rsp+15359]
						mov			word[rsp+15357],	r8w
						jmp			.end_of_loop

		.flush:
						mov			rdi,				qword[rsp+15400]
						xor			rsi,				rsi
						mov			rdx,				2
						mov			rcx,				qword[rsp+15376]
						call		file_writer

						cmp			rax,				-1
						je			.write_err

						xor			rax,				rax
						mov			ax,					word[rsp+15357]
						jmp			.end

		.in_dico:
						xor			rdi,				rdi
						mov			di,					word[rsp+15359]
						lea			rsi,				[rsp]
						call		find_first_pattern_asm

						xor			rdi,				rdi
						mov			di,					word[rsp+15359]
						xor			rsi,				rsi
						mov			si,					ax
						lea			rdx,				[rsp]
						call		new_entry_asm
						
						mov			rdi,				qword[rsp+15400]
						xor			rsi,				rsi
						mov			si,					word[rsp+15357]
						lea			rdx,				[rsp]
						mov			rcx,				qword[rsp+15376]
						call		entry_writer_asm

						cmp			rax,				-1
						je			.write_err

						jmp			.end_of_loop

		.end_of_loop:
						mov			r8w,				word[rsp+15357]
						mov			word[rsp+15359],	r8w
						mov			word[rsp+15357],	0
						jmp			.loop

	.read_err:
						mov			rdx,				read_err_msg
						jmp			.err

	.write_err:
						mov			rdx,				write_err_msg
						jmp			.err

	.corrupted:
						mov			rdx,				corrupted_err_msg

	.err:
						mov			rdi,				qword[rsp+15392]
						mov			rsi,				qword[rsp+15400]
						call		quit_2_fd_asm

	.end:
						mov			rsp,				rbp
						pop			rbp
						ret


					section		.data
read_err_msg:		db			"can't read from source file", 0
write_err_msg:		db			"can't write to destination file", 0
corrupted_err_msg:	db			"corrupted file", 0
reset_code:			dq			256
stop_code:			dq			257
incr_code:			dq			258
dico_start:			dq			259
