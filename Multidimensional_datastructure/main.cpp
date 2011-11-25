#include <cstdio>
#include "MD_kdtree.h"
#include "MD_naive.h"
using namespace std;

md_kdtree p,q;
md_naive u;
int main(){
    //freopen(".in","r",stdin);
    //freopen(".out","w",stdout);
    //p=q;
    p.insert(dot(1,1,"hehe"));
    p.insert(dot(1,1,"hehe2"));
    p.delbyname("hehe");
    //vector<dot> t = p.exportdata();
    p.insert(dot(0,0));
    return 0;
}
