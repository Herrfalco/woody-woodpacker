				global sc
				global sc_end
				global sc_data
				global sc_data_end
				global sc_entry
				global sc_old_entry

sc:
				push	rdi
				push	rsi
				push	rdx

   				mov		rdi,		1
   				lea		rsi,		[rel sc_msg]
   				mov		rdx,		sc_msg_end - sc_msg
   				mov		rax,		1
   				syscall

				lea		rax,		[rel sc]
				sub		rax,		qword[rel sc_entry]
				add		rax,		qword[rel sc_old_entry]

				pop		rdx
				pop		rsi
				pop		rdi

				jmp		rax
sc_end:

sc_data:
sc_entry:
				dq		0
sc_old_entry:
				dq		0
sc_msg:
				db		"....WOODY....", 10
sc_msg_end:
sc_data_end:
