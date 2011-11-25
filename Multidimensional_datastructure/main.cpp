#include <cstdio>
#include <map>
#include <iostream>
#include "MD_kdtree.h"
#include "MD_naive.h"
using namespace std;

md_kdtree kdt;
md_naive sim;

char buf[100];

bool chkvec(const vector<dot>& u, const vector<dot>& v){
    if(u.size()!=v.size()) return false;
    for(int i=0;i<u.size();++i)
        if(find(v.begin(),v.end(),u[i])==v.end())
            return false;
    return true;
}

void test(md_base& ds){
    FILE* tlog = fopen("test.log","w");
    int n = 10000;
    for(int i = 0; i < n; ++i){
        int t = rand()%15;
        if(t < 4){
            int x = rand()%1000;
            int y = rand()%1000;
            int r = rand()%1000000;
            int s = rand()%1000000;
            sprintf(buf,"%d.%d",r,s);
            dot tmp(x,y,buf);

            dot ret = sim.findbypos(tmp);
            if(ret.equalpos(nulldot)){
                sim.insert(tmp);
                ds.insert(tmp);
            }
            fprintf(tlog,"insert %d %d %s\n",x,y,buf);
        } else
        if(t < 5){
            int x = rand()%1000;
            int y = rand()%1000;
            dot tmp(x,y);
            if(!sim.findbypos(tmp).equalfull(ds.findbypos(tmp)))
                printf("wrong pos rand  %d %d\n",x,y);
            fprintf(tlog,"query pos %d %d\n",x,y);
        } else
        if(t < 8){
            dot tmp = sim.getrand();
            if(!ds.findbypos(tmp).equalfull(tmp))
                printf("wrong pos  %d %d\n",tmp[0],tmp[1]);
                //else puts("good");
            fprintf(tlog,"query pos %d %d\n",tmp[0],tmp[1]);
        } else
        if(t<10){
            dot tmp = sim.getrand();
            vector<dot> rt1,rt2;
            ds.findbyname(tmp.name,rt2);
            sim.findbyname(tmp.name,rt1);
            if(!chkvec(rt1,rt2))
                printf("wrong name  %d %d\n",tmp[0],tmp[1]);
            fprintf(tlog,"query name %s\n",tmp.name.c_str());
        } else
        if(t<10){
            int x = rand()%1000;
            int y = rand()%1000;
            int r = rand()%1000;
            dot d(x,y);
            vector<dot> rt1,rt2;
            sim.search(d,r,rt1);
            ds.search(d,r,rt2);
            if(!chkvec(rt1,rt2))
                printf("wrong search  %d %d %d\n",x,y,r);
            fprintf(tlog,"search %d %d %d\n",x,y,r);
        } else
        if(t<14){
            dot tmp = sim.getrand();
            sim.delbypos(tmp);
            ds.delbypos(tmp);
            fprintf(tlog,"del pos %d %d\n",tmp[0],tmp[1]);
        } else{
            dot tmp = sim.getrand();
            sim.delbyname(tmp.name);
            ds.delbyname(tmp.name);
            fprintf(tlog,"del name %s\n",tmp.name.c_str());
        }
        if(i%50==0)printf("%d\n",i);
        if(sim.size()!=ds.size()){
            puts("wrong size");
            vector<dot> rt1,rt2;
            sim.exportdata(rt1);
            ds.exportdata(rt2);
            t=20;
        }
        fflush(tlog);
    }
    fclose(tlog);
}
int main(){
    test(kdt);
    return 0;
}
