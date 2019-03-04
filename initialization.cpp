#include "kmeans.hpp"

/**
*	Random selection of k points(simplest) using reservoir sampling
*	@param matrix{vector<vector<int> >}	- dataset
*	@param k{int}						- # of centroids 
*	Return a vector of the k-centroids
*/
int KMeans::initialize(vector<vector<double> >& matrix){
	cout << "init_method: \t\t" 		<< init_method 		<< endl
		 << "assigment_method: \t"		<< assigment_method << endl
		 << "update_method: \t\t"		<< update_method 	<< endl
		 << "metric: \t\t"				<< metric			<< endl
		 << "Clusters: \t\t"			<< K 				<< endl;

	if(K > total_points)
	{
		cout << "Error: Number of clusters > Number of datapoints\n";
		exit(EXIT_FAILURE);
	}

	if(init_method.compare("kmeans++") == 0 || init_method.compare("kmeansplusplus") == 0 )
		kmeansplusplus(matrix);
	else if(init_method.compare("random_k_selection") == 0 )
		random_k_selection(matrix);
	else if(init_method.compare("reservoir_sampling") == 0 )
		reservoir_sampling(matrix);
	else
	{
		cout << "Error: Init method '" << init_method << "' doesn't exists!" << endl
			 << "Try: kmeans++ or random_k_selection"	 << endl;
		exit(EXIT_FAILURE);
	}

	return 0;
}

/**
*	Random selection of k points(simplest) using reservoir sampling
*	@param matrix{vector<vector<int> >}	- dataset
*	@param k{int}						- # of centroids 
*	Return a vector of the k-centroids
*/
void KMeans::reservoir_sampling(vector<vector<double> >& matrix){
	vector<vector<double> > centroids;
	int n = matrix.size();
	int d = matrix[1].size();
	for (int i = 0; i < K; i++) 
		centroids.push_back(matrix[i]); 
	
	srand(time(NULL)); 
	for (int i = 0; i < n; i++) { 
		 
		int r = rand() % (i+1); // Pick a random index from 0 to i.

		// If the randomly  picked index is smaller than k, then replace 
		// the element present at the index with new element from stream 
		if (r < K){
			for ( int j = 0; j < d; j++ )
				centroids[r][j] = matrix[i][j];
		}
	} 
	for (int i = 0; i < K; ++i)
		clusters[i].setCentralValue(centroids[i]); 							
	
}


/**
*	Random selection of k points(simplest) using reservoir sampling
*	@param matrix{vector<vector<int> >}	- dataset
*	@param k{int}						- # of centroids 
*	Return a vector of the k-centroids
*/
void KMeans::random_k_selection(vector<vector<double> >& matrix){
	vector<vector<double> > centroids;
	int n = matrix.size();
	srand(time(NULL)); 
	vector<int> rands;
	for (int i = 0; i < K; i++){
		int v;
		int r;
		while(true){
			bool isUnique = 0;
			r = rand() % (n);
			vector<double> matrix_coord(next(matrix[r].begin()),matrix[r].end());
			for (int j = 0; j < centroids.size(); ++j)
			{
				vector<double> getCoord(next(centroids[j].begin()),centroids[j].end());
				if(getCoord == matrix_coord)
					isUnique = 1;
			}

			if(isUnique==0)
			{
				v = r;
				break;
			}
		}
		centroids.push_back(matrix[v]);

		clusters[i].setCentralValue(matrix[v]); 	
		
	}

	//print_matrix_double(centroids);

}


/**
*	Driver function to sort the vector elements by second element of pairs
*	@param matrix{vector<vector<long double> >}	- dataset
*	@param k{int}							- # of centroids 
*	Return a vector of the k-centroids
*/
bool sortbysec(const pair<int,double> &a, const pair<int,double> &b) { 
    return (a.second < b.second); 
} 


/**
*	Random 
*	@param matrix{vector<vector<double> >}	- dataset
*	@param k{int}							- # of centroids 
*	Return a vector of the k-centroids
*/
void KMeans::kmeansplusplus(vector<vector<double> >& matrix){
	cout << "kmeans++()" << endl;
	int n = matrix.size();
	int t;
	srand(time(NULL)); 
	int r = rand() % (n);
	vector<vector<double> > centroids;	//centroids.resize(k);
	vector<double> take_centroid( next(matrix[r].begin()), matrix[r].end() );	// Get one p vector-one row from the 2D matrix
	vector<double> take_centroid2( matrix[r].begin(), matrix[r].end() );		// Get one p vector-one row from the 2D matrix

	t = 1;
	centroids.push_back(take_centroid2);
	
	while( K != t ){


		double dist;
		double sum_dist = 0.0;


		//have a vector of pairs and store: <id,distance>
		vector<pair<int, double > > my_v;
		vector<double> partial_sums;
		int id;
		// I have to find every D(i) between the centroid and every datapoint
		for (int i = 0; i < n; ++i)
		{
			vector<double> take_datapoint( next(matrix[i].begin()), matrix[i].end() );	// Get one p vector-one row from the 2D matrix
			bool flag_c = false;
			for (int j = 0; j < centroids.size(); ++j)
			{
				vector<double> take_centro(next(centroids[j].begin()),centroids[j].end());
				if(take_datapoint == take_centro)
					flag_c = true;
			}
			if(flag_c == true)	// if datapoint is centroid
				continue;
			if(metric.compare("euclidean") == 0)
				dist = calculateEuclidean_new(take_datapoint, take_centroid);
			else
				dist = calculateCosine_new(take_datapoint, take_centroid);

			partial_sums.push_back(dist*dist);		

			sum_dist += dist*dist;//pow(dist,2);
			id = matrix[i][0];
			my_v.push_back(make_pair(id, dist*dist));

		} // end for

		double sum_all = 0.0;

		sort(my_v.begin(),my_v.end(), sortbysec);
		sort (partial_sums.begin(), partial_sums.end());

		auto it = max_element(begin(partial_sums), end(partial_sums));

		double uniformValue = uniform_random_generator(0,*it);
		vector<double>::iterator low,up;
		low = lower_bound (partial_sums.begin(), partial_sums.end(), uniformValue);
		t++;
		int index = my_v[low - partial_sums.begin() /*-1*/].first;
		if(index == 0)
			index = 1;
		vector<double> take_d( next(matrix[index-1].begin()), matrix[index-1].end() );	// Get one p vector-one row from the 2D matrix
		vector<double> take_d2( matrix[index-1].begin(), matrix[index-1].end() );	// Get one p vector-one row from the 2D matrix


		centroids.push_back(take_d2);

		// for all non centroid values:
		for (int i = 0; i < n; ++i)
		{
			bool flag_c = false;
			vector<double> take_data( next(matrix[i].begin()), matrix[i].end() );	// Get one p vector-one row from the 2D matrix

			for (int j = 0; j < centroids.size(); ++j)
			{
				vector<double> take_centro(next(centroids[j].begin()),centroids[j].end());

				if(take_data == take_centro)
					flag_c = true;
			}
			if(flag_c == true)	// if datapoint is centroid
				continue;
			// if datapoint isn't centroid


		}

	};



	for (int i = 0; i < K; ++i)
		clusters[i].setCentralValue(centroids[i]); 	

}