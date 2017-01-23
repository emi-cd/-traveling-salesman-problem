#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

class pst{
private:
    vector<double> x;
    vector<double> y;
    map<int,bool> mp;
    double distance;
    double calculate_distance(const double &x1,const double &y1,const double &x2,const double &y2);
    double search_near_vertex(int &next);
public:
    //pst();
    //~pst();
    void read_file(char const *argv);
    void search_best_way();
    double get_distance();
    
};

double make_number(const string &str,const int index1,const int index2 = 0){
    string ret ="";
    for (int i = index2; i < index1; ++i){
        ret +=str[i];
    }
    return atof(ret.c_str());
}

void pst::read_file(char const *argv){
	ifstream fin(argv);
	string str;
    int index;
    int i=0;

    if( !fin ){
        cout << "Error: cannot open file(" << argv << ")" << endl;
        exit(1);
    }
    getline(fin,str);
    while(getline(fin,str)){
        index = (int)str.find(",");
        x.push_back(make_number(str,index));
        y.push_back(make_number(str,str.length(),index+1));
        i++;
        }    	
}

double pst::calculate_distance(const double &x1,const double &y1,const double &x2,const double &y2){
    int ret=0;
    ret = pow(x1-x2,2) + pow(y1-y2,2);
    return sqrt(ret);
}

//点indexから近い点を探す
double pst::search_near_vertex(int &next){
    double ret=INFINITY;
    double tmp_distance;
    int index = next;

    for (int i = 0; i < x.size(); ++i){
        if(i != index){
            tmp_distance = calculate_distance(x[index],y[index],x[i],y[i]);
            //printf("distance;%dto%d : %lf\n",index,i,distance );
            //条件は距離が他の辺より短くてまだ繋がっていない
            if(tmp_distance <= ret && mp[i] != true){
                ret = tmp_distance;
                next = i;
            }
        }
    }
    mp[next] = true;
    return ret;
}

void pst::search_best_way(){
    mp[0] = true;
    double dummy_distance = 0;
    int next = 0;
 
    while(dummy_distance != INFINITY){
        distance = dummy_distance;
        cout << next << endl;
        dummy_distance += search_near_vertex(next);
    }
    distance += calculate_distance(x[next],y[next],x[0],y[0]);
}

double pst::get_distance(){
    return distance;
}

int main(int argc, char const *argv[]){
    pst namae;

	if(argc != 2){         //ファイルの数調整
		cout << "Please put one file!" << endl;
		exit(-1);
	}

	namae.read_file(argv[1]);
    namae.search_best_way();
    cout << "distance:" << namae.get_distance() << endl;

	return 0;
}