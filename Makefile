NAME = minishell
CC = cc
CFLAGS = -Wall
SRC = src/main.c
OBJ = ${SRC:.c=.o}
LIBFT_DIR = libraries/libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a
all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_LIB)
	$(CC) $(OBJ) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

$(LIBFT_LIB):
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make clean -C $(LIBFT_DIR)
	rm -f $(OBJ)

fclean:clean
	make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re:fclean all

.PHONY: re fclean clean 