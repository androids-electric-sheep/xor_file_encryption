#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

struct file_view {
  unsigned long file_size;
  char *file_data;
};

struct file_view *read_file(char *filename) {
  struct stat st;
  stat(filename, &st);
  const unsigned long file_size = st.st_size;

  FILE *fd = fopen(filename, "r");
  if (fd == NULL) {
    perror("unable to open file");
    return NULL;
  }

  char *file_data = (char *)malloc(file_size);
  fread(file_data, file_size, 1, fd);
  fclose(fd);

  struct file_view *view = (struct file_view *)malloc(sizeof(struct file_view));
  view->file_size = file_size;
  view->file_data = file_data;
  return view;
}

void xor_encrypt(char *data, unsigned long data_length, const char *xor_key) {
  unsigned int key_length = strlen(xor_key);
  for (int i = 0; i < data_length; ++i) {
    data[i] = data[i] ^ xor_key[i % key_length];
  }
}

int main(int argc, char **argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s INPUT_FILE OUTPUT_FILE KEY\n", argv[0]);
    return EXIT_FAILURE;
  }
  const char *xor_key = argv[3];

  // Encrypt file
  struct file_view *view = read_file(argv[1]);
  xor_encrypt(view->file_data, view->file_size, xor_key);

  // Write out result
  FILE *fd = fopen(argv[2], "w");
  fwrite(view->file_data, view->file_size, 1, fd);
  fclose(fd);

  // Cleanup
  free(view->file_data);
  free(view);
  return EXIT_SUCCESS;
}
