#include <string.h>
#include <iostream>
#include <filesystem>
#include <dlfcn.h>
#include "../common/AlgorithmRegistrar.h"

namespace fs = std::filesystem;

void get_algo_libs(const char* libs_dir,std::vector<void *>& v)
{
    for (const auto& dirent : fs::directory_iterator(fs::path(libs_dir)))
    {
        if (dirent.path().extension() == ".so") 
        {
            void *library_handle = dlopen(dirent.path().c_str(), RTLD_LAZY);
            if (!library_handle)
            {
                std::cout << "error loading library: " << dlerror() << std::endl;
                exit(1);
            }

            v.push_back(library_handle);
        }
    }
}


void clean_algo_libs(std::vector<void *>& v)
{
    for (auto& lib_p : v)
    {
        dlclose(lib_p);
    }
}



int main(int argc, char** argv) 
{
    std::vector<void *> algo_libs;
    
    get_algo_libs(argv[1],algo_libs);

    std::cout << "Running on " << AlgorithmRegistrar::getAlgorithmRegistrar().count() << " algorithms." << std::endl;
    for(const auto& algo: AlgorithmRegistrar::getAlgorithmRegistrar()) {
        std::cout << algo.name() << std::endl;
        auto algorithm = algo.create();
        std::cout << algo.name() << ": " << static_cast<int>(algorithm->nextStep()) << std::endl;
    }

    AlgorithmRegistrar::getAlgorithmRegistrar().clear();
    clean_algo_libs(algo_libs);
    return 0;
}
