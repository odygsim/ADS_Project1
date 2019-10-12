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
#	  ____              _____ _
#	 / ___| _     _    |  ___| | __ _  __ _ ___
#	| |   _| |_ _| |_  | |_  | |/ _` |/ _` / __|
#	| |__|_   _|_   _| |  _| | | (_| | (_| \__ \
#	 \____||_|   |_|   |_|   |_|\__,_|\__, |___/
#	                                  |___/
#

# Default compiler C++ flags
#
CXXFLAGS =		-std=c++11

#..............................................................................#
# Host C++ flags
HOST_CXXFLAGS =		$(CXXFLAGS)\
			$(USER_HOST_CXXFLAGS)\
			$(USER_CXXFLAGS)

#..............................................................................#
# Target C++ flags
TARGET_CXXFLAGS =	$(CXXFLAGS)\
			$(USER_TARGET_CXXFLAGS)\
			$(USER_CXXFLAGS)

################################################################################
#	 ____                   __                _____ _
#	|  _ \ _ __ ___ _ __   / /__  ___  _ __  |  ___| | __ _  __ _ ___
#	| |_) | '__/ _ \ '_ \ / / __|/ _ \| '__| | |_  | |/ _` |/ _` / __|
#	|  __/| | |  __/ |_) / /\__ \ (_) | |    |  _| | | (_| | (_| \__ \
#	|_|   |_|  \___| .__/_/ |___/\___/|_|    |_|   |_|\__,_|\__, |___/
#	               |_|                                      |___/
#

# Warnings and error notification
CPPFLAGS =		-w\
			-Wall\
			-Werror\
			-pedantic-errors


# Debug flags
#TODO FIX this
#ifeq ($(CONF),rel)
  # Optimization
#  CPPFLAGS +=		-O3
#  CPPFLAGS +=		-DNDEBUG
#else
  # Debug flags
CPPFLAGS +=		-g 

  # Optimization
CPPFLAGS +=		-Og
#endif


#..............................................................................#
# Host compiler preprossesor flags
HOST_CPPFLAGS =		$(CPPFLAGS)\
			$(USER_HOST_CPPFLAGS)\
			$(USER_CPPFLAGS)

#..............................................................................#
# Target compiler preprossesor flags
TARGET_CPPFLAGS =	$(CPPFLAGS)\
			$(USER_TARGET_CPPFLAGS)\
			$(USER_CPPFLAGS)

#..............................................................................#
# Test compiler preprossesor flags
TEST_CPPFLAGS =		$(CPPFLAGS)\
			$(USER_TEST_CPPFLAGS)\
			$(USER_CPPFLAGS)

################################################################################
#	 ___            _           _         __ _
#	|_ _|_ __   ___| |_   _  __| | ___   / _| | __ _  __ _ ___
#	 | || '_ \ / __| | | | |/ _` |/ _ \ | |_| |/ _` |/ _` / __|
#	 | || | | | (__| | |_| | (_| |  __/ |  _| | (_| | (_| \__ \
#	|___|_| |_|\___|_|\__,_|\__,_|\___| |_| |_|\__,_|\__, |___/
#	                                                 |___/
#

# Include directories
INCLUDES =		-I$(INC_DIR)

#..............................................................................#
# Host include directories
HOST_INCLUDES =		$(INCLUDES)\
			$(USER_HOST_INCLUDES)\
			$(USER_INCLUDES)

#..............................................................................#
# Target include directories
TARGET_INCLUDES =	$(INCLUDES)\
			$(USER_TARGET_INCLUDES)\
			$(USER_INCLUDES)

#..............................................................................#
# Test include directories
TEST_INCLUDES =		$(INCLUDES)\
			$(USER_TEST_INCLUDES)\
			$(USER_INCLUDES)

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
TESTS_DIR ?=	 	tests/
#LIB_DIR ?=		lib/


# Target name
#ifeq ($(TARGET_NAME),)
  #$(error "TARGET_NAME" is not specified)
#endif

#TARGET_DIR ?=		$(TARGET_NAME)_$(CONF)/
#HOST_DIR ?=		$(MACHINE)_$(CONF)/


# Object files
# TARGET_OBJS =		$(addprefix $(OBJ_DIR)$(TARGET_DIR),$(TARGET_CXX_SRCs:%.cpp=%.o))
# object files

