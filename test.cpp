#include <iostream>
#include <armadillo>
#include<fstream>

using namespace std;
using namespace arma;

// Armadillo documentation is available at:
// http://arma.sourceforge.net/docs.html

int
main(int argc, char** argv){
	
	char theline [100] ;
	
	//read file
	fstream fin;
	fin.open("traindata.txt",ios::in);
	double data[200][5];


	//initial
	for(int i=0; i<200; i++){
		data[i][0] = 1;
		for(int j=1; j<5; j++){
			data[i][j] = 0 ;
		}
	}
	//分割字串存入二維陣列
	char *token = NULL;
	char *next_token = NULL;
	char seps[]   = " ,\t\n";
	int a=0;
	while(fin.getline(theline,sizeof(theline),'\n')){
		int b = 0;
		token = strtok_s( theline, seps, &next_token);
		while (token != NULL){
			data[a][b+1] = atof(token);
            token = strtok_s( NULL, seps, &next_token);
			b++;
		}
		a++;
	}
	fin.close();


	mat Mdata(200,3);
	for(int i=0; i<200; i++){
		for(int j=0; j<3; j++){
			Mdata(i,j) = data[i][j];
		}
	}
	mat Tdata(3,200);
	for(int i=0; i<3; i++){
		for(int j=0; j<200; j++){
			Tdata(i,j) = data[j][i];
		}
	}
	mat ZTZ(3,3);
	//initial
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			ZTZ(i,j) = 0;
		}
	}
	//calculate
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			for(int k=0; k<200; k++){
				ZTZ(i,j) = ZTZ(i,j) + Tdata(i,k)*Mdata(k,j);
			}
		}
	}
	//plus lamda
	for(int i=0; i<3; i++){
		ZTZ(i,i) = ZTZ(i,i) + 11.26;
	}

	//inverse
	mat inverse(3,3);
	inverse = inv(ZTZ);

	mat inZT(3,200);
	//initial
	for(int i=0; i<3; i++){
		for(int j=0; j<200; j++){
			inZT(i,j) = 0;
		}
	}
	//calculate
	for(int i=0; i<3; i++){
		for(int j=0; j<200; j++){
			for(int k=0; k<3; k++){
				inZT(i,j) = inZT(i,j) + inverse(i,k)*Tdata(k,j);
			}
		}
	}

	//wREG
	double wREG[3] = {0,0,0};
	for(int i=0; i<3; i++){
		for(int j=0; j<200; j++){
			wREG[i] = wREG[i] + inZT(i,j)*data[j][3];
		}
	}

	//calculate ein
	double ein = 0;
	for(int i=0; i<200; i++){
		double temp = wREG[0]*data[i][0] + wREG[1]*data[i][1] + wREG[2]*data[i][2];
		int h = 0;
		if(temp>0){
			h = 1;
		}else{
			h = -1;
		}
		if(h==data[i][3]){

		}else{
			ein++;
		}
	}
	ein = ein/200;
	cout<<"Ein"<<endl;
	cout<<ein<<endl;

	//read file
	fin.open("testdata.txt",ios::in);
	double test[1000][5];


	//initial
	for(int i=0; i<1000; i++){
		test[i][0] = 1;
		for(int j=1; j<5; j++){
			test[i][j] = 0 ;
		}
	}
	//分割字串存入二維陣列
	char *token2 = NULL;
	char *next_token2 = NULL;
	a=0;
	while(fin.getline(theline,sizeof(theline),'\n')){
		int b = 0;
		token2 = strtok_s( theline, seps, &next_token2);
		while (token2 != NULL){
			test[a][b+1] = atof(token2);
            token2 = strtok_s( NULL, seps, &next_token2);
			b++;
		}
		a++;
	}
	
	//calculate eout
	double eout = 0;
	for(int i=0; i<1000; i++){
		double temp = wREG[0]*test[i][0] + wREG[1]*test[i][1] + wREG[2]*test[i][2];
		int h = 0;
		if(temp>0){
			h = 1;
		}else{
			h = -1;
		}
		if(h==test[i][3]){

		}else{
			eout++;
		}
	}
	eout = eout/1000;
	cout<<"Eout"<<endl;
	cout<<eout<<endl;


  system("pause");
  return 0;
  }