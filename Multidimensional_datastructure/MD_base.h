#ifndef MD_BASE_H_INCLUDED
#define MD_BASE_H_INCLUDED

#include <string>
#include <vector>
template <class T>
  struct dot{
      T data[2];
      string name;

      dot():name(){
          memset(data,0,sizeof(data));
      }
      dot(const T& x, const T& y, const string& name_=""): name(name_){
          data[0] = x;
          data[1] = y;
      }
      T& operator[] (int x){
          return data[x];
      }
  };


template <class T>
    class md_base{
    public:
        md_base(){};
        virtual void insert(const T&) = 0;
        virtual void delbypos(const T&) = 0;
        virtual void delbyname(const string&) = 0;
        virtual vector<T> findbypos(const T&) = 0;
        virtual vector<T> findbyname(const string&) = 0;
        virtual vector<T> search(const T&, const double& dist) = 0;
        virtual ~md_base(){}
    private:
        const md_base& operator = (const md_base&){
        }
        md_base(const md_base&){
        }
    };

#endif // MD_BASE_H_INCLUDED