#..............................................................................#

################################################################################
#	 _____           _
#	|_   _|__   ___ | |___
#	  | |/ _ \ / _ \| / __|
#	  | | (_) | (_) | \__ \
#	  |_|\___/ \___/|_|___/
#
#

#..............................................................................#
# Host toolchain
CC_HOST ?=		gcc
CXX_HOST ?=		g++
SZ_HOST ?=		size
#..............................................................................#
OBJS1 = ${OBJ_DIR}mainLsh.o ${OBJ_DIR}FunctionH.o ${OBJ_DIR}util.o ${OBJ_DIR}Point.o
OBJS2 = ${OBJ_DIR}mainHypercube.o ${OBJ_DIR}FunctionH.o ${OBJ_DIR}util.o
OBJS3 = ${OBJ_DIR}mainCurveLsh.o ${OBJ_DIR}FunctionH.o ${OBJ_DIR}util.o
OBJS4 = ${OBJ_DIR}mainCurveHypercube.o ${OBJ_DIR}FunctionH.o ${OBJ_DIR}util.o
OBJS5 = ${OBJ_DIR}mainCurveProjection.o ${OBJ_DIR}FunctionH.o ${OBJ_DIR}util.o
OBJS = $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5)

# create / compile the individual files >> separately < <
${OBJ_DIR}mainLsh.o: ${SRC_DIR}mainLsh.cpp
	${COMPILING}
	$(MKDIR_P)		$(dir $@)
	@$(CXX_HOST) 	-c $(TARGET_CXXFLAGS) $(HOST_CPPFLAGS) $(HOST_INCLUDES) ${SRC_DIR}mainLsh.cpp -o $@

${OBJ_DIR}mainHypercube.o : ${SRC_DIR}mainHypercube.cpp
	${COMPILING}
	$(MKDIR_P)		$(dir $@)
	@$(CXX_HOST) 	-c $(TARGET_CXXFLAGS)  $(HOST_CPPFLAGS) $(HOST_INCLUDES) ${SRC_DIR}mainHypercube.cpp -o $@

${OBJ_DIR}mainCurveLsh.o : ${SRC_DIR}mainCurveLsh.cpp
	${COMPILING}
	$(MKDIR_P)		$(dir $@)
	@$(CXX_HOST) 	-c $(TARGET_CXXFLAGS) $(HOST_CPPFLAGS) $(HOST_INCLUDES)  ${SRC_DIR}mainCurveLsh.cpp -o $@

${OBJ_DIR}mainCurveHypercube.o : ${SRC_DIR}mainCurveHypercube.cpp
	${COMPILING}
	$(MKDIR_P)		$(dir $@)
	@$(CXX_HOST) 	-c $(TARGET_CXXFLAGS) $(HOST_CPPFLAGS) $(HOST_INCLUDES)${SRC_DIR}mainCurveHypercube.cpp -o $@

${OBJ_DIR}mainCurveProjection.o : ${SRC_DIR}mainCurveProjection.cpp
	${COMPILING}
	$(MKDIR_P)		$(dir $@)
	@$(CXX_HOST) 	-c $(TARGET_CXXFLAGS) $(HOST_CPPFLAGS) $(HOST_INCLUDES) ${SRC_DIR}mainCurveProjection.cpp -o $@

${OBJ_DIR}FunctionH.o : ${SRC_DIR}FunctionH.cpp
	${COMPILING}
	$(MKDIR_P)		$(dir $@)
	@$(CXX_HOST) 	-c $(TARGET_CXXFLAGS) $(HOST_CPPFLAGS) $(HOST_INCLUDES) ${SRC_DIR}FunctionH.cpp -o $@

${OBJ_DIR}util.o : ${SRC_DIR}util.cpp
	${COMPILING}
	$(MKDIR_P)		$(dir $@)
	@$(CXX_HOST) 	-c $(TARGET_CXXFLAGS) $(HOST_CPPFLAGS) $(HOST_INCLUDES) ${SRC_DIR}util.cpp -o $@

