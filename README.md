# k-means
A k-means clustering

#### Initialization ####
1. Random selection of k points (simplest)<br>
2. K-means++<br>
3. Reservoir Sampling <br>
#### Assignment ####
1. Lloyd’s assignment <br>
2. Assignment by Range search with LSH <br>
3. Assignment by Range search with Hypercube <br>
#### Update ####
1. K-means <br>
2. Partitioning Around Medoids (PAM) improved like Lloyd’s <br>

##### Input ##### 
A tab-separated or comma-separated file with the following form<br>
```
item_id1 X11 X12 ...
......              
item_idN XN1 XN2 ...
```
X_ij are integer values of item_N<br>

##### Output ##### 
[view output](github.com)
<br>
## Compile & Run ##
```$ make```  
```$./cluster –i input_file –c configuration_file -o output_file -d metric -n random_k_selection```
<br>
Parameters Explained: <br>
	-i: dataset 		{comma separated or tab separated files} <br>
	-c: conf_file 	<br>
	-o: output_file<br>
	-d: metric	 	{cosine, euclidean}	<br>
	-m: max_iterations 	{15 by default, if not provided}<br>
	-a: assigment_method 	{lsh, hc, lloyd}<br>
	-n: init_method 	{kmeans++, random_k_selection, reservoir_sampling}, random_k_selection by default<br>
	-u: update_method 	{kmeans, PAM}, kmeans by default<br>
	-p: print_clusters_flag {flag to enable writing id in every cluster in log file, enable with 1}<br>
  
