NAME = minishell
CC = cc
CFLAGS = -Wall
SRC = 	src/main.c src/parser/tokenizer.c src/parser/tokenizer_utils.c src/parser/parser.c\
		src/parser/parser_utils.c src/parser/parser_utils_1.c src/utils/utils.c
OBJ = ${SRC:.c=.o}
LIBFT_DIR = libraries/libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a
all: $(NAME)
	@echo "Compiling..."

$(NAME): $(OBJ) $(LIBFT_LIB) $(LIBFT_DIR)/libft.h
	@$(CC) $(OBJ) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

$(LIBFT_LIB):
	@make -s -C $(LIBFT_DIR)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make -s clean -C $(LIBFT_DIR)
	@rm -f $(OBJ)

fclean:clean
	@make -s fclean -C $(LIBFT_DIR)
	@rm -f $(NAME)

re:fclean all

.PHONY: re fclean clean 