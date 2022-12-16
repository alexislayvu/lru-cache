#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CACHE_SIZE 20

typedef struct {
  int fib_num;
  int fib_value;
  int recently_used_counter;
} LRU;

LRU lru_cache[CACHE_SIZE];
int memoization_cache[CACHE_SIZE];
float when_called = 0;
float found_result_in_cache = 0;

void print_run_instructions();
unsigned long long lru_cache_fibonacci(int num);
unsigned long long memoization_cache_fibonacci(int num);
unsigned long long iterative_fibonacci(int num);
unsigned long long recursive_fibonacci(int num);

int main(int argc, char *argv[]) {
  /* if the user asks for help (-h)*/
  if (strcmp(argv[1], "-h") == 0) {
    print_run_instructions();
    exit(1);
  }

  if (argc < 3) {
    printf("Not enough arguments supplied. Type './main -h' for more "
           "information.\n");
    exit(1);
  }

  int num_of_tests = atoi(argv[1]);
  int max_fib_value = atoi(argv[2]);

  for (int ix = 0; ix < num_of_tests; ++ix) {
    int fib_to_get = rand() % max_fib_value;
    lru_cache_fibonacci(fib_to_get);
  }

  float cache_hit_ratio = (found_result_in_cache / when_called) * 100;

  printf("\nCache size %d, max_fib_value = %d, %.2f%% cache hit ratio\n",
         CACHE_SIZE, max_fib_value, cache_hit_ratio);

  return 0;
}

/* print instructions on how to run the program */
void print_run_instructions() {
  printf("\nHow to run the program:\n");
  printf("./main [num_of_tests] [max_fib_value]\n\n");
  printf("num_of_tests - The number of tests you want to run.\n");
  printf("max_fib_value - The maximum fib value you want to calculate.\n\n");
  printf("Here is an example of what you can type in the Console (try it out "
         "yourself):\n");
  printf("./main 40 25\n");
}

/* compute the Nth Fibonacci number using an LRU cache */
unsigned long long lru_cache_fibonacci(int num) {
  when_called++;

  /* if the value is found in lru_cache, return it */
  for (int ix = 0; ix < CACHE_SIZE; ++ix) {
    if (lru_cache[ix].fib_num == num) {
      found_result_in_cache++;
      lru_cache[ix].recently_used_counter++;

      return lru_cache[ix].fib_value;
    }
  }

  int calculate_fib_value = iterative_fibonacci(num);

  /* if there is an empty cache slot, put it there */
  for (int iy = 0; iy < CACHE_SIZE; ++iy) {
    if (lru_cache[iy].fib_value == 0) {
      lru_cache[iy].fib_num = num;
      lru_cache[iy].fib_value = calculate_fib_value;
      lru_cache[iy].recently_used_counter = 0;

      return lru_cache[iy].fib_value;
    }
  }

  /* if there is no empty slot, replace the least recently used entry */
  int index = 0;
  int lru_entry = lru_cache[0].recently_used_counter;

  for (int iz = 0; iz < CACHE_SIZE; ++iz) {
    if (lru_cache[iz].recently_used_counter < lru_entry) {
      lru_entry = lru_cache[iz].recently_used_counter;
      index = iz;
    }
  }

  lru_cache[index].fib_num = num;
  lru_cache[index].fib_value = calculate_fib_value;
  lru_cache[index].recently_used_counter = 0;

  return calculate_fib_value;
}

/* compute the Nth Fibonacci number using a memoization cache */
unsigned long long memoization_cache_fibonacci(int num) {
  if (num == 0) {
    return 0;
  } else if (num == 1) {
    return 1;
  }

  if (memoization_cache[num] != 0) {
    return memoization_cache[num];
  }

  int result = memoization_cache_fibonacci(num - 1) +
               memoization_cache_fibonacci(num - 2);

  memoization_cache[num] = result;

  return result;
}

/* compute the Nth Fibonacci number iteratively */
unsigned long long iterative_fibonacci(int num) {
  int num1 = 1;
  int num2 = 1;
  int result = 0;

  for (int i = 0; i < num; ++i) {
    if (i < 2) {
      result = 1;
    } else {
      result = num1 + num2;
      num1 = num2;
      num2 = result;
    }
  }

  return result;
}

/* compute the Nth Fibonacci number recursively */
unsigned long long recursive_fibonacci(int num) {
  if (num == 0) {
    return 0;
  } else if (num == 1) {
    return 1;
  }

  return recursive_fibonacci(num - 1) + recursive_fibonacci(num - 2);
}
