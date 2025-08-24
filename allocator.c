#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef char ALIGN[16];

// Union instead of struct want to Header fullfill alignment 16 bytes
union header {
  struct {
    size_t size;        // 8 Byte if architecture 64 bit
    unsigned is_free;   // 1 = free, 0 = use // 4 byte
    union header *next; // 8 byte
  } s;
  ALIGN stub;
};

// max(sizeof(struct), sizeof(stub)) = max(20, 16)

typedef union header header_t;

header_t *head = NULL, *tail = NULL;
pthread_mutex_t global_malloc_lock;

// malloc(unsigned long)

header_t *get_free_block(size_t size) {
  header_t *curr = head;
  while (curr) {
    if (curr->s.is_free && curr->s.size >= size) {
      return curr;
    }
    curr = curr->s.next;
  }
  return NULL;
}

void free(void *block) {
  header_t *header, *tmp;
  void *programbreak;

  if (!block) {
    return;
  }

  pthread_mutex_lock(&global_malloc_lock);
  header = (header_t *)block - 1;
  programbreak = sbrk(0);

  if ((char *)block + header->s.size == programbreak) {
    if (head == tail) {
      head = tail = NULL;
    } else {
      tmp = head;
      if (tmp->s.next == tail) {
        tmp->s.next = NULL;
        tail = tmp;
      }
    }
  }

  sbrk(0 - header->s.size - sizeof(header_t));

  pthread_mutex_unlock(&global_malloc_lock);
}

void *malloc(size_t size) {
  size_t total_size;
  void *block;
  header_t *header;

  if (!size) {
    return NULL; // C standard if malloc(0) returning NULL or dummy pointer
  }

  pthread_mutex_lock(&global_malloc_lock);
  header = get_free_block(size);
  if (header) {
    header->s.is_free = 0;
    pthread_mutex_unlock(&global_malloc_lock);
    // header point to  metadata, header + 1 pointer after
    // header location data can use on each user
    return (void *)(header + 1);
  }

  total_size = sizeof(header_t) + size;
  block = sbrk(total_size);

  // if sbrk failed because doesn't have enough memory returning NULL
  if (block == (void *)-1) {
    pthread_mutex_unlock(&global_malloc_lock);
    return NULL;
  }

  header = block;
  header->s.size = size;
  header->s.is_free = 0;
  header->s.next = NULL;

  if (!head) {
    head = header;
  }

  if (tail) {
    tail->s.next = header;
  }

  tail = header;
  pthread_mutex_unlock(&global_malloc_lock);

  return (void *)(header + 1);
}

// int *arr = (int*)malloc(4 * sizeof(int));
// total_size = sizeof(header_t) + 16 = 32 + 16 = 48
// srbk(48)

void *calloc(size_t num, size_t nsize) {
  size_t size;
  void *block;

  if (!num || !nsize) {
    return NULL;
  }

  // calloc(10, size(int)) = 10 * 4 = 40 byte (if int = 4 byte)
  size = num * nsize;

  if (nsize != size / num) {
    return NULL; // prevent integer overflow
    // if size too lower it can be raise buffer overflow
  }
  block = malloc(size);

  if (!block) {
    return NULL;
  }
  memset(block, 0, size);

  return block;

  // memset(pointer, 0, n) -> 0x00
  // num = 0x8000000000, nsize = 0;
  // size = num * size - 0x4000000000000000 -> overflow -> 0x000000000 (if 32
  // bit)

  // num = 10, nsizem = 20
}

void *realloc(void *block, size_t size) {
  header_t *header;
  void *ret;

  if (!block || !size) {
    return malloc(size);
  }

  header = (header_t *)block - 1; // block pointer to data

  if (header->s.size >= size) {
    return block;
  }

  ret = malloc(size);
  if (ret) {
    memcpy(ret, block, header->s.size);
    free(block);
  }
  return ret;
}

void print_mem_list() {
  header_t *curr = head;
  printf("head = %p, tail = %p \n", (void *)head, (void *)tail);
  while (curr) {
    printf("address = %p, size = %zu, is_free = %u, next = %p\n", (void *)curr,
           curr->s.size, curr->s.is_free, (void *)curr->s.next);
    curr = curr->s.next;
  }
}

int main() {
  printf("malloc testing \n");
  int *first_pointer = (int *)malloc(16);
  printf("malloc(16) -> %p\n", first_pointer);
  print_mem_list();
  printf("\n");

  int *second_pointer = (int *)malloc(32);
  printf("malloc(32) -> %p\n", second_pointer);
  print_mem_list();
  printf("\n");

  free(first_pointer);
  print_mem_list();
  printf("\n");

  int *third_pointer = (int *)calloc(4, sizeof(int));
  printf("calloc(4, int) -> %p\n", third_pointer);
  print_mem_list();
  printf("\n");

  third_pointer = (int *)realloc(third_pointer, 64);
  printf("realloc(third_pointer, 64) -> %p\n", third_pointer);
  print_mem_list();
  printf("\n");

  free(second_pointer);
  free(third_pointer);
  print_mem_list();
  return 0;
}
