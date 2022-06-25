					global		aes_fd_dec_asm

					extern		round_keys_asm
					extern		quit_2_fd_asm
					extern		sat_sub_asm
					extern		round_up_asm
					extern		get_rnb_asm
					extern		aes_data_dec_asm

					section		.text
aes_fd_dec_asm:	
					push		rbp
					mov			rbp,				rsp

					push		rdi										; fd_dst	+1304
					push		rsi										; fd_src	+1296
					push		rdx										; key		+1288

					mov			rax,				qword[rkey_arr_sz]	; r_keys	+1048
					mov			rbx,				4					; buff		+24
					xor			rdx,				rdx					;
					mul			rbx										;
					add			rax,				qword[buff_sz]		;
					sub			rsp,				rax					;

					sub			rsp,				24					; up_sz		+16
																		; src_sz	+8
																		; write_sz	+0

					mov			rdi,				qword[rsp+1288]
					lea			rsi,				[rsp+1048]
					call		round_keys_asm

					mov			rdi,				qword[rsp+1296]
					lea			rsi,				[rsp+8]
					mov			rdx,				8
					xor			rax,				rax
					syscall

					cmp			rax,				8				
					jl			.read_err
	.loop:
					cmp			qword[rsp+8],		0				
					je			.end

					mov			rdi,				qword[rsp+1296]
					lea			rsi,				[rsp+24]
					mov			rdx,				qword[buff_sz]
					xor			rax,				rax
					syscall

					cmp			rax,				0
					jl			.read_err
					je			.end

					mov			rax,				qword[buff_sz]
					mov			rbx,				qword[rsp+8]
					cmp			rbx,				rax
					jg			.max_sz

					mov			qword[rsp],			rbx
					jmp			.after_max_sz
	.max_sz:
					mov			qword[rsp],			rax
	.after_max_sz:
					mov			rdi,				qword[rsp]	
					mov			rsi,				16
					call		round_up_asm

					mov			qword[rsp+16],		rax

					mov			rdi,				qword[key_sz]
					call		get_rnb_asm

					mov			rcx,				rax
					lea			rdx,				[rsp+1048]
					mov			rsi,				qword[rsp+16]
					lea			rdi,				[rsp+24]
					call		aes_data_dec_asm

					mov			rdi,				qword[rsp+1304]
					lea			rsi,				[rsp+24]
					mov			rdx,				qword[rsp]
					mov			rax,				1
					syscall

					cmp			rax,				0
					jl			.write_err
					cmp			rax,				qword[rsp]
					jne			.write_err

					mov			rdi,				qword[rsp+8]
					mov			rsi,				qword[buff_sz]
					call		sat_sub_asm

					mov			qword[rsp+8],		rax

					jmp			.loop
	.end:
					mov			rsp,				rbp
					pop			rbp
					ret
	.read_err:
					mov			rdi,				qword[rsp+1296]
					mov			rsi,				qword[rsp+1304]
					mov			rdx,				read_err_msg
					call		quit_2_fd_asm
	.write_err:
					mov			rdi,				qword[rsp+1296]
					mov			rsi,				qword[rsp+1304]
					mov			rdx,				write_err_msg
					call		quit_2_fd_asm

					section		.data
key_sz:				dq			256
rkey_arr_sz:		dq			60
buff_sz:			dq			1024
read_err_msg:		db			"can't read from source file", 0
write_err_msg:		db			"can't write to destination file", 0
