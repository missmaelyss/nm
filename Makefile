NAME_NM = ft_nm

NAME_OTOOL = ft_otool

CC = gcc

FLAGS =  -g

INC = -I include

SRC_NAME_NM =	handle.c \
				nm.c \
				print_output.c \
				quick_sort.c \
				tools.c \
				tools2.c

SRC_NAME_OTOOL =

SRC_PATH = src/

SRC_NM = $(addprefix $(SRC_PATH),$(SRC_NAME_NM))

SRC_NAME_OTOOL = $(addprefix $(SRC_PATH),$(SRC_NAME_OTOOL))

OBJ_NAME_NM = $(SRC_NAME_NM:.c=.o)

OBJ_NAME_OTOOL = $(SRC_NAME_OTOOL:.c=.o)

OBJ_PATH = obj/

OBJ_NM = $(addprefix $(OBJ_PATH),$(OBJ_NAME_NM))

OBJ_OTOOL = $(addprefix $(OBJ_PATH),$(OBJ_NAME_OTOOL))

all : $(NAME_NM)

$(NAME_NM): $(OBJ_PATH) $(OBJ_NM)
	$(CC) $(OBJ_NM) -o $(NAME_NM)
	@echo "\x1b[32;01m$@ SUCCESSFULLY CREATED !\x1b[32;00m"


$(NAME_OTOOL): $(OBJ_PATH) $(OBJ_OTOOLNAME_OTOOL)
	$(CC) $(OBJ_OTOOLNAME_OTOOL) -o $(NAME_OTOOL)
	@echo "\x1b[32;01m$@ SUCCESSFULLY CREATED !\x1b[32;00m"

$(OBJ_PATH):
	@mkdir -p $@

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(INC) $(FLAGS) -o $@ -c $< $(INC)

clean:
	@printf "%-50s" "deleting objects..." 
	@rm -rf $(OBJ_NM)
	@rm -rf $(OBJ_PATH)
	@printf "\033[1;32m[OK]\033[0m\n"

fclean: clean
	@printf "%-50s" "deleting executable..." 
	@rm -rf $(NAME_NM)
	@printf "\033[1;32m[OK]\033[0m\n"

re: fclean all
