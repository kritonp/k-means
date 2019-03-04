#ifndef LSH_HPP
#define LSH_HPP

#include "../utils.hpp"
#include "HashTable.hpp"
#include "../base.hpp"

class HT;

class LSH: public Base{
	private :
	 	vector<HT> hts;						// A vector of L-HT
		vector<vector<vector<double> > >vs;	// A vector of 2D's matrices in order to use the same for query set
		vector< double> ts;					// A vector to store t's in order to use the same for query set
		int k;								// Number of k h(i) functions
		int L;								// Number of HTs
		string metric;						// {euclidean,cosine}, default: euclidean
		int TableSize;						// TableSize for euclidean: n/2, TableSize for cosine: 2^k	
		double R;							// Radius, is in queryset file, 1st line

	public:
		LSH(int x, int y, string z, int w, double u) : k(x), L(y), metric(z), TableSize(w), R(u) {} //
		int insert(vector<vector< double> >&) override;
		vector<vector<int> > search(vector<vector< double> >&, vector<vector< double> >&) override;

		// int get_k(){return k;}
		// int get_L(){return L;}
		// string get_metric(){return metric;}

		void print_vector_of_HT();
		void print_vector_of_vs();
};


#endif