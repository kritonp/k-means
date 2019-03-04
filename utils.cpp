#include "utils.hpp"


/**	
*	Return a tuple of {input_file, query_set, output_file, k, L}
*	If k,L doesn't provided by the user give default values
*	@param argc{int}  	- number of arguments
*	@param argv{char*}	- command line arguments
*/
tuple<string, string, string, string, int, string, string, string, int> cmd_arguments(int argc, char* argv[]){
	string input_file, conf_file, output_file, metric;
	string assigment_method = "lloyd"; 			// default
	string init_method = "random_k_selection"; 	// default
	string update_method = "kmeans";			// default
	int max_n_of_iterations = 15;      			// default
	int print_clusters_flag = 0;				// default
	int i_flag = 0;
	int c_flag = 0;
	int o_flag = 0;
	int m_flag = 0;
	//cout << "You have entered " << argc - 1  << " arguments:" << "\n"; 
	if (argc < 7)
	{
		cout << "Error: Missing Arguments" << endl;
		cout << "Wrong input" << endl;
		exit(-1);
	}
	else
	{
	    for (int i = 0; i < argc; ++i)
	    {
	    	if(strcmp (argv[i],"-i") == 0 && i_flag == 0){
				input_file = argv[i+1];
	    		i_flag = 1;
			}else if(strcmp (argv[i],"-c") == 0 && c_flag == 0){
				conf_file = argv[i+1];
				c_flag = 1;
			}else if(strcmp (argv[i],"-d") == 0 && m_flag == 0){
				metric = argv[i+1];
				m_flag = 1;
			}else if(strcmp (argv[i],"-o") == 0 && o_flag == 0){
				output_file = argv[i+1];
				o_flag = 1;
			}else if(strcmp (argv[i],"-m") == 0){
				max_n_of_iterations = atoi(argv[i+1]);
			}else if(strcmp (argv[i],"-a") == 0){
				assigment_method = argv[i+1];
			}else if(strcmp (argv[i],"-n") == 0){
				init_method = argv[i+1];
			}else if(strcmp (argv[i],"-u") == 0){
				update_method = argv[i+1];
			}else if(strcmp (argv[i],"-p") == 0){
				print_clusters_flag = atoi(argv[i+1]);
			}
		}
	}

	if(i_flag != 1 || c_flag != 1 || o_flag != 1 || m_flag != 1)
	{
		cout << "Values -i, -c, -o, -m are unique and required " << endl;
		exit(-1);
	}	
	if(metric.compare("cosine") != 0 && metric.compare("euclidean") != 0)
		metric = "euclidean";

	return make_tuple(string(input_file), string(conf_file), string(output_file), string(metric), max_n_of_iterations, string(assigment_method), string(init_method), string(update_method), print_clusters_flag);
}

/**	
*	Return a tuple of {k, h, L, R}
*	If k,L the above values doesn't provided by the user give default values
*	@param fileName{string}  	- configure filenam
*/
tuple<int, int, int, double> configure_file(string fileName){
	int k = 0;
	int h = 4;
	int L = 5;
	double R = 1.0;
	string k_str = "0";		// default
	string h_str = "4";		// default
	string L_str = "5";		// default
	string R_str = "1.0";	// default

	ifstream in(fileName);
	if (in)
	{		
		string line;
		while ( getline(in, line) )
		{

			// Break down the row into column values
			stringstream split(line);
			//cout << line << endl;

			string value;
			stringstream iss(line);
			int n = 2;
			while (split >> value)
			{
				if(value == "number_of_clusters:"){
					while(n-- > 0 && (iss >> k_str))
						k_str = k_str;
				}else if(value == "number_of_hash_functions:"){
					while(n-- > 0 && (iss >> h_str))
						h_str = h_str;
				}else if(value == "number_of_hash_tables:"){
					while(n-- > 0 && (iss >> L_str))
						L_str = L_str;
				}else if(value == "R:"){
					while(n-- > 0 && (iss >> R_str))
						R_str = R_str;
				}

			} // end while

		} // end outter while

	} // end if
	else
	{ 
		cout << "Error: Unable to open file: " << fileName << endl; 
		exit(-1);
	}	

	k = stoi(k_str);
	h = stoi(h_str);
	L = stoi(L_str);
	R = stod(R_str);

	return make_tuple(k, h, L, R);
}

/**	
*	Create a 2D Matrix as well
*	@param fileName{string} 			- dataset file name
*	@param matrix{vector<vector<int> >} - 2D matrix
*	Works for both csv and tab separated values
*/
int read_file(string fileName, vector<vector< double> >& matrix){
	ifstream in(fileName);
	int i;
	int flag = 1;
	string first_line;
	char delim = ',';	// default

	if (in)
	{		
		string line;
		while ( getline(in, line) )
		{
			if (flag == 1)
			{ // Get the First Line to proccess it and find the delimeter
				flag = 2;
				first_line = line;
				if (first_line.find(',') != string::npos){
					delim = ',';
				}
				else if(first_line.find('\t') != string::npos){
					delim = '\t';
				}else{
					cout << "Error: File doesn't follow the correct delimeters" << endl;
					exit(-1);
				}
			}
			matrix.push_back(vector< double>());
			// Break down the row into column values
			stringstream split(line);
			double val;
			while(getline(split, line, delim))
			{
				val = stold(line);		
				matrix.back().push_back(val);
			}				
			
		}
	}
	else
	{ 
		cout << "Error: Unable to open file: " << fileName << endl; 
		exit(-1);
	}

	return 0;
}


