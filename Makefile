NAME = minishell
CC = cc
CFLAGS = -Wall
SRC = src/parser/parser.c src/main.c \
	  libraries/libft/ft_strcat.c libraries/libft/ft_strlen.c libraries/libft/ft_split.c\
	  libraries/libft/ft_strchr.c libraries/libft/ft_strdup.c
OBJ = ${SRC:.c=.o}

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean:clean
	rm -f $(NAME)

re:fclean all

.PHONY: re fclean clean 