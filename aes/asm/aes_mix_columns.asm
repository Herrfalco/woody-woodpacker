					global		mix_columns_asm

					section		.text
mix_columns_asm:	
					push		rbp
					mov			rbp,				rsp

					sub			rsp,				16
					mov			rdx,				mix_mat
					mov			rdx,				[rdx+rsi*16]

					xor			r8,					r8
	.for_1
					cmp			r8,					4
					je			.end

					xor			r9,					r9
		.for_2
					cmp			r9,					16
					je			.for_1_inc

					xor			r10,				r10
			.for_3
					cmp			r10,				4
					je			.for_2_inc

					mov			al,					r9
					add			al,					r10
					cmp			byte[rdx+al],		6
					je			.eq


						
					jmp			.eq_end
					.eq

					mov			al,					rdx
					add			al,					:

					.eq_end


			.for_3_inc
					inc			r10,
					jmp			.for_3

		.for_2_inc
					add			r9,					4
					jmp			.for_2

	.for_1_inc
					inc			r8
					jmp			.for_1



	.end:
					add			rsp,				16
					pop			rbp
					ret

					section		.data
mix_mat:			db			0,1,6,6,6,0,1,6,6,6,0,1,1,6,6,0,5,3,4,2,2,5,3,4,4,2,5,3,3,4,2,5
