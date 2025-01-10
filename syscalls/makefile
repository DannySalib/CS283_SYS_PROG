CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = scdirect
SOURCES = scdirect

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

debug: $(SOURCES)
	$(CC) $(CFLAGS) -O0 -o $(TARGET)_debug $^

clean:
	rm -f $(TARGET) $(TARGET)_debug

.PHONY: clean debug