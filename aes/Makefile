NAME	=	aes_test
SRCS	=	aes.c \
			aes_fd.c \
			aes_steps.c \
			aes_test.c \
			aes_utils.c \
			../test_utils.c \
			../utils.c
ASM		=	asm/aes_decode_bloc.o \
			asm/aes_sub_bytes.o \
			asm/aes_shift_rows.o
OBJS	=	$(SRCS:.c=.o)
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -no-pie
RM		=	rm -rf
GARB	=	.rand \
			.crypt \
			.out

all		:	$(NAME)

$(NAME)	:	$(OBJS)
			make -C asm
			$(CC) $(CFLAGS) $^ $(ASM) -o $@

%.o	:	%.c
			$(CC) $(CFLAGS) -c $< -o $@

clean	:
			$(RM) $(OBJS)

fclean	:	clean
			$(RM) $(NAME) $(GARB)
			make -C asm fclean

re		:	fclean all
