#include <iostream>
#include <vector>
#include <unordered_map>
#include <thread>
#include <utility>      
#include <unistd.h>
#define MIN_PID 300
#define MAX_PID 5000

using namespace std;

class Process_Manager {
private:
    unordered_map<int, int>* pids;
public:    
    // allocate space right when the process manager is created
    Process_Manager() {
        allocate_map();
    } 
    // Initializes a data structure for representing pids; returns -1 if unsuccessful and 1 if successful
    int allocate_map() {
        try {
            pids = new unordered_map<int,int>(); 
            pids->reserve(MAX_PID - MIN_PID + 1); // make sure system has the minimum requrements for storing the pids
            for (int i = MIN_PID; i <= MAX_PID; i++) { // pid values are from 300 - 5000
                pids->insert(make_pair(i, 0)); // set initial values to 0 - available pids
            }
            return 1; 
        } catch (...) {
            cout << "error couldn't allocate map" << endl;
            return -1;
        }
    }
    // Allocates and returns a pid; returns -1 if if unable to allocate a pid (all pids are in use)
    int allocate_pid() {
        // search for available pids within the MIN/MAX range
        for (int i = MIN_PID; i <= MAX_PID; i++) {
            auto test = pids->find(i); 
            // check if pid is available 
            if(test->second == 0) {
                test->second = 1; // update pid value to unavailable: 1 
                return test->first; // return available pid to be used 
            }   
        }
        return -1; // no pids are available
    }
    // released pid becomes available for use
    void release_pid(int pid) {
        //make sure pid is in range
        if(MIN_PID <= pid && pid <= MAX_PID) {
            auto test = pids->find(pid);          
            test->second = 0; // make pid available
        } else { 
            cout << "Invalid PID" << endl;
        }
    }
};

Process_Manager proc_manager;

void pseudoProcess() {
    int pid = proc_manager.allocate_pid();

    if(pid == -1) {
        cout << "\nno PIDs are available" << endl;
    } else {
        cout << "\nAllocated PID: " << pid << " to a process" << endl;
        sleep(rand() % 100 + 1); // sleeps upto 100 seconds
        proc_manager.release_pid(pid);
        cout << "\nReleased PID: " << pid << " from a process" << endl;
    } 
}

int main(){
	int thread_amount = 50;

	//Add threads into the thread pool
	vector<thread> thread_pool;
	for(int i = 0; i < thread_amount; i++){
    	thread myThread = thread([](){ 
    	    pseudoProcess(); 
    	});
    	thread_pool.push_back(move(myThread));
    }
	//join all the threads in the thread pool	
	for(auto& myThread : thread_pool){
    	myThread.join();
	}
    return 0;
} 