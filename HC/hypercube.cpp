#include "hypercube.hpp"

/**	
*	Return none
*	@param matrix{vector<vector<int> >&}	- vector that dataset is stored
*/
int HYPERCUBE::insert(vector<vector< double> >& matrix){
	int d = matrix[0].size() - 1; // ignore first element bc its the id_value;
	int n = matrix.size();
	double t;
	vector<double> v_i;
	vector<vector<double> > v;
	HT ht; 
	ht.resize_HT(TableSize);
	// Create v_k permanent vectors for each HashTable, Store them into v vector
	for ( int i = 0; i < k; ++i){
		v_i = gaussian_distribution_vector(d);
		v.push_back(v_i);
	}
	vs.push_back(v); //keep same v for every L-table
	if(metric == "euclidean"){
		t = uniform_random_generator(0.0, W); // t should be common
		ts.push_back(t);
	}


	for ( int i = 0; i < n; i++){	// For every p vector
		//vector< double> take_row( next(matrix[i].begin()), matrix[i].end());	// Get one p vector-one row from the 2D matrix
		vector< double> take_whole_row( matrix[i].begin(), matrix[i].end() );	// Get one p vector-one row from the 2D matrix
		vector< double> take_row( next(take_whole_row.begin()), take_whole_row.end() );	// Get one p vector-one row from the 2D matrix
		int id = take_whole_row[0];
		string g = "";
		for(int z = 0 ; z < k; z++){	// For every p, calculate k h-functions
			vector<double> get_v(v[z].begin(), v[z].end());

			assert(get_v.size() == take_row.size()); // p vector and v vector must have same size
			double inner = inner_product(begin(take_row), end(take_row), begin(get_v), 0.0);
			int h;	
			if(metric == "euclidean"){
				int found = 0;
				int fc;
				t = (inner + t) / W; 	
				h = floor(t);
				// Check if we alrdeady flip the coin for h
				for ( const auto& aa : tuple_list ) {
					//cout << "h: " << get<0>(aa) << " | " << "fc: " << get<1>(aa) << endl;
					if( get<0>(aa) == h){
						fc = get<1>(aa);
						found = 1;
						break;
					}
				}
				// if we haven't flip a coin for h then flip one
				if( found == 0 ){
					fc = rand() % 2;
					tuple_list.push_back(tuple<int, int>(h,fc));
				}
				g =  g + to_string(fc); 
			}else{ // cosine
				if(inner < 0)
					h = 0;
				else
					h = 1;
				g =  g + to_string(h); // g is a bit string

			}
		}//end k-times loop(z-iterator)

		int util = bitset<64>(g).to_ullong();
		int f = util % TableSize;


		ht.insert_HT(f, id, g); //f is index, i is id, g we dont need that
	} // end i loop, means end with every p-vector, aka every row of the 2D matrix

	hts.push_back(ht);
	//print_hc_HT();
	return 0;
}

