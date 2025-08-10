NAME = minishell
CC = cc
CFLAGS = -Wall -g
SRC = 	src/main.c src/utils/utils.c src/utils/utils_2.c src/utils/setup_signals.c src/utils/display_error.c \
		src/parser/tokinizition/tokenizer.c src/parser/tokinizition/tokenizer_utils.c src/parser/tokinizition/qouted_command.c src/parser/tokinizition/simple_command.c src/parser/tokinizition/simple_command_env.c src/parser/tokinizition/operator_command.c src/parser/tokinizition/heredoc_command.c \
		src/parser/parsing/parser.c src/parser/parsing/parser_utils.c src/parser/parsing/parser_utils_1.c src/parser/parsing/parse_heredocs.c src/parser/parsing/validate_syntaxe.c\
      	src/executor/builtins/builtin_cd.c src/executor/builtins/builtin_cmd.c src/executor/builtins/builtin_echo.c src/executor/builtins/builtin_pwd.c src/executor/builtins/builtin_export_sorted_env.c\
		src/executor/builtins/builtin_exit.c src/executor/builtins/builtin_export.c  src/executor/builtins/builtin_env.c src/executor/builtins/builtin_unset.c\
	  	src/executor/setup_redirections.c src/executor/externals/externals.c src/executor/externals/find_command_in_path.c  src/executor/executor.c \
		src/executor/pipe/pipeline.c src/executor/pipe/executor_pipe_helpers.c src/executor/env_operations.c \

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
