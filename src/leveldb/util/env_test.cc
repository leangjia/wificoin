// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/env.h"

#include "port/port.h"
#include "util/testharness.h"

namespace leveldb {

static const int kDelayMicros = 100000;
<<<<<<< HEAD
static const int kReadOnlyFileLimit = 4;
static const int kMMapLimit = 4;

class EnvTest {
=======

class EnvPosixTest {
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
 private:
  port::Mutex mu_;
  std::string events_;

 public:
  Env* env_;
<<<<<<< HEAD
  EnvTest() : env_(Env::Default()) { }
=======
  EnvPosixTest() : env_(Env::Default()) { }
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
};

static void SetBool(void* ptr) {
  reinterpret_cast<port::AtomicPointer*>(ptr)->NoBarrier_Store(ptr);
}

<<<<<<< HEAD
TEST(EnvTest, RunImmediately) {
  port::AtomicPointer called (NULL);
  env_->Schedule(&SetBool, &called);
  env_->SleepForMicroseconds(kDelayMicros);
  ASSERT_TRUE(called.NoBarrier_Load() != NULL);
}

TEST(EnvTest, RunMany) {
=======
TEST(EnvPosixTest, RunImmediately) {
  port::AtomicPointer called (NULL);
  env_->Schedule(&SetBool, &called);
  Env::Default()->SleepForMicroseconds(kDelayMicros);
  ASSERT_TRUE(called.NoBarrier_Load() != NULL);
}

TEST(EnvPosixTest, RunMany) {
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
  port::AtomicPointer last_id (NULL);

  struct CB {
    port::AtomicPointer* last_id_ptr;   // Pointer to shared slot
    uintptr_t id;             // Order# for the execution of this callback

    CB(port::AtomicPointer* p, int i) : last_id_ptr(p), id(i) { }

    static void Run(void* v) {
      CB* cb = reinterpret_cast<CB*>(v);
      void* cur = cb->last_id_ptr->NoBarrier_Load();
      ASSERT_EQ(cb->id-1, reinterpret_cast<uintptr_t>(cur));
      cb->last_id_ptr->Release_Store(reinterpret_cast<void*>(cb->id));
    }
  };

  // Schedule in different order than start time
  CB cb1(&last_id, 1);
  CB cb2(&last_id, 2);
  CB cb3(&last_id, 3);
  CB cb4(&last_id, 4);
  env_->Schedule(&CB::Run, &cb1);
  env_->Schedule(&CB::Run, &cb2);
  env_->Schedule(&CB::Run, &cb3);
  env_->Schedule(&CB::Run, &cb4);

<<<<<<< HEAD
  env_->SleepForMicroseconds(kDelayMicros);
=======
  Env::Default()->SleepForMicroseconds(kDelayMicros);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
  void* cur = last_id.Acquire_Load();
  ASSERT_EQ(4, reinterpret_cast<uintptr_t>(cur));
}

struct State {
  port::Mutex mu;
  int val;
  int num_running;
};

static void ThreadBody(void* arg) {
  State* s = reinterpret_cast<State*>(arg);
  s->mu.Lock();
  s->val += 1;
  s->num_running -= 1;
  s->mu.Unlock();
}

<<<<<<< HEAD
TEST(EnvTest, StartThread) {
=======
TEST(EnvPosixTest, StartThread) {
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
  State state;
  state.val = 0;
  state.num_running = 3;
  for (int i = 0; i < 3; i++) {
    env_->StartThread(&ThreadBody, &state);
  }
  while (true) {
    state.mu.Lock();
    int num = state.num_running;
    state.mu.Unlock();
    if (num == 0) {
      break;
    }
<<<<<<< HEAD
    env_->SleepForMicroseconds(kDelayMicros);
=======
    Env::Default()->SleepForMicroseconds(kDelayMicros);
>>>>>>> 50d0f227934973e5559f2db2f3bb9b69428605a1
  }
  ASSERT_EQ(state.val, 3);
}

}  // namespace leveldb

int main(int argc, char** argv) {
  return leveldb::test::RunAllTests();
}
