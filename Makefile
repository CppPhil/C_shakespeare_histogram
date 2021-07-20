CC = clang
FLAGS = -g -Wall -Wextra

all: ShakespeareHistogramApp

ShakespeareHistogramApp: histogram.o is_all_whitespace.o main.o
	$(CC) $(FLAGS) histogram.o is_all_whitespace.o main.o -o ShakespeareHistogramApp

histogram.o: shakespeare_histogram/histogram.c
	$(CC) $(FLAGS) -c shakespeare_histogram/histogram.c

is_all_whitespace.o: shakespeare_histogram/is_all_whitespace.c
	$(CC) $(FLAGS) -c shakespeare_histogram/is_all_whitespace.c

main.o: shakespeare_histogram/main.c
	$(CC) $(FLAGS) -c shakespeare_histogram/main.c

clean:
	rm -f ShakespeareHistogramApp *.o
