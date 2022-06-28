NAME	=	lzw_test
SRCS	=	lzw.c \
			dico.c \
			lzw_test.c \
			data_rw.c \
			../utils/test_utils.o

ASM		=	asm/new_entry.o \
			asm/find_first_pattern.o \
			asm/entry_writer.o \
			asm/unlzw.o \
			asm/unlzw_chunk.o \
			asm/value_reader.o \
			asm/file_writer.o \
			asm/file_reader.o \
			../utils/asm/get_fd_size.o \
			../utils/asm/seek.o \
			../utils/asm/str_len.o \
			../utils/asm/quit.o \
			../utils/asm/sat_sub.o \
			../utils/asm/b_zero.o 

OBJS	=	$(SRCS:.c=.o)

CC		=	gcc

CFLAGS	=	-Wall -Wextra -Werror -no-pie

RM		=	rm -rf

all		:	$(NAME)

$(NAME)	:	$(OBJS)
			make -C ../utils
			make -C asm
			$(CC) $(CFLAGS) $^ $(ASM) -o $@

%.o		:	%.c
			$(CC) $(CFLAGS) -c $< -o $@

clean	:
			$(RM) $(OBJS)

fclean	:	clean
			make -C ../utils fclean
			make -C asm fclean
			$(RM) $(NAME)

re		:	fclean all
