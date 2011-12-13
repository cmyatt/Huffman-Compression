#ifndef PROFILER_H
#define PROFILER_H

#include "defs.h"

void init_profile_t(profile_t *p, char *name) {
    p->name = name;
    p->end = p->tick_count = 0;
    p->secs = 0.0f;
    p->start = clock();
}


void update_profile_t(profile_t *p) {
    p->tick_count = p->end - p->start;
    p->secs = (float)p->tick_count / (float)CLOCKS_PER_SEC;
}


void print_profile_t(profile_t *p) {
    printf("== %s ==\n", p->name);
    printf("clock ticks: %ld\n", p->tick_count);
    printf("time (secs): %f\n", p->secs);
}


void init_profiler(profiler *p, int num_profiles) {
    p->capacity = num_profiles;
    p->size = 0;
    p->list = (profile_t *) malloc(p->capacity * sizeof(profile_t));
}


void start_profile(profiler *p, char *name) {
    p->size++;
    if (p->size >= p->capacity) {
        p->capacity *= MEM_COEFF;
        profile_t temp[p->size-1];
        int i;

        for (i = 0; i < p->size-1; ++i) {
            temp[i] = p->list[i];
        }

        free(p->list);
        p->list = (profile_t *) malloc(p->capacity * sizeof(profile_t));
        for (i = 0; i < p->size-1; ++i) {
            p->list[i] = temp[i];
        }
    }
    profile_t elem;
    init_profile_t(&elem, name);
    p->list[p->size-1] = elem;
}


void stop_profile(profiler *p, char *name) {
    int i;
    for (i = 0; i < p->size; ++i) {
        if (strcmp(p->list[i].name, name) == 0) {
            p->list[i].end = clock();
            update_profile_t(&(p->list[i]));
            break;
        }
    }
}


void print_profile(profiler *p, char *name) {
    int i;
    for (i = 0; i < p->size; ++i) {
        if (strcmp(p->list[i].name, name) == 0) {
            print_profile_t(&(p->list[i]));
            break;
        }
    }
}


void print_all(profiler *p) {
    int i;
    for (i = 0; i < p->size; ++i) {
        print_profile_t(&(p->list[i]));
        printf("\n");
    }
}


void free_profiler(profiler *p) {
    free(p->list);
}

#endif
