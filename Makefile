CC = g++
CPPFLAGS = -Wall -O2

ifneq ($(OS), 'Windows_NT')
    CPPFLAGS += -D_POSIX_C_SOURCE
endif

LIBS := -lpthread -lm
LDFLAGS :=
SANITIZERFLAGS := -fsanitize=address -fsanitize=undefined

SRCDIR := src
OBJDIR := obj
BINDIR := bin
INCDIR := include
MODDIR := models
DEPDIR := deps

TARGET := structur

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

debug: CPPFLAGS += -Og -g3 -fno-omit-frame-pointer $(SANITIZERFLAGS)
debug: LDFLAGS += $(SANITIZERFLAGS)
debug: $(BINDIR)/$(TARGET)

release: CPPFLAGS += -O3 -g0 -DNDEBUG
release: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	@$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) $(DEPDIR)
	@$(CC) -I$(INCDIR) -c $(CPPFLAGS) -o $@ \
	    -MT $@ -MMD -MP \
	    -MF $(patsubst $(OBJDIR)/%.o, $(DEPDIR)/%.d, $@) \
	    $<

$(BINDIR) $(OBJDIR) $(DEPDIR):
	@mkdir -p $@

clean:
	@rm -rf $(OBJDIR) $(DEPDIR) $(BINDIR)/$(TARGET)

.PHONY: debug release clean

-include $(patsubst $(SRCDIR)/%.cpp, $(DEPDIR)/%.d, $(SOURCES))