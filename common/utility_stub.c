/* Copyright (c) 2010 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Stub implementations of utility functions which call their linux-specific
 * equivalents.
 */

#include "utility.h"

#include <stdio.h>
#include <stdlib.h>

void* Malloc(size_t size) {
  void* p = malloc(size);
  if (!p) {
    /* Fatal Error. We must abort. */
    abort();
  }
  return p;
}

void Free(void* ptr) {
  free(ptr);
}

void* Memcpy(void* dest, const void* src, size_t n) {
  return memcpy(dest, src, n);
}

void* Memset(void* dest, const uint8_t c, size_t n) {
  while (n--) {
    *((uint8_t*)dest++) = c;
  }
  return dest;
}

int SafeMemcmp(const void* s1, const void* s2, size_t n) {
  int match = 0;
  const unsigned char* us1 = s1;
  const unsigned char* us2 = s2;
  while (n--) {
    if (*us1++ != *us2++)
      match = 1;
  }

  return match;
}

void* StatefulMemcpy(MemcpyState* state, void* dst,
                     uint64_t len) {
  if (state->overrun)
    return NULL;
  if (len > state->remaining_len) {
    state->overrun = 1;
    return NULL;
  }
  Memcpy(dst, state->remaining_buf, len);
  state->remaining_buf += len;
  state->remaining_len -= len;
  return dst;
}

const void* StatefulMemcpy_r(MemcpyState* state, const void* src,
                             uint64_t len) {
  if (state->overrun)
    return NULL;
  if (len > state->remaining_len) {
    state->overrun = 1;
    return NULL;
  }
  Memcpy(state->remaining_buf, src, len);
  state->remaining_buf += len;
  state->remaining_len -= len;
  return src;
}