NAME=Philo
CC=cc
CFLAGS=-Wall -Werror -Wextra -g 
SRCS=srcs/main.c srcs/philo_utils.c srcs/instructions.c srcs/init.c srcs/struct_utils.c
OBJS=$(SRCS:.c=.o)
HEADER=srcs/philo.h

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "\n\nPhilo created"

%.o : %.c $(HEADER)
	@$(CC) $(CFLAGS) $< -c -o $@
	@echo -n '.....'

clean:
	@rm -f $(OBJS)
	@echo "All objects clean"

fclean: clean
	@rm -f $(NAME)
	@echo "And the executable too"

re: fclean all

.PHONY: all fclean clean re
