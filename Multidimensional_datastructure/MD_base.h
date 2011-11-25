#ifndef MD_BASE_H_INCLUDED
#define MD_BASE_H_INCLUDED

#include <string>
#include <vector>
#include <cstring>
#include <cmath>
using namespace std;

const double eps = 1e-8;
const int oo = 2147483647;

struct dot{
    int data[2];
    string name;

    dot():name(){
      memset(data,0,sizeof(data));
    }
    dot(const int& x, const int& y, const string& name_=""): name(name_){
      data[0] = x;
      data[1] = y;
    }
    int& operator[] (int x){
      return data[x];
    }
    int operator[] (int x) const{
      return data[x];
    }
    bool equalpos(const dot& x) const{
        return data[0]==x[0] && data[1]==x[1];
    }
    bool equalfull(const dot& x) const{
        return equalpos(x) && name==x.name;
    }
    bool equalsmart(const dot& x) const{
        return equalfull(x) || (name=="" && equalpos(x));
    }
};
const dot nulldot(oo,oo);

class md_base{
public:
    md_base(){};
    virtual void insert(const dot&) = 0;
    virtual void delbypos(const dot&) = 0;
    virtual void delbyname(const string&) = 0;
    virtual dot findbypos(const dot&) = 0;
    virtual dot findbyname(const string&) = 0;
    virtual void search(const dot&, const double& d,vector<dot>&) = 0;

    virtual void importdata(const vector<dot>&) = 0;
    virtual void exportdata(vector<dot>&) = 0;
    virtual ~md_base(){}
private:
    const md_base& operator = (const md_base&){ return *this;}
    md_base(const md_base&){}
};


inline double sqr(const double& x){
    return x*x;
}
inline double dist(const dot& x, const dot& y){
    return sqrt(sqr(x[0]-y[0]) + sqr(x[1]-y[1]));
}
inline bool instrictcontain(const dot& c, const double& r, int x, int y){
    return dist(c,dot(x,y)) < r + eps;
}
inline bool instrictcontain(const dot& c, const double& r, const dot& x){
    return dist(c,x) < r + eps;
}
#endif // MD_BASE_H_INCLUDED
