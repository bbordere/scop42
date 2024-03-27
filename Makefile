CXX = c++
CC = cc

CXXFLAGS = -g3 -std=c++17 -I includes/ -I external/ -MMD -MP -march=native
CXXFLAGS += -Wall -Wextra -Werror -Wpedantic -Wuninitialized -O3

CFLAGS = -Wall -Wextra -Werror

SRCS_DIR = srcs/
OBJ_DIR = obj/
DEP_DIR = deps/

FILES = $(SRCS_DIR)main.cpp $(SRCS_DIR)BmpImage.cpp $(SRCS_DIR)App.cpp\
		$(SRCS_DIR)Shader.cpp $(SRCS_DIR)Utils.cpp $(SRCS_DIR)File3D.cpp\
		$(SRCS_DIR)Camera.cpp $(SRCS_DIR)Face.cpp $(SRCS_DIR)Light.cpp\
		$(SRCS_DIR)KeyManager.cpp $(SRCS_DIR)SkyBox.cpp $(SRCS_DIR)Object.cpp\
		$(SRCS_DIR)ShadowMap.cpp $(SRCS_DIR)Texture.cpp $(SRCS_DIR)BoundingBox.cpp\

OBJS = $(patsubst $(SRCS_DIR)%.cpp, $(OBJ_DIR)%.o, $(FILES))

DEPS = $(patsubst $(SRC_DIR)%.cpp, $(DEP_DIR)%.d, $(notdir $(FILES)))

$(OBJ_DIR)%.o: $(SRCS_DIR)%.cpp
	@printf "$(YELLOW)Compiling file: %s$(RESET)" $(notdir $<)
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)
	@$(CXX) $(CXXFLAGS) -MF $(patsubst $(OBJ_DIR)%.o, $(DEP_DIR)%.d, $@) -c $< -o $@
	@printf "$(CLR_L)"

NAME = scop

CLR_L = \33[2K\r
BLUE = \033[34m
RESET = \033[0m
GREEN =	\033[32m
YELLOW = \033[33m

all: $(NAME)

$(NAME): $(OBJS)
	printf "$(YELLOW)Compiling executable ...$(RESET)"
	$(CC) external/glad/glad.c $(CFLAGS) -c -o $(OBJ_DIR)glad.o
	$(CXX) $(OBJS) $(CXXFLAGS) $(OBJ_DIR)glad.o -lglfw -ldl -lGL -o $(NAME)
	printf "$(CLR_L)"
	printf "$(GREEN)Compilation complete !\n"
	printf "$(BLUE)"
	printf "   ____            \n"
	printf "  / __/______  ___ \n"
	printf " _\ \/ __/ _ \/ _ \ \n"
	printf "/___/\__/\___/ .__/\n"
	printf "            /_/    \n"
	printf "$(RESET)"


clean:
	rm -rf $(OBJ_DIR) $(DEP_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re

.SILENT: clean fclean $(NAME)

