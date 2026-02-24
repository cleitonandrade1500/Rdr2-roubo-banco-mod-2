# Makefile Corrigido para Havana (RDR2 PS4 1.32)
TARGET = RDR2-SP-Base.sprx
OBJS = src/main.o src/script.o

# Compilador e Bibliotecas
CXX = clang++
CXXFLAGS = -target x86_64-pc-freebsd-elf -fPIC -O2 -Iinclude -fno-exceptions -fno-rtti
LDFLAGS = -target x86_64-pc-freebsd-elf -shared -fuse-ld=lld

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) src/*.o
