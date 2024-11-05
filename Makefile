CC = g++
CFLAGS = `pkg-config --cflags gtkmm-4.0` `pkg-config --cflags gtk4-layer-shell-0` -O3 -s
LIBS = `pkg-config --libs gtkmm-4.0` `pkg-config --libs gtk4-layer-shell-0`
SRC = src/main.cpp
OUT = wn_bar

all:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LIBS)

clean:
	rm -f $(OUT)