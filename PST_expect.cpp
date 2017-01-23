#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

class PST_expect{
private:
    vector<double> x;
    vector<double> y;
    map<int,bool> mp;
    double distance;
    vector<int> lst;
    void search_near_city(int &next);
    void hakaru();
public:
    int expect(int base,map<int,bool> mp);
    PST_expect(vector<double> x1,vector<double> y1,map<int,bool> mp);
    //~PST_expect();
    void search_best_way_5(int next,map<int,bool> mp);
};

//引数として渡された二点間の距離を計算する
double calculate_distance(const double &x1,const double &y1,const double &x2,const double &y2){
    int ret=0;
    ret = pow(x1-x2,2) + pow(y1-y2,2);
    return sqrt(ret);
}

void PST_expect::hakaru(){
    distance = 0;
    for (int i = 0; i < lst.size()-1; ++i){
        distance += calculate_distance(x[lst[i]],y[lst[i]],x[lst[i+1]],y[lst[i+1]]);
    }
    distance += calculate_distance(x[lst[0]],y[lst[0]],x[lst[lst.size()-1]],y[lst[lst.size()-1]]);
}

PST_expect::PST_expect(vector<double> x1,vector<double> y1,map<int,bool> mp1){
    x = x1;
    y = y1;
    mp = mp1;
    distance = 0;
}

void PST_expect::search_near_city(int &next){
    double distance;
    double keep = INFINITY;
    int index = next;
    mp[index] = true;

    for (int i = 0; i < x.size(); ++i){
        if(mp[i] != true){
            distance = calculate_distance(x[index],y[index],x[i],y[i]);
            if(distance < keep){
                keep = distance;
                next = i;
            }
        }
    }
    //distance = calculate_distance(x[index],y[index],x[next],y[next]);
    mp[next] = true;
}

void PST_expect::search_best_way_5(int next,map<int,bool> mp1){
    distance = 0;
    mp = mp1;
    for(int i=0;i < 5;++i){
        lst.push_back(next);
        search_near_city(next);
    }
    hakaru();
    lst.clear();
}

int  PST_expect::expect(int base,map<int,bool> mp1){
    double tmp;
    int near_city;

    for (int i = 0; i < lst.size(); ++i){
        if(mp[i] != true){
            lst.push_back(base);
            search_best_way_5(i,mp1);
            if (tmp > distance){
                tmp = distance;
                cout << "i:" << endl;
                near_city = i;
            }
        }
    }
    cout << "near_city:" << near_city << endl;
    return near_city;
}

int deside_next_city(vector<double> x,vector<double> y,map<int,bool> mp,int base){
    PST_expect exp(x,y,mp);

    return exp.expect(base,mp);
}

class pst{
private:
    vector<double> x;
    vector<double> y;
    map<int,bool> mp;
    double distance;
    vector<int> lst;
    void search_near_city(int &next);

public:
    //pst();
    //~pst();
    void read_file(char const *argv);
    void search_best_way();
    double get_distance();
    void show();
    void hakaru();
};

//ファイルから読み込んだ文字を数字に直す
double make_number(const string &str,const int index1,const int index2 = 0){
    string ret ="";
    for (int i = index2; i < index1; ++i){
        ret +=str[i];
    }
    return atof(ret.c_str());
}

//ファイルを読んでvector<double>x,yに入れる
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

//近くの頂点を探し、引数nextを次の点の番号に更新
void pst::search_near_city(int &next){
    double distance;
    double keep = INFINITY;
    int index = next;
    mp[index] = true;

    for (int i = 0; i < x.size(); ++i){
        if(mp[i] != true){
            distance = calculate_distance(x[index],y[index],x[i],y[i]);
            if(distance <= keep){
                keep = distance;
                next = i;
            }
        }
    }

    mp[next] = true;
}

//近い点を選んでいき、vector<int>に点の番号を入れていく
void pst::search_best_way(){
    int next = 0;
 
    for(int i=0;i < x.size();++i){
        mp[next] = true;
        lst.push_back(next);
        next = deside_next_city(x,y,mp,next);
    }
}

//distanceを返す
double pst::get_distance(){
    return distance;
}

//通る点の順番を表示
void pst::show(){
    for (int i = 0; i < lst.size(); ++i){
        cout << lst[i] << endl;
    }
}

//lstをもとに距離を計算
void pst::hakaru(){
    for (int i = 0; i < lst.size()-1; ++i){
        distance += calculate_distance(x[lst[i]],y[lst[i]],x[lst[i+1]],y[lst[i+1]]);
    }
    distance += calculate_distance(x[lst[0]],y[lst[0]],x[lst[lst.size()-1]],y[lst[lst.size()-1]]);
}



int main(int argc, char const *argv[]){
    pst namae;

	if(argc != 2){         //ファイルの数調整
		cout << "Please put one file!" << endl;
		exit(-1);
	}



	namae.read_file(argv[1]);
    namae.search_best_way();
    namae.hakaru();
    namae.show();
    cout << "distance:" << namae.get_distance() << endl;

	return 0;
}