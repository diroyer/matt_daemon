# MAKEFILE

DEBUG := 1

.DEFAULT_GOAL := all

.ONESHELL:

override threads := $(shell nproc --all)

# set make flags
override MAKEFLAGS += --warn-undefined-variables --no-builtin-rules \
					  --jobs=$(threads) \
					  --no-print-directory

#--output-sync=target \


override srcdir := src

override incdir := inc

override srcs	:= $(shell find $(srcdir) -type f -name "*.cpp")

override objs	:= $(srcs:.cpp=.o)

override deps	:= $(srcs:.cpp=.d)

override incs	:= $(shell find $(incdir) -type f -name "*.hpp")

override log	:= /var/log/matt_daemon/matt_daemon.log

override lock	:= /var/lock/matt_daemon.lock

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

override name := Matt_daemon



# -- S P E C I A L  T A R G E T S ---------------------------------------------


.PHONY: all clean fclean re log restart kill


# -- T A R G E T S ------------------------------------------------------------

all: $(compile_commands) $(name)

$(name): $(objs)
	@$(cxx) $^ -o $@ $(ldflags)
	echo 'linking' $@

$(compile_commands): $(srcs) Makefile
	@$(call generate_compile_commands)
	echo 'generating' $@

-include $(deps)
%.o: %.cpp Makefile
	@$(cxx) $(depsflags) $(cxxflags) -c $< -o $@
	echo 'compiling' $<

clean:
	@rm -vf $(objs) $(deps) $(compile_commands)

fclean: clean restart
	@rm -vrf $(log) $(lock) $(name) '.cache'

re: fclean all


log:
	@if [ ! -f $(log) ]; then
		echo 'log file not found'
	else
		tail -f $(log)
	fi

restart: kill
	@rm -vf $(lock) $(log)


signal := KILL

kill:
	@pid=$$(ps aux | grep $(name) | grep -v 'grep' | awk '{print $$2}')
	if [ -z "$$pid" ]; then
		echo 'process not found'
	else
		kill -s $(signal) $$pid
	fi


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
