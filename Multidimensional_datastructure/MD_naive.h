#ifndef MD_NAIVE_H_INCLUDED
#define MD_NAIVE_H_INCLUDED

#include "MD_base.h"

template <class T>
    class md_naive : public md_base<T>{
    public:
        md_naive(){};
        void insert(const T&){}
        void delbypos(const T&){}
        void delbyname(const string&){}
        vector<T> findbypos(const T&){}
        vector<T> findbyname(const string&){}
        vector<T> search(const T&, const double& dist){};
        virtual ~md_naive(){};
    };

#endif // MD_NAIVE_H_INCLUDED
