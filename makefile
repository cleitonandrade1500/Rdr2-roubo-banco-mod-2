TARGET = ModJohn.sprx
OBJS = source/main.o

CXX = clang++
CXXFLAGS = -target x86_64-pc-freebsd-elf -fPIC -O2 -Iinclude -fno-exceptions -fno-rtti
LDFLAGS = -target x86_64-pc-freebsd-elf -shared -fuse-ld=lld -nostdlib

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)
source/%.o: source/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
clean:
	rm -f $(TARGET) source/*.o
