#ifndef MD_NAIVE_H_INCLUDED
#define MD_NAIVE_H_INCLUDED

#include "MD_base.h"

class md_naive : public md_base{
private:
    vector<dot> data;
public:
    void insert(const dot& x){
        data.push_back(x);
    }
    void delbypos(const dot& x){
        for(int i = 0; i < data.size(); ++i)
            if( x.equalsmart(data[i]) )
                data.erase(data.begin()+i--);
    }
    void delbyname(const string& s){
        for(int i = 0; i < data.size(); ++i)
            if( s == data[i].name )
                data.erase(data.begin()+i--);
    }
    dot findbypos(const dot& x){
        for(int i = 0; i < data.size(); ++i)
            if( x.equalpos(data[i]) )
                return(data[i]);
        return nulldot;
    }
    dot findbyname(const string& s){
        for(int i = 0; i < data.size(); ++i)
            if( s == data[i].name )
                return(data[i]);
        return nulldot;
    }
    void search(const dot& x, const double& d,vector<dot>& ret){
        for(int i = 0; i < data.size(); ++i)
            if( dist(x,data[i]) < d+eps )
                ret.push_back(data[i]);
    }

    void importdata(const vector<dot>& indata){
        data = indata;
    }
    void exportdata(vector<dot>& ret){
        ret = data;
    }
	void testupdate(){
	}
};

#endif // MD_NAIVE_H_INCLUDED
