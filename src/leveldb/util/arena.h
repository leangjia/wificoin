// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_UTIL_ARENA_H_
#define STORAGE_LEVELDB_UTIL_ARENA_H_

<<<<<<< HEAD
#include <vector>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include "port/port.h"
=======
#include <cstddef>
#include <vector>
#include <assert.h>
#include <stdint.h>
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

namespace leveldb {

class Arena {
 public:
  Arena();
  ~Arena();

  // Return a pointer to a newly allocated memory block of "bytes" bytes.
  char* Allocate(size_t bytes);

  // Allocate memory with the normal alignment guarantees provided by malloc
  char* AllocateAligned(size_t bytes);

  // Returns an estimate of the total memory usage of data allocated
<<<<<<< HEAD
  // by the arena.
  size_t MemoryUsage() const {
    return reinterpret_cast<uintptr_t>(memory_usage_.NoBarrier_Load());
=======
  // by the arena (including space allocated but not yet used for user
  // allocations).
  size_t MemoryUsage() const {
    return blocks_memory_ + blocks_.capacity() * sizeof(char*);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
  }

 private:
  char* AllocateFallback(size_t bytes);
  char* AllocateNewBlock(size_t block_bytes);

  // Allocation state
  char* alloc_ptr_;
  size_t alloc_bytes_remaining_;

  // Array of new[] allocated memory blocks
  std::vector<char*> blocks_;

<<<<<<< HEAD
  // Total memory usage of the arena.
  port::AtomicPointer memory_usage_;
=======
  // Bytes of memory in blocks allocated so far
  size_t blocks_memory_;
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1

  // No copying allowed
  Arena(const Arena&);
  void operator=(const Arena&);
};

inline char* Arena::Allocate(size_t bytes) {
  // The semantics of what to return are a bit messy if we allow
  // 0-byte allocations, so we disallow them here (we don't need
  // them for our internal use).
  assert(bytes > 0);
  if (bytes <= alloc_bytes_remaining_) {
    char* result = alloc_ptr_;
    alloc_ptr_ += bytes;
    alloc_bytes_remaining_ -= bytes;
    return result;
  }
  return AllocateFallback(bytes);
}

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_UTIL_ARENA_H_
