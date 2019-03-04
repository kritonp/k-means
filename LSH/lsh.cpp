#include "lsh.hpp"
/**	
*	Create L HT and insert the datapoints to them according to metric
*	Return none
*	@param matrix{vector<vector<int> >&}	- vector that dataset is stored
*/
int LSH::insert(vector<vector<double> >& matrix){
	int d = matrix[0].size() - 1;	// ignore first element bc its the id_value
	int n = matrix.size();
	double t;
	vector<double> v_i;
	vector<vector<double> > v;

	HT ht; 
	ht.resize_HT(TableSize);

	// Create v_k permanent vectors for each HashTable, Store them into v vector
	for (int i = 0; i < k; ++i){
		v_i = gaussian_distribution_vector(d);
		v.push_back(v_i);
	}
	vs.push_back(v); //keep same v for every L-table

	if(metric == "euclidean"){
		t = uniform_random_generator(0.0, W); 	// t should be common
		ts.push_back(t);						// thats why i have L t's in ts vector
	}

	//print_matrix_double(v);
	for (int i = 0; i < n; i++){	// For every p vector
		vector< double> take_whole_row( matrix[i].begin(), matrix[i].end() );	// Get one p vector-one row from the 2D matrix
		vector< double> take_row( next(take_whole_row.begin()), take_whole_row.end() );	// Get one p vector-one row from the 2D matrix
	
		string g = "";
		int id = take_whole_row[0];
		// cout << id << endl;
		// for (int xxx = 0; xxx < take_row.size(); ++xxx)
		// {
		// 	cout << take_row[xxx] << " ";
		// }cout << endl;

		// Create g
		for(int z = 0 ; z < k; z++){	// For every p, calculate k h-functions
			vector<double> get_v(v[z].begin(), v[z].end());

			assert(get_v.size() == take_row.size()); // p vector and v vector must have same size
			double inner = inner_product(begin(take_row), end(take_row), begin(get_v), 0.0);
			int h;
			if(metric == "cosine"){
				if(inner < 0)
					h = 0;
				else
					h = 1;
			}else{
				t = (inner + t) / W; 
				h = floor(t);	
			}

			g =  g + to_string(h);
		}//end k-times loop(z-iterator)
		hash<string> hasher;
		size_t hash = hasher(g);
		int f = hash % TableSize;
		//cout << f << " " << g << endl;

		ht.insert_HT(f, id, g); // Since ids are increamenting from 0...N-1 then i is the id
	} // end i loop, means end with every p-vector, aka every row of the 2D matrix
	hts.push_back(ht);
	return 0;
}


/**	
*	Return 
*	@param query_matrix{vector<vector<int> >&}	- vector that query_set is stored
*	@param matrix{vector<vector<int> >&}		- vector that data_set is stored
*	@param output_file{string}					- name of logfile
*/
vector<vector<int> > LSH::search(vector<vector<double> >& matrix, vector<vector<double> >& query_matrix){
	double t;
	vector<vector<int> > cluster_id_elements;

	for (int i = 0; i < query_matrix.size(); i++){	// For every q vector
		cluster_id_elements.push_back(vector<int>());

		vector<double> take_row( next(query_matrix[i].begin()), query_matrix[i].end());	// Get one q vector-one row from the 2D matrix
		// cout << "$query: ";
		// print_vector(take_row);

		vector<int> checked_ids;

		//Iterate for every HT
		for(int j = 0; j < L; j++){
			if(metric == "euclidean")
				t = ts[j];
			
			string g = "";
			for(int z = 0 ; z < k; z++){	// For every p, calculate k h-functions
				vector<double> get_v(vs[j][z].begin(), vs[j][z].end());

				assert(get_v.size() == take_row.size()); // p vector and v vector must have same size
				double inner = inner_product(begin(take_row), end(take_row), begin(get_v), 0.0);
				int h;
				if(metric == "cosine"){
					if(inner < 0)
						h = 0;
					else
						h = 1;
				}else{ //euclidean
					t = (t + inner) / W;
					h = floor(t);
				}
				
				g =  g + to_string(h);
			}//end k-times loop(z-iterator)



			hash<string> hasher;
			size_t hash = hasher(g);
			int f = hash % TableSize;	
			//cout << f << " " << g << endl;
			int flagati = 0;
			flagati = hts[j].search_HT(f, g); //will search on the specific f bucket
			// if search_HT found same g in a bucket then return 1
			if( flagati == 1 ){
				
				vector<int> elem;	// in vector elem store the bucket ids
				elem = hts[j].return_bucket_elements_HT_new(f, g);


				for ( int c = 0; c < elem.size(); ++c){
					int continue_flag = 0;
					// Check if an id has been checked already
					if(checked_ids.empty() == 0) {
						for (int xi = 0; xi < checked_ids.size(); ++xi){
							if( elem[c] == checked_ids[xi])
								continue_flag = 1;
						}

					}
					if(continue_flag == 1){
						continue;
					}
					//elem[c] is the ids of matrix array[dataset array]

					checked_ids.push_back(elem[c]);


				}// end loop for bucket elements

			} 

		}// end L-HT loop
		cluster_id_elements[i] = checked_ids;

		checked_ids.clear();


	}


	return cluster_id_elements;
}


/**	
*	Return none
*	@params none
*	Debugging purposes only
*/
void LSH::print_vector_of_HT(){
	cout << "\nPrint " << L << " HTs" << endl;

	for (int i = 0; i < L; ++i){ // this should be iterated L times
		hts[i].print_HT();
	}
}


/**	
*	Return none
*	@params none
*	Debugging purposes only
*/
// void LSH::print_vector_of_vs(){
// 	cout << "\nPrint " << L << " vss" << endl;

// 	for (int i = 0; i < L; ++i){ 
// 		print_matrix_double(vs[i]);
// 	}
// }


