#ifndef KMEANS_HPP
#define KMEANS_HPP

#include "utils.hpp"
class HYPERCUBE;

class LSH;

class Cluster;
class Base;

class KMeans{
	private:
		int K; 						// number of clusters
		int total_points;			// matrix.size() aka n
		int max_iterations;			// max # of iterations for the KMeans algorithm
		string assigment_method;	// {LSH,HC,LLOYD}
		string init_method;			// {random_k_selection, kmeans++, reservoir_sampling}
		string update_method;		// {kmeans, PAM}
		string metric;				// Calculate {euclidean, cosine} distance
		vector<Cluster> clusters;	// A vector of Cluster objects


	public:
		KMeans(int x, int y, int z, string w, string i, string u, string m) : K(x), total_points(y), max_iterations(z), assigment_method(w), init_method(i), update_method(u), metric(m) {}
		// int get_K(){return K;}
		// int get_max_iterations(){return max_iterations;}
		// int get_total_points(){return total_points;}
		int get_number_of_clusters(){return clusters.size();}
		int get_number_of_points_in_a_cluster(int );
		void print_every_cluster();

		int run_kmeans(vector<vector< double> >& , string, int, Base* );

		int initialize(vector<vector< double> >&);
		void update_kmeans();	
		void assigment_lloyd(vector<vector<double> >& );
		void assigment_ptr(vector<vector< double> >& , Base*);

		bool stop_cond(vector<vector<double> >& );

		//INIT METHODS
		void random_k_selection(vector<vector< double> >& );
		void kmeansplusplus(vector<vector< double> >& );
		void reservoir_sampling(vector<vector< double> >& );
		
		//SILHOUETTE
		vector<double> silhouette();
		void create_log(string, double, vector<double>&, int );
		double calculate_a(vector< double>&, int );
		double calculate_b(vector< double>&, int );
		int find_2nd(vector<double>&, vector<double>& );


		void check_duplicates();
};


#endif