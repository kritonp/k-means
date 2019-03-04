#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath> 			// std::floor/ceil/pow
#include <random>			// std::normal_distribution
#include <tuple>			// std::make_tuple
#include <functional> 		// std::minus, std::divides 
#include <numeric> 			// std::inner_product 
#include <cassert>			// std::assert
#include <unordered_map> 	// hash table
#include <list> 			// std::lists
#include <iterator> 
#include <bitset>			// std::bitset
#include <functional> 		// std::hash
#include <unordered_map> 	// std::hash
#include <algorithm>
#include <iomanip>      	// std::setprecision
#include <ctime>


#define MIN 0.0
#define MAX 4.0
#define MEAN 0.0
#define VARIANCE 1.0
#define W 5.0
using namespace std;

#ifndef UTILS_HPP
#define UTILS_HPP

#include "LSH/lsh.hpp"
#include "HC/hypercube.hpp"
#include "kmeans.hpp"
#include "cluster.hpp"
// #include "base.hpp"

void delete_centroids(vector<vector< double> >& );

tuple<string, string, string, string, int, string, string, string, int> cmd_arguments(int argc,char* argv[]);
tuple<int, int, int, double> configure_file(string);

int read_file(string, vector<vector< double> >& );
int read_file_umap(string , unordered_map<string, vector<int> >& );

void print_umap(unordered_map<string, vector<int> >& );

double random_generator(double, double);
 double uniform_random_generator(double, double);
vector<double> gaussian_distribution_vector(int);

void print_matrix(vector<vector<int> >&);			//debugging purposes
void print_matrix_double(vector<vector< double> >&);	//debugging purposes
void print_vector(vector< double>& );

double calculateCosine(vector<vector< double> >& , vector< double> , int );
double calculateEuclidean(vector<vector< double> >& , vector< double> , int  );

double calculateEuclidean_new(vector< double>& , vector< double>& );
double calculateCosine_new(vector<double>& , vector<double>&);

double calculateEuclidean_coord(vector< double>& , vector< double>& );
double calculateCosine_coord(vector<double>& , vector<double>&);

double closest_NN(vector<vector< double> >& , vector< double>& , string);
void help();
#endif