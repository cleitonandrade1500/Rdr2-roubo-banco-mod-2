# Makefile RDR2 PS4 1.32 - John Bodyguard Mod
TARGET = ModJohn.sprx
OBJS = src/main.o

CXX = clang++
# Flags para gerar um binário completo de PS4
CXXFLAGS = -target x86_64-pc-freebsd-elf -fPIC -O2 -fno-exceptions -fno-rtti
LDFLAGS = -target x86_64-pc-freebsd-elf -shared -fuse-ld=lld

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) src/*.o
