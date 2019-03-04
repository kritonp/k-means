#ifndef BASE_HPP
#define BASE_HPP


class Base{

	public:
		virtual int insert(vector<vector< double> >& matrix) = 0;
		virtual vector<vector<int> > search(vector<vector< double> >&, vector<vector< double> >&) = 0;


};


#endif