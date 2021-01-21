/*
 * @Author: tekky
 * @Date: 2020-11-25 14:21:22
 * @Last Modified by: tekky
 * @Last Modified time: 2020-11-25 17:34:54
 */

#ifndef __SHM_RINGBUFFER_HPP__
#define __SHM_RINGBUFFER_HPP__

#include "utils.hh"
#include <cstdio>
#include <fcntl.h> /* O_CREAT O_RDWR */
#include <string>
#include <sys/mman.h> /* shm_open, mmap... */
#include <sys/sem.h>
#include <unistd.h>

enum Auth { owner, slave };

// shared memory based ring buffer.
// NOTE: here type T must be POD type.
template <typename T> class ShmRingBuffer {
public:
  explicit ShmRingBuffer(int semid, Auth auth = slave, std::size_t cap = 20,
                         char const *path = DEFAULT_PATH);
  ~ShmRingBuffer() {}

  // interface...
  void push_back(T const &); // write item T to ring buffer

  T pop_front(); // read from ring buffer.

private:
  typedef struct {
    std::size_t _capacity;
    int _begin; // read pointer of ring buffer.
    int _end;   // write pointer of ring buffer.
  } ShmHandler;

  bool init(std::size_t cap);
  void P(int index);
  void V(int index);

private:
  // std::size_t m_capacity;
  ShmHandler *m_hdr;
  T *m_item;
  int m_semid;
  Auth m_auth;
  std::string m_shm_path;
  std::size_t m_shm_size;
};

template <typename T>
ShmRingBuffer<T>::ShmRingBuffer(int semid, Auth auth, std::size_t cap,
                                char const *path)
    : // m_capacity(cap),
      m_hdr(nullptr), m_item(nullptr), m_semid(semid), m_auth(auth),
      m_shm_path(path), m_shm_size(0) {
  init(cap);
}

template <typename T> bool ShmRingBuffer<T>::init(std::size_t cap) {
  m_shm_size = sizeof(ShmHandler) + cap * sizeof(T);
  int shm_fd =
      shm_open(m_shm_path.c_str(), O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
  if (shm_fd < 0) {
    perror("shm_open error");
    return false;
  }

  if (m_auth == owner) {
    ftruncate(shm_fd, m_shm_size);
  }

  void *pbuf = nullptr; // shared memory address.
  pbuf = mmap(NULL, m_shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (pbuf == (void *)-1) {
    perror("mmap failed");
    return false;
  }

  m_hdr = reinterpret_cast<ShmHandler *>(pbuf);

  m_item = reinterpret_cast<T *>((char *)m_hdr + sizeof(ShmHandler));

  if (m_auth == owner) {
    m_hdr->_capacity = cap;
    m_hdr->_begin = m_hdr->_end = 0;
  }
  return true;
}

template <typename T> void ShmRingBuffer<T>::push_back(T const &e) {
  P(0);
  memcpy(m_item + m_hdr->_end, &e, sizeof(e));
  m_hdr->_end = (m_hdr->_end + 1) %
                m_hdr->_capacity; // make sure index is in range [0..._capacity)
  V(1);
}

template <typename T> T ShmRingBuffer<T>::pop_front() {
  P(1);
  T ans;
  ans = *(m_item + m_hdr->_begin);
  m_hdr->_begin = (m_hdr->_begin + 1) % m_hdr->_capacity;
  V(0);
  return ans;
}

template <typename T> void ShmRingBuffer<T>::P(int index) {
  struct sembuf buf;
  memset(&buf, 0, sizeof(buf));
  buf.sem_num = index;
  buf.sem_op = -1;
  semop(m_semid, &buf, 1);
}

template <typename T> void ShmRingBuffer<T>::V(int index) {
  struct sembuf buf;
  memset(&buf, 0, sizeof(buf));
  buf.sem_num = index;
  buf.sem_op = 1;
  semop(m_semid, &buf, 1);
}

#endif /* __SHM_RINGBUFFER_HPP__ */
