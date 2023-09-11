NAME=circuit

OS = $(shell uname)
ARCH = $(shell uname -m)

SRC_DIR     :=	src

SRCS        :=	main.c \
			   	array.c

INCS = ./include

SRCS        := $(SRCS:%=$(SRC_DIR)/%)

BUILD_DIR   := .build
OBJS        := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS        := $(OBJS:.o=.d)

LIB_DIR = 
LIBS = 

CC          :=cc
CBFLAGS		:=-g3 -fsanitize=address,undefined $(DFLAGS)
CFLAGS      :=-Wall -Wextra -Werror $(CBFLAGS) 
CPPFLAGS    :=$(addprefix -I,$(INCS)) -MMD -MP
LDFLAGS     :=$(addprefix -L,$(dir $(LIBS_TARGET))) $(CBFLAGS)
LDLIBS      :=$(addprefix -l,$(LIBS))

all: $(NAME)

$(NAME): $(LIBS) $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(NAME)
	$(call print_linking, $(NAME))

$(LIBS):
	$(MAKE) -C $(@D)

$(OBJS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(DIR_DUP)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	@$(call print_obj,$@)

# cleans only the project.
clean:
	$(RM) $(OBJS) $(DEPS)
	$(call print_clean,$(addsuffix \n,$(OBJS)))

#  lib clean, clean all library objects.
lclean:
	for f in $(dir $(LIBS)); do echo "${GREEN}Cleaning: ${CYAN} $$f ${NC} $$"; $(MAKE) -C $$f clean; done

# full clean, clean all objects and libraries and binaries
fclean: clean
	for f in $(dir $(LIBS)); do $(MAKE) -C $$f fclean; done
	$(RM) $(NAME)
	$(call print_fclean,$(NAME))

re: fclean all


.PHONY: re fclean clean lclean all

export DFLAGS
export print_linking
export print_fclean
export print_clean
export print_target
export print_obj

# COLORS
export GREEN = \033[1;32m
export YELLOW = \033[0;33m
export BLUE = \033[1;34m
export CYAN = \033[1;36m
export NC = \033[0m
