#Credits to https://github.com/TediCreations/GNUMakefile4TDD/blob/master/common.mk
#
################################################################################
#	  _______          _             _     _
#	 |__   __|        | |           (_)   | |
#	    | | ___   ___ | |   _____  ___ ___| |_ ___
#	    | |/ _ \ / _ \| |  / _ \ \/ / / __| __/ __|
#	    | | (_) | (_) | | |  __/>  <| \__ \ |_\__ \
#	    |_|\___/ \___/|_|  \___/_/\_\_|___/\__|___/
#
#

GPP_EXISTS :=		$(shell command -v g++ 2> /dev/null)
GDB_EXISTS :=		$(shell command -v gdb 2> /dev/null)
VALGRIND_EXISTS :=	$(shell command -v valgrind 2> /dev/null)
SED_EXISTS :=		$(shell command -v sed 2> /dev/null)
PERL_EXISTS :=		$(shell command -v perl 2> /dev/null)
GIT_EXISTS :=		$(shell command -v git 2> /dev/null)

#..............................................................................#
# Tools

ECHO ?=			echo
ECHO_N ?=		echo -n
MKDIR_P ?=		@mkdir -p
RM_FR ?=		@rm -fR
MV_F ?=			@mv -f
TOUCH ?=		touch
SED ?=			sed
# SHELL :=/bin/bash
################################################################################
#	 _    _ _   _ _ _ _   _
#	| |  | | | (_) (_) | (_)
#	| |  | | |_ _| |_| |_ _  ___  ___
#	| |  | | __| | | | __| |/ _ \/ __|
#	| |__| | |_| | | | |_| |  __/\__ \
#	 \____/ \__|_|_|_|\__|_|\___||___/
#
#

COLORS ?= YES
ifneq ($(COLORS),YES)
  ifneq ($(COLORS),NO)
    $(error "COLORS" variable can not be "$(COLORS)")
  endif
endif

ifeq ($(COLORS),NO)

  # Messages
  BUILD_SUCCESS ?=	@$(ECHO) "\tBuild successful"
  CLEANING ?=		@$(ECHO) "\tCleaning project\t"

  COMPILING ?=		@$(ECHO) "\tCompiling $<\t"
  BUILDING ?=		@$(ECHO) "\tBuilding  $@\t"
  RUNNING ?=		@$(ECHO) "\tRunning   $^"
  VERSION ?=		@$(ECHO) "\tVersion"
  DOCUMENTATION ?=	@$(ECHO) "\tDocumentation"


else

  # TPUT COLORS for Help function
  TPUT_GREEN =		$(shell tput -Txterm setaf 2)
  TPUT_WHITE =		$(shell tput -Txterm setaf 7)
  TPUT_YELLOW =		$(shell tput -Txterm setaf 3)
  TPUT_RESET =		$(shell tput -Txterm sgr0)


  RED =			@tput bold && tput -Txterm setaf 1
  GREEN =		@tput bold && tput -Txterm setaf 2
  YELLOW =		@tput bold && tput -Txterm setaf 3
  BLUE =		@tput bold && tput -Txterm setaf 4
  RESET =		tput -Txterm sgr0

  # Messages
  BUILD_SUCCESS ?=	$(GREEN)  && $(ECHO)   "\tBuild successful"	&& $(RESET)
  CLEANING ?=		$(RED)    && $(ECHO)   "\tCleaning project\t"	&& $(RESET)

  COMPILING ?=		$(YELLOW) && $(ECHO_N) "\tCompiling"		&& $(RESET) && $(ECHO) " $<\t"
  BUILDING ?=		$(YELLOW) && $(ECHO_N) "\tBuilding "		&& $(RESET) && $(ECHO) " $@\t"
  RUNNING ?=		$(YELLOW) && $(ECHO_N) "\tRunning  "		&& $(RESET) && $(ECHO) " $^"
  VERSION ?=		$(YELLOW) && $(ECHO)   "\tVersion"		&& $(RESET)
  DOCUMENTATION ?=	$(YELLOW) && $(ECHO)   "\tDocumentation"	&& $(RESET)

endif

#..............................................................................#

#..............................................................................#

# Add the following 'help' target to your Makefile
# And add help text after each target name starting with '\#\#'
# A category can be added with @category
# Credits to: https://gist.github.com/prwhite/8168133
HELP_FUNC := \
	%help; \
	while(<>) { \
		if(/^([a-z0-9_-]+):.*\#\#(?:@(\w+))?\s(.*)$$/) { \
			push(@{$$help{$$2}}, [$$1, $$3]); \
		} \
	}; \
	print "Usage: make $(TPUT_YELLOW)[target]$(TPUT_RESET)\n\n"; \
	for ( sort keys %help ) { \
		print "$(TPUT_WHITE)$$_:$(TPUT_RESET)\n"; \
		printf("  $(TPUT_YELLOW)%-20s$(TPUT_RESET) \
			$(TPUT_GREEN)%s$(TPUT_RESET)\n", $$_->[0], \
			$$_->[1]) for @{$$help{$$_}}; \
		print "\n"; \
	}
