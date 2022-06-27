							global		find_first_pattern_asm
							section		.text

find_first_pattern_asm:
							push		rbp
							mov			rbp,						rsp

							xor			rdx,						rdx
							mov			dx,							di
							mov			rdi,						rdx

	.loop:
							mov			rax,						rdi
							cmp			rdi,						qword[dico_start]
							jl			.end

							sub			rdi,						qword[dico_start]
							mov			di,							word[rsi+rdi*4+8]
							jmp			.loop


	.end:
							mov			rsp,						rbp
							pop			rbp
							ret

							section		.data
dico_start:					dq			259
