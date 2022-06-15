SRCS	=	aes_decode_bloc.asm \
			aes_sub_bytes.asm \
			aes_shift_rows.asm
OBJS	=	$(SRCS:.asm=.o)
CC		=	nasm
CFLAGS	=	-felf64
RM		=	rm -rf

all		:	$(OBJS)

%.o		:	%.asm
			$(CC) $(CFLAGS) $< -o $@

clean	:
			$(RM) $(OBJS)

fclean	:	clean

re		:	fclean all
