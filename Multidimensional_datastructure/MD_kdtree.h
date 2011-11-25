#ifndef MD_KDTREE_H_INCLUDED
#define MD_KDTREE_H_INCLUDED

#include "MD_base.h"
#include<map>
#include<algorithm>

inline bool inrange(const double& x,const double& y,const double& z){
    return x-eps<z && z<y+eps;
}

inline bool intersect(int u1,int v1,int u2,int v2, const dot& c, const double& r){
    return instrictcontain(c,r,u1,v1) || instrictcontain(c,r,u1,v2)
        || instrictcontain(c,r,u2,v1) || instrictcontain(c,r,u2,v2)
        || ( inrange(u1-r,u2+r,c[0]) && inrange(v1,v2,c[1]))
        || ( inrange(v1-r,v2+r,c[1]) && inrange(u1,u2,c[0]));
}
inline bool dotless0(const dot* const &x, const dot* const &y){
    return x->data[0]<y->data[0];
}
inline bool dotless1(const dot* const &x, const dot* const &y){
    return x->data[1]<y->data[1];
}

struct kdt_node{
    dot value;
    int dis;
    kdt_node *lc, *rc;
    bool alive;
    kdt_node():value(),dis(-1),lc(NULL),rc(NULL),alive(true){}
    kdt_node(const dot& v, const int& d, kdt_node *l, kdt_node *r)
        :value(v),dis(d),lc(l),rc(r),alive(true){}
};

class md_kdtree : public md_base{
private:
    kdt_node *root;
    multimap<string, kdt_node*> namemap;
    typedef multimap<string, kdt_node*>::iterator mapiter;
    int sz,cap;

    kdt_node* kdtfindbypos(const dot& x){
        kdt_node *cur = root->lc;
        for(int dc=0; cur && !x.equalpos(cur->value); dc^=1)
            if(x[dc] < cur->value[dc])
                cur = cur->lc;
            else cur = cur->rc;
        return cur;
    }
    bool kdtdel(kdt_node* tg){
        if(!tg || tg->alive==false) return false;
        tg->alive = false;
        --sz;
        if(sz*2<cap){
            vector<dot> tmp;
            exportdata(tmp);
            clear();
            kdtbuild(&root->lc,0,tmp);
            return true;
        } else return false;
    }
    void kdtsearch(kdt_node* cur, const dot& c, const double& d,
                   int u1,int v1,int u2,int v2,
                    vector<dot>& ret)const{
        if(!cur)return;
        if(!intersect(u1,v1,u2,v2,c,d)) return;
        if(cur->alive && instrictcontain(c,d,cur->value))
            ret.push_back(cur->value);
        if(cur->dis==0){
            kdtsearch(cur->lc,c,d,u1,v1,cur->value[0]-1,v2,ret);
            kdtsearch(cur->rc,c,d,cur->value[0],v1,u2,v2,ret);
        } else{
            kdtsearch(cur->lc,c,d,u1,v1,u2,cur->value[1]-1,ret);
            kdtsearch(cur->rc,c,d,u1,cur->value[1],u2,v2,ret);
        }
    }
    void kdttravel(kdt_node* cur, vector<dot>& ret){
        if(cur){
            kdttravel(cur->lc,ret);
            if(cur->alive)ret.push_back(cur->value);
            kdttravel(cur->rc,ret);
        }
    }
    void build(kdt_node** cur, int dc, const dot*** arr,
               const dot** tmp, int x, int y){
        if(x>=y){ *cur = 0; return;}
        int z=x+(y-x)/2,z1=z,z2=z, w=arr[dc][z]->data[dc];
        while(z1>x && arr[dc][z1-1]->data[dc]==w) --z1;
        while(z2+1<y && arr[dc][z2-1]->data[dc]==w) ++z2;
        if(z-z1<z2-z || (z2>0 && arr[dc][z2-1]->data[dc]==w)) z=z1; else z=z2;

        int p=x, q=z+1, w1=arr[dc][z]->data[dc], w2=arr[dc][z]->data[dc^1];

        *cur = new kdt_node(*(arr[dc][z]), dc, NULL, NULL);
        ++cap;++sz;
        namemap.insert(make_pair(arr[dc][z]->name, *cur));

        memcpy(tmp, arr[dc^1]+x, sizeof(arr[dc][0])*(y-x));
        for(int i=0; i<y-x; ++i)
            if(tmp[i]->data[dc] < w1)
                arr[dc^1][p++] = tmp[i];
            else if(!(tmp[i]->data[dc]==w1 && tmp[i]->data[dc^1]==w2))
                    arr[dc^1][q++] = tmp[i];

        if(p!=z || q!=y){
            puts("err");
            printf("%d %d\n",w1,w2);
            for(int i=x;i<y;++i) printf("(%d %d)",tmp[i]->data[0],tmp[i]->data[1]);puts("");
            for(int i=x;i<y;++i) printf("(%d %d)",arr[0][i]->data[0],arr[0][i]->data[1]);puts("");
            for(int i=x;i<y;++i) printf("(%d %d)",arr[1][i]->data[0],arr[1][i]->data[1]);puts("");
            fflush(stdout);
        }
        build(&(*cur)->lc, dc^1, arr, tmp, x, z);
        build(&(*cur)->rc, dc^1, arr, tmp, z+1, y);
    }
    void kdtbuild(kdt_node** cur, int dc,const vector<dot>& ind){
        const dot* *arrx = new const dot* [ind.size()];
        const dot* *arry = new const dot* [ind.size()];
        const dot* *tmp  = new const dot* [ind.size()];
        const dot* *arr[2] = {arrx,arry};
        for(int i = 0; i<ind.size(); ++i)
            arr[0][i] = arr[1][i] = &(ind[i]);
        sort(arr[0],arr[0]+ind.size(),dotless0);
        sort(arr[1],arr[1]+ind.size(),dotless1);

        build(cur,dc,arr,tmp,0,ind.size());

        delete [] arrx;
        delete [] arry;
        delete [] tmp;
    }
    void kdtfree(kdt_node* cur){
        if(cur){
            kdtfree(cur->lc);
            kdtfree(cur->rc);
            delete cur;
        }
    }
public:
    md_kdtree(): root( new kdt_node( dot(oo,oo), 0, NULL, NULL) ),
            namemap(),sz(0),cap(0)
        {}
    ~md_kdtree(){
        clear();
        delete root;
    }
    int size(){
        return sz;
    }
    void clear(){
        namemap.clear();
        sz = cap = 0;
        kdtfree(root->lc);
    }