/**	
*	Return None
*	Print a 2-Dimensional Matrix of Doubles
*	@param matrix{vector<vector<int> >&}  - a vector of vector of doubles
*/
void print_matrix_double(vector<vector< double> >& matrix){
	cout << "\tPrint 2D-Vector: \n";
	for ( int i = 0; i < matrix.size(); i++)
	{
		cout <<"\t";
		for (int j = 0; j < matrix[i].size(); j++)
			cout /*<< setprecision(22)*/ << matrix[i][j] << ' ';
		
		cout << '\n';
	}
}


/**	
*	Return none
*	@param centroids{vector<vector< double> >&}  	- centroids values
*	Delete centroid values
*/
void delete_centroids(vector<vector< double> >& centroids){
	for (int i = 0; i < centroids.size(); ++i)
	{
		centroids[i].clear();
		//centroids[i].shrink_to_fit();
	}
	centroids.clear();
	//centroids.shrink_to_fit();
}



double calculateEuclidean_coord(vector< double>& take_datapoint, vector< double>& take_centroid){
	double dist = 0.0;
	double temp;
	assert(take_datapoint.size() == take_centroid.size());	
	for ( int i = 1; i < take_datapoint.size(); i++)
	{
		temp = take_datapoint[i] - take_centroid[i];
		temp = temp*temp;
		dist = dist + temp;			
	}
	dist = sqrt(dist);
	assert(dist>=0);
	return dist;
}

//values[0,2]
double calculateCosine_coord(vector<double>& take_datapoint, vector<double>& take_centroid){
	double x = 0.0;
	double y = 0.0;
	double dist = 0.0;
	double inner_product_value = 0.0;
	double xy = 0.0;
	double cosine_value = 0.0;

	assert(take_datapoint.size() == take_centroid.size());	
	for ( int i = 1; i < take_datapoint.size(); i++)
	{
		x += take_datapoint[i]*take_datapoint[i];
		y += take_centroid[i]*take_centroid[i];
		inner_product_value += take_datapoint[i]*take_centroid[i];

	}
	xy = sqrt(x) * sqrt(y);

	cosine_value = inner_product_value / xy;
	//long double xx = inner_product_value / xy;
	dist = 1 - cosine_value;
	if(dist < 0 || dist > 2){
		//cout << "CosineSimilarity Failed: " << cosine_value <<" "<<dist << " " <<xx<< endl;
		dist = 0;
	}
	//cout << "\nCosine Distance is: " << dist << endl;
	assert(dist>=0);
	return dist;
}


/**	
*	Return real random number in range [min,max)
*	@param min{double}  - 
*	@param max{double}	- 
*	https://stackoverflow.com/questions/10776073/random-double-between-min-and-max?rq=1	
*/
double random_generator(double min, double max){
	return (max - min) * ( (double)rand() / (double)RAND_MAX ) + min;
}


/**	
*	Return uniform real random number in range [min,max)
*	@param min{double}  - 
*	@param max{double}	- 
*	https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
*/
double uniform_random_generator(double min, double max){
	random_device rd;  //Will be used to obtain a seed for the random number engine
	mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	uniform_real_distribution<> dis(min, max);
	double t;
	//dis: transform the random unsigned int generated by gen into a double in [1, 2)
	t = dis(gen);	//Each call to dis(gen) generates a new random double

	return t;
}

/**	
*	Return a gaussian_distribution_vector v
*	@param d{int}  - vector dimension
*	MEAN,VARIANCE are defined in utils.h file
*/
vector<double> gaussian_distribution_vector(int d){	//d is the number of columns
	int i;
	random_device rd; //https://stackoverflow.com/questions/22907473/c-default-random-engine-creates-all-the-time-same-series-of-numbers
	default_random_engine generator(rd());
	normal_distribution<double> distribution(MEAN,VARIANCE);
	vector<double> v;

	for ( i = 0; i < d; ++i)
	{
		double number = distribution(generator);
		v.push_back(number);
	}


	return v;
}

