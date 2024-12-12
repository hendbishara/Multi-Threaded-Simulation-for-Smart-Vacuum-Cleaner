
#include <boost/asio.hpp>
#include <list>
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <filesystem>
#include <dlfcn.h>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <latch>


#include "common/AlgorithmRegistrar.h"
#include "simulator/MySimulator.h"
#include "../common/AbstractAlgorithm.h"



using TIME_UNIT = std::chrono::milliseconds;
using time_point = std::chrono::time_point<std::chrono::system_clock>;


struct ArgumentsRead{
    std::string house_folder_path;
    bool house_folder_found = false;
    std::string alg_folder_path;
    bool alg_folder_found = false;
    int thread_number = 10;
    bool summary_only = false;

};


/**
 * @brief prints the thread id
 * @param std:thread::id
 * @return std::string
 */
std::string threadIdToString(const std::thread::id& id) {
    std::ostringstream oss;
    oss << id;
    return oss.str();
}

/**
 * @brief creates an error file with the given file name and the given message
 * @param std:string message, std::string file_name
 * @return void
 */
void logError(const std::string& message, const std::string& filename){
    //open error file
    std::string err_file_name = filename + ".error";
    std::ofstream errorFile(err_file_name, std::ios::out | std::ios::trunc);

    if(!errorFile){
        std::cout << "Failed to open error log file for :" + filename <<std::endl;
        return;
    }

    errorFile << message << std::endl;

    //close file
    errorFile.close();
}


class Task{
    std::unique_ptr<MySimulator> simulator;
    std::unique_ptr<AbstractAlgorithm> algo;
    bool create_file;
    std::string alg_name;
    std::string house_name;
    int final_score;
    bool error_occured = false;
    boost::asio::io_context& ioContext;
    std::latch* work_done;
    std::atomic<bool> guard;
    std::jthread my_thread;
    int result;
    int max_steps;
    int init_dirt;


    static void timerHandler(const boost::system::error_code& ec, Task& task, time_point start, pthread_t thread_handler){
        if (ec == boost::asio::error::operation_aborted){
            //timer was canceled
        }

        else if (!ec) {
            //Timer expired
            task.result = task.getMaxSteps() * 2 + task.getInitDirt() * 300 + 2000;
            pthread_cancel(thread_handler);
            task.getWorkDone()->count_down();
        }
        
    }
    
    
    public:

    Task(std::unique_ptr<MySimulator> s, std::unique_ptr<AbstractAlgorithm>  a, bool b, std::string a_name,std::string h_name, boost::asio::io_context& ioContext): simulator(std::move(s)),algo(std::move(a)),create_file(b),alg_name(a_name),house_name(h_name),ioContext(ioContext), result(-1){};
    
    std::latch* getWorkDone() const{
        return work_done;
    }
    int getInitDirt() const{
        return init_dirt;
    }
    int getMaxSteps() const{
        return max_steps;
    }

        
    /**
     * @brief initializes the reading of the house in the current simulator 
     * @param std:string path
     * @return void
     */
    void readHouse(std::string path){
        
        int res = simulator->readHouseFile(path);

        if(res == 1){
            //create error file - could not open file
            logError("Error: cound not open house file", house_name);
            error_occured = true;
            
            
        }

        if(res==2){
            //create error file - house does not folllow rules
            logError("Error: house does not follow required rules", house_name);
            error_occured = true;
            
        }

        max_steps = simulator->getMaxSteps(); 
        init_dirt = simulator->getInitDirt();
    }


    /**
     * @brief initializes the algorithm in the task's simulator 
     * 
     * @return void
     */
    void setAlgo(){
        simulator->setAlgorithm(*algo);
        simulator->setAlgName(alg_name);
    }

    /**
     * @brief run's the task and updates the score
     * 
     * @return void
     */
    void runThisTask(){
        simulator->create_out_file(create_file);
        simulator->run();
        final_score = simulator->getScore();
    }

    /**
     * @brief runs the tasks witha timer
     *
     * @return void
     */
    void run(){
        my_thread = std::jthread([this] {
            //set up boost timer
            boost::asio::steady_timer timer(ioContext, TIME_UNIT(max_steps));
            auto curr_time = std::chrono::system_clock::now();
            auto thread_handler = pthread_self();

            //Timer handler
            timer.async_wait([&](const boost::system::error_code& ec){
                timerHandler(ec, *this, curr_time, thread_handler);
            });

            //run algorithm on house
            this->runThisTask();

            //timer cancelation and result handling
            timer.cancel();
            bool expected = false;
            if(guard.compare_exchange_strong(expected,true)){
                result = final_score;
                work_done->count_down();
            }
        });
    }


    std::string getAlgName() const { return alg_name; }
    std::string getHouseName() const { return house_name; }
    int getFinalScore() const { return result; }
    bool getErr() const { return error_occured;}
    void setLatch(std::latch* wd){
        work_done = std::move(wd);
    }

};


