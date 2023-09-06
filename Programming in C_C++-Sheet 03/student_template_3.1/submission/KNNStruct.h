#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>



struct DYNPoint {
	std::vector<float> data;

	// We use a static function to return a DYNPoint object (this pattern is sometimes called a "factory method")
	// We can mark functions as nodiscard to indicate that we must use the return value in some way (e.g, it doesn't get discarded as ).
	[[nodiscard]] static DYNPoint createRandomPoint(unsigned int size, int minimum=-5000, int maximum=5000) {
		DYNPoint p;
		if (size > 0 && minimum <= maximum) {			
			// STUDENT TODO: add your code	
			p.data.reserve(sizeof(float)*size);
			for (unsigned int i=0;i<size;i++){
				p.data.push_back(float(std::rand())*(float(maximum-minimum)/float(RAND_MAX))+float(minimum));
				
			}		
		}
		return p;
	}
};

// struct COMPARATOR {
// 	float(*function_ptr_Distance)(const DYNPoint &, const DYNPoint &);
// 	DYNPoint A;
// 	COMPARATOR(float(*_function_ptr_Distance)(const DYNPoint &, const DYNPoint &),DYNPoint &_A)
// 	: function_ptr_Distance(_function_ptr_Distance){
// 		A=_A;
// 	}
// 	bool compare(const std::pair<DYNPoint,unsigned int> p1, const std::pair< DYNPoint,unsigned int> p2) {
// 		return (function_ptr_Distance(p1.first,A)<function_ptr_Distance(p2.first,A));
// 	}

// };

struct KNN {
	KNN( float(*_function_ptr_Distance)(const DYNPoint &, const DYNPoint &))
		: function_ptr_Distance(_function_ptr_Distance)
	{
		
	}

	std::vector<std::pair<DYNPoint, unsigned int>> trainingData{}; // default initialize on creation

	float(*function_ptr_Distance)(const DYNPoint &, const DYNPoint &) = 0;

	// just sets the training data (no actual training required)
	void TrainKNN(const std::vector<std::pair<DYNPoint, unsigned int>> &dataset) {
		trainingData.clear();
		trainingData = dataset;
	}
	// bool distComparator = [=,&A](std::pair<DYNPoint,unsigned int> pair1, std::pair<DYNPoint,unsigned int> pair2){ 
	// 	return true;//(function_ptr_Distance(pair1.first,A)<function_ptr_Distance(pair2.first,A)); 
	// 	}; 
	// float identity(float value) {
	// 	return value;
	// } 

	void sort(std::vector<std::pair<float,unsigned int>> &vec,const unsigned int k) const{
		size_t minPos = 0;
		for (size_t i=0;i<k-1;i++){
			minPos =i;
			for (size_t j=i+1;j<k;j++){
				if (vec[j].first<vec[minPos].first){
					minPos = j;

				}
				if (minPos!=i){
					std::swap(vec[minPos],vec[i]);
				}
			}
		}
	}
	void sortSecond(std::vector<std::pair<float,unsigned int>> &vec,const unsigned int k) const{
		size_t minPos = 0;
		for (size_t i=0;i<k-1;i++){
			minPos =i;
			for (size_t j=i+1;j<k;j++){
				if (vec[j].second<vec[minPos].second){
					minPos = j;

				}
				if (minPos!=i){
					std::swap(vec[minPos],vec[i]);
				}
			}
		}
	}

	void sortSecond(std::vector<std::pair<unsigned int,size_t>> &vec,const unsigned int k) const{
		size_t minPos = 0;
		for (size_t i=0;i<k-1;i++){
			minPos =i;
			for (size_t j=i+1;j<k;j++){
				if (vec[j].second<vec[minPos].second){
					minPos = j;

				}
				if (minPos!=i){
					std::swap(vec[minPos],vec[i]);
				}
			}
		}
	}
	
