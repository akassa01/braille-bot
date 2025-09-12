# Compiler
CXX      = g++

# Compiler Flags
CXXFLAGS = -g3 -std=c++11 -Wall -Wextra -Wpedantic -Wshadow \
           -I/opt/homebrew/opt/boost/include -I/opt/homebrew/include/poppler/cpp

# Linker Flags (Boost & Poppler)
LDFLAGS  = -g3 -L/opt/homebrew/opt/boost/lib -lpthread -L/opt/homebrew/lib -lpoppler-cpp

# Object files
OBJS = pdf_to_braille.o

# Target executable
braille: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

# Rule for compiling .cpp files
%.o: %.cpp pdf_to_braille.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean: 
	rm -f *.o *~ a.out braille