/**	
*	Return none
*	@param query_matrix{vector<vector<int> >&}	- vector that query_set is stored
*	@param matrix{vector<vector<int> >&}		- vector that data_set is stored
*	@param output_file{string}					- name of logfile
*/
vector<vector<int> > HYPERCUBE::search(vector<vector<double> >& matrix, vector<vector<double> >& query_matrix){
	vector<vector<int> > cluster_id_elements;

	int h;
	double dist;
	double t;
	if(metric == "euclidean")
		t = ts[0];
	vector<int> vertex_id;	//store all vertex ids in vertex_id vector(001, 010, 100, 110 etc)
	for (int i = 0; i < TableSize; ++i){
		vertex_id.push_back(i);
	}

	for (int i = 0; i < query_matrix.size(); i++){	// For every 1 vector
		cluster_id_elements.push_back(vector<int>());

		vector<double> take_row( next(query_matrix[i].begin()), query_matrix[i].end());	// Get one p vector-one row from the 2D matrix
		// cout << "query_ ";
		// print_vector(take_row);

		vector<int> neigbors;				// a vector to store hc neighbor vertices
		string g = "";
		for(int z = 0 ; z < k; z++){	// For every q, calculate k h-functions
			vector<double> get_v(vs[0][z].begin(), vs[0][z].end());

			assert(get_v.size() == take_row.size()); // p vector and v vector must have same size
			double inner = inner_product(begin(take_row), end(take_row), begin(get_v), 0.0);
			if(metric == "euclidean"){
				t = (inner + t) / W; 	
				int h = floor(t);		
				int fc;
				int found = 0;

				for ( const auto& aa : tuple_list ) {
					//cout << "h: " << get<0>(aa) << " | " << "fc: " << get<1>(aa) << endl;
					if( get<0>(aa) == h){
						fc = get<1>(aa);
						found = 1;
						break;
					}
				}

				if( found == 0 ){
					fc = rand() % 2;
					tuple_list.push_back(tuple<int, int>(h,fc));
				}
				g =  g + to_string(fc); 
			}else{ // cosine
				if(inner < 0)
					h = 0;
				else
					h = 1;
				g =  g + to_string(h);
			}
		}//end k-times loop(z-iterator)

		int util = bitset<64>(g).to_ullong(); 	//unsigned long long util;
		int f = util % TableSize;
	
		vector<int> elem;	//cout << "f=" << f << " : " << bitset<3>(f) << endl;
		elem = hts[0].return_bucket_elements_HT(f);
		//cluster_id_elements[i] = elem;
		//int pushed = abs(M - elem.size());
		int pushed;
		if (M > elem.size())
			pushed = elem.size();
		else
			pushed = M;
		//cout << "Pus " << pushed << endl;
		for (int x = 0; x < pushed; ++x)
			cluster_id_elements[i].push_back(elem[x]);
		
		// for (int z = 0; z < elem.size(); ++z)
		// 	cout << elem[z] << endl;
		

		int M_flag = 0;
		int remain_M = 0;
		// I will not check next neigbors if if i have already checked M elements
		if( elem.size() < M)
			M_flag = 1;

		
		// I will move to the neighbors of this vertex 
		if( M_flag == 1 ){
			remain_M = M - hts[0].bucket_size_HT(f); 
			//cout << "remain: " << remain_M << endl;
			// Find neigbors of f
			for ( int yy = 0; yy < vertex_id.size(); ++yy){
				if( bitset<64>(vertex_id[yy]^f).count() == 1 ){
					neigbors.push_back(vertex_id[yy]);
				}
			}

			elem.clear();
			int neigbors_vertices = neigbors.size();
			if(neigbors_vertices > probes - 1)	// Check only probes neigbors
				neigbors_vertices = probes - 1;

			int flag_break = 0;
			for ( int yy = 0; yy < neigbors_vertices; ++yy){
				//cout << "next neigbor" << endl;
				elem = hts[0].return_bucket_elements_HT(neigbors[yy]);
				// for (int z = 0; z < elem.size(); ++z)
				// 	cout << elem[z] << endl;

				int push;
				if (remain_M > elem.size())
					push = elem.size();
				else
					push = remain_M;
				for (int x = 0; x < push; ++x){
					cluster_id_elements[i].push_back(elem[x]);
					//cout << "reminder: " << elem[x] << endl;
				}
				
				for ( int c = 0; c < hts[0].bucket_size_HT(neigbors[yy]); ++c){
					if ( c == remain_M + 1){
						flag_break = 1;
						break;
					}

				}
				if(flag_break == 1)
					break;
				remain_M = remain_M - hts[0].bucket_size_HT(neigbors[yy]);
				elem.clear();

			}//end for neigbors

		}//end if
		

	} // end for every q query



	return cluster_id_elements;
}


void HYPERCUBE::print_hc_HT(){
	hts[0].print_HT_HC();
}