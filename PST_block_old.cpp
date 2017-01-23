#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <list>

using namespace std;

double search_max_number(const vector<double> &target){
    double max = 0;
    for (int i = 0; i < target.size(); ++i){
        if (max < target[i]){
            max = target[i];
        }
    }
    return max;
}

int make_block_number(const int x,const int y,const int max){
    int i,j;
    int block_number;

    i = x/500;
    j = y/500;
    block_number = i * max + j;

    return block_number;
}

double make_number(const string &str,const int index1,const int index2 = 0){
    string ret ="";
    for (int i = index2; i < index1; ++i){
        ret +=str[i];
    }
    return atof(ret.c_str());
}

void read_file(char const *argv,vector<double> &x,vector<double> &y){
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

double calculate_distance(const double &x1,const double &y1,const double &x2,const double &y2){
    int ret;
    ret = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
    if (ret <= 0){
        return ret;
    }
    return sqrt(ret);
}

//点indexから近い点を探す
double search_near_vertex(const vector<double> &x,const vector<double> &y,int &next,map<int,bool> &mp){
    double ret=INFINITY;
    double distance;
    int index = next;

    for (int i = 0; i < x.size(); ++i){
        if(i != index && mp[i] != true){
            distance = calculate_distance(x[index],y[index],x[i],y[i]);
            //printf("distance;%dto%d : %lf\n",index,i,distance );
            //条件は距離が他の辺より短くてまだ繋がっていない
            if(distance <= ret){
                ret = distance;
                next = i;
            }
        }
    }

    mp[next] = true;
    return ret;
}

double search_best_way(const vector<double> &x,const vector<double> &y,map<int,bool> mp,int &next){
    mp[0] = true;
    double distance = 0;
    double dummy_distance = 0;
    int next = 0;
 
    while(dummy_distance != INFINITY){
        distance = dummy_distance;
        cout << next << endl;
        dummy_distance += search_near_vertex(x,y,next,mp);
    }

    return distance;
}

double connect(vector<double> &x1,vector<double> &y1,vector<double> &x2,vector<double> &y2,int &last){
	return calculate_distance(x1[last],y1[last],x2[0],y2[0]);
}

void mitei(vector<double> &x,vector<double> &y,map<int,bool> &mp){//リスト
    int max_x = search_max_number(x);
    int max_y = search_max_number(y);

    map<int,vector<double> > block_x;
    map<int,vector<double> > block_y;

    for (int i = 0; i < (max_x+1) * (max_y+1); ++i){
        vector<double> x_block;
        vector<double> y_block;
        if(make_block_number(x[i],y[i],max_x)==i){
            x_block.push_back(x[i]);
            y_block.push_back(y[i]);
        }
        block_x[i] = x_block;
        block_y[i] = y_block;
    }

    double distance = 0;
    for (int i = 0; i < (max_x+1) * (max_y+1); ++i){
        distance += search_best_way(block_x[i],block_y[i],mp,next);
        distance += connect(block_x[i],block_y[i],block_x[i+1],block_y[i+1],next);
    }
    cout << "new distance:" << distance << endl;
}

int main(int argc, char const *argv[]){
    vector<double> x;
    vector<double> y;
    double distance;
    map<int,bool> mp;

	if(argc != 2){         //ファイルの数調整
		cout << "Please put one file!" << endl;
		exit(-1);
	}

	read_file(argv[1],x,y);
    distance = search_best_way(x,y,mp);
    cout << "distance:" << distance << endl;
    //mitei(x,y,mp);

	return 0;
}