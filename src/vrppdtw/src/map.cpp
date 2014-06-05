// constructing maps
#include <iostream>
#include <map>

bool fncomp (char lhs, char rhs) {return lhs<rhs;}

struct classcomp {
          bool operator() (const char& lhs, const char& rhs) const
                    {return lhs<rhs;}
};

int main ()
{
          std::map<char,int> first;

            first['a']=10;
              first['b']=30;
                first['c']=50;
                  first['d']=70;

                    std::map<char,int> second (first.begin(),first.end());

                      std::map<char,int> third (second);

                        std::map<char,int,classcomp> fourth;  p               // class as Compare

                          bool(*fn_pt)(char,char) = fncomp;
                            std::map<char,int,bool(*)(char,char)> fifth (fn_pt); // function pointer as Compare

                              return 0;
}
