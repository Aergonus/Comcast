#include <vector>
#include <string>
class A{
      public:
        std::vector<std::string>& GetVec(){return vec;}
      private:
        std::vector<std::string> vec;
    };

class B{
      public:
        //constructor
        B(A& a) : vec_ref(a.GetVec()){}
      private:
        std::vector<std::string>& vec_ref;
     };

int main(){
  A a;
  B b(a);
}
