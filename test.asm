					global		main

					section		.text
main:	
					push		rbp
					mov			rbp,			rsp

					and			rsp,			0xfffffffffffffff0
					sub			rsp,			24

					jmp			rax

					mov			rsp,			rbp
					pop			rbp
					ret

