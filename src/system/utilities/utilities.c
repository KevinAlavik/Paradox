#include <nighterm/nighterm.h>
#include <printf.h>
#include <stdarg.h>
#include <strings.h>
#include <system/utilities/utilities.h>

char *get_nighterm_return_string(int return_code) {
  switch (return_code) {
  case NIGHTERM_FONT_INVALID:
    return "NIGHTERM_FONT_INVALID";
  case NIGHTERM_INVALID_FRAMEBUFFER_ADDRESS:
    return "NIGHTERM_INVALID_FRAMEBUFFER_ADDRESS";
  case NIGHTERM_INVALID_FRAMEBUFFER_SIZE:
    return "NIGHTERM_INVALID_FRAMEBUFFER_SIZE";
  case NIGHTERM_INVALID_FRAMEBUFFER_PITCH:
    return "NIGHTERM_INVALID_FRAMEBUFFER_PITCH";
  case NIGHTERM_INVALID_FRAMEBUFFER_BPP:
    return "NIGHTERM_INVALID_FRAMEBUFFER_BPP";
  case NIGHTERM_MALLOC_IS_NULL:
    return "NIGHTERM_MALLOC_IS_NULL";
  case NIGHTERM_SUCCESS:
    return "NIGHTERM_SUCCESS";
  default:
    return "Unknown return code";
  }
}

int endsWith(const char *str, const char *suffix) {
  size_t str_len = strlen(str);
  size_t suffix_len = strlen(suffix);

  if (str_len < suffix_len) {
    return 0; // String is shorter than the suffix
  }

  return strcmp(str + (str_len - suffix_len), suffix) == 0;
}

int tolower(int c) {
  if (c >= 'A' && c <= 'Z') {
    return c + ('a' - 'A');
  } else {
    return c;
  }
}