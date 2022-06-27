					global		unlzw_asm
					extern		unlzw_chunk_asm
					extern		get_fd_size_asm
					extern		quit_2_fd_asm
					extern		b_zero_asm
					section		.text

unlzw_asm:
					push		rbp
					mov			rbp,				rsp
		
					push		rdi							;	dst				+2136
					push		rsi							;	src				+2128
															;	file_sz			+2106
															;	w_buff.bytes[]	+1082
															;	w_buff.b_size	+1074
															;	w_buff.idx		+1066
															;	w_buff.dword	+1062
															;	w_buff.dw_size	+1054
															;	w_buff.error	+1053
															;	r_buff.bytes[]	+29
															;	r_buff.b_size	+21
															;	r_buff.idx		+13
															;	r_buff.dword	+9
															;	r_buff.dw_size	+1
					sub			rsp,				2128	;	r_buff.error	+0

					mov			rdi,				rsp
					mov			rsi,				2128
					call		b_zero_asm

					mov			rdi,				[rsp+2128]
					call		get_fd_size_asm

					mov			qword[rsp+2106],	rax
					cmp			rax,				0
					jl			.clean_exit

					cmp			rax,				1
					jl			.end

	.loop:
					mov			rdi,				qword[rsp+2136]
					mov			rsi,				qword[rsp+2128]
					lea			rdx,				qword[rsp]
					lea			rcx,				qword[rsp+1053]
					call		unlzw_chunk_asm

					cmp			rax,				qword[stop_code]
					je			.end

					jmp			.loop


	.clean_exit:
					mov			rdi,				qword[rsp+2136]
					mov			rsi,				qword[rsp+2128]
					mov			rdx,				file_sz_err
					call		quit_2_fd_asm
					
	.end:
					mov			rsp,				rbp
					pop			rbp
					ret

					section		.data
file_sz_err:		db			"can't get file size", 0
stop_code:			dq			257
