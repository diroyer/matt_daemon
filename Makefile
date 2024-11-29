# MAKEFILE

DEBUG	:= 1

.DEFAULT_GOAL := all

.ONESHELL:

override srcdir := src

override incdir := inc

override srcs	:= $(shell find $(srcdir) -type f -name "*.cpp")

override objs	:= $(srcs:.cpp=.o)

override deps	:= $(srcs:.cpp=.d)

override incs	:= $(shell find $(incdir) -type f -name "*.hpp")

override log	:= /var/log/matt_deamon/matt_deamon.log

override lock	:= /var/lock/matt_deamon.lock

override cxx	:= ccache clang++

override std	:= -std=c++2a

ifeq ($(DEBUG), 1)
override opt	:= -O0
override dbg	:= -g -Wno-unused-parameter -Wno-unused-variable 
else
override opt	:= -O3
override dbg	:=
endif

override cxxflags	:= $(std) $(dbg) $(opt) -Wall -Wextra -Werror -Wpedantic -I$(incdir)

override ldflags	:=

override depsflags	= -MT $@ -MMD -MP -MF $*.d

override compile_commands := compile_commands.json

NAME := Matt_daemon

all: $(compile_commands) $(NAME)

$(NAME): $(objs)
	$(cxx) $^ -o $@ $(ldflags)

$(compile_commands): $(srcs) Makefile
	$(call generate_compile_commands)

-include $(deps)

%.o: %.cpp Makefile
	$(cxx) $(depsflags) $(cxxflags) -c $< -o $@

clean:
	rm -f $(objs) $(deps) $(compile_commands)

fclean: clean
	rm -f $(log) $(lock) $(NAME) 

re: fclean all

.PHONY: all clean fclean re

# -- F U N C T I O N S --------------------------------------------------------

define generate_compile_commands
	@echo '[' > $@
	for file in $(srcs); do
		echo '\t{\n\t\t"directory": "'$(CURDIR)'",' >> $@
		echo '\t\t"file": "'$$file'",' >> $@
		echo '\t\t"output": "'$${file%.cpp}'.o",' >> $@
		echo '\t\t"arguments": [' >> $@
		echo '\t\t\t"$(cxx)",' >> $@
		for flag in $(cxxflags); do
			echo '\t\t\t"'$$flag'",' >> $@
		done
		echo '\t\t\t"-c",\n\t\t\t"'$$file'",' >> $@
		echo '\t\t\t"-o",\n\t\t\t"'$${file%.cpp}'.o"\n\t\t]\n\t},' >> $@
	done
	truncate -s -2 $@
	echo '\n]' >> $@
endef
