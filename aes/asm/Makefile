SRCS	=	aes_decode_bloc.asm \
			aes_sub.asm \
			aes_shift.asm \
			aes_add_rkeys.asm \
			aes_mix_columns.asm \
			aes_data_dec.asm \
			aes_get_rnb.asm \
			aes_round_keys.asm
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
