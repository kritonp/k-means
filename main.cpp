#include "utils.hpp"

int main(int argc, char** argv) { 
	vector<vector<double> > matrix;			// dataset matrix

	auto data = cmd_arguments(argc, argv);
	string dataset			= get<0>(data);
	string conf_file 		= get<1>(data);
	string output_file		= get<2>(data);
	string metric			= get<3>(data);
	int max_iterations		= get<4>(data);
	string assigment_method	= get<5>(data);
	string init_method		= get<6>(data);
	string update_method	= get<7>(data);
	int print_clusters_flag	= get<8>(data);

	
	if( read_file(dataset, matrix) != 0 )
		cerr << "Error: read_file()" << endl;

	auto conf_data = configure_file("cluster.conf");
	int k 	 = get<0>(conf_data);
	int h 	 = get<1>(conf_data);
	int L 	 = get<2>(conf_data);
	double R = get<3>(conf_data);
	/*
	cout << "-i: " << dataset 		 		<< endl
		 << "-c: " << conf_file 	 		<< endl
		 << "-o: " << output_file	 		<< endl
		 << "-d: " << metric		 		<< endl
		 << "-m: " << max_iterations 		<< endl
		 << "-a: " << assigment_method 		<< endl
		 << "-n: " << init_method 			<< endl
		 << "-u: " << update_method 		<< endl
		 << "-p: " << print_clusters_flag 	<< endl
		 << "k: "  << k						<< endl
		 << "h: "  << h						<< endl
		 << "L: "  << L						<< endl
		 << "R: "  << R						<< endl;	
	*/

	Base* ptr = NULL;
	if(assigment_method.compare("HC") == 0 || assigment_method.compare("hc") == 0){
		int probes = 1;
		int M = 20000000;
		int TableSize = pow(2,h);
		HYPERCUBE *ptr_cube = new HYPERCUBE(h, M, probes, metric, TableSize, R);
		ptr = ptr_cube;
		ptr->insert(matrix);


	}else if(assigment_method.compare("LSH") == 0 || assigment_method.compare("lsh") == 0){
		int TableSize = matrix.size() / 8;
		LSH *ptr_lsh = new LSH(h, L, metric, TableSize, R);
		ptr = ptr_lsh;
		for ( int i = 0; i < L; ++i) 
			ptr->insert(matrix);
	}


	KMeans kmeans(k, matrix.size(), max_iterations, assigment_method, init_method, update_method, metric);
	if (kmeans.run_kmeans(matrix, output_file, print_clusters_flag, ptr) != 0 )
		cout << "Error: run_kmeans()" << endl;

	delete ptr;
	ptr = NULL;

	
	return 0;
}