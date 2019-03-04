#include "HashTable.hpp"

/**	
*	Resizes the vector so that it contains n elements.
*	Return none
*	@param TableSize{int} - size of HT
*/
void HT::resize_HT(int TableSize){
	dHT.resize(TableSize);
}


/**	
*	Return HT size
*	@param none
*/
int HT::size_HT(){
	return dHT.size();
}


/**	
*	Return bucket size
*	@param index{int} - index in HT 
*/
int HT::bucket_size_HT(int index){
	int size = 0;
	for(auto &j : dHT[index]){
		size++;
	}
	//cout << "bucket_size_HT_" << index << " " << size<<endl;
	return size;
}


/**	
*	Insert new element <item_id, g value> to specific bucket in HT
*	Return none
*	@param f{int}		- index
*	@param id{int}		- item id
*	@param g{string}	- g value
*/
void HT::insert_HT(int f, int id, string g){
	dHT[f].push_back( tuple<int, string>(id, g) );
}


/**	
*	Return none
*	@param none
*/
void HT::print_HT(){
	cout << "print HT" <<  endl;
	for (int i = 0; i < dHT.size(); ++i){
		cout <<"i= "<< i << ": ";
		for(auto &j : dHT[i]){
			cout << "<"<< get<0>(j) << "," << get<1>(j) << "> ";
		}
		cout << endl;
	}
}

void HT::print_HT_HC(){
	cout << "print HT" <<  endl;
	for (int i = 0; i < dHT.size(); ++i){
		cout <<"i= "<< bitset<11>(i) << ": ";	//the value in bitset should be the #of bits
		for(auto &j : dHT[i]){
			cout << "<item_"<< get<0>(j) << ">, ";//<< "," << get<1>(j) << "> ";
		}
		cout << endl << endl;
	}
}

/**	
*	Print a specific bucket from the HT
*	Return none
*	@param index{int} - HT index to bucket
*/
void HT::print_bucket_HT(int index){
	cout << "print bucket: " ;

	for(auto &j : dHT[index]){
		cout << "<"<< get<0>(j) << "," << get<1>(j) << "> ";
	}
	cout << endl;
	
}


/**	
*	Return every item from a specific bucket 
*	Return vector <id>
*	@param index{int} - bucket index
*/
vector<int> HT::return_bucket_elements_HT(int index){
	vector<int> elements;
	for(auto &j : dHT[index]){
		//cout << "<"<< get<0>(j) << "," << get<1>(j) << "> ";
		elements.push_back(get<0>(j));
	}

	return elements;
}

vector<int> HT::return_bucket_elements_HT_new(int index, string g){
	vector<int> elements;
	for(auto &j : dHT[index]){
		//cout << "<"<< get<0>(j) << "," << get<1>(j) << "> ";
		//elements.push_back(get<0>(j));
		if( g == get<1>(j) ){
			elements.push_back(get<0>(j));	
		}
	}
	//cout << endl;
	return elements;
}

/**	
*	Return 1 if query found in bucket
*	@param index{int}	- index
*	@param g{string}	- g value
*/
int HT::search_HT(int index, string g){
	// Here i have to iterate through the bucket and search for similar vectors
	// i MUST return every item_id to calculate euclidean distance with (q,p)
	
	for(auto &j : dHT[index]){
		//cout << "<"<< get<0>(j) << "," << get<1>(j) << "> "; //anoigw
		if( g == get<1>(j) ){
			//cout << "FOUND same G on HT: " << jj << endl;//return the item_id
			//cout << "found item: <"<< get<0>(j) << "," << get<1>(j) << "> " << endl;
			//print_bucket_HT(index);
			return 1;
		}
	}
	//cout << endl;

}