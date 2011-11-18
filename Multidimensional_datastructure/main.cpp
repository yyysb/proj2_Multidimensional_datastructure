#include <cstdio>
#include "MD_naive.h"

using namespace std;

md_naive p;
int main(){
    //freopen(".in","r",stdin);
    //freopen(".out","w",stdout);
    //p=q;
    p.insert(dot(1,1,"hehe"));
    p.insert(dot(1,1,"hehe2"));
    p.delbyname("hehe");
    vector<dot> t = p.exportdata();
    p.insert(dot(0,0));
    return 0;
}
