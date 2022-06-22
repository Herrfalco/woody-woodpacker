NAME	=	aes_test
SRCS	=	aes_test.c
OBJS	=	$(SRCS:.c=.o)
AES		=	../aes.o \
			../aes_fd.o \
			../aes_utils.o \
			../../test_utils.o \
			../../utils.o \
			../asm/aes_decode_bloc.o \
			../asm/aes_sub.o \
			../asm/aes_shift.o \
			../asm/aes_add_rkeys.o \
			../asm/aes_mix_columns.o \
			../asm/aes_data_dec.o \
			../asm/aes_get_rnb.o \
			../asm/aes_round_keys.o
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -no-pie
RM		=	rm -rf

all		:	$(NAME)

$(NAME)	:	$(OBJS)
			make -C ..
			$(CC) $(CFLAGS) $^ $(AES) -o $@

%.o	:	%.c
			$(CC) $(CFLAGS) -c $< -o $@

clean	:
			$(RM) $(OBJS)

fclean	:	clean
			$(RM) $(NAME)
			make -C .. fclean

re		:	fclean all
