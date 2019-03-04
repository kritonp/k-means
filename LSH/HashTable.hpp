#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include "../utils.hpp"

class HT{
	private:
		vector< list<tuple< int, string > > > dHT;	// A vector of lists of <item_id, g_value>
	public:
		HT(){}
		~HT(){}
		void print_HT();
		void print_HT_HC();
		void print_bucket_HT(int);
		void resize_HT(int);
		void insert_HT(int, int, string);
		int search_HT(int, string);
		int size_HT();
		int bucket_size_HT(int);
		vector<int> return_bucket_elements_HT(int);
		vector<int> return_bucket_elements_HT_new(int, string);

};


#endif