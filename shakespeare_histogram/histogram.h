#ifndef INCG_HISTOGRAM_H
#define INCG_HISTOGRAM_H
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct HistogramOpaque Histogram;

Histogram* histogramCreate();

void histogramFree(Histogram* histogram);

bool histogramAdd(Histogram* histogram, const char* word);

size_t histogramFetchCountOf(Histogram* histogram, const char* word);

void histogramPrint(Histogram* histogram, FILE* stream);
#endif // INCG_HISTOGRAM_H
