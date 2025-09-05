#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

class Process{
    string name;
    int arrivalTime;
    int burstTime;
    public:
    Process(string n="",int a=0,int b=0){
        name=n;
        arrivalTime=a;
        burstTime=b;
    }
    
    void decreaseBurstTime(){
        burstTime--;
    }

    friend class RR;
};

class RR{
    queue<Process> readyQ;
    vector<Process> tasks;

    public:

    void printQueue(){
        cout<<"Read queue : "<<endl;
        queue<Process> temp=readyQ;
        int i=1;
        if(temp.empty())cout<<"No process."<<endl;
        while(!temp.empty()){
            cout.width(10);
            cout<<i<<".\t"<<temp.front().name;
            cout.width(5);
            cout<<temp.front().burstTime<<endl;
            temp.pop();
            i++;
        }
        cout<<endl;
    }

    void sortTasks(){
        sort(tasks.begin(),tasks.end(),[](const Process &a,const Process &b){
            return a.arrivalTime < b.arrivalTime;
        });
    }
    


    void clearProcesses(){
        tasks.clear();
    }

    void addProcess(string name,int arrival,int burst){
        Process p(name,arrival,burst);
        tasks.push_back(p);
    }

    void printProcesses(){
        cout.width(10);
        cout<<"Name";
        cout.width(14);
        cout<<"Arrival time";
        cout.width(12);
        cout<<"Burst time";
        cout<<endl;

        for(auto process:tasks){
            cout.width(10);
            cout<<process.name;
            cout.width(10);
            cout<<process.arrivalTime;
            cout.width(10);
            cout<<process.burstTime;
            cout<<endl;
        }
    }

    void executeTasks(){
        sortTasks();
        auto it=tasks.begin();
        int time=0;
        int CPUTime=2;
        bool isChanged=true;

        while(!readyQ.empty() || it!=tasks.end()){
            while(it!=tasks.end() && (*it).arrivalTime==time){
                readyQ.push(*it);
                it++;
                isChanged=true;
            }

            if(!CPUTime){
                if(readyQ.front().burstTime)    
                    readyQ.push(readyQ.front());
                readyQ.pop();
                isChanged=true;
                CPUTime=2;
            }

            if(!readyQ.front().burstTime){
                readyQ.pop();
                isChanged=true;
                CPUTime=2;
            }

            cout<<"Time :\t "<<time<<"------------------"<<endl;
            if(isChanged) {
                cout<<"Running : "<<readyQ.front().name<<"\t"<<readyQ.front().burstTime<<endl;
                printQueue();
                isChanged=false;
            } 

            time++;
            readyQ.front().burstTime--;
            CPUTime--;
        }
    }
   



};

int main(){
    
    RR r;
    r.addProcess("P1",2,5);
    r.addProcess("P2",0,3);
    r.addProcess("P3",0,5);
    r.addProcess("P4",0,4);
    r.addProcess("P5",0,8);
    r.addProcess("P6",4,4);
    r.printProcesses();

    // r.sortTasks();
    // r.printProcesses();

    r.executeTasks();

    return 0;
}