${OBJ_DIR}Point.o : ${SRC_DIR}Point.cpp
	${COMPILING}
	$(MKDIR_P)		$(dir $@)
	@$(CXX_HOST) 	-c $(TARGET_CXXFLAGS) $(HOST_CPPFLAGS) $(HOST_INCLUDES) ${SRC_DIR}Point.cpp -o $@


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
# TEST
.PHONY: runtestslsh
runtestslsh: ##@tests Run lsh tests
runtestslsh:
	@${MKDIR_P} ${TESTS_DIR}
	${RUNNING}
	@$(ECHO)
	$(info running small dataset)
	${BIN_DIR}lsh –d ${TESTS_DIR}sample_datasets/siftsmall/input_small_id –q ${TESTS_DIR}sample_datasets/siftsmall/query_small_id –k 4 -L 5 -o ${TESTS_DIR}outsmall.txt
# 	$(info running big dataset)
# 	${BIN_DIR}lsh –d ${TESTS_DIR}sample\ datasets/siftbig/input_big_id –q ${TESTS_DIR}sample\ datasets/siftbig/query_big_id –k 4 -L 5 -o ${TESTS_DIR}outbig.txt

.PHONY: runtestscube
runtestcube: ##@tests Run lsh test
runtestcube:
	@${MKDIR_P} ${TESTS_DIR}
	${RUNNING}
	@$(ECHO)
	$(info running small dataset)
	${BIN_DIR}cube –d ${TESTS_DIR}sample datasets/siftsmall/input_small_id –q ${TESTS_DIR}sample datasets/siftsmall/query_small_id –k 4 -M 5 -probes 100 -o ${TESTS_DIR}
	$(info running big dataset)
	${BIN_DIR}cube –d ${TESTS_DIR}sample datasets/siftbig/input_big_id –q ${TESTS_DIR}sample datasets/siftbig/query_big_id –k 4 -M 5 -probes 100 -o ${TESTS_DIR}
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
lsh: ${BIN_DIR}lsh
${BIN_DIR}lsh: $(OBJS1)
	${BUILDING}
	$(MKDIR_P)		$(dir $@)
	@$(CXX_HOST)  $(OBJS1) -o $@
	${BUILD_SUCCESS}

cube: ##@build cube with points.
cube: ${BIN_DIR}cube
${BIN_DIR}cube: 
	$(error *** Creating cube executable ***  )
	${BUILDING}
	$(MKDIR_P)		$(dir $@)
	$(OBJS2)
	@$(CXX_HOST)  $(OBJS2) -o $@

curve_grid_lsh: ##@build curve Lsh.
curve_grid_lsh:${BIN_DIR}curve_grid_lsh
${BIN_DIR}curve_grid_lsh:
	$(error *** Creating Curve Grid Lsh executable ***  )
	${BUILDING}
	$(MKDIR_P)		$(dir $@)
	$(OBJS3)
	@$(CXX_HOST)  $(OBJS3) -o $@

curve_grid_cube: ##@build curveCube.
curve_grid_cube: ${BIN_DIR}curve_grid_cube
${BIN_DIR}curve_grid_cube:
	$(error *** Creating Curve Grid HyperCube executable ***  )
	${BUILDING}
	$(MKDIR_P)		$(dir $@)
	$(OBJS4)
	@$(CXX_HOST)  $(OBJS4) -o $@

curve_grid_projection: ##@build curveProjection.
curve_grid_projection: ${BIN_DIR}curve_grid_projection
${BIN_DIR}curve_grid_projection:
	$(error *** Creating Curve Grid Projection executable ***  )
	${BUILDING}
	$(MKDIR_P)		$(dir $@)
	$(OBJS5)
	@$(CXX_HOST)  $(OBJS5) -o $@

.PHONY: clean
clean:
	$(CLEANING)
	$(RM_FR)		$(OBJ_DIR)
	$(RM_FR)		$(BIN_DIR)

.PHONY: rebuild
rebuild: ##@build Rebuilds project.
rebuild: clean all

count:
	wc ${SRC_DIR}*.cpp ${INC_DIR}/*.h

# flags
#CFLAGS = -g -c -Wall $(DEPS)

# general dependencies
#DEPS = -I ./inc/

# main file names
TARGET1 = mainLsh
TARGET2 = mainHypercube
TARGET3 = mainCurveLsh
TARGET4 = mainCurveHypercube
TARGET5 = mainCurveProjection
TARGETS = $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5)

# standard
STD = #-std=c98


