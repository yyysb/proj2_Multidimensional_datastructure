#ifndef MD_NAIVE_H_INCLUDED
#define MD_NAIVE_H_INCLUDED

#include "MD_base.h"

#include<map>
class md_naive : public md_base{
private:
    vector<dot> data;
    multimap<string, dot> namemap;
public:
    md_naive(){};
    void insert(const dot& x){
        data.push_back(x);
        namemap.insert( make_pair(x.name,x) );
    }
    void delbypos(const dot& x){
        for(int i = 0; i < data.size(); ++i)
            if( x.e)
    }
    void delbyname(const string&){}
    vector<dot> findbypos(const dot&){}
    vector<dot> findbyname(const string&){}
    vector<dot> search(const dot&, const double& dist){};
};

#endif // MD_NAIVE_H_INCLUDED
