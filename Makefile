NAME = my_shell
CC = gcc
CFLAGS = -Wall -Werror -Wextra
SRC = *.c 
SRC:=(addprefix src/, $(SRC))
OBJ = $(SRC:.c=.o)
OBJ_DIR = obj
LIB_DIR = lib
LIBFT = $(LIB_DIR)/libft.a
PRINTF = $(LIB_DIR)/libprintf.a
OBJ := $(addprefix $(OBJ_DIR)/, $(notdir $(OBJ)))
RM = rm -f

all: $(NAME)

$(LIBFT):
	make -C $(LIB_DIR)/libft

$(PRINTF):
	make -C $(LIB_DIR)/libprintf

$(NAME): $(OBJ_DIR) $(OBJ) $(LIBFT) $(PRINTF)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIB_DIR) $(LIBFT) $(PRINTF) -o $(NAME)

$(OBJ_DIR):
	ifndef $(OBJ_DIR)
		mkdir -p $(OBJ_DIR)
	endif

:$(OBJ_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
	if [ -d $(OBJ_DIR) ]; then rmdir $(OBJ_DIR); fi

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re