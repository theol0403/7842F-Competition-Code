################################################################################
######################### User configurable parameters #########################
# filename extensions
CEXTS:=c
ASMEXTS:=s S
CXXEXTS:=cpp c++ cc

# probably shouldn't modify these, but you may need them below
ROOT=.
FWDIR:=$(ROOT)/firmware
BINDIR=$(ROOT)/bin
SRCDIR=$(ROOT)/src
INCDIR=$(ROOT)/include


WARNFLAGS+= -Wall -Wno-psabi -Wno-unused-variable -Wno-sign-compare
EXTRA_CFLAGS=
EXTRA_CXXFLAGS=

# Set to 1 to enable hot/cold linking
USE_PACKAGE:=1

# Set this to 1 to add additional rules to compile your project as a PROS library template
IS_LIBRARY:=0
# TODO: CHANGE THIS!
LIBNAME:=lib7842
VERSION:=1.0.0
EXCLUDE_SRC_FROM_LIB=$(SRCDIR)/7842FMain/RobotConfig.cpp $(SRCDIR)/7842FMain/DriverControl.cpp $(SRCDIR)/7842FMain/7842FMain.cpp $(wildcard $(SRCDIR)/7842FMain/Auton/**/*.cpp) $(wildcard $(SRCDIR)/7842FMain/Auton/*.cpp)

# files that get distributed to every user (beyond your source archive) - add
# whatever files you want here. This line is configured to add all header files
# that are in the the include directory get exported
TEMPLATE_FILES=$(INCDIR)/**/*.h $(INCDIR)/**/*.hpp

.DEFAULT_GOAL=quick

-include ./common.mk
