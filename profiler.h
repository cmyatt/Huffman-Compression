#ifndef PROFILER_H
#define PROFILER_H

#include "defs.h"

typedef struct {
    char *name;
    long start, end;
    long tick_count;
    float secs;
} profile_t;


typedef struct {
    profile_t *list;
    int capacity, size;
} profiler;

void init_profile_t(profile_t *p, char *name);
void update_profile_t(profile_t *p);
void print_profile_t(profile_t *p);

void init_profiler(profiler *p, int num_profiles);
void start_profile(profiler *p, char *name);
void stop_profile(profiler *p, char *name);
void print_profile(profiler *p, char *name);
void print_all(profiler *p);
void free_profiler(profiler *p);

#endif
