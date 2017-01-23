#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

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
    int ret=0;
    ret = pow(x1-x2,2) + pow(y1-y2,2);
    return sqrt(ret);
}

//点indexから近い点を探す
//x,yのvectorとつながっているかを示すmapが引数
double search_near_vertex(const vector<double> &x,const vector<double> &y,int &next,map<int,bool> &mp){
    double ret=INFINITY;  //最終的な一番近いとことの距離
    double distance,distance5;
    int index = next;
    mp[index] = true;

    for (int i = 0; i < x.size(); ++i){
        if(mp[i] != true){
            distance = calculate_distance(x[index],y[index],x[i],y[i]);
            //printf("distance;%dto%d : %lf\n",index,i,distance );
            //条件は距離が他の辺より短くてまだ繋がっていない
            if(distance <= ret){
                ret = distance;
                next = i;
            }
        }
    }
    
    return ret;
}


double expect5(map<int,bool> mp,const vector<double> &x,const vector<double> &y,int target,int &next){
    next = target;  //調べたい点
    double distance = 0;
    double dummy_distance = 0;
 
    for(int i=0;i < 5;i++){
        distance = dummy_distance;
        dummy_distance += search_near_vertex(x,y,next,mp);
        if (dummy_distance == INFINITY){
            break;
        }
    }
    return distance;
}

double search_near_vertex_5(const vector<double> &x,const vector<double> &y,int &next,map<int,bool> &mp){
    double ret_5=INFINITY;  //最終的な一番近いとことの距離
    double ret=INFINITY;
    double distance,distance5;
    int index = next;
    mp[index] = true;
    int next1 = 0;

    for (int i = 0; i < x.size(); ++i){
        if(mp[i] != true){
            distance5 = expect5(mp,x,y,i,next1);
            distance5 += calculate_distance(x[index],y[index],x[i],y[i]);
            distance5 += calculate_distance(x[0],y[0],x[next1],y[next1]);
            printf("distance5;%dto%d : %lf\n",index,next1,distance5 );
            //条件は距離が他の辺より短くてまだ繋がっていない
            if(distance5 <= ret_5){
                ret_5 = distance5;
                ret = calculate_distance(x[index],y[index],x[i],y[i]);
                next = i;
            }
        }
    }
    //cout << "選んだ点は" << next << endl;
    return ret;
}


double search_best_way(const vector<double> &x,const vector<double> &y){
    map<int,bool> mp;
    double distance = 0;
    double dummy_distance = 0;
    int next = 0;
 
    while(dummy_distance != INFINITY){
        distance = dummy_distance;
        cout << next << endl;
        dummy_distance += search_near_vertex_5(x,y,next,mp);
    }
    distance += calculate_distance(x[next],y[next],x[0],y[0]);
    return distance;
}

int main(int argc, char const *argv[]){
    vector<double> x;
    vector<double> y;
    double distance;

	if(argc != 2){         //ファイルの数調整
		cout << "Please put one file!" << endl;
		exit(-1);
	}

	read_file(argv[1],x,y);
    distance = search_best_way(x,y);
    cout << "distance:" << distance << endl;

	return 0;
}