CC := gcc
CXX := g++


# GLFW e GLAD
glfw := #Caminho para o GLFW
glfw_inc := $(glfw)/include
glfw_lib := $(glfw)/lib64

glad := #Caminho para o GLAD
glad_inc := $(glad)/include


INCS := -I$(glfw_inc) -I$(glad_inc)
LIBS :=  -L$(glfw_lib) 
LDFLAGS := $(LIBS) -lglfw3 -lopengl32 -lglu32 -lgdi32
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