/**	
*	Return cosine similarity distance(1 - cos(p,q))
*	@param matrix{vector<vector<int> >&}	- vector that dataset is stored
*	@param q{vector<int>}					- its q from the query set
*	@param id{int}							- its the item id from the dataset matrix
*/
double calculateCosine(vector<vector< double> >& matrix, vector< double> q, int id ){
	double x = 0;
	double y = 0;
	double dist = 0.0;
	double inner_product_value = 0.0;
	double xy = 0.0;
	double cosine_value = 0.0;
	// cout  << "id: " << id << endl;
	// for (int m = 0; m < q.size(); ++m)
	// {
	// 	cout << q[m] << " ";
	// }cout << endl;
	for ( int i = 1; i < matrix[id-1].size(); i++)
	{
		//cout << matrix[id-1][i] << ' ';
		x = x + pow(matrix[id-1][i], 2);
		y = y + pow(q[i-1], 2);
		inner_product_value = inner_product_value + matrix[id-1][i]*q[i-1];

	}//cout<<endl;
	xy = sqrt(x) * sqrt(y);
	cosine_value = inner_product_value / xy;
	//cout<< "cosine_val: "<< cosine_value<<" inner_product_value: " << inner_product_value << " xy: "<< xy  << endl;
	dist = 1 - cosine_value;

	//cout << "\nCosine Distance is: " << dist << endl;
	return dist;
}


/**	
*	Return euclidean distance
*	@param matrix{vector<vector<int> >&}	- vector that dataset is stored
*	@param q{vector<int>}					- its q from the query set
*	@param id{int}							- its the item id from the dataset matrix
*/
double calculateEuclidean(vector<vector< double> >& matrix, vector< double> q, int id ){
	double dist = 0.0;
	double temp;
	// cout  << "id: " << id << endl;
	// 	for (int m = 0; m < q.size(); ++m)
	// 	{
	// 		cout << q[m] << " ";
	// 	}cout << endl;
	for ( int i = 1; i < matrix[id-1].size(); i++)
	{
		//cout << matrix[id-1][i] << ' ';
		temp = matrix[id-1][i] - q[i-1];
		temp = temp*temp;//pow(temp,2);
		dist = dist + temp;			
	}//cout << endl;
	dist = sqrt(dist);
	//cout << "\nEuclidean Distance is: " << dist << endl;
	return dist;
}


/**	
*	Return euclidean distance
*	@param matrix{vector<vector<int> >&}	- vector that dataset is stored
*	@param q{vector<int>}					- its q from the query set
*	@param id{int}							- its the item id from the dataset matrix
*/
double calculateEuclidean_new(vector< double>& take_datapoint, vector< double>& take_centroid){
	double dist = 0.0;
	double temp;
	assert(take_datapoint.size() == take_centroid.size());	
	for ( int i = 0; i < take_datapoint.size(); i++)
	{
		temp = take_datapoint[i] - take_centroid[i];
		temp = temp*temp;//pow(temp,2);
		dist = dist + temp;			
	}
	dist = sqrt(dist);	
	return dist;
}
double calculateCosine_new(vector<double>& take_datapoint, vector<double>& take_centroid){
	double x = 0.0;
	double y = 0.0;
	double dist = 0.0;
	double inner_product_value = 0.0;
	double xy = 0.0;
	double cosine_value = 0.0;

	assert(take_datapoint.size() == take_centroid.size());	
	for ( int i = 0; i < take_datapoint.size(); i++)
	{
		x += take_datapoint[i]*take_datapoint[i];
		y += take_centroid[i]*take_centroid[i];
		inner_product_value += take_datapoint[i]*take_centroid[i];

	}
	xy = sqrt(x) * sqrt(y);

	cosine_value = inner_product_value / xy;
	//long double xx = inner_product_value / xy;
	dist = 1 - cosine_value;
	if(dist < 0 || dist > 2){
		//cout << "CosineSimilarity Failed: " << cosine_value <<" "<<dist << " " <<xx<< endl;
		dist = 0;
	}
	//cout << "\nCosine Distance is: " << dist << endl;
	assert(dist>=0);
	return dist;
}


/*** For Project_1 ***/



/**	
*	Return [id,distance] of the real minimum distance between search query and p from dataset
*	@param matrix{vector<vector<int> >&}	- vector that dataset is stored
*	@param q{vector<int>}					- its q from the query set
*/
double closest_NN(vector<vector< double> >& matrix, vector< double>& query, string metric){
	int id_found;
	double dist;
	double min;
	//cout << "closest_NN (((((()))))\n";
	for ( int i = 0; i < matrix.size(); ++i)
	{

		if(metric == "euclidean")
			dist = calculateEuclidean(matrix, query, i+1);
		else
			dist = calculateCosine(matrix, query, i+1);
		
		if(i == 0)
			min = dist;
		//cout << "dist: " << dist << endl;
		if(dist < min)
		{
			min = dist;
			id_found = i;
		}
	}
	return min;
}

//debug only
void print_vector(vector< double>& my_vector){
	for (int i = 0; i < my_vector.size(); ++i)
		cout << my_vector[i] << " ";
	
	cout << endl;
}


void help(){
	cout << "./cluster -i dataset.txt -c conf.txt -o output.txt -d euclidean" << endl;
}