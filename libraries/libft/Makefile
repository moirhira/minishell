NAME = libft.a
SRC  = 	ft_split.c ft_strcat.c ft_strchr.c ft_strdup.c \
		ft_strlen.c  ft_strcmp.c ft_strlcpy.c\
		ft_isalnum.c ft_substr.c ft_strjoin.c ft_calloc.c\
		ft_bzero.c ft_strncmp.c
OBJ  = $(SRC:.c=.o)
CC   = cc
FLAGS = -Wall

all : $(NAME)

$(NAME): $(OBJ)
	@ar rcs $(NAME) $(OBJ)

%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@

clean : 
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re : clean fclean all
