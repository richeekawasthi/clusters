#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
using namespace std;

float randompt(){
	srand(time(nullptr));
	int lowest=0,highest=1,range=highest-lowest;
	float a=lowest+range * (rand() / (RAND_MAX + 1.0));
	cout << a << endl;
	return a;
}

float dist(vector<float> a, vector<float> b){
	float result=0;
	int d=a.size();
	for(int i=0;i<d;i++){
		result += (a[i]-b[i])*(a[i]-b[i]);
	}
	return sqrt(result);
}

void printvector(vector<vector<float>> a){
	for(int i=0;i<a.size();i++){
		for(int j=0;j<a[i].size();j++){
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
}

int equalvector(vector<vector<float>> a, vector<vector<float>> b){
	for(int i=0;i<a.size();i++){
		for(int j=0;j<a[i].size();j++){
			if(a[i][j]!=b[i][j]){
				return 0;
			}
		}
	}
	return 1;
}

vector<vector<float>> classify(int m, int d, int k, vector<vector<float>> data, vector<vector<float>> centroid){
	vector<vector<float>> distance,result;
	for(int i=0;i<k;i++){
		vector<float> temp,temp1;
		for(int j=0;j<m;j++){
			temp.push_back(dist(data[j],centroid[i]));
			temp1.push_back(0);
		}
		distance.push_back(temp);
		result.push_back(temp1);
	}
	for(int i=0;i<m;i++){
		int minindex=0;
		for(int j=0;j<k;j++){
			if(distance[j][i]<distance[minindex][i]){
				minindex=j;
			}
		}
		result[minindex][i]=1;
	}
	return result;
}

vector<vector<float>> updatecentroid(int m,int d,int k,vector<vector<float>> classification,vector<vector<float>> data){
	vector<vector<float>> newcentroid;
	for(int i=0;i<k;i++){
		vector<float> temp;
		for(int j=0;j<d;j++){
			temp.push_back(0);
		}
		int count=0;
		for(int j=0;j<m;j++){
			if(classification[i][j]==1){
				count++;
				for(int p=0;p<d;p++){
					temp[p]+=data[j][p];
				}
			}
		}
		for(int j=0;j<d;j++){
			if(count!=0){
				temp[j]=temp[j]/count;
			}
		}
		newcentroid.push_back(temp);
	}
	return newcentroid;
}

vector<vector<float>> kcluster(int m,int d,int k, vector<vector<float>> data){
	vector<vector<float>> centroid,classification;
	for(int i=0;i<k;i++){
		centroid.push_back(data[i]);
	}
	/*vector <float> temp,temp1;
	temp.push_back(0.91);
	temp.push_back(0.4);
	temp.push_back(0.2);
	centroid.push_back(temp);
	temp1.push_back(0.76);
	temp1.push_back(0.34);
	temp1.push_back(1.2);
	centroid.push_back(temp1);*/
	//cout << "First Centroid" << endl;
	//printvector(centroid);
	classification=classify(m,d,k,data,centroid);
	//cout << "First Classification" << endl;
	//printvector(classification);
	int count=0;
	//cout << "hello" << endl;
	while(equalvector(classification,classify(m,d,k,data,updatecentroid(m,d,k,classification,data)))==0 || count<10){
	//for(int i=0;i<10;i++){
		centroid=updatecentroid(m,d,k,classification,data);
		//cout << "Centroid Number " << count+2 << endl;
		//printvector(centroid);
		classification=classify(m,d,k,data,centroid);
		//cout << "Classification Number " << count+2 << endl;
		//printvector(classification);
		count++;
		if(count>100){
			//cout << "Broken" << endl;
			break;
		}
	}
	return centroid;
}

int main(int argc, char *argv[]){
	ifstream infile;
	infile.open(argv[1]);
	if(!infile){
		cerr << "Unable to open data file" << endl;
		return 1;
	}
	int k = stoi(argv[2]);
	vector <vector <float>> data;
	vector<string> file;
	string line;
	while(getline(infile,line)){
		file.push_back(line);
	}
	int m=file.size();
	for(int j=0;j<m;j++){
		string to_split = file[j];
		vector<float> array;
		size_t pos = 0, found;
		while((found = to_split.find_first_of(' ', pos)) != std::string::npos) {
			array.push_back(stof(to_split.substr(pos, found - pos)));
			pos = found+1;
		}
		array.push_back(stof(to_split.substr(pos)));
		data.push_back(array);
   	}
	int d=data[0].size();
	vector<vector<float>> finalcentroid=kcluster(m,d,k,data);
	printvector(finalcentroid);
	ofstream outfile;
	outfile.open("clusters.txt");
	for(int i=0;i<k;i++){
		for(int j=0;j<d;j++){
			outfile << finalcentroid[i][j];
			if(j!=d-1){
				outfile << " ";
			}
		}
		if(i!=k-1){
			outfile << endl;
		}
	}

	//testing code below
	//printvector(data);
	/*vector<float> a,b,c,e;
	a.push_back(1);
	a.push_back(1);
	b.push_back(2);
	b.push_back(1);
	c.push_back(4);
	c.push_back(3);
	e.push_back(5);
	e.push_back(4);
	vector<vector<float>> centroid,testdata;
	centroid.push_back(a);
	centroid.push_back(b);
	testdata.push_back(a);
	testdata.push_back(b);
	testdata.push_back(c);
	testdata.push_back(e);
	vector<vector<float>> testclassification=classify(4,2,2,testdata,centroid);
	centroid=updatecentroid(4,2,2,testclassification,testdata);
	testclassification=classify(4,2,2,testdata,centroid);
	printvector(testclassification);*/
	//vector<vector<float>> finalcentroid=kcluster(4,2,2,testdata);
	//printvector(finalcentroid);
	return 0;
}