auto algo_registrar = AlgorithmRegistrar::getAlgorithmRegistrar();




/**
 * @brief reads command line arguments and updates the arguments read struct
 * @param int args, char** argv, Argumentsread& args
 * @return void
 */

void readArgs(int argc, char** argv,ArgumentsRead& args){
    
    std::string args_list[] = {"-house_path=", "-algo_path=", "-summary_only", "-num_threads="};
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        //search for each prefix in args_list
        if (arg.find(args_list[0]) == 0) {
            args.house_folder_path = arg.substr(args_list[0].length());
            args.house_folder_found = true;
        } else if (arg.find(args_list[1]) == 0) {
            args.alg_folder_path = arg.substr(args_list[1].length());
            args.alg_folder_found = true;
        } else if (arg.find(args_list[2]) == 0) {
            args.summary_only = true;
        } else if (arg.find(args_list[3]) == 0) {
            args.thread_number = std::stoi(arg.substr(args_list[3].length()));
        }
    }

    //check if house folder and algorithm folder were assigned, if not assign current directory
    if (!args.house_folder_found) {
        args.house_folder_path = ".";
    }
    if (!args.alg_folder_found) {
        args.alg_folder_path = ".";
    }


}


/**
 * @brief given a directory path, finds all files wth the desired extension
 * @param std:string dirpath, std::string extension
 * @return std::vector<std::string> 
 */
std::vector<std::string> parseDir(std::string dirpath, std::string extension) {
  std::vector<std::string> files = {};
  if (!dirpath.empty()) {
    std::filesystem::path fpath{dirpath};
    if (!std::filesystem::is_directory(fpath)) {
      std::cout << fpath << " is not a directory" << std::endl;
      return {};
    }
    for (auto const &dir_entry : std::filesystem::directory_iterator{fpath}) {
        if (!dir_entry.is_directory() && dir_entry.path().has_extension() && dir_entry.path().extension() == extension) {
            files.push_back(dir_entry.path().string());
      }
    }
  }

  return files;
}


/**
 * @brief initializes a task for each algorim-house pair
 * @param houses,args,ioContex,housepath
 * @return void
 */
std::vector<std::unique_ptr<Task>> assignTasks(const std::vector<std::string>& houses,const ArgumentsRead& args, boost::asio::io_context& ioContext,std::string house_path){
    std::unique_ptr<AbstractAlgorithm> ptr;
    std::vector<std::unique_ptr<Task>> tasks;

    for (const std::string& h : houses){
        for (auto alg : AlgorithmRegistrar::getAlgorithmRegistrar()){
            try{
                ptr = alg.create();
                if (!ptr){
                    
                    logError("Error: factory didn't create a valid algorithm", alg.name());
                    continue;
                }
            }

            catch (...){
                //Add to error file
                //out_error << "algo failed to create" << std::endl;
            }
            
            const std::string extension = ".house";
            size_t extlen = extension.length();
            std::string house_name = h.substr(0,h.length()-extlen);
            if(house_name.find(house_path) == 0){
                house_name = house_name.substr(house_path.length()+1);
            }
            
            //std::unique_ptr<MySimulator> sim = std::make_unique<MySimulator>();
            std::unique_ptr<Task> t = std::make_unique<Task>(std::make_unique<MySimulator>(),std::move(ptr),args.summary_only,alg.name(),house_name, ioContext); //check make unique for abstract algorithm
            
            t->readHouse(h);
            
            if(!(t->getErr())){
                t->setAlgo();
                tasks.emplace_back(std::move(t));
            }
        
            
            
        }
    }


    return tasks;

}


/**
 * @brief properly opens the algorithms library
 * @param alg_files
 * @return std::vector<void*>
 */
std::vector<void*> openAlgs(std::vector<std::string>& alg_files){
    std::vector<void*> v;
    for (const std::string lib : alg_files){
        size_t count_before = AlgorithmRegistrar::getAlgorithmRegistrar().count();
        void *library_handle = dlopen(lib.c_str(), RTLD_LAZY);

        if (!library_handle)
            {
                //create .error file
                const std::string extension = ".so";
                size_t extlen = extension.length();
                std::string name = lib.substr(0,lib.length()-extlen);
                logError("Error loading algorith library!", name);
                
            }
        else if(AlgorithmRegistrar::getAlgorithmRegistrar().count() == count_before){
            //create .error file
                const std::string extension = ".so";
                size_t extlen = extension.length();
                std::string name = lib.substr(0,lib.length()-extlen);
                logError("Error: Number of items in registrar could not be increased after opening library", name);
                
        }

        else{
            v.push_back(library_handle);
        }
        

    } 

    return v;

}


/**
 * @brief properly closes the opened algorithm libraries
 * @param std::vector<void*>&
 * @return void
 */
