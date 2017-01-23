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
    vector<int> lst;
    double calculate_distance(const double &x1,const double &y1,const double &x2,const double &y2);
    void search_near_city(int &next);
    void real_irekae(int i,int j);
    vector<int> irekae(int i,int j,vector<int> lst);
    void compair(double base,int number);
public:
    //pst();
    //~pst();
    void read_file(char const *argv);
    void search_best_way();
    void excange();
    void show();
    double hakaru(vector<int> lst);
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
        lst.push_back(i);
        i++;
        }    	
}

//引数として渡された二点間の距離を計算する
double pst::calculate_distance(const double &x1,const double &y1,const double &x2,const double &y2){
    int ret=0;
    ret = pow(x1-x2,2) + pow(y1-y2,2);
    return sqrt(ret);
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
        lst.push_back(next);
        search_near_city(next);
    }
}

//iからjの点をいれかえる
vector<int> pst::irekae(int i,int j,vector<int> lst){
    int tmp;

    tmp = lst[i];
    lst[i] = lst[j];
    lst[j] = tmp;

    return lst;
}

void pst::real_irekae(int i,int j){
    int tmp;

    tmp = lst[i];
    lst[i] = lst[j];
    lst[j] = tmp;

}

//lstをもとに距離を計算
double pst::hakaru(vector<int> lst){
	double distance = 0;
    for (int i = 0; i < lst.size()-1; ++i){
        distance += calculate_distance(x[lst[i]],y[lst[i]],x[lst[i+1]],y[lst[i+1]]);
    }
    distance += calculate_distance(x[lst[0]],y[lst[0]],x[lst[lst.size()-1]],y[lst[lst.size()-1]]);
    return distance;
}

//baseより組み替えた方が近くなる場合はlstの順番を変える
void pst::compair(double base,int number){
    double comp;
    int tmp = 0;
    double base1;
    vector<int> dummy_lst;


    for (int i = number; i < lst.size()-1; ++i){
    		dummy_lst = irekae(number,i,lst);
    		comp = hakaru(dummy_lst);
    		//cout << "comp:" << comp << "  base:" << base << endl;
    	   	if (comp < base){
    	   		base = comp;
    	   		tmp = i;
    	   	}
        }

    if (tmp != 0){
        real_irekae(number,tmp);
	}
}

//辺と辺を組み替える
void pst::excange(){
    double base;
    for (int i = 0; i < lst.size()-1; ++i){
        base = hakaru(lst);
        compair(base,i);
    } 
}

//通る点の順番を表示
void pst::show(){
	double distance;
    for (int i = 0; i < lst.size(); ++i){
        cout << lst[i] << endl;
    }
    distance = hakaru(lst);
    cout << "distance:" << distance << endl;
}

int main(int argc, char const *argv[]){
    pst namae;

	if(argc != 2){         //ファイルの数調整
		cout << "Please put one file!" << endl;
		exit(-1);
	}



	namae.read_file(argv[1]);
    //namae.search_best_way();
    namae.excange();
    namae.show();

	return 0;
}