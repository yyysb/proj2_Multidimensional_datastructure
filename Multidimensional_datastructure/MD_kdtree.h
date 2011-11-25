#ifndef MD_KDTREE_H_INCLUDED
#define MD_KDTREE_H_INCLUDED

#include "MD_base.h"
#include<map>

inline bool inrange(const double& x,const double& y,const double& z){
    return x-eps<z && z<y+eps;
}

inline bool intersect(int u1,int v1,int u2,int v2, const dot& c, const double& r){
    return instrictcontain(c,r,u1,v1) || instrictcontain(c,r,u1,v2)
        || instrictcontain(c,r,u2,v1) || instrictcontain(c,r,u2,v2)
        || ( inrange(u1,u2,c.data[0]) && inrange(v1,v2,c.data[1]))
        || ( inrange(u1,u2,c.data[0]) && (abs(c.data[1]-v1+0.0)<r+eps || abs(c.data[1]-v2+0.0)<r+eps))
        || ( inrange(v1,v2,c.data[0]) && (abs(c.data[0]-u1+0.0)<r+eps || abs(c.data[0]-u2+0.0)<r+eps));
}
struct kdt_node{
    dot value;
    int dis;
    kdt_node *lc, *rc;
    kdt_node():value(),dis(-1),lc(NULL),rc(NULL){}
    kdt_node(const dot& v, const int& d, kdt_node *l, kdt_node *r)
        :value(v),dis(d),lc(l),rc(r){}
};

class md_kdtree : public md_base{
private:
    kdt_node *root;
    map<string, kdt_node*> namemap;
    typedef map<string, kdt_node*>::iterator mapiter;

    kdt_node* kdtfindbypos(const dot& x){
        kdt_node *cur = root->lc;
        for(int dc=1; cur && !x.equalpos(cur->value); dc^=1)
            if(x.data[dc] < cur->value.data[dc])
                cur = cur->lc;
            else cur = cur->rc;
        return cur;
    }
    void kdtdel(kdt_node* tg){
    }

    void kdtsearch(kdt_node* cur, const dot& c, const double& d,
                   int u1,int v1,int u2,int v2,
                    vector<dot>& ret)const{
        if(!cur)return;
        if(!intersect(u1,v1,u2,v2,c,d)) return;
        if(instrictcontain(c,d,cur->value)) ret.push_back(cur->value);
        if(cur->dis==0){
            kdtsearch(cur->lc,c,d,u1,v1,cur->value.data[0]-1,v2,ret);
            kdtsearch(cur->rc,c,d,cur->value.data[0],v1,u2,v2,ret);
        } else{
            kdtsearch(cur->lc,c,d,u1,v1,u2,cur->value.data[1]-1,ret);
            kdtsearch(cur->rc,c,d,u1,cur->value.data[0],u2,v2,ret);
        }
    }
    void kdttravel(kdt_node* cur, vector<dot>& ret){
        if(cur){
            kdttravel(cur->lc,ret);
            ret.push_back(cur->value);
            kdttravel(cur->rc,ret);
        }
    }
public:
    md_kdtree(): root( new kdt_node( dot(oo,oo), 0, NULL, NULL) )
        {}
    ~md_kdtree(){
        //delete root;
    }
    void insert(const dot& x){
        int dc =1;
        kdt_node **cur = &root->lc;
        for(; *cur; dc^=1)
            if(x.data[dc] < (*cur)->value.data[dc])
                cur = &(*cur)->lc;
            else cur = &(*cur)->rc;
        *cur = new kdt_node(x, dc, NULL, NULL);

        namemap.insert(make_pair(x.name, *cur));
    }

    void delbypos(const dot& x){
        kdt_node* target = kdtfindbypos(x);
        if(target) kdtdel(target);
    }
    void delbyname(const string& s){
        mapiter target = namemap.find(s);
        if(target!=namemap.end()) kdtdel(target->second);
    }
    dot findbypos(const dot& x){
        kdt_node* target = kdtfindbypos(x);
        if(target) return target->value;
            else return nulldot;
    }
   dot findbyname(const string& s){
        mapiter target = namemap.find(s);
        if(target!=namemap.end()) return target->second->value;
            else return nulldot;
    }
    void search(const dot& x, const double& d, vector<dot>& ret){
        kdtsearch(root,x,d,-oo,-oo,oo,oo,ret);
    }

    void importdata(const vector<dot>& indata){
        //data = indata;
    }
    void exportdata(vector<dot>& ret){
        kdttravel(root->lc, ret);
    }
	void testupdate(){
	}
};

#endif // MD_KDTREE_H_INCLUDED
