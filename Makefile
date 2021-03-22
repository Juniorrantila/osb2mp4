CC       := clang
UNIT     := unit
INSTALL  := install

INCLUDE  := include /mnt/d/libs/opencv/build/include
INCLUDE  := $(addprefix -I, $(INCLUDE))
CFLAGS   := -std=c++17 -O3 $(INCLUDE)

LIB_DIRS := 
LIB_DIRS := $(addprefix -L, $(LIB_DIRS))
LIBS     := 
LIBS     := $(addprefix -l, $(LIBS))

BIN      := osb2mp4
SUBDIRS  := tests

BUILDDIR := build
SRCDIR   := src
CPPSRC   := $(wildcard $(SRCDIR)/*.cpp)
OBJS     := $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(CPPSRC))

TESTDIR  := tests/bins

.PHONY: all
all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LIB_DIRS) $(LIBS) $^ -o $@

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: test
test:
	@$(UNIT) $(TESTDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: install
install: $(BIN)
	$(INSTALL) $(BIN) /usr/local/bin

.PHONY: uninstall
uninstall:
	$(RM) /usr/local/bin/$(BIN)

.PHONY: clean
clean:
	$(RM) $(BIN)
	$(RM) $(BUILDDIR)/*.o
	$(MAKE) clean -C $(SUBDIRS)
