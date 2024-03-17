#include "mem_pool.h"

struct MyStruct {
    int d_[3];
};

int main(int, char **){
    using namespace common;

    MemPool<double> prim_pool(50);
    MemPool<MyStruct> struct_pool(50);

    for (auto i=0; i<50; i++){
        auto p_ret = prim_pool.allocate(i);
        auto s_ret = struct_pool.allocate(MyStruct{i, i+1, i+2});

        std::cout << "prim element : " << *p_ret << " allocated at : " << p_ret << std::endl;
        std::cout << "struct element : " << s_ret->d_[0] << "," << s_ret->d_[1] << "," << s_ret->d_[2] << " allocated at : " << s_ret << std::endl;

        if ( i % 5 == 0){
            std::cout << "Deallocating prim element : " << *p_ret << " allocated at : " << p_ret << std::endl;
            std::cout << "Deallocating struct element : " << s_ret->d_[0] << "," << s_ret->d_[1] << "," << s_ret->d_[2] << " allocated at : " << s_ret << std::endl;

            prim_pool.deallocate(p_ret);
            struct_pool.deallocate(s_ret);
        }
    }

    return 0;
}