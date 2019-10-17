CC=gcc
CFLAGS=-g -std=c99 -Wall
#LDFLAGS=-lm

test2a:
	./part2a #no parameters
	./part2a "only_one_parameter"
	./part2a "biggest parameter" "at" "start"
	./part2a "biggest" "parameter" "at" "end" "very very very big parameter"
	./part2a "answer" "somewhere" "in" "the" "middle"
	./part2a "two" "strings" "the" "same" "length" "ha ha !"

test2b:
	./part2b && exit 1 || [ $$? -eq 1 ] #no parameters
	./part2b "too" "many" "parameters" && exit 1 || [ $$? -eq 1 ]
	./part2b "-g" "50" && exit 2 || [ $$? -eq 2 ]
	./part2b "-c" "fifty" && exit 3 || [ $$? -eq 3 ]
	./part2b "-g" "fifty" && exit 2 || [ $$? -eq 2 ]
	./part2b "-c" "-275" && exit 4 || [ $$? -eq 4 ]
	./part2b "-f" "-35" && exit 4 || [ $$? -eq 4 ]
	./part2b "-c" "-273.15"
	./part2b "-f" "0"
	./part2b "-c" "10"
	./part2b "-f" "50"
