NAME_NM = ft_nm

NAME_OTOOL = ft_otool

CC = gcc

FLAGS =  -g

INC = -I include

SRC_NAME_NM =	fill_section.c \
				find_order.c \
				handle.c \
				nm.c \
				print.c \
				print_output.c \
				quick_sort.c \
				tools.c \
				tools2.c \
				tools3.c

SRC_NAME_OTOOL =	handle_otool.c \
					otool.c \
					print_otool.c \
					print.c \
					section_otool.c \
					tools.c \
					tools2.c \
					tools3.c

OBJ_PATH = obj/

SRC_PATH = src/

OBJ_NAME_NM = $(SRC_NAME_NM:.c=.o)

OBJ_NAME_OTOOL = $(SRC_NAME_OTOOL:.c=.o)

SRC_NM = $(addprefix $(SRC_PATH), $(SRC_NAME_NM))

SRC_OTOOL = $(addprefix $(SRC_PATH), $(SRC_NAME_OTOOL))

OBJ_NM = $(addprefix $(OBJ_PATH),$(OBJ_NAME_NM))

OBJ_OTOOL = $(addprefix $(OBJ_PATH), $(OBJ_NAME_OTOOL))

all : $(NAME_OTOOL) $(NAME_NM)

$(NAME_NM): $(OBJ_PATH) $(OBJ_NM)
	$(CC) $(OBJ_NM) -o $(NAME_NM)
	@echo "\x1b[32;01m$@ SUCCESSFULLY CREATED !\x1b[32;00m"

$(NAME_OTOOL): $(OBJ_PATH) $(OBJ_OTOOL)
	$(CC) $(OBJ_OTOOL) -o $(NAME_OTOOL)
	@echo "\x1b[32;01m$@ SUCCESSFULLY CREATED !\x1b[32;00m"

$(OBJ_PATH):
	@mkdir -p $@

$(OBJ_PATH)%.o: $(SRC_PATH)%.c 
	$(CC) $(INC) $(FLAGS) -o $@ -c $< 

clean:
	@printf "%-50s" "deleting objects..." 
	@rm -rf $(OBJ_NM)
	@rm -rf $(OBJ_OTOOL)
	@rm -rf $(OBJ_PATH)
	@printf "\033[1;32m[OK]\033[0m\n"

fclean: clean
	@printf "%-50s" "deleting executable..." 
	@rm -rf $(NAME_NM)
	@rm -rf $(NAME_OTOOL)
	@printf "\033[1;32m[OK]\033[0m\n"

re: fclean all