################################################################################
#	  _____           _           _
#	 |  __ \         (_)         | |
#	 | |__) | __ ___  _  ___  ___| |_
#	 |  ___/ '__/ _ \| |/ _ \/ __| __|
#	 | |   | | | (_) | |  __/ (__| |_
#	 |_|   |_|  \___/| |\___|\___|\__|
#	                _/ |
#	               |__/
#

# Name of the build output
#ifeq ($(EXEC),)
  #$(error "EXEC" is not specified)
#endif

# Project version
ifdef GIT_EXISTS
  PROJECT_NUMBER ?=	$(shell git describe --always --dirty=$" (with uncommitted changes)" --long --tags)
endif
#..............................................................................#
# Machine

ifdef SYSTEMROOT
  MACHINE =		win32
  $(error Windows compatibility is not verified)
else
  ifeq ($(shell uname), Linux)
    MACHINE =		posix
  else
    $(error Makefile does not support this OS)
  endif
endif

#..............................................................................#
# Directories

BIN_DIR ?=		bin/
OBJ_DIR ?=		obj/
INC_DIR ?=		inc/
SRC_DIR ?=		src/
DOC_DIR ?=		doc/
TMP_DIR ?=		tmp/
#LIB_DIR ?=		lib/
TESTS_DIR ?=	 	tests/

# Target name
#ifeq ($(TARGET_NAME),)
  #$(error "TARGET_NAME" is not specified)
#endif

#TARGET_DIR ?=		$(TARGET_NAME)_$(CONF)/
#HOST_DIR ?=		$(MACHINE)_$(CONF)/


# Object files
# TARGET_OBJS =		$(addprefix $(OBJ_DIR)$(TARGET_DIR),$(TARGET_CXX_SRCs:%.cpp=%.o))
# object files
OBJS1 = ${OBJ_DIR}mainLsh.o ${OBJ_DIR}FunctionH.o ${OBJ_DIR}util.o
OBJS2 = ${OBJ_DIR}mainHypercube.o ${OBJ_DIR}FunctionH.o ${OBJ_DIR}util.o
OBJS3 = ${OBJ_DIR}mainCurveLsh.o ${OBJ_DIR}FunctionH.o ${OBJ_DIR}util.o
OBJS4 = ${OBJ_DIR}mainCurveHypercube.o ${OBJ_DIR}FunctionH.o ${OBJ_DIR}util.o
OBJS5 = ${OBJ_DIR}mainCurveProjection.o ${OBJ_DIR}FunctionH.o ${OBJ_DIR}util.o
OBJS = $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5)

#OBJS = main.o Grid.o Hero.o Item.o Living.o Market.o Monster.o Spell.o Square.o
# create / compile the individual files >> separately < <
${OBJ_DIR}mainLsh.o : ${SRC_DIR}mainLsh.cpp
# 	${COMPILING}
	$(CC) $(CFLAGS) ${SRC_DIR}mainLsh.cpp -o ${OBJ_DIR}mainLsh.o

${OBJ_DIR}mainHypercube.o : ${SRC_DIR}mainHypercube.cpp
	${COMPILING}
	$(CC) $(CFLAGS) ${SRC_DIR}mainHypercube.cpp -o ${OBJ_DIR}mainCurveHypercube.o

${OBJ_DIR}mainCurveLsh.o : ${SRC_DIR}mainCurveLsh.cpp
	${COMPILING}
	$(CC) $(CFLAGS) ${SRC_DIR}mainCurveLsh.cpp -o ${OBJ_DIR}mainCurveLsh.o

${OBJ_DIR}mainCurveHypercube.o : ${SRC_DIR}mainCurveHypercube.cpp
	${COMPILING}
	$(CC) $(CFLAGS) ${SRC_DIR}mainCurveHypercube.cpp -o ${OBJ_DIR}mainCurveHypercube.o

${OBJ_DIR}mainCurveProjection.o : ${SRC_DIR}mainCurveProjection.cpp
	${COMPILING}
	$(CC) $(CFLAGS) ${SRC_DIR}mainCurveProjection.cpp -o ${OBJ_DIR}mainCurveProjection.o

${OBJ_DIR}FunctionH.o : ${SRC_DIR}FunctionH.cpp
	${COMPILING}
	$(CC) $(CFLAGS) ${SRC_DIR}FunctionH.cpp -o ${OBJ_DIR}FunctionH.o

${OBJ_DIR}util.o : ${SRC_DIR}util.cpp
	${COMPILING}
	$(CC) $(CFLAGS) ${SRC_DIR}util.cpp -o ${OBJ_DIR}util.o

