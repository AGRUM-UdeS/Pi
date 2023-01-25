#ifndef MEASURE_H
#define MEASURE_H

typedef struct measure_t {
  int a;
} measure_t;

typedef struct measure_state_t {
int a;
} measure_state_t;

measure_state_t measure_sm(measure_t* measurements);

#endif
