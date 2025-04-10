CC             := clang
CFLAGS         := -Wall -Wextra -O2
TARGET         := cexan
SOURCES        := $(wildcard *.c)
MANPAGE        := cexan.1
INSTALL_DIR    := /usr/bin/
MAN_DIR        := /usr/share/man/man1/
PWD            := $(shell pwd)

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) -o $(PWD)/$(TARGET) $(CFLAGS) $(SOURCES)

install: $(TARGET)
	@install -D -m 755 $(PWD)/$(TARGET) $(INSTALL_DIR)

install_all: install
	@gzip -f $(MANPAGE)
	@install -D -m 644 $(PWD)/$(MANPAGE).gz $(MAN_DIR)

uninstall:
	@rm $(MAN_DIR)/$(MANPAGE).gz $(INSTALL_DIR)/$(TARGET)

check-man:
	@if [ ! -f $(MAN_DIR)/$(MANPAGE).gz ]; then \
		echo -e "\033[31mError: $(MANPAGE).gz not found.\033[0m"; \
		exit 1; \
	fi
	@if ! man $(TARGET) &> /dev/null; then \
		echo -e "\033[31mError: $(MANPAGE).gz cannot be formatted.\033[0m"; \
		exit 1; \
	fi
	@echo -e "\033[32m$(MANPAGE).gz checked successfully.\033[0m"

.PHONY: all clean install install_all uninstall check-man
