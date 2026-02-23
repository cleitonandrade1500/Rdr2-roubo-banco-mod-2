TARGET = ModJohn.sprx
OBJS = main.o
CXX = clang++
CXXFLAGS = -target x86_64-pc-freebsd-elf -fPIC -O2 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -c
LDFLAGS = -target x86_64-pc-freebsd-elf -shared -fuse-ld=lld -nostdlib

all: $(TARGET)
$(TARGET): $(OBJS)
	ld.lld $(LDFLAGS) -o $@ $(OBJS)
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ main.cpp
clean:
	rm -f $(TARGET) $(OBJS)
