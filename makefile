TARGET = ModJohn.sprx
OBJS = src/main.o

CXX = clang++
CXXFLAGS = -target x86_64-pc-freebsd-elf -fPIC -O2 -fno-exceptions -fno-rtti -fno-use-cxa-atexit
LDFLAGS = -target x86_64-pc-freebsd-elf -shared -fuse-ld=lld -nostdlib

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
clean:
	rm -f $(TARGET) src/*.o
