CC=clang
INCDIR=include
SRCDIR=src
BUILDDIR=build
OBJDIR=$(BUILDDIR)/obj
TESTDIR=tests
TESTOBJDIR=$(TESTDIR)/obj
TESTBUILDDIR=$(TESTDIR)/build
OPT=-O2
# Avoid in release flags -> -g -fsanitize=address
# Avoid in development flags -> -O2
CFLAGS=-std=c99 -Wall -Wextra -I$(INCDIR) -pipe -pedantic -D_FORTIFY_SOURCE=2 -D_GNU_SOURCE $(OPT) \
	   -fstack-protector-all -fPIE -MMD -MP \
	   -g
LDFLAGS=-pie
TESTLIB=-lcriterion

SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
DEPS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.d, $(SRCS))
EXEC=$(BUILDDIR)/memalloc

TESTSRCS=$(wildcard $(TESTDIR)/*.c)
TESTOBJS=$(patsubst $(TESTDIR)/%.c, $(TESTOBJDIR)/%.o, $(TESTSRCS))
TESTDEPS=$(patsubst $(TESTDIR)/%.c, $(TESTOBJDIR)/%.d, $(TESTSRCS))
TESTBINS=$(patsubst $(TESTOBJDIR)/%.o, $(TESTBUILDDIR)/%, $(TESTOBJS))

-include $(DEPS)
-include $(TESTDEPS)

.PHONY: all run test clean-obj clean-test clean

all: $(EXEC)

run: $(EXEC)
	@$(EXEC)

$(EXEC): $(OBJS) | $(BUILDDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TESTBUILDDIR) $(TESTOBJS) $(TESTBINS)
	@for test in $(TESTBINS); do \
		./$$test || exit 1; \
	done

$(TESTBUILDDIR)/%: $(TESTOBJDIR)/%.o $(OBJS) | $(TESTBUILDDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< $(filter-out $(OBJDIR)/main.o, $(OBJS)) -o $@ $(TESTLIB)

$(TESTOBJDIR)/%.o: $(TESTDIR)/%.c | $(TESTOBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR) $(OBJDIR) $(TESTBUILDDIR) $(TESTOBJDIR):
	mkdir -p $@

clean-obj:
	rm -rf $(OBJDIR) $(TESTOBJDIR)

clean-test:
	rm -rf $(TESTBINS) $(TESTOBJDIR)

clean: clean-obj clean-test
	rm $(EXEC)

# only if passing arguments are needed
# %:
# 	@:
