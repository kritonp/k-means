#include "kmeans.hpp"

/**	
*	Return vector<double> a list of K + 1 double silhouette values
*	The last one is the total silhouette value
*	@params none
*/
vector<double> KMeans::silhouette(){
	clock_t start_silhouette = clock();	
	double overall_silhouette_evaluation;
	vector<double> cluster_sil_val;
	int empty = 0;
	for (int i = 0; i < clusters.size(); ++i)
	{

		vector<vector<double> > cluster_points = clusters[i].return_cluster_points();
		vector<double> centro = clusters[i].get_centroid();
		vector<double> centroid_c(next(centro.begin()),centro.end());
		double avg_cluster_silhouette = 0.0;
		//cout << "centroid: ";		//print_vector(centro);
		if(clusters[i].get_number_of_elements_in_cluster() == 0){
			empty++;
			continue;
		}
		for (int j = 0; j < cluster_points.size(); ++j)
		{

			vector< double> take_datapoint(next(cluster_points[j].begin()),cluster_points[j].end());

			double a = calculate_a(take_datapoint, i);

			int min_cluster_id = find_2nd(take_datapoint, centroid_c);

			double b = calculate_b(take_datapoint, min_cluster_id);
			
			double silhouette_evaluation;
			if( a == b )
				silhouette_evaluation = 0;
			else if( a > b )
				silhouette_evaluation = b / a - 1;
			else if( a < b )
				silhouette_evaluation = 1 - a / b;
			if(silhouette_evaluation < -1.0 || silhouette_evaluation > 1.0 ){
				cout << "Error: silhouette value must be in range [-1.0, 1.0]" << endl;
				cout << "it was " << silhouette_evaluation;
				cout << " with a: " << a << " b: " << b << endl;
				cout << "min_cluster_id: " << min_cluster_id << endl;
				cout << "ClusterSize: "<< cluster_points.size() << endl;
			}
			
			avg_cluster_silhouette += silhouette_evaluation;
			
		} // end for each element in cluster
		
		avg_cluster_silhouette = avg_cluster_silhouette / cluster_points.size();
		cluster_sil_val.push_back(avg_cluster_silhouette);
		overall_silhouette_evaluation += avg_cluster_silhouette;
	} // end for each cluster

	overall_silhouette_evaluation = overall_silhouette_evaluation / (K - empty);
	cluster_sil_val.push_back(overall_silhouette_evaluation);
	
	clock_t end_silhouette = clock();	
	double silhouette_time = (double)(end_silhouette - start_silhouette) / CLOCKS_PER_SEC;
	cout << "Silhouette overall Eval: " << cluster_sil_val[K-empty]	<< "[" <<silhouette_time <<"s]"<< endl;
	//cout << "Empty Clusters: " << empty << endl;
	return cluster_sil_val;
}


/**	
*	Return id of the 2nd best cluster
*	@param take_datapoint{vector<double>& }	- Cluster Centroid
*	@param centroid_c{vector<double>& }		- every other centroid
*	
*/
int KMeans::find_2nd(vector<double>& take_datapoint, vector< double>& centroid_c){
	bool flag = false;
	double min = -1;
	int min_cluster_id = -1;
	double dist;
	for (int i = 0; i < K; i++)
	{
		vector< double> centro = clusters[i].get_centroid();
		vector< double> take_centroid(next(centro.begin()),centro.end());
		if(take_datapoint == take_centroid || centroid_c == take_centroid)
			continue;
		if(metric.compare("euclidean") == 0)
			dist = calculateEuclidean_new(take_datapoint,take_centroid);
		else
			dist = calculateCosine_new(take_datapoint,take_centroid);

		if(flag == false){
			min = dist;
			min_cluster_id = i;
			flag = true;
		}
		if(dist < min){
			min = dist;
			min_cluster_id = i;
		}
	} // end for each centroid
	//cout << "min_cluster_id: " <<min_cluster_id<< " with minDist: " << min << endl;

	return min_cluster_id;
}


/**	
*	Return a value
*	@param take_datapoint{vector<double>& }	- Cluster Centroid
*	@param cluster_id{int }					- clusters id
*	
*/
double KMeans::calculate_a(vector< double>& take_datapoint, int cluster_id){
	vector<vector<double> > cluster_points = clusters[cluster_id].return_cluster_points();
	double a = 0;
	double dist;
	for (int i = 0; i < cluster_points.size(); i++)
	{
		vector< double> take_datapoint_2(next(cluster_points[i].begin()),cluster_points[i].end());
		if(take_datapoint == take_datapoint_2)
			continue;
		//print_vector(take_datapoint_2);
		if(metric.compare("euclidean") == 0)
			dist = calculateEuclidean_new(take_datapoint,take_datapoint_2);
		else
			dist = calculateCosine_new(take_datapoint,take_datapoint_2);
		a += dist;
	}
	a = a / cluster_points.size();
	return a;
}

/**	
*	Return b value
*	@param take_datapoint{vector<double>& }	- Cluster Centroid
*	@param cluster_id{int }					- clusters id
*	
*/
double KMeans::calculate_b(vector< double>& datapoint, int cluster_id){
	double avg = 0;
	vector<vector<double> > cluster_points = clusters[cluster_id].return_cluster_points();
	double dist;
	for (int i = 0; i < cluster_points.size(); i++)
	{
		vector<double> take_datapoint(next(cluster_points[i].begin()),cluster_points[i].end());
		if(metric.compare("euclidean") == 0)
			dist = calculateEuclidean_new(datapoint,take_datapoint);
		else
			dist = calculateCosine_new(datapoint,take_datapoint);
		
		avg += dist;
	}
	avg = avg / cluster_points.size();

	return avg;
}


/**	
*	Return none
*	Keep a log file as asked
*	@param output_file{string }			- Output file name
*	@param clustering_time{double }		- cluster overall time
*	@param s{vector<double>& }			- silhouette values
*	@param print_clusters_flag{int }	- print id's if enabled
*/
void KMeans::create_log(string output_file,double clustering_time, vector<double>& s, int print_clusters_flag){
	ofstream outfile(output_file);	// Keep a log file
	outfile << "Algorithm: " 		<< assigment_method 	<< endl
			<< "Metric: "	 		<< metric			 	<< endl
			<< "Clustering Time: "  << clustering_time 		<< "s"	<< endl
			<< "Silhouette: [" ;
	for (int i = 0; i < s.size(); i++)
	{
		outfile << s[i];
		if(i < (s.size()-1))
			outfile << ", ";
	}
	outfile << "]"<< endl;
	int sum = 0;
	for (int i = 0; i < K; i++)
	{
		sum += clusters[i].get_number_of_elements_in_cluster();
		vector< double> cen = clusters[i].get_centroid();
		outfile << "Cluster-" << i << " {size: " << clusters[i].get_number_of_elements_in_cluster() 
				<< ", centroid: ";
		for (int j = 1; j < cen.size(); j++)	// start from 1, skip id
			outfile << cen[j] << " ";
		outfile << "}" << endl;
	}
	//cout << "#Elements in Clusters[the last one]: "<< sum << endl;

	if(print_clusters_flag == 1)
	{
		outfile << endl << endl;
		for (int i = 0; i < K; ++i)
			clusters[i].output_clusters(i, outfile);
	}

	outfile.close(); // Close log file
}