NAME	=	aes_test
SRCS	=	aes.c \
			aes_fd.c \
			aes_test.c \
			aes_utils.c \
			../test_utils.c \
			../utils.c
ASM		=	asm/aes_decode_bloc.o \
			asm/aes_sub.o \
			asm/aes_shift.o \
			asm/aes_add_rkeys.o \
			asm/aes_mix_columns.o \
			asm/aes_data_dec.o \
			asm/aes_get_rnb.o \
			asm/aes_round_keys.o
OBJS	=	$(SRCS:.c=.o)
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -no-pie
RM		=	rm -rf

all		:	$(NAME)

$(NAME)	:	$(OBJS)
			make -C asm
			$(CC) $(CFLAGS) $^ $(ASM) -o $@

%.o	:	%.c
			$(CC) $(CFLAGS) -c $< -o $@

clean	:
			$(RM) $(OBJS)

fclean	:	clean
			$(RM) $(NAME)
			make -C asm fclean

re		:	fclean all
