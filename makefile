NAME=$(shell basename $(CURDIR))

BINDIR=bin
OBJDIR=obj
DEPDIR=dep

SRCDIR=src
INCDIR=include


BIN=$(BINDIR)/$(NAME).out

# Compiler settings
CPP=g++
DEPFLAGS := -MP -MMD
OPT := -O2
DEBUGFLAGS := -fno-stack-protector -Wstringop-overflow=0 -Wformat-overflow=0
INCLUDE := -I$(INCDIR) -I$(COMMON_INC)

CPPFLAGS := -m32 -std=c++17 $(INCLUDE) $(OPT) $(DEPFLAGS) # $(DEBUGFLAGS) -m32
LDLIBS := -lssl -lcrypto


# File list generation
SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS=$(patsubst $(SRCDIR)/%.cpp,$(DEPDIR)/%.d,$(SRCS))

all: $(INCDIR)/embed.h $(BIN)


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

EMBED=embed

$(EMBED)/bin/$(EMBED).out: 
	$(MAKE) -C $(EMBED)/

# $(EMBED)ded executable
$(EMBED).enc: $(EMBED)/bin/$(EMBED).out $(EMBED).passwd
	$(info $@...)
	@ openssl enc -aes-256-cbc -in $< -out $@ -kfile $(EMBED).passwd 2>/dev/null
# -p

$(INCDIR)/$(EMBED).h: $(EMBED).enc
	$(info $@...)
	@ xxd -i $< > $@



# include the dependencies
-include $(DEPS)

.PHONY: clean test

clean:
	@ (cd embed/ && make clean)
	rm -f $(OBJDIR)/*.o $(DEPDIR)/*.d $(BINDIR)/*.out $(INCDIR)/embed.h embed.enc

%:		# Do nothing if an arg is not a `Make Goal`
	@:


# Default target
# all: layer1 layer2_3.enc

# detection.o: detection.cpp
# 	$(CXX) $(CXXFLAGS) -c detection.cpp -o detection.o

# layer1: layer1.cpp crypto.h
# 	$(CXX) $(CXXFLAGS) layer1.cpp -o layer1 $(DEPFLAGS)

# layer2_3.enc: layer2_3.cpp crypto.h dhexchange.o
# 	$(CXX) $(CXXFLAGS) layer2_3.cpp dhexchange.o -o layer2_3 $(DEPFLAGS)
# 	openssl enc -aes-256-cbc -in layer2_3 -out layer2_3.enc -pass pass:abcde

# # Clean up
# clean:
# 	rm -rf *.o *.enc *.dec layer1 layer2_3

# # Phony targets
# .PHONY: all clean