	// float  (*func)( const DYNPoint &, const DYNPoint &) = function_ptr_Distance;
	void insertSorted(std::vector<std::pair<float,unsigned int>> &vec,const std::pair<float,unsigned int> newElem,const unsigned int &k)const{
		vec[k-1] = newElem;
		size_t pos = k-1;
		while ( vec[pos].first<vec[pos-1].first && pos>0){
			std::swap(vec[pos],vec[pos-1]);
			pos--;
		}	
	}
	void printVector(const std::vector<std::pair<float,unsigned int>> &vec,const bool trig)const{
		if (trig){
		for (size_t i=0;i<vec.size();i++){
			std::cout << "("<<vec[i].first << ","<<vec[i].second << "), ";
		}
		std::cout << std::endl;
		}
	}
	void printVector(const DYNPoint &vec,const bool trig)const{
		if (trig){
		std::cout << "(";
		for (size_t i=0;i<vec.data.size();i++){
			std::cout <<vec.data[i] <<", ";
			
		}
		std::cout << "); ";
		}
	}
	void printVector( const std::vector<std::pair<DYNPoint,unsigned int>> &vec,const bool trig)const{
		if (trig){
		for (size_t i=0;i<vec.size();i++){
			printVector(vec[i].first,trig);
			std::cout << vec[i].second<< std::endl;
		}
		std::cout << std::endl;
		}
	}

	
	int classify(const unsigned int k, const DYNPoint &A, const unsigned int &label) const {

		int class_label = -1;

		if (k && function_ptr_Distance && trainingData.size()) {

			// STUDENT TODO: your code
			bool trigger = false;
			std::vector<std::pair<float,unsigned int>> distances;
			distances.reserve(sizeof(std::pair<float,unsigned int>)*k);
			
			size_t trainSize =trainingData.size();
			size_t size = (k>trainSize?trainSize:k);
			float distance;
			std::pair<float,unsigned int> pair;
			if (trigger){
			printVector(trainingData,trigger);
			std::cout <<"A:";
			printVector(A,trigger);
			std::cout << " "<< label<<std::endl;}
			for (size_t i=0;i<(k>trainSize?trainSize:k);i++){
				distance=function_ptr_Distance(trainingData[i].first,A);
				pair.first = distance;
				pair.second = trainingData[i].second;
				
				distances.push_back(pair);
			}
			if (trigger){
			std::cout << distances.size()<<std::endl;
			std::cout << "distances after 1. loop"<<std::endl;}
			printVector(distances,trigger);
			sort(distances,size);
			for (size_t iter=k; iter<(k>trainSize?k:trainSize);iter++){
				distance=function_ptr_Distance(trainingData[iter].first,A);
				if (distance<distances[k-1].first){
					pair.first = distance;
					pair.second = trainingData[iter].second;

					if (trigger){
					std::cout <<pair.first<<" "<< pair.second<<std::endl;
					printVector(trainingData[iter].first,trigger);
					std::cout <<" "<< trainingData[iter].second	<<std::endl;}	

					insertSorted(distances, pair, size);
				}
				
				
			}
			if (trigger) std::cout << "distances after 2. loop"<<std::endl;
			printVector(distances,trigger);
			sortSecond(distances, size);
			std::vector<std::pair<unsigned int, size_t>> nnLabels;
			std::pair<unsigned int, size_t> newLabel{distances[0].second,0};
			nnLabels.push_back(newLabel);
			for (size_t iterator=0;iterator<(k>trainSize?trainSize:k);iterator++){
				if (distances[iterator].second!=(*--nnLabels.end()).first){
					newLabel.first = distances[iterator].second;
					// newLabel.second =0;
					nnLabels.push_back(newLabel);
				}
				(*--nnLabels.end()).second += 1;
			}
			sortSecond(nnLabels, nnLabels.size());
			class_label = (*--nnLabels.end()).first;
		}
		return class_label;
	}
};

float DistanceManhattan(const DYNPoint &A, const DYNPoint &B) {

	// STUDENT TODO: your code
	size_t size = A.data.size();
	
	if (size==B.data.size()){
		float sum =0;
		for (size_t i=0;i<size;i++){
			sum += std::abs(A.data[i]-B.data[i]);
		}
		return sum/float(size);
	}

	return std::nanf("");

}

float DistanceEuclid(const DYNPoint &A, const DYNPoint &B) {

	// STUDENT TODO: your code
	size_t size = A.data.size();
	if (size==B.data.size()){
		float sum =0;
		for (size_t i=0;i<size;i++){
			sum += (A.data[i]-B.data[i])*(A.data[i]-B.data[i]);
		}
		return std::sqrt(sum/float(size));
	}

	return std::nanf("");

}

void createDataset(std::vector<std::pair<DYNPoint, unsigned int>> &dataset, const unsigned int amount, const unsigned int class_label,
				const unsigned int point_size, const int minimum, const int maximum) {

	if (amount > 0 && point_size>0 && minimum <= maximum) {

	// STUDENT TODO: your code
	dataset.reserve(dataset.size()+sizeof(std::pair<DYNPoint,unsigned int>)*amount);
	std::pair<DYNPoint,unsigned int> pair{DYNPoint::createRandomPoint(point_size,minimum,maximum),class_label};
	for (unsigned int points=0;points<amount;points++){
		dataset.push_back(pair);
		pair.first = DYNPoint::createRandomPoint(point_size,minimum,maximum);

	}

	}
}

void evaluateKNN(const std::vector<std::pair<DYNPoint, unsigned int>> &dataset, const KNN &Classifier, const unsigned int k) {
	if (!dataset.empty()) {
		float acc = 0;
		// std::cout << dataset.size() << std::endl;
		for (size_t i = 0; i < dataset.size(); i++) {
			// std::rand();
			// int pos = (std::rand()*dataset.size())/RAND_MAX;
			if (static_cast<unsigned int>(Classifier.classify(k, dataset[i].first,dataset[i].second)) == dataset[i].second)
				acc++;
		}
		std::cout << "Accuracy: " << acc / float(dataset.size()) << std::endl;
	}
}









