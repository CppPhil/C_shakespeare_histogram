#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "histogram.h"

struct Pair {
    char * word;
    size_t count;
};

struct HistogramImpl {
    struct Pair *data;
    size_t       pairCount;
};

static struct HistogramImpl *toImpl(Histogram *histogram)
{
    return (struct HistogramImpl *) histogram;
}

static Histogram *toOpaque(struct HistogramImpl *histogramImpl)
{
    return (Histogram *) histogramImpl;
}

Histogram *histogramCreate()
{
    struct HistogramImpl *hist = malloc(sizeof(struct HistogramImpl));

    if (hist == NULL) {
        return NULL;
    }

    hist->data      = NULL;
    hist->pairCount = 0;

    return toOpaque(hist);
}

void histogramFree(Histogram *histogram)
{
    struct HistogramImpl *hist = toImpl(histogram);

    if (hist == NULL) {
        return;
    }

    if (hist->data == NULL) {
        free(hist);
        return;
    }

    for (size_t i = 0; i < hist->pairCount; ++i) {
        free(hist->data[i].word);
    }

    free(hist->data);
    free(hist);
}

static int bsearchCmp(const void *a, const void *b)
{
    const char *       wordToLookFor = a;
    const struct Pair *pair          = b;

    return strcmp(wordToLookFor, pair->word);
}

static char *duplicateString(const char *string)
{
    if (string == NULL) {
        return NULL;
    }

    const size_t length = strlen(string);
    char *       copy   = malloc(length + 1);

    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, string, length + 1);

    return copy;
}

static void lowercase(char *string)
{
    while (*string != '\0') {
        *string = tolower(*string);
        ++string;
    }
}

static bool isLowercase(char c)
{
    return c >= 0x61 && c <= 0x7A;
}

static void removeNonLowercaseCharacters(char *string)
{
    size_t stringLength = strlen(string);

    for (size_t i = 0; i < stringLength; ++i) {
        if (!isLowercase(string[i])) {
            memmove(string + i, string + i + 1, stringLength - i - 1);
            string[stringLength - 1] = '\0';
            --stringLength;
            --i;
        }
    }
}

static size_t
insertionPoint(const char *wordToInsert, struct Pair *data, size_t elementCount)
{
    if (elementCount < 1) {
        return 0;
    }

    ptrdiff_t low  = 0;
    ptrdiff_t high = elementCount - 1;
    ptrdiff_t middle;
    ptrdiff_t position;

    while (low <= high) {
        middle = low + (high - low) / 2;

        const int code = strcmp(wordToInsert, data[middle].word);

        if (code == 0) {
            return middle;
        }
        else if (code < 0) {
            high     = middle - 1;
            position = middle;
        }
        else {
            low      = middle + 1;
            position = low;
        }
    }

    return position;
}

bool histogramAdd(Histogram *histogram, const char *word)
{
    struct HistogramImpl *hist = toImpl(histogram);

    char *wordToInsert = duplicateString(word);

    if (wordToInsert == NULL) {
        return false;
    }

    lowercase(wordToInsert);
    removeNonLowercaseCharacters(wordToInsert);

    struct Pair *pairFound = bsearch(
        wordToInsert,
        hist->data,
        hist->pairCount,
        sizeof(struct Pair),
        &bsearchCmp);

    if (pairFound == NULL) {
        const size_t insertionPointIndex
            = insertionPoint(wordToInsert, hist->data, hist->pairCount);

        const size_t newPairCount = hist->pairCount + 1;
        struct Pair *newMemory
            = realloc(hist->data, sizeof(struct Pair) * newPairCount);

        if (newMemory == NULL) {
            free(wordToInsert);
            return false;
        }

        memmove(
            newMemory + insertionPointIndex + 1,
            newMemory + insertionPointIndex,
            (hist->pairCount - insertionPointIndex) * sizeof(struct Pair));

        newMemory[insertionPointIndex] = (struct Pair){wordToInsert, 1};
        hist->data                     = newMemory;
        hist->pairCount                = newPairCount;
    }
    else {
        ++pairFound->count;
        free(wordToInsert);
    }

    return true;
}

size_t histogramFetchCountOf(Histogram *histogram, const char *word)
{
    struct HistogramImpl *hist = toImpl(histogram);

    char *wordToFind = duplicateString(word);

    if (wordToFind == NULL) {
        return 0;
    }

    lowercase(wordToFind);
    removeNonLowercaseCharacters(wordToFind);

    struct Pair *pairFound = bsearch(
        wordToFind,
        hist->data,
        hist->pairCount,
        sizeof(struct Pair),
        &bsearchCmp);

    free(wordToFind);

    if (pairFound == NULL) {
        return 0;
    }

    return pairFound->count;
}

static int qsortCmp(const void *a, const void *b)
{
    const struct Pair *lhs = a;
    const struct Pair *rhs = b;

    if (lhs->count > rhs->count) {
        return -1;
    }

    if (lhs->count < rhs->count) {
        return 1;
    }

    return 0;
}

void histogramPrint(Histogram *histogram, FILE *stream)
{
    struct HistogramImpl *hist = toImpl(histogram);

    struct Pair *pairs = malloc(hist->pairCount * sizeof(struct Pair));

    if (pairs == NULL) {
        return;
    }

    memcpy(pairs, hist->data, hist->pairCount * sizeof(struct Pair));
    qsort(pairs, hist->pairCount, sizeof(struct Pair), &qsortCmp);

    for (size_t i = 0; i < hist->pairCount; ++i) {
        fprintf(stream, "\"%s\" => %zu\n", pairs[i].word, pairs[i].count);
    }

    free(pairs);
}
