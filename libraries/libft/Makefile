NAME = libft.a
SRC  = ft_split.c ft_strcat.c ft_strchr.c ft_strdup.c ft_strlen.c split_token.c
OBJ  = $(SRC:.c=.o)
CC   = cc
FLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean : 
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re : clean fclean all
