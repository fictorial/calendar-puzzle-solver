#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "pcg_basic.h"

typedef int board[7][7];

#define INVALID_COORD(x, y) ((y<2 && x==6) || (y==6 && x>2) || y>6 || x>6 || y<0 || x<0)

typedef int pieceform[4][4];

typedef struct {
  int id;
  int nforms;
  pieceform forms[8];
} piece;

#define MAX_PIECE_SQUARES_SET 6

piece pieces[8] = {
  { 1, 8,
    {
      { {1, 1, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {1, 1, 1, 1}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {1, 1, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, },
      { {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, },
      { {1, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {0, 0, 0, 1}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, },
      { {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, },
    }
  },
  { 2, 8,
    {
      { {1, 1, 1, 1}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {0, 1, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, },
      { {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, },
      { {1, 1, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {0, 0, 1, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, },
      { {1, 0, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, },
    },
  },
  { 3, 8,
    {
      { {1, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {1, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, },
      { {0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, },
      { {0, 1, 1, 1}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {0, 0, 1, 1}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, },
      { {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, },
    },
  },
  { 4, 2,
    {
      { {1, 1, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {1, 1, 0, 0}, {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, },
    },
  },
  { 5, 4,
    {
      { {1, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {1, 1, 1, 0}, {1, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {1, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, },
      { {1, 1, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, },
    },
  },
  { 6, 4,
    {
      { {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, },
      { {0, 0, 1, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, },
      { {0, 1, 1, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, },
      { {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, },
    },
  },
  { 7, 8,
    {
      { {1, 0, 0, 0}, {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, },
      { {0, 1, 0, 0}, {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, },
      { {1, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, },
      { {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, },
      { {0, 1, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {1, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {1, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
      { {1, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, },
    },
  },
  { 8, 4,
    {
      { {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, },
      { {1, 1, 1, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, },
      { {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, },
      { {0, 0, 1, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, },
    },
  }
};

char *labels[][7] = {
  {"Jan", "Feb", "Mar", "Apr", "May", "Jun", NULL},
  {"Jul", "Aug", "Sep", "Oct", "Nov", "Dec", NULL},
  {"1", "2", "3", "4", "5", "6", "7"},
  {"8", "9", "10", "11", "12", "13", "14"},
  {"15", "16", "17", "18", "19", "20", "21"},
  {"22", "23", "24", "25", "26", "27", "28"},
  {"29", "30", "31", NULL, NULL, NULL, NULL}
};

typedef struct {
  char key[7];  /* e.g. "Jan-31" */
  board b;
} solution;

solution solutions[372];
int n_solved = 0;
pthread_mutex_t mutex;

pcg32_random_t rng;

void print_board(board b) {
  for (int y=0; y<7; ++y) {
    for (int x=0; x<7; ++x) {
      if (INVALID_COORD(x, y)) continue;
      if (b[y][x])
        fprintf(stderr, "%d ", b[y][x]);
      else
        fprintf(stderr, "- ");
    }
    fprintf(stderr, "\n");
  }
}

void print_pieceform(pieceform pf, int pid) {
  for (int y=0; y<4; ++y) {
    for (int x=0; x<4; ++x)
      if (pf[y][x])
        fprintf(stderr, "%d ", pid);
      else
        fprintf(stderr, "- ");
    fprintf(stderr, "\n");
  }
  fprintf(stderr, "\n");
}

void print_pieceforms() {
  for (int i=0; i<8; ++i) {
    piece *p = &pieces[i];
    fprintf(stderr, "piece %d has %d forms:\n\n", i, p->nforms);
    for (int j=0; j < p->nforms; ++j)
      print_pieceform(p->forms[j], p->id);
  }
}

void print_solutions() {
  printf("const solutions = {\n");

  for (int i = 0; i < 372; ++i) {
    solution *s = &solutions[i];

    printf("  \"%s\": [\n", s->key);

    for (int y=0; y<7; ++y) {
      printf("  [");

      for (int x=0; x<7; ++x) {
        if (INVALID_COORD(x,y)) continue;
        printf("%d,", s->b[y][x] == 0 ? -1 : s->b[y][x]);
      }

      printf("],\n");
    }

    printf("  ],\n");
  }

  printf("};\n");
}

int place_at(board b, int pid, pieceform pf, int x, int y) {
  int coords[MAX_PIECE_SQUARES_SET][2];
  int ci = 0;

  for (int fy = 0; fy < 4; ++fy) {
    int cy = y + fy;

    for (int fx = 0; fx < 4; ++fx) {
      if (!pf[fy][fx]) continue;

      int cx = x + fx;
      if (INVALID_COORD(cx, cy)) return 0;
      if (b[cy][cx]) return 0;

      coords[ci][0] = cy;
      coords[ci][1] = cx;
      ++ci;
    }
  }

  for (int i = 0; i < ci; ++i)
    b[coords[i][0]][coords[i][1]] = pid;

  return 1;
}

int place(board b, int pid, pieceform pf) {
  for (int y=0; y<7; ++y) {
    for (int x=0; x<7; ++x) {
      if (INVALID_COORD(x,y)) continue;
      if (place_at(b, pid, pf, x, y)) return 1;
    }
  }
  return 0;
}

int check(board b, char **out_month, char **out_day) {
  int months_uncovered = 0;
  int months_covered = 0;
  int days_uncovered = 0;
  int days_covered = 0;
  int total_uncovered = 0;

  // TODO would it be faster to avoid the branches?

  for (int y=0; y<7; ++y) {
    int is_month = y < 2;

    for (int x=0; x<7; ++x) {
      if (INVALID_COORD(x,y)) continue;

      if (b[y][x]) {
        if (is_month) {
          if (++months_covered == 12) return 0;
        } else {
          if (++days_covered == 31) return 0;
        }
      } else {
        if (is_month) {
          if (++months_uncovered == 2) return 0;
          *out_month = labels[y][x];
        } else {
          if (++days_uncovered == 2) return 0;
          *out_day = labels[y][x];
        }
      }
    }
  }

  return 1;
}

void shuffle(int a[], int n) {
  for (int i = n - 1; i > 0; i--) {
    int j = pcg32_boundedrand_r(&rng, i+1);
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
  }
}

void *solve(void *_) {
  board b;
  int piece_order[8] = {0,1,2,3,4,5,6,7};

  while (n_solved < 372) {
    memset(b, 0, sizeof(board));

    shuffle(piece_order, 8);

    int n_placed = 0;
    int did_place = 0;

    for (int i = 0; i < 8; ++i) {
      piece *p = &pieces[piece_order[i]];

      int form_order[p->nforms];
      for (int j = 0; j < p->nforms; ++j)
        form_order[j] = j;
      shuffle(form_order, p->nforms);

      for (int j = 0; j < p->nforms; ++j) {
        if (place(b, p->id, p->forms[form_order[j]])) {
          did_place = 1;
          ++n_placed;
          break;
        }
      }

      if (!did_place)
          break;
    }

    if (n_placed == 8) {
      char *month, *day;

      if (check(b, &month, &day)) {
        pthread_mutex_lock(&mutex);

        char key[7];
        snprintf(key, 7, "%s-%s", month, day);

        int is_new_solution = 1;
        for (int k=0; k < n_solved; ++k) {
          solution *s = &solutions[k];
          if (strncmp(s->key, key, 7) == 0) {
            is_new_solution = 0;
            break;
          }
        }

        if (is_new_solution) {
          solution *s = &solutions[n_solved++];

          strncpy(s->key, key, 7);
          memcpy(s->b, b, sizeof(board));

          fprintf(stderr, "%.3f%% done           \r", (n_solved/372.0) * 100.0);
        }

        pthread_mutex_unlock(&mutex);
      }
    }
  }

  fprintf(stderr, "\n");
  return (void *)NULL;
}

int main(int argc, char **argv) {
  if (argc == 1) {
    fprintf(stderr, "usage: %s <cpu_count>\n", argv[0]);
    return 1;
  }

  int n_cpus = atoi(argv[1]);
  if (n_cpus <= 0 || n_cpus > 64) {
    fprintf(stderr, "usage: %s <cpu_count>\n", argv[0]);
    return 1;
  }

  int rounds = 5;
  pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&rounds);

  pthread_t threads[n_cpus];

  for (int i = 0; i < n_cpus; ++i)
    pthread_create(&threads[i], NULL, &solve, NULL);

  for (int i = 0; i < n_cpus; ++i)
    pthread_join(threads[i], NULL);

  print_solutions();

  return 0;
}
