CXX = clang++

CXXFLAGS = -g3 -std=c++17 -I includes/ -MMD -MP -march=native
 
SRCS_DIR = srcs/
OBJ_DIR = obj/
DEP_DIR = deps/

FILES = $(SRCS_DIR)main.cpp $(SRCS_DIR)BmpImage.cpp $(SRCS_DIR)App.cpp\
		$(SRCS_DIR)Shader.cpp $(SRCS_DIR)Utils.cpp $(SRCS_DIR)Model.cpp\
		$(SRCS_DIR)Camera.cpp $(SRCS_DIR)Face.cpp $(SRCS_DIR)Light.cpp\
		$(SRCS_DIR)KeyManager.cpp


OBJS = $(patsubst $(SRCS_DIR)%.cpp, $(OBJ_DIR)%.o, $(FILES))
DEPS = $(patsubst $(SRC_DIR)/%.cpp, $(DEP_DIR)/%.d, $(FILES))

$(OBJ_DIR)%.o: $(SRCS_DIR)%.cpp
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)
	$(CXX) $(CXXFLAGS) -MF $(patsubst $(OBJ_DIR)%.o, $(DEP_DIR)%.d, $@) -c $< -o $@

NAME = scop

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) -lglfw -lGL -lGLEW -o $(NAME) -lGLU

clean:
	rm -rf $(OBJ_DIR) $(DEP_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re