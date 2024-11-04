NAME=$(shell basename $(CURDIR))

BINDIR=bin
OBJDIR=obj
DEPDIR=dep

SRCDIR=src
INCDIR=inc
COMMON_INC := ../include


BIN=$(BINDIR)/$(NAME).out


# Compiler settings
CPP=g++
DEPFLAGS := -MP -MMD
OPT := -O2
DEBUGFLAGS := -fno-stack-protector -Wstringop-overflow=0 -Wformat-overflow=0
INCLUDE := -I$(INCDIR) -I$(COMMON_INC)

CPPFLAGS := -m32 -std=c++17 -g $(INCLUDE) $(OPT) $(DEPFLAGS) # $(DEBUGFLAGS) -m32
LDLIBS := -lssl -lcrypto

# File list generation
SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS=$(patsubst $(SRCDIR)/%.cpp,$(DEPDIR)/%.d,$(SRCS))


all: inc/secret.h $(BIN)

# Link the final binary
$(BIN): $(OBJS) | $(BINDIR)
	@ $(CPP) $(CPPFLAGS) -o $@ $^ $(LDLIBS)
	$(info ./$@)

# Compile to objects
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) $(DEPDIR)
	$(info $@...)
	@ $(CPP) $(CPPFLAGS) -MF $(DEPDIR)/$*.d -c -o $@ $<


# Create dirs if they don't exist
$(OBJDIR): ; @mkdir -p $(OBJDIR)
$(BINDIR): ; @mkdir -p $(BINDIR)
$(DEPDIR): ; @mkdir -p $(DEPDIR)


# secret
secret.enc: secret.txt secret.passwd
	$(info $@...)
	@ openssl enc -aes-256-cbc -in $< -out $@ -kfile secret.passwd 2>/dev/null
# -p

inc/secret.h: secret.enc
	$(info $@...)
	@ xxd -i $< > $@

# include the dependencies
-include $(DEPS)

.PHONY: clean test

clean:
	rm -f $(OBJDIR)/*.o $(DEPDIR)/*.d $(BINDIR)/*.out inc/secret.h secret.enc

%:		# Do nothing if an arg is not a `Make Goal`
	@:
