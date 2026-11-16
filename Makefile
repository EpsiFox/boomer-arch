CFLAGS=-std=c17 -g -fno-common -Wextra -Wpedantic

SRC_FILES = src/config.c src/la.c src/main.c src/navigation.c src/screenshot.c

boomer-arch: 
	gcc -o boomer-arch $(SRC_FILES) -lX11

install:
	@echo "Installing boomer-arch..."
	@cp boomer-arch /usr/local/bin/
	@echo "Done."

uninstall:
	@echo "Uninstalling boomer-arch..."
	@rm /usr/local/bin/boomer-arch
	@echo "Done."

clean:
	@echo "Deleting application from this folder..."
	-@rm -f boomer-arch *.o 2>/dev/null || true
	@echo "Done."

.PHONY: clean



