NAME = minishell
CC = cc
CFLAGS = -Wall -g
SRC = src/main.c src/parser/tokenizer.c src/parser/tokenizer_utils.c src/parser/parser.c\
      src/parser/parser_utils.c src/parser/parser_utils_1.c src/utils/utils.c src/executor/builtin_cd.c src/executor/builtin_cmd.c src/executor/builtin_env.c\
      src/executor/builtin_export.c src/executor/builtin_pwd.c  src/executor/builtin_exit.c src/executor/builtin_echo.c src/executor/builtin_unset.c src/executor/find_command_in_path.c  src/executor/helper.c \
	  src/executor/setup_redirections.c src/executor/single_command.c src/executor/sort_in_tab.c src/executor/sorted_env.c src/executor/executor.c src/executor/parse_heredocs.c src/utils/setup_signals.c \
	  src/utils/utils_2.c src/executor/pipeline.c src/executor/executor_pipe_helpers.c

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
