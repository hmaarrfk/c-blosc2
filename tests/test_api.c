/*********************************************************************
  Blosc - Blocked Shuffling and Compression Library

  Unit tests for Blosc API.

  Copyright (C) 2021  The Blosc Developers <blosc@blosc.org>
  https://blosc.org
  License: BSD 3-Clause (see LICENSE.txt)

  See LICENSE.txt for details about copyright and rights to use.
**********************************************************************/

#include "test_common.h"

int tests_run = 0;

/* Global vars */
void* src, * srccpy, * dest, * dest2;
int nbytes, cbytes;
int clevel = 3;
int doshuffle = 1;
int typesize = 4;
int size = 1 * MB;


static char* test_cbuffer_sizes(void) {
  size_t nbytes_, cbytes_, blocksize;

  blosc_cbuffer_sizes(dest, &nbytes_, &cbytes_, &blocksize);
  mu_assert("ERROR: nbytes incorrect(1)", nbytes == size);
  mu_assert("ERROR: nbytes incorrect(2)", nbytes_ == (size_t)nbytes);
  mu_assert("ERROR: cbytes incorrect", cbytes_ == (size_t)cbytes);
  mu_assert("ERROR: blocksize incorrect", blocksize >= 128);
  return 0;
}

static char* test_cbuffer_metainfo(void) {
  size_t typesize_;
  int flags;

  blosc_cbuffer_metainfo(dest, &typesize_, &flags);
  mu_assert("ERROR: typesize incorrect", typesize_ == (size_t)typesize);
  mu_assert("ERROR: shuffle incorrect", (flags & BLOSC_DOSHUFFLE) == doshuffle);
  return 0;
}


static char* test_cbuffer_versions(void) {
  int version_;
  int versionlz_;

  blosc_cbuffer_versions(dest, &version_, &versionlz_);
  mu_assert("ERROR: version incorrect", version_ == BLOSC_VERSION_FORMAT);
  mu_assert("ERROR: versionlz incorrect", versionlz_ == BLOSC_BLOSCLZ_VERSION_FORMAT);
  return 0;
}


static char* test_cbuffer_complib(void) {
  const char* complib;

  complib = blosc_cbuffer_complib(dest);
  mu_assert("ERROR: complib incorrect", strcmp(complib, "BloscLZ") == 0);
  return 0;
}

static char *test_nthreads(void) {
  int16_t nthreads;

  nthreads = blosc_set_nthreads(4);
  mu_assert("ERROR: set_nthreads incorrect", nthreads == 1);
  nthreads = blosc_get_nthreads();
  mu_assert("ERROR: get_nthreads incorrect", nthreads == 4);
  return 0;
}

static char *test_blocksize(void) {
  int blocksize;

  blocksize = blosc_get_blocksize();
  mu_assert("ERROR: get_blocksize incorrect", blocksize == 0);

  blosc_set_blocksize(4096);
  blocksize = blosc_get_blocksize();
  mu_assert("ERROR: get_blocksize incorrect", blocksize == 4096);
  return 0;
}


static char* all_tests(void) {
  mu_run_test(test_cbuffer_sizes);
  mu_run_test(test_cbuffer_metainfo);
  mu_run_test(test_cbuffer_versions);
  mu_run_test(test_cbuffer_complib);
  mu_run_test(test_nthreads);
  mu_run_test(test_blocksize);
  return 0;
}

#define BUFFER_ALIGN_SIZE   8

int main(void) {
  char* result;

  install_blosc_callback_test(); /* optionally install callback test */
  blosc_init();
  blosc_set_nthreads(1);

  /* Initialize buffers */
  src = blosc_test_malloc(BUFFER_ALIGN_SIZE, size);
  srccpy = blosc_test_malloc(BUFFER_ALIGN_SIZE, size);
  dest = blosc_test_malloc(BUFFER_ALIGN_SIZE, size);
  dest2 = blosc_test_malloc(BUFFER_ALIGN_SIZE, size);
  memset(src, 0, size);
  memcpy(srccpy, src, size);

  /* Get a compressed buffer */
  cbytes = blosc_compress(clevel, doshuffle, typesize, size, src, dest, size);

  /* Get a decompressed buffer */
  nbytes = blosc_decompress(dest, dest2, size);

  /* Run all the suite */
  result = all_tests();
  if (result != 0) {
    printf(" (%s)\n", result);
  }
  else {
    printf(" ALL TESTS PASSED");
  }
  printf("\tTests run: %d\n", tests_run);

  blosc_test_free(src);
  blosc_test_free(srccpy);
  blosc_test_free(dest);
  blosc_test_free(dest2);

  blosc_destroy();

  return result != 0;
}
