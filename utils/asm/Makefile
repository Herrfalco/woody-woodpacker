SRCS	=	str_len.asm \
			quit.asm \
			sat_sub.asm \
			seek.asm
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
