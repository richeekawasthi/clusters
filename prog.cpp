#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
using namespace std;

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
			temp[j]=temp[j]/count;
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
	//printvector(centroid);
	classification=classify(m,d,k,data,centroid);
	int count=0;
	//cout << "hello" << endl;
	while(equalvector(classification,classify(m,d,k,data,updatecentroid(m,d,k,classification,data)))==0){
		centroid=updatecentroid(m,d,k,classification,data);
		classification=classify(m,d,k,data,centroid);
		count++;
		if(count>100){
			cout << "Broken" << endl;
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
	string test,test1;
	infile >> test >> test1;
	int m=stoi(test);
	int d=stoi(test1);
	vector <vector <float>> data;
	for(int i=0;i<m;i++){
		vector <float> temp;
		for(int j=0;j<d;j++){
			infile >> test;
			temp.push_back(stof(test));
		}
		data.push_back(temp);
	}
	vector<vector<float>> finalcentroid=kcluster(m,d,k,data);
	printvector(finalcentroid);


	//testing code only
	//printvector(data);
	vector<float> a,b,c,e;
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
	/*vector<vector<float>> testclassification=classify(4,2,2,testdata,centroid);
	centroid=updatecentroid(4,2,2,testclassification,testdata);
	testclassification=classify(4,2,2,testdata,centroid);
	printvector(testclassification);*/
	//vector<vector<float>> finalcentroid=kcluster(4,2,2,testdata);
	//printvector(finalcentroid);
	return 0;
}