# Configurações de Compilação PS4
TARGET = RDR2_JohnMod.sprx
OBJS = main.o

CC = clang
CXX = clang++
LD = ld.lld

CFLAGS = -target x86_64-pc-freebsd-elf -fPIC -O2 -fno-exceptions
LDFLAGS = -target x86_64-pc-freebsd-elf -shared -fuse-ld=lld

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
