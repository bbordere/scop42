CXX = clang++
CC = clang

CXXFLAGS = -g3 -std=c++17 -I includes/ -I external/  -MMD -MP -march=native \
			-Wall -Wextra -Werror
CFLAGS = -Wall -Wextra -Werror
 
SRCS_DIR = srcs/
OBJ_DIR = obj/
DEP_DIR = deps/

FILES = $(SRCS_DIR)main.cpp $(SRCS_DIR)BmpImage.cpp $(SRCS_DIR)App.cpp\
		$(SRCS_DIR)Shader.cpp $(SRCS_DIR)Utils.cpp $(SRCS_DIR)File3D.cpp\
		$(SRCS_DIR)Camera.cpp $(SRCS_DIR)Face.cpp $(SRCS_DIR)Light.cpp\
		$(SRCS_DIR)KeyManager.cpp $(SRCS_DIR)SkyBox.cpp $(SRCS_DIR)Object.cpp\
		$(SRCS_DIR)ShadowMap.cpp $(SRCS_DIR)Texture.cpp $(SRCS_DIR)BoundingBox.cpp

OBJS = $(patsubst $(SRCS_DIR)%.cpp, $(OBJ_DIR)%.o, $(FILES))
DEPS = $(patsubst $(SRC_DIR)%.cpp, $(DEP_DIR)%.d, $(notdir $(FILES)))

$(OBJ_DIR)%.o: $(SRCS_DIR)%.cpp
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)
	$(CXX) $(CXXFLAGS) -MF $(patsubst $(OBJ_DIR)%.o, $(DEP_DIR)%.d, $@) -c $< -o $@

NAME = scop

all: glad_lib $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(OBJ_DIR)glad.o -lglfw -ldl -lGL -o $(NAME) -lGLU

glad_lib:
	@mkdir -p $(OBJ_DIR)
	$(CC) external/glad/glad.c $(CFLAGS) -c -o $(OBJ_DIR)glad.o


clean:
	rm -rf $(OBJ_DIR) $(DEP_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re glad_lib