################################################################################
#	 _____  _                                    _
#	|  __ \| |                                  | |
#	| |__) | |__   ___  _ __  _   _   _ __ _   _| | ___  ___
#	|  ___/| '_ \ / _ \| '_ \| | | | | '__| | | | |/ _ \/ __|
#	| |    | | | | (_) | | | | |_| | | |  | |_| | |  __/\__ \
#	|_|    |_| |_|\___/|_| |_|\__, | |_|   \__,_|_|\___||___/
#	                           __/ |
#	                          |___/
#
#

#..............................................................................#
#	Documentation

.PHONY: doc
doc: ##@doc Generates documentation.
doc: 
	@echo PROJECT_NUMBER =	${PROJECT_NUMBER} "Beta"
	@echo PROJECT_NAME =	"Algorithmic Data Science Project 1"
	@echo PROJECT_BRIEF =	""

.PHONY: help
help: ##@options Shows a list of all available make options.
ifndef PERL_EXISTS
	$(warning "Please set variable 'COLOR' to NO!")
endif
	@perl 			-e '$(HELP_FUNC)' $(MAKEFILE_LIST)

#..............................................................................#
#	Analysis

.PHONY: valgrind
valgrind: ##@analysis Valgrind dynamic analysis.
valgrind: all
ifndef VALGRIND_EXISTS
	$(error "Please install 'valgrind' dynamical analyser!")
endif
	$(error set executable)
	#@valgrind		$(BIN_DIR)$(HOST_DIR)$(EXEC)

.PHONY: todo
todo: ##@analysis Check for programmer notes in code.
	@egrep			-nr -Rw --color 'bug|BUG|Bug' $(SRC_DIR) $(INC_DIR)	|| true
	@egrep			-nr -Rw --color 'todo|TODO|Todo' $(SRC_DIR) $(INC_DIR) || true
	@egrep			-nr -Rw --color 'test|TEST|Test' $(SRC_DIR) $(INC_DIR) || true

#..............................................................................#
.PHONY: all
all: ##@build Creates all targets.
all: 
	@echo  Creates all targets $(TARGETS)
	$(error *** CAUTION: NOT ALL TARGETS IMPLEMENTED YET $(TARGETS) ***  )
	$(info *** Creating $(TARGETS) executables ***  )
	$(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5)

lsh: ##@build lsh with points.
lsh: $(OBJS1)
# 	@echo Creates $(TARGET1)
	$(info *** Creating lsh executable ***  )
	$(CC) -g $(OBJS1) -o $(OUT1)
	${BUILD_SUCCESS}

cube: ##@build cube with points.
cube: 
	@echo Creates $(TARGET2)
	$(OBJS2)
	$(info *** Creating cube executable ***  )
	$(CC) -g $(OBJS2) -o $(OUT2)

curvelsh: ##@build curve Lsh.
curvelsh: 
	@echo Creates $(TARGET3)
	$(OBJS3)
	$(error *** Creating Curve Grid Lsh executable ***  )
	$(CC) -g $(OBJS3) -o $(OUT3)

curvecube: ##@build curveCube.
curvecube: 
	@echo Creates $(TARGET4)
	$(OBJS4)
	$(error *** Creating Curve Grid HyperCube executable ***  )
	$(CC) -g $(OBJS4) -o $(OUT4)

curveprojection: ##@build curveProjection.
curveprojection:
	@echo Creates $(TARGET5)
	$(OBJS5)
	$(error *** Creating Curve Grid Projection executable ***  )
	$(CC) -g $(OBJS5) -o $(OUT5)

.PHONY: clean
clean:
	$(CLEANING)
	$(RM_FR)		$(OBJ_DIR)*.o
	$(RM_FR)		$(BIN_DIR)*

.PHONY: rebuild
rebuild: ##@build Rebuilds project.
rebuild: clean all

count:
	wc ${SRC_DIR}*.cpp ${INC_DIR}/*.h

# flags
CFLAGS = -g -c -Wall $(DEPS)

# general dependencies
DEPS = -I ./inc/

# main file names
TARGET1 = mainLsh
TARGET2 = mainHypercube
TARGET3 = mainCurveLsh
TARGET4 = mainCurveHypercube
TARGET5 = mainCurveProjection
TARGETS = $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5)

# out file name
OUT1 = ${BIN_DIR}lsh
OUT2 = ${BIN_DIR}cube
OUT3 = ${BIN_DIR}curve_grid_lsh
OUT4 = ${BIN_DIR}curve_grid_hypercube
OUT5 = ${BIN_DIR}curve_grid_projection
OUTS = $(OUT1) $(OUT2) $(OUT3) $(OUT4) $(OUT5)

# standard
STD = #-std=c98


