NAME = my_shell
CC = gcc
CFLAGS = -Wall -Werror -Wextra
SRC = main.c \
	  parser.c \

SRC:=$(addprefix src/, $(SRC))
OBJ = $(SRC:.c=.o)
OBJ_DIR = obj
LIB_DIR = ./lib
LIBFT_DIR = $(LIB_DIR)/libft
PRINTF_DIR = $(LIB_DIR)/ft_printf
LIBFT = $(LIBFT_DIR)/libft.a
PRINTF = $(PRINTF_DIR)/libftprintf.a
OBJ:=$(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRC))
RM = rm -f

all: $(NAME)

$(LIBFT):
	@echo "Compiling Libft..."
	@$(MAKE) -C $(LIBFT_DIR) > /dev/null
	@echo "✓ Libft compiled."

$(PRINTF):
	@echo "Compiling ft_printf..."
	@$(MAKE) -C $(PRINTF_DIR) > /dev/null
	@echo "✓ ft_printf compiled."

$(NAME): $(LIBFT) $(PRINTF) $(OBJ)
	@echo "Linking object files..."
	@$(CC) $(OBJ) $(LIBFT) $(PRINTF) -o $(NAME)
	@echo "✓ $(NAME) created."

$(OBJ_DIR):
	@echo "Creating object directory..."
	@mkdir -p $(OBJ_DIR) > /dev/null
	@echo "✓ Object directory created."

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
	@echo "removing object files..."
	@if [ -d $(OBJ_DIR) ]; then rmdir $(OBJ_DIR); fi
	@$(MAKE) -C $(LIBFT_DIR) clean > /dev/null
	@$(MAKE) -C $(PRINTF_DIR) clean > /dev/null
	@echo "✓ Cleaned."

fclean: clean
	@echo "removing executable..."
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean > /dev/null
	@$(MAKE) -C $(PRINTF_DIR) fclean > /dev/null
	@echo "✓ Executable removed."

re: fclean all

.PHONY: all clean fclean re