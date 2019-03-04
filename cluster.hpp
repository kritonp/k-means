#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "utils.hpp"

class Cluster {

	private:
		int id_cluster;
		int number_of_elements_in_cluster = 0;
		vector<vector<double> > cluster_points;

		vector<double> central_values; // cluster centroid


	public:
		Cluster(int x) : id_cluster(x) {}

		void add_datapoints(vector<double>& ); 			// points.push_back(point);
		void print_cluster(int );
		bool removePoint(int id_point );

		vector<vector< double> > return_cluster_points(){return cluster_points;}
		vector< double> new_centroid();
		vector< double> new_centroid_PAM(string);
		int getTotalPoints();
		int getID(){return id_cluster;}
		void setCentralValue(vector< double>& values){
			central_values = values;
			//print_vector(central_values);
			//central_values[index].push_back(values);
		}
		vector< double> get_centroid(){
			return central_values;
		}

		void print_centralvalues();
		void set_id_cluster(int index){
			id_cluster = index;
		}
		int get_id_cluster(){
			return id_cluster;
		}
		void empty_cluster();
		int get_number_of_elements_in_cluster(){return number_of_elements_in_cluster;}
		void output_clusters(int, ofstream& );

};


#endif