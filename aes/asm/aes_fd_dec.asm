					global		aes_fd_dec

					extern		round_keys_asm

					section		.text
aes_fd_dec:	
					push		rbp
					mov			rbp,				rsp

					push		rdi
					push		rsi
					push		rdx

					mov			rax,				qword[rkey_arr_sz]	;
					mov			rbx,				4					;
					mul			rbx										; r_keys	+1048
					add			rax,				qword[buff_sz]		;
					sub			rsp,				rax					; buff		+24

					sub			rsp,				24					; src_sz	+8
																		; write_sz	+0

					mov			rdi,				rdx
					mov			rsi,				rsp
					call		round_keys_asm

	.end:
					mov			rsp,				rbp
					pop			rbp
					ret

					section		.data
rkey_arr_sz:		dq			60
buff_sz:			dq			1024
