NAME = my_shell
CC = gcc
CFLAGS = -Wall -Werror -Wextra
SRC = main.c \
	  parser.c

BLT_DIR = builtins/
SRC += $(BLT_DIR)exit.c

LEXER_DIR = lexer/
SRC +=  $(LEXER_DIR)token_utils.c \
		$(LEXER_DIR)variable_utils.c \
		$(LEXER_DIR)var_extraction_utils.c \
		$(LEXER_DIR)word_utils.c \
		$(LEXER_DIR)lexer_words.c \
		$(LEXER_DIR)lexer_logical.c \
		$(LEXER_DIR)lexer_arrows.c \
		$(LEXER_DIR)lexer_quotes.c \
		$(LEXER_DIR)lexer_variables.c \
		$(LEXER_DIR)lexer_utils.c \
	    $(LEXER_DIR)lexer.c

PARSER_UTILS_DIR = parser/
SRC +=	$(PARSER_UTILS_DIR)command_utils.c \
		$(PARSER_UTILS_DIR)general_utils.c \
		$(PARSER_UTILS_DIR)heredoc_utils.c \
		$(PARSER_UTILS_DIR)node_utils.c \
		$(PARSER_UTILS_DIR)redirect_utils.c \
		$(PARSER_UTILS_DIR)parser_expansion.c \
		$(PARSER_UTILS_DIR)parser_heredoc.c \
		$(PARSER_UTILS_DIR)parser_special.c \
		$(PARSER_UTILS_DIR)parser_syntax.c \
		$(PARSER_UTILS_DIR)parser_utils.c \
		$(PARSER_UTILS_DIR)parser_command.c \
		$(PARSER_UTILS_DIR)parser_errors.c \
		$(PARSER_UTILS_DIR)parser_build.c \
		$(PARSER_UTILS_DIR)syntax_checks.c \
		$(PARSER_UTILS_DIR)syntax_utils.c \

SIGNAL_UTILS_DIR = signals/
SRC += $(SIGNAL_UTILS_DIR)signals.c \
	   $(SIGNAL_UTILS_DIR)signals_setup.c

DISPLAY_DIR = display/
SRC += $(DISPLAY_DIR)banner.c \
	   $(DISPLAY_DIR)prompt.c

DEBUG_DIR = debug/
SRC += $(DEBUG_DIR)lexer_debug.c \
	   $(DEBUG_DIR)parser_debug.c \
	   $(DEBUG_DIR)parser_debug_utils.c

UTILS_DIR = utils/
SRC += $(UTILS_DIR)exit_status.c

## OLD UTILS (commented for now)
## SRC += $(UTILS_DIR)string_utils.c \
	   $(UTILS_DIR)array_utils.c \
	   $(UTILS_DIR)file_utils.c \
	   $(UTILS_DIR)memory_utils.c \
	   $(UTILS_DIR)linked_list_utils.c \
	   $(UTILS_DIR)input_utils.c \
	   $(UTILS_DIR)signal_utils.c \
	   $(UTILS_DIR)command_utils.c \
	   $(UTILS_DIR)execution_utils.c
## PARSER_DIR = parser/
## SRC += $(PARSER_DIR)lexer.c \
	   $(PARSER_DIR)tokenizer.c \
	   $(PARSER_DIR)syntax_analyzer.c \
	   $(PARSER_DIR)heredoc.c \
	   $(PARSER_DIR)expander.c \
	   $(PARSER_DIR)parser_utils.c
## EXECUTION_DIR = execution/
## SRC += $(EXECUTION_DIR)executor.c \
	   $(EXECUTION_DIR)redirection.c \
	   $(EXECUTION_DIR)pipeline.c \
	   $(EXECUTION_DIR)command_execution.c \
	   $(EXECUTION_DIR)execution_utils.c
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
	@$(CC) $(OBJ) $(LIBFT) $(PRINTF) -lreadline -o $(NAME)
	@echo "✓ $(NAME) created."

$(OBJ_DIR):
	@echo "Creating object directory..."
	@mkdir -p $(OBJ_DIR) > /dev/null
	@echo "✓ Object directory created."

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "removing object files..."
	@$(RM) -r $(OBJ_DIR)
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