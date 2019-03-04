OBJS = main.o utils.o kmeans.o cluster.o silhouette.o initialization.o assigment.o LSH/lsh.o LSH/HashTable.o HC/hypercube.o
SOURCE = main.cpp utils.cpp kmeans.cpp cluster.cpp silhouette.cpp initialization.cpp assigment.cpp LSH/lsh.cpp LSH/HashTable.cpp HC/hypercube.cpp
HEADER = utils.hpp kmeans.hpp cluster.hpp LSH/lsh.hpp LSH/HashTable.hpp HC/hypercube.hpp base.hpp
OUT = cluster
CC = g++
CXXFLAGS=-std=c++11 -O3
#FLAGS = -g -Wall
all: $(OBJS)
	$(CC) -Wall -Werror -g $(OBJS) -o $(OUT)

#main.o: main.cpp
#	$(CC) $(FLAGS) main.cpp

#utils.o: utils.cpp
#	$(CC) $(FLAGS) utils.cpp

# clean up
clean:
	rm -f $(OBJS) $(OUT)