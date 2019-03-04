#include "kmeans.hpp"

/**	
*	Return 0 in case of success
*	@param matrix{vector<vector<double> >&}	- vector that dataset is stored
*	@param output_file{string}				- output filename to store log data
*	@param print_clusters_flag{int}			- print cluster's items id's if enabled
*	@param ptr{Base*}						- Base class for HC/LSH
*/
int KMeans::run_kmeans(vector<vector<double> >& matrix, string output_file, int print_clusters_flag, Base* ptr){

	// CREATE K clusters with id 0,...,K-1
	for (int i = 0; i < K; i++){
		Cluster cluster(i);
		clusters.push_back(cluster);
	}	

	// INITIALIZE k centroids
	if( initialize(matrix) != 0 )
		cerr << "Error: initialize()" << endl;

	clock_t start = clock();
	int loops = 0;
	while(true)
	{
		clock_t loop_start_time = clock();
		cout << "Loop_" << loops + 1 ;
		// ASSIGMENT datpoints
		if(assigment_method.compare("lloyd") == 0 || assigment_method.compare("LLOYD") == 0)
			assigment_lloyd(matrix);
		else
			assigment_ptr(matrix, ptr);

		// PRINT Clusters
		// print_every_cluster();

		vector<vector<double> > previous_centoids;
		for (int i = 0; i < K; i++)
		{
			previous_centoids.push_back(vector<double>());
			previous_centoids[i] = clusters[i].get_centroid();
		}

		// UPDATE centroids
		update_kmeans();

		// STOP CONDITION
		bool break_cond = stop_cond(previous_centoids);


		if( break_cond == true || loops == max_iterations - 1)
		{
			clock_t stop = clock();	
			double clustering_time = (double)(stop - start) / CLOCKS_PER_SEC;

			if(loops == max_iterations - 1){
				cout << "\nMax iterations reached. STOP!" << endl;
				cout << "k-clusters: " << get_number_of_clusters() << " loops: " << loops + 1 << endl;
			}
			else{
				cout << "\nNo change on the Centroids. STOP!" << endl
					 << "k-clusters: " << get_number_of_clusters() << " loops: " << loops + 1 << endl;
			}
			vector<double> silhouette_values = silhouette();

			create_log(output_file, clustering_time, silhouette_values, print_clusters_flag);
			return 0;
		}

		// DELETE elements in Clusters
		for (int i = 0; i < K; i++)
			clusters[i].empty_cluster();

		loops++;
		clock_t loop_end_time = clock();
		double loop_time = (double)(loop_end_time - loop_start_time) / CLOCKS_PER_SEC;
		cout << " ["<< loop_time << "s]" << endl;
	}

}


/**	
*	Return none
*	Call the specific update method
*/
void KMeans::update_kmeans(){
	if(update_method.compare("kmeans") == 0)
	{
		for (int i = 0; i < K; i++)
		{
			vector<double> neo_centroides = clusters[i].new_centroid();
			clusters[i].setCentralValue(neo_centroides);
		}

	}
	else if(update_method.compare("PAM") == 0 || update_method.compare("pam") == 0)
	{
		for (int i = 0; i < K; i++)
		{
			vector<double> neo_centroides = clusters[i].new_centroid_PAM(metric);
			clusters[i].setCentralValue(neo_centroides);
		}

	}
	else
	{
		cout << "Error: update_method doesn't exists" 	<< endl
			 << "Try: kmeans or PAM" 					<< endl;
		exit(EXIT_FAILURE);
	}

	return;
}

/**	
*	Return true if the centroids remain the same as before
*	Otherwise return false
*	@param previous_centoids{vector<vector<double> >&}	- Centroids
*/
bool KMeans::stop_cond(vector<vector<double> >& previous_centoids){
	int break_cond = 0;
	for (int i = 0; i < K; ++i)
	{
		vector<double> t1(next(previous_centoids[i].begin()),previous_centoids[i].end());
		vector<double> tmp = clusters[i].get_centroid();
		vector<double> t2(next(tmp.begin()),tmp.end());
		assert(t1.size() == t2.size()); 
		if(t1 == t2)
			break_cond++;
		
	}
	if(break_cond == K)
		return true;
	else
		return false;

}



//debbuging purposes only
void KMeans::print_every_cluster(){
	for (int i = 0; i < K; ++i)
	{
		cout << "**********************\n";
		clusters[i].print_cluster(i);
	}
}

