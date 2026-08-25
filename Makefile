NAME = taskSplitter
CC = gcc
CFLAGS = 
CPPFLAGS = -MMD
RM = rm -rf
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d $(addprefix $(NAME),.d))
LDLIBS = readline


all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJS) -o $(NAME) $(addprefix -l,$(LDLIBS))

clean:
	$(RM) $(OBJS) $(DEPS) $(NAME)

ifneq ($(findstring com.termux,$(PREFIX)),) # Verify if the environnement is termux (if "com.termux" is in $PREFIX)
install: all
	@echo "Installing (termux detected)"
	cp $(NAME) $(PREFIX)/bin
	
uninstall:
	@echo "Uninstalling (termux detected)"
	rm $(PREFIX)/bin/$(NAME)
else 
install: all
	@echo "Installing"
	cp $(NAME) /bin
uninstall:
	@echo "Uninstalling"
	rm /bin/$(NAME)
endif

-include $(DEPS)

.PHONY: all clean install uninstall