void clean_algo_libs(std::vector<void*>& v) {
    
    for (auto& lib_p : v) {
        if (lib_p) { // Ensure the pointer is not null
            
            // Attempt to close the library
            if (dlclose(lib_p) != 0) { // Check if dlclose fails
                const char* error = dlerror();
                if (error != nullptr) {
                    std::cerr << "Error closing library: " << error << std::endl;
                } else {
                    std::cerr << "Unknown error occurred while closing library." << std::endl;
                }
            } 
            lib_p = nullptr; // Set pointer to nullptr after closing
        } else {
            std::cerr << "Encountered null library handle." << std::endl;
        }
    }
}


/**
 * @brief writes the scores of each task in an csv file
 * @param tasks, filename
 * @return void
 */
void writeTasksToCSV(const std::vector<std::unique_ptr<Task>>& tasks, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    std::set<std::string> house_names;
    std::set<std::string> algo_names;

    // Collect unique house names and algorithm names
    for (const auto& task : tasks) {
        house_names.insert(task->getHouseName());
        algo_names.insert(task->getAlgName());
    }

    // Convert sets to vectors for easier indexing
    std::vector<std::string> house_names_vec(house_names.begin(), house_names.end());
    std::vector<std::string> algo_names_vec(algo_names.begin(), algo_names.end());

    // Write header
    file << "Algorithm";
    
    for (const auto& house_name : house_names_vec) {
        file << "," << house_name;
    }
    file << "\n";

    // Map to store results
    std::map<std::string, std::map<std::string, int>> results;

    // Organize results into the map
    for (const auto& task : tasks) {
        results[task->getAlgName()][task->getHouseName()] = task->getFinalScore();
    }

    // Write rows for each algorithm
    for (const auto& algo_name : algo_names_vec) {
        file << algo_name;
        for (const auto& house_name : house_names_vec) {
            auto algo_it = results.find(algo_name);
            if (algo_it != results.end()) {
                auto house_it = algo_it->second.find(house_name);
                if (house_it != algo_it->second.end()) {
                    file << "," << house_it->second;
                } else {
                    file << ",0";  // Default value if no score available
                }
            } else {
                file << ",0";  // Default value if no score available
            }
        }
        file << "\n";
    }

    file.close();
}


/**
 * @brief creates and manages a pool of threads than run the tasks
 * @param tasks, ioContex, max_threads
 * @return std::vector<std::unique_ptr<Task>>
 */

std::vector<std::unique_ptr<Task>> run_tasks(std::vector<std::unique_ptr<Task>>& tasks, boost::asio::io_context& ioContext, int max_threads){
    std::latch work_done(tasks.size());
    std::vector<std::jthread> thread_pool;
    std::mutex mutex;
    std::mutex gard;
    std::vector<std::unique_ptr<Task>> saved_tasks;

    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        (*it)->setLatch(&work_done);
    }

    for(size_t i=0; i< max_threads; i++){
        thread_pool.emplace_back([&ioContext, &work_done, &tasks,&mutex, &gard, &saved_tasks](){
            while (true){
                std::unique_ptr<Task> task;
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    if (tasks.empty()){
                        break;
                    } 
                    task = std::move(tasks.back());
                    tasks.pop_back();
                }
                task -> run();
                {
                    std::lock_guard<std::mutex> lock(gard);
                    saved_tasks.emplace_back(std::move(task));
                }
                
                


                
            }
        });
    }
    work_done.wait();
    for (auto& tr: thread_pool){
        if(tr.joinable()) tr.join();
    }

    return saved_tasks;

}



int main(int argc, char** argv){
    struct ArgumentsRead args;

    boost::asio::io_context ioContext;
    auto workGuard = boost::asio::make_work_guard(ioContext);
    std::jthread ioThread([&ioContext]() {
        ioContext.run();
    });


    //read arguments
    readArgs(argc,argv,args);
    
    //get house files
    auto house_files = parseDir(args.house_folder_path,".house");

    //open algorithms libraries 
    auto alg_files = parseDir(args.alg_folder_path, ".so");


    //check if files were found
    if(house_files.size() == 0){
        std::cout << "No House Files Found in directory!" << std::endl;
        ioContext.stop();

        return 1;
    }


    if(alg_files.size() == 0){
        std::cout << "No algorithm Files Found in directory!" << std::endl;
        ioContext.stop();

        return 1;
    }


    //get algorithms from registrar
    std::vector<void*> opened_algorithms = openAlgs(alg_files);

    //create simulator tasks to run house-algo pairs
    std::vector<std::unique_ptr<Task>> tasks = assignTasks(house_files,args,ioContext,args.house_folder_path);

    
    //run simulators
    //add multi-threading 
    std::vector<std::unique_ptr<Task>> saved_tasks = run_tasks(tasks,ioContext, args.thread_number);

    ioContext.stop();

    //get scores file
    writeTasksToCSV(saved_tasks, "summary.csv");

    saved_tasks.clear();

    AlgorithmRegistrar::getAlgorithmRegistrar().clear();
    clean_algo_libs(opened_algorithms);
    
    return 0;

}