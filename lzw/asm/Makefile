SRCS	=	new_entry.asm \
			find_first_pattern.asm

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