    void insert(const dot& x){
        ++sz;
        int dc =0;
        kdt_node **cur = &root->lc;
        for(; *cur; dc^=1){
            if(x.equalpos((*cur)->value)){
                (*cur)->alive = true;
                namemap.insert(make_pair(x.name, *cur));
                return;
            }
            if(x[dc] < (*cur)->value[dc])
                cur = &(*cur)->lc;
            else cur = &(*cur)->rc;
        }
        *cur = new kdt_node(x, dc, NULL, NULL);
        ++cap;
        namemap.insert(make_pair(x.name, *cur));
    }

    void delbypos(const dot& x){
        kdt_node* target = kdtfindbypos(x);
        if(target)//{
            //int fsz=sz;
            kdtdel(target);
            //if(sz+1!=fsz)puts("err");
        //}
    }
    void delbyname(const string& s){
        mapiter x,y;
        while(1){
            x = namemap.lower_bound(s);
            y = namemap.upper_bound(s);
            bool flag=true;
            for(mapiter it=x; it!=y; ++it)
                if(kdtdel(it->second)){
                    flag=false;break;
                }
            if(flag)break;
        }
        namemap.erase(x,y);
    }

    dot findbypos(const dot& x){
        kdt_node* target = kdtfindbypos(x);
        if(target && target->alive) return target->value;
            else return nulldot;
    }
    void findbyname(const string& s, vector<dot>& ret){
        mapiter x = namemap.lower_bound(s),
                y = namemap.upper_bound(s);
        for(mapiter it=x; it!=y; ++it)
            if(it->second->alive){
                ret.push_back(it->second->value);
            }
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
};

#endif // MD_KDTREE_H_INCLUDED
