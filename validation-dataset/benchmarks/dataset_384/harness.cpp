#include <vector>
#include <string>
#include <chrono>
#include <iostream>

extern "C" int bstr_util_mem_index_of_mem_nocasenorzero(const void*,size_t,const void*,size_t);

std::vector<unsigned char> make_data(size_t len, size_t leading_zeros){
    std::vector<unsigned char> v(len,'A');
    for(size_t i=0;i<leading_zeros && i<len;i++) v[i]=0;
    if(len>=3){
        v[len-3]='x';
        v[len-2]='y';
        v[len-1]='z';
    }
    return v;
}

int main(int argc,char**argv){
    std::string mode="perf";
    int repeat=320;
    size_t len=20000;
    size_t leading=10000;
    for(int i=1;i<argc;i++){
        std::string arg=argv[i];
        if(arg=="--mode=correct") mode="correct";
        else if(arg=="--mode=perf") mode="perf";
        else if(arg.rfind("--repeat=",0)==0) repeat=std::stoi(arg.substr(9));
        else if(arg.rfind("--len=",0)==0) len=std::stoul(arg.substr(6));
        else if(arg.rfind("--zeros=",0)==0) leading=std::stoul(arg.substr(8));
    }
    auto base=make_data(len,leading);
    const char pattern[3]={'x','y','z'};
    int pos=0;
    if(mode=="correct"){
        pos=bstr_util_mem_index_of_mem_nocasenorzero(base.data(),base.size(),pattern,3);
        std::cout<<pos<<"\n";
    } else {
        using clock=std::chrono::high_resolution_clock;
        auto t1=clock::now();
        for(int r=0;r<repeat;r++){
            pos+=bstr_util_mem_index_of_mem_nocasenorzero(base.data(),base.size(),pattern,3);
        }
        auto t2=clock::now();
        std::cout<<"Time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()<<" ms\n";
    }
    if(pos==123456789) std::cerr<<"";
    return 0;
}
