#include "cluster.hpp"
/**	
*	Return none
*	@param datapoint{vector<double>&}	- datapoint
*	Add a vector in the Cluster
*/
void Cluster::add_datapoints(vector<double>& datapoint){
	number_of_elements_in_cluster++;
	vector<double> temp;

	for (int i = 0; i < datapoint.size(); i++)
		temp.push_back(datapoint[i]);     // Push the int into the temporary vector<int>
	
	cluster_points.push_back(temp);
}


/**	
*	Return none
*	Empty cluster from datapoints, set #elements to 0
*/
void Cluster::empty_cluster(){
	number_of_elements_in_cluster = 0;
	for (int i = 0; i < cluster_points.size(); ++i){
		cluster_points[i].clear();
	}
	cluster_points.clear();

}


/**	
*	Return none
*	@param id{int}	- id of the cluster
*	Print clusters points
*/
void Cluster::print_cluster(int id){
	cout << "Print Cluster" << "[" << id << "]" << endl;
	//cout << "#elements_in_cluster: " << number_of_elements_in_cluster << endl;
	cout << "centroid: ";
	print_vector(central_values);
	for ( int i = 0; i < cluster_points.size(); i++)
	{
		cout <<"\t  ";
		cout << cluster_points[i][0] << endl;
		// for (int j = 0; j < cluster_points[i].size(); j++)
		// 	cout << cluster_points[i][j] << ' ';
		// cout << endl;
	}

}

/**	
*	Return none
*	@param outfile{ofstream& }	- output filename
*	@param id{int }				- id of the cluster
*	Print clusters id into log file if -p parameter is set to 1
*/
void Cluster::output_clusters(int id, ofstream& outfile){
	outfile << "Cluster-" << id << " {";
	for ( int i = 0; i < cluster_points.size(); i++)
	{
		outfile << cluster_points[i][0];
		if(i < (cluster_points.size()-1))
			outfile << ", ";
	}
	outfile << "}" << endl;
}



/**	
*	Return vector<double> a new vector centroid
*	update_method = kmeans
*/
vector<double> Cluster::new_centroid(){
	vector<double> new_centroid_value;
	//cout << "#elems: " << cluster_points.size() << " " << number_of_elements_in_cluster <<endl;
	if(cluster_points.size() == 0){
		//cout << "Cluster_" << id_cluster <<" is Empty" << endl;
		vector<double> cc = get_centroid();
		return cc;
	}
	
	for (int j = 0; j < cluster_points[0].size(); ++j)
	{
		double x = 0.0;
		if(j == 0)	// store 1st the id of the centroid
			new_centroid_value.push_back(cluster_points[j][0]);
		else
		{
			for (int i = 0; i < number_of_elements_in_cluster; ++i)
			{
				x += cluster_points[i][j];
			}
			new_centroid_value.push_back(x / number_of_elements_in_cluster);
		}
	}
	
	return new_centroid_value;
}



/**	
*	Return vector<double> a new vector centroid
*	@param metric{string}	-  euclidean or cosine
*	update_method = PAM
*/
vector<double> Cluster::new_centroid_PAM(string metric){
	vector<double> new_centroid_value;
	bool flag = false;
	double min;
	double sum_dist;
	if(cluster_points.size() == 0){
		vector<double> cc = get_centroid();
		return cc;
	}
	
	for (int i = 0; i < number_of_elements_in_cluster; ++i)
	{
		sum_dist = 0.0;
		double dist;
		for (int j = 0; j < number_of_elements_in_cluster; ++j)
		{
			if(cluster_points[i] == cluster_points[j])
				continue;
			if(metric.compare("euclidean") == 0)
				dist = calculateEuclidean_coord(cluster_points[i], cluster_points[j]);
			else if(metric.compare("cosine") == 0)
				dist = calculateCosine_coord(cluster_points[i], cluster_points[j]);

			sum_dist += dist;

		}
		if(flag == false)
		{
			flag = true;
			min = sum_dist;
			new_centroid_value.clear();
			new_centroid_value = cluster_points[i];
		}
		if(sum_dist < min)
		{
			min = sum_dist;
			new_centroid_value.clear();
			new_centroid_value = cluster_points[i];
		}
		
	}

	return new_centroid_value;
}



/**	
*	Return none
*	Print centroid
*/
void Cluster::print_centralvalues(){
	for (int i = 0; i < central_values.size(); ++i){
		cout << central_values[i] << " ";
	}cout << endl;

}