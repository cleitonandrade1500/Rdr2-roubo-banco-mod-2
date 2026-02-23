TARGET = ModJohn.sprx
OBJS = main.o

CXX = clang++
# Flags de compilação para PS4
CXXFLAGS = -target x86_64-pc-freebsd-elf -fPIC -O2 -fno-exceptions -fno-rtti -fno-use-cxa-atexit
# Flags de linkagem corrigidas
LDFLAGS = -target x86_64-pc-freebsd-elf -shared -fuse-ld=lld -nostdlib

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ main.cpp

clean:
	rm -f $(TARGET) $(OBJS)
