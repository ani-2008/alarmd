CC = gcc
CFLAGS = -Wall
LIBS = -lm -lpthread
TARGET = alarmd
SRC = src/alarmd.c
SOUND_FILE = assets/alarm.wav
INSTALL_PATH = /usr/local/bin
ASSET_PATH = /usr/local/share/alarmd

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

install: all
	install -d $(INSTALL_PATH)
	install -d $(ASSET_PATH)
	install -m 755 $(TARGET) $(INSTALL_PATH)
	install -m 644 $(SOUND_FILE) $(ASSET_PATH)
	@echo "Installed! Run with: alarmd YYYY MM DD HH MM SS &"

uninstall:
	rm -f $(INSTALL_PATH)/$(TARGET)
	rm -rf $(ASSET_PATH)

clean:
	rm -f $(TARGET)
