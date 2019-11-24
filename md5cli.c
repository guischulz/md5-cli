#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "extern/klib/ketopt.h"
#include "extern/postgres/md5.h"
#include "types.h"

#define MD5_DIGEST_LEN 32

int main(int argc, char *argv[]) {
  ketopt_t opt;
  int iterations;
  int c;
  int i;
  char *p;
  const char *passwd;
  char *digest_buf;
  char *iter_buf;
  bool ok;

  // options
  iterations = 1;

  // command line parsing
  opt = KETOPT_INIT;
  while ((c = ketopt(&opt, argc, argv, 1, "i:", NULL)) >= 0) {
    if (c == 'i') {
      iterations = strtol(opt.arg, &p, 10);
      if (iterations <= 0) {
        fprintf_s(stderr, "ERROR: Iterations must be a positive number\n");
        exit(1);
      }

    } else if (c == '?') {
      fprintf(stderr, "ERROR: Unknown option '-%c'\n", opt.opt ? opt.opt : ':');
      exit(1);
    } else if (c == ':') {
      fprintf(stderr, "ERROR: Missing option argument '-%c'\n",
              opt.opt ? opt.opt : ':');
      exit(1);
    }
  }

  if (argc - opt.ind < 1) {
    fprintf_s(stderr, "ERROR: Missing password argument\n");
    exit(1);
  }

  passwd = argv[opt.ind];

  digest_buf = malloc(MD5_DIGEST_LEN + 1);
  if (!digest_buf) {
    fprintf(stderr, "ERROR: Malloc failed\n");
    exit(1);
  }

  ok = pg_md5_hash(passwd, strlen(passwd), digest_buf);

  if (iterations > 1) {
    iter_buf = malloc(MD5_DIGEST_LEN + 1);

    for (i = 1; i < iterations && ok; i++) {
      memcpy(iter_buf, digest_buf, MD5_DIGEST_LEN);
      ok = pg_md5_hash(iter_buf, MD5_DIGEST_LEN, digest_buf);
    }
  }

  if (ok)
    printf_s("%s", digest_buf);
  else
    fprintf(stderr, "ERROR: Internal error\n");

  free(digest_buf);
  return (!ok);
}
