#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define green_text(text) "\x1b[32m" text "\x1b[0m"
#define red_text(text) "\x1b[31m" text "\x1b[0m"

size_t bytes_per_line = 16;
size_t address_width = 8;

void
print_address(const size_t address, const size_t address_width)
{
  printf("%0*lx    ", (int)address_width, address);
}

void
print_hex_values(const unsigned char* buffer,
                 const size_t bytes_per_line,
                 size_t actual_bytes)
{
  size_t j;
  for (j = 0; j < actual_bytes; ++j) {
    printf("%02x ", buffer[j]);
    if ((j + 1) % 4 == 0) {
      putchar(' ');
    }
  }
  for (j = actual_bytes; j < bytes_per_line; ++j) {
    putchar(' ');
    if ((j + 1) % 4 == 0) {
      putchar(' ');
    }
  }
}

void
print_ascii_values(const char* buffer,
                   size_t bytes_per_line,
                   size_t actual_bytes)
{
  size_t j;

  for (j = 0; j < actual_bytes; ++j) {
    unsigned char ch = buffer[j];
    if (ch >= 32 && ch <= 126) {
      printf("%c", ch);
    } else {
      putchar('.');
    }
  }
  for (j = actual_bytes; j < bytes_per_line; ++j) {
    putchar(' ');
  }
}

int
handle_file(const char* file_path)
{
  FILE* file_fd;
  char buffer[bytes_per_line];
  size_t address = 0;
  size_t bytes_read;

  if ((file_fd = fopen(file_path, "rb")) == NULL) {
    fprintf(
      stderr, red_text("cexan: %s: no such file or directory\n"), file_path);
    return 1;
  }

  while ((bytes_read = fread(buffer, 1, bytes_per_line, file_fd)) > 0) {
    print_address(address, address_width);
    print_hex_values((unsigned char*)buffer, bytes_per_line, bytes_read);
    putchar(' ');
    print_ascii_values(buffer, bytes_per_line, bytes_read);
    putchar('\n');
    address += bytes_read;
  }

  fclose(file_fd);
  return 0;
}

int
stream_handle_file()
{
  // TODO: реализовать обработку файла построчно
  printf("TODO stream\n");
  return 0;
}

int
main(const int argc, const char* argv[])
{
  int i, size, result_handle_file;
  char* endptr;
  char file_path[1024] = { 0 };
  int file_count = 0;

  if (argc == 1) {
    result_handle_file = stream_handle_file();
    return result_handle_file;
  } else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
    printf("usage: cexan [OPTIONS] [FILE]\n"
           "Options:\n"
           "\t--help, -h\tDisplay this help message\n"
           "\t--bytes, -b\tThe number of bytes in the line (Default %zu)\n",
           bytes_per_line);
    return 0;
  }
  for (i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--bytes") == 0) {
      if (i + 1 >= argc) {
        fprintf(
          stderr, red_text("%s: --bytes option requires a value\n"), argv[0]);
        return 1;
      }
      size = strtoul(argv[++i], &endptr, 10);
      if (*endptr != '\0' || size <= 0) {
        fprintf(stderr, red_text("cexan: invalid value: '%s'\n"), argv[i]);
        return 1;
      }
      bytes_per_line = (size_t)size;
    } else {
      if (file_count > 0) {
        fprintf(stderr, red_text("cexan: multiple files not supported\n"));
        return 1;
      }
      if (strlen(argv[i]) >= 1024) {
        fprintf(
          stderr,
          red_text(
            "cexan: the file path must be no more than 1024 characters\n"));
        return 1;
      }
      strncpy(file_path, argv[i], sizeof(file_path) - 1);
      ++file_count;
    }
  }
  if (file_count == 0) {
    result_handle_file = stream_handle_file();
  } else {
    result_handle_file = handle_file(file_path);
  }
  return result_handle_file;
}
