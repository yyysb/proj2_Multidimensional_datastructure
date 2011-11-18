#ifndef MD_BASE_H_INCLUDED
#define MD_BASE_H_INCLUDED

#include <string>
#include <vector>
#include <cstring>
using namespace std;

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
        return equalfull(x) || x.name=="" && equalpos(x);
    }
};

class md_base{
public:
    md_base(){};
    virtual void insert(const dot&) = 0;
    virtual void delbypos(const dot&) = 0;
    virtual void delbyname(const string&) = 0;
    virtual vector<dot> findbypos(const dot&) = 0;
    virtual vector<dot> findbyname(const string&) = 0;
    virtual vector<dot> search(const dot&, const double& dist) = 0;
    virtual ~md_base(){}
private:
    const md_base& operator = (const md_base&){}
    md_base(const md_base&){}
};

#endif // MD_BASE_H_INCLUDED
