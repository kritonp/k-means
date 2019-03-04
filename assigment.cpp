#include "kmeans.hpp"

/**	
*	Return none
*	Assign datapoints to cluster following the HC or LSH algorithm
*	@params matrix{vector<vector< double> >&} - dataset
*	@params ptr{Base*} 						  - HC or LSH
*/
void KMeans::assigment_ptr(vector<vector< double> >& matrix, Base* ptr){
	vector<vector<double> > query_centroid;
	// Store K centroids to query_centroid matrix in order to process them in LSH/HC implementations
	for (int i = 0; i < K; ++i)
	{
		vector<double> centroid = clusters[i].get_centroid();
		query_centroid.push_back(vector<double>());
		query_centroid[i] = centroid;

	}
	//print_matrix_double(query_centroid);

	// Have a matrix to store ids that belong to every cluster after the assignment
	vector<vector<int> > ids_per_cluster;
	ids_per_cluster = ptr->search(matrix, query_centroid);

	//int sum = 0;
	// for (int i = 0; i < ids_per_cluster.size(); ++i)
	// {
	// 	cout << i << ". ";
	// 	sum += ids_per_cluster[i].size();
	// 	for (int j = 0; j < ids_per_cluster[i].size(); ++j)
	// 	{
	// 		cout << ids_per_cluster[i][j] << " ";
	// 	}cout << endl;
	// }
	//cout << "# returned: " << sum << endl;
	
	vector<int> assigned_ids; // A vector to keep tracking which datapoints have been assigned by the LSH/HC
	int add_raw = 0;
	bool flag_found = false; 
	for (int i = 0; i < ids_per_cluster.size(); ++i)
	{
		//cout << "Cluster: " << i << endl;
		for (int j = 0; j < ids_per_cluster[i].size(); ++j)
		{
			vector<int> cluster_to_look;
			cluster_to_look.push_back(i);

			for (int z = i+1; z < ids_per_cluster.size(); ++z)
			{	vector<int>::iterator it;
				it = find(ids_per_cluster[z].begin(),ids_per_cluster[z].end(), ids_per_cluster[i][j]);

				if(it != ids_per_cluster[z].end() ){
					int index = distance(ids_per_cluster[z].begin(), it);

					cluster_to_look.push_back(z);
					//cout << "double: id_" << arr[i][j] << " in clusters: " << z << " index: " << index << endl;
					flag_found = true;
					ids_per_cluster[z].erase (ids_per_cluster[z].begin()+index); //arr[i].erase (arr[i].begin()+j);

				}
			}
			if(flag_found == true){
				//cout << "find dist for elem w/ id_"<< ids_per_cluster[i][j] << " compare min_dist between centroids: ";
				//vector<double> take_datapoint(next(matrix[ids_per_cluster[i][j]-1].begin()), matrix[ids_per_cluster[i][j]-1].end());
				bool flag_dist = false;
				double min_dist;
				int cluster_id_min_dist = -1;
				double dist;
				for (int q = 0; q < cluster_to_look.size(); ++q)
				{
					//cout << cluster_to_look[q] << " ";
					//vector<double> take_centroid_coord(next(query_centroid[cluster_to_look[q]].begin()), query_centroid[cluster_to_look[q]].end());
					//cout << endl;
					//print_vector(query_centroid[cluster_to_look[q]]);
					if(metric.compare("euclidean") == 0)
						dist = calculateEuclidean_coord(matrix[ids_per_cluster[i][j]-1],query_centroid[cluster_to_look[q]]);
					else if(metric.compare("cosine") == 0)
						dist = calculateCosine_coord(matrix[ids_per_cluster[i][j]-1],query_centroid[cluster_to_look[q]]);
	
					if( flag_dist == false){
						min_dist = dist;
						cluster_id_min_dist = cluster_to_look[q];
						flag_dist = true;
					}
					if(dist < min_dist){
						min_dist = dist;
						cluster_id_min_dist = cluster_to_look[q];
					}
				}
				flag_found = false;
				clusters[cluster_id_min_dist].add_datapoints(matrix[ids_per_cluster[i][j]-1]);
				assigned_ids.push_back(ids_per_cluster[i][j]);
			}
			else{
				add_raw++;
				clusters[i].add_datapoints(matrix[ids_per_cluster[i][j]-1]);
				assigned_ids.push_back(ids_per_cluster[i][j]);
			}
		}
	}

	//cout << "add_raw: " << add_raw << endl;
	//print_every_cluster();
	//sort(assigned_ids.begin(),assigned_ids.end());
	// for (int i = 0; i < assigned_ids.size(); ++i)
	// {
	// 	cout << assigned_ids[i] << endl;
	// }

	if(assigned_ids.size() == matrix.size())
		return;
	// else i have datapoints that haven't been assigned by the LSH/HC
	// so i have to compare their distances with K centroids 
	// and assign them to the closest
	vector<int> unassigned_ids;
	for (int i = 0; i < matrix.size(); ++i)
	{
		if( find(assigned_ids.begin(),assigned_ids.end(),i+1) != assigned_ids.end() == 0 )
			unassigned_ids.push_back(i+1);
	}
	for (int i = 0; i < unassigned_ids.size(); ++i)
	{
		double min;
		double dist;
		int min_id_cluster;
		for (int j = 0; j < K; ++j)
		{
			if(metric.compare("euclidean") == 0)
				dist = calculateEuclidean_coord(matrix[unassigned_ids[i]-1],query_centroid[j]);
			else
				dist = calculateCosine_coord(matrix[unassigned_ids[i]-1],query_centroid[j]);

			if( i == 0){
				min = dist;
				min_id_cluster = j;
			}
			if(dist < min){
				min = dist;
				min_id_cluster = j;
			}
		}
		clusters[min_id_cluster].add_datapoints(matrix[unassigned_ids[i]-1]);
	}

	return;	
}



/**	
*	Return none
*	Assign datapoints to cluster following the lloyd's algorithm
*	@params matrix{vector<vector< double> >&} - dataset
*/
void KMeans::assigment_lloyd(vector<vector< double> >& matrix){
	for (int i = 0; i < matrix.size(); i++)
	{
		vector<double> take_data = matrix[i];	// Get one p vector-one row from the 2D matrix
		double min;
		double dist;
		bool flag = true;
		int cluster_id = -1;
		// Find nearest centroid 
		for (int j = 0; j < K; j++)
		{
			vector<double> take_centro = clusters[j].get_centroid();
			
			if(metric.compare("euclidean") == 0)
				dist = calculateEuclidean_coord(take_data, take_centro);
			else if(metric.compare("cosine") == 0)
				dist = calculateCosine_coord(take_data, take_centro);
			
			// Now i have to find the nearest centroid for the i-th datapoint
			if( flag == true )
			{
				min = dist;
				cluster_id = j;
				flag = false;
			}
			if( dist < min )
			{
				min = dist;
				cluster_id = j;
			}
			

		} // end j

		clusters[cluster_id].add_datapoints(take_data);
	}
}

