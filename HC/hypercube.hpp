#ifndef HYPERCUBE_HPP
#define HYPERCUBE_HPP

#include "../utils.hpp"
#include "../LSH/HashTable.hpp"
#include "../base.hpp"

class HT;


class HYPERCUBE: public Base{
	private :
		vector<HT> hts;						// A vector of L-HT
		vector<vector<vector<double> > >vs; // A vector of 2D's matrices in order to use the same for query set
		vector<double> ts;					// A vector to store t's in order to use the same for query set
		vector< tuple<int,int> > tuple_list;// Store h's if already flipped and their value: {0,1}


		int k;								// Number of k h(i) functions
		int M;								// Number of points that i will check in total
		int probes;							// Number of neigbor vetrices that i will check in total
		string metric;						// {euclidean,cosine}, default: euclidean
		int TableSize;						// TableSize for euclidean: 2^log10(n)
		double R;							// Radius, is in queryset file, 1st line

	public:
		HYPERCUBE(int x, int y, int p, string z, int w, double u) : k(x), M(y), probes(p),metric(z), TableSize(w), R(u) {} //
		int insert(vector<vector< double> >& ) override;
		vector<vector<int> > search(vector<vector< double> >&, vector<vector< double> >&) override;	
		
		void print_hc_HT();					//debug purposes only
};



#endif