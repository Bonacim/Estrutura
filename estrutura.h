#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef HEADERS
const int max_iteration = 1000;
void read_inputs();
void prepare_environment();
bool converged();
void calculate_forces();
void calculate_dispacements();
void render_window();
void print_failure();
#define HEADERS
#endif
