CC=clang
INCDIR=include
SRCDIR=src
OBJDIR=obj
BUILDDIR=build
CFLAGS=-Wall -Wextra -I$(INCDIR) -g -pipe -pedantic \
	   -fstack-protector-all
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
EXEC=$(BUILDDIR)/memalloc

all: $(EXEC)
	@$(EXEC)

$(EXEC): $(OBJS) | $(BUILDDIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR) $(OBJDIR) $(SRCDIR):
	mkdir -p $@

clean-obj:
	rm -rf $(OBJDIR)

clean: clean-obj
	rm -rf $(EXEC)
