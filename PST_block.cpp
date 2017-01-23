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
    double distance;
    int last;
    int flag;
    double calculate_distance(const double &x1,const double &y1,const double &x2,const double &y2);
    double search_near_vertex(int &next,map<int,bool> &mp,const vector<double> &x,const vector<double> &y);
    void connect(vector<double> &x1,vector<double> &y1,vector<double> &x2,vector<double> &y2);
    int make_block_number(const double x,const double y,const int max);
    double search_max_number(const vector<double> &target);
public:
    pst();
    //~pst();
    void read_file(char const *argv);
    void search_best_way(const vector<double> &x,const vector<double> &y);
    double get_distance();
    void mitei();
    
};

pst::pst(){
    distance = 0;
    last = 0;
}

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
double pst::search_near_vertex(int &next,map<int,bool> &mp,const vector<double> &x,const vector<double> &y){
    double ret=INFINITY;
    double tmp_distance;
    int index = next;
    mp[index] = true;
    for (int i = 0; i < x.size(); ++i){
        if(mp[i] != true){
            tmp_distance = calculate_distance(x[index],y[index],x[i],y[i]);
            //printf("distance;%dto%d : %lf\n",index,i,distance );
            //条件は距離が他の辺より短くてまだ繋がっていない
            if(tmp_distance <= ret){
                ret = tmp_distance;
                next = i;
            }
        }
    }
    return ret;
}

void pst::search_best_way(const vector<double> &x,const vector<double> &y){
    map<int,bool> mp;
    mp[0] = true;
    double tmp_distance = 0;
    double dummy_distance = 0;
    int next = 0;

    cout << "テスト" << endl;
    if(x.size() != 0){
    	cout << "実行" << endl;
    	while(dummy_distance != INFINITY){
    	    tmp_distance = dummy_distance;
    	    last = next;
    	    dummy_distance += search_near_vertex(next,mp,x,y);
    	}
    	tmp_distance += calculate_distance(x[next],y[next],x[0],y[0]);
    	distance += tmp_distance;
    	cout << "distance:" << distance << endl;
    	flag = 1;
	}
}

double pst::get_distance(){
    return distance;
}

int pst::make_block_number(const double x,const double y,const int max){
    int i,j;
    int block_number;

    i = x/500;
    j = y/500;
    block_number = i + ((max/500+1) * j);
    return block_number;
}

void pst::connect(vector<double> &x1,vector<double> &y1,vector<double> &x2,vector<double> &y2){
    distance += calculate_distance(x1[last],y1[last],x2[0],y2[0]);
    	cout << "last:" << last << endl;

}

double pst::search_max_number(const vector<double> &target){
    double max = 0;
    for (int i = 0; i < target.size(); ++i){
        if (max < target[i]){
            max = target[i];
        }
    }
    return max;
}

void pst::mitei(){//リスト
    int max_x = search_max_number(x);
    int max_y = search_max_number(y);

    map<int,vector<double> > block_x;
    map<int,vector<double> > block_y;

    for (int i = 0; i < (max_x/500+1) * (max_y/500+1); ++i){
        vector<double> x_block;
        vector<double> y_block;
        for (int j = 0; j < x.size(); ++j){
            if(make_block_number(x[j],y[j],max_x)==i){
            x_block.push_back(x[j]);
            y_block.push_back(y[j]);
            }
        }
        block_x[i] = x_block;
        block_y[i] = y_block;
    }

    
    for (int i = 0; i < (max_x/500+1) * (max_y/500+1); ++i){
    	flag = 0;
        search_best_way(block_x[i],block_y[i]);
        //cout << "segmen" << endl;
        cout << flag << endl;
    	if(flag){
    		if (i+1 >= (max_x/500+1) * (max_y/500+1)){
    			connect(block_x[i],block_y[i],block_x[0],block_y[0]);
    		}
        	connect(block_x[i],block_y[i],block_x[i+1],block_y[i+1]);
        }
        //cout << "i:" << i << endl;
    }
}

int main(int argc, char const *argv[]){
    pst namae;

	if(argc != 2){         //ファイルの数調整
		cout << "Please put one file!" << endl;
		exit(-1);
	}

	namae.read_file(argv[1]);
    //namae.search_best_way();
    namae.mitei();
    cout << "distance:" << namae.get_distance() << endl;

	return 0;
}