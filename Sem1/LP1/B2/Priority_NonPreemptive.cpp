#include <iostream>
#include<vector>
#include<queue>
#include<chrono>
#include<algorithm>
#include <thread>
#include<chrono>
using namespace std;

class Process{
    string name;
    int arrivalTime;
    int burstTime;
    int turnaroundTime;
    int waitTime;
    int remainingTime;
    int priority;
    bool isRunning;
    public:
    Process(string n="",int a=0,int b=0,int p=0,int t=0,int w=0){
        name=n;
        arrivalTime=a;
        burstTime=b;
        priority=p;
        isRunning=false;
        turnaroundTime=t;
        waitTime=w;
        remainingTime=b;  ///when object is created remaing execution is same as burst time
    }
    void calculateWait(){
        waitTime=turnaroundTime-burstTime;
    }
    friend class PriorityNonPreemptive;
};


class PriorityNonPreemptive{
    vector<Process> tasks;

    // public:
    struct ComparePriority {
        bool operator()(const Process *a, const Process *b) {
            return a->priority > b->priority && !a->isRunning;
        }
    };
    priority_queue<Process*,vector<Process*>,ComparePriority> readyQ;

    void sortTasksArrivalTime(vector<Process>& tasks){
        sort(tasks.begin(), tasks.end(), [](const Process &a, const Process &b) {
            return a.arrivalTime < b.arrivalTime;
        });
    }

    void displayCurrState(){
        if(readyQ.empty())  {
            cout<<"Running : No Process Running"<<endl;
            return;
        }
        priority_queue<Process*,vector<Process*>,ComparePriority> temp=readyQ;
        cout<<"Running : "<< temp.top()->name<<"\t"<<temp.top()->priority<<"\t"<<temp.top()->remainingTime<<endl;
        cout<<"Read queue : "<<endl;
        int i=1;
        while(!temp.empty()){
            cout.width(10);
            cout<<"\t"<<temp.top()->name;
            cout.width(5);
            cout<<temp.top()->priority;
            cout.width(5);
            cout<<temp.top()->remainingTime<<endl;
            temp.pop();
            i++;
        }
        cout<<endl;
    }

    public:
    
    void clearProcesses(){
        tasks.clear();
        while(!readyQ.empty()) readyQ.pop();
    }
    
    void displayProcesses(){
        if(!tasks.size()){
            cout<<"No processes !"<<endl;
            return;
        }

        cout.width(10);
        cout<<"Name";
        cout.width(14);
        cout<<"Arrival Time";
        cout.width(12);
        cout<<"Burst Time";
        cout.width(12);
        cout<<"Priority";
        cout<<endl;
        
        for(auto process:tasks){
            cout.width(10);
            cout<<process.name;
            cout.width(10);
            cout<<process.arrivalTime;
            cout.width(10);
            cout<<process.burstTime;
            cout.width(10);
            cout<<process.priority;
            cout<<endl;
        }
    }

    void displayAnalysis(vector<Process>& tasks){
        cout.width(10);
        cout<<"Name";
        cout.width(14);
        cout<<"Arrival Time";
        cout.width(12);
        cout<<"Burst Time";
        cout.width(10);
        cout<<"Priority";
        cout.width(18);
        cout<<"Turnaround Time";
        cout.width(12);
        cout<<"Wait Time";
        cout<<endl;
        
        for(auto process:tasks){
            cout.width(10);
            cout<<process.name;
            cout.width(10);
            cout<<process.arrivalTime;
            cout.width(10);
            cout<<process.burstTime;
            cout.width(10);
            cout<<process.priority;
            cout.width(10);
            cout<<process.turnaroundTime;
            cout.width(10);
            cout<<process.waitTime;
            cout<<endl;
        }
    }

    void executeTask(){
        if(!tasks.size()) {
            cout<<"No process to execute."<<endl;
            return;
        }
        sortTasksArrivalTime(tasks);
        auto it=tasks.begin();
        int time=0;
        
        while(!readyQ.empty() || it != tasks.end()){
            while (it != tasks.end() &&  (*it).arrivalTime==time){
                readyQ.push(&(*it));
                it++;
            }
            
            if(!readyQ.empty() && !readyQ.top()->remainingTime){
                readyQ.top()->turnaroundTime=time-readyQ.top()->arrivalTime;
                readyQ.top()->calculateWait();
                readyQ.pop();
                readyQ.top()->isRunning=true;
            }
            
            cout<<endl<<"Time :\t "<<time<<" : "<<endl;
            displayCurrState();
            
            time++;
            if(!readyQ.empty()) readyQ.top()->remainingTime--;
            this_thread::sleep_for(chrono::seconds(1));
        }
        displayAnalysis(tasks);
        clearProcesses();
        
    }
    
    void addProcess(string name="",int arrival=0,int burst=0,int priority=0){
        if(!burst)return;
        Process p(name,arrival,burst,priority);
        tasks.push_back(p);
    }

    void addProcesses(){
        clearProcesses();
        int n;
        cout<<"Enrer number of processes : ";
        cin>>n;
        for(int i=0;i<n;i++){
            string name;
            int arrival,burst=0,priority=0;
            cout<<"Enter Process P"<<i+1<<" ---> "<<endl;
            cout<<"Enter name : ";
            cin>>name;
            cout<<"Enter arrival time : ";
            cin>>arrival;
            while(burst<=0){
                cout<<"Enter burst time : ";
                cin>>burst;
                if(burst<=0) cout<<"Burst time chould greater 0."<<endl;
            }
            while(priority<1 || priority>10){
                cout<<"Enter priority (lower number highest priority 1-10) : ";
                cin>>priority;
                if(priority<1 || priority>10) cout<<"Enter right priority (1-10) ! "<<endl;
            }
            cout<<endl;
            addProcess(name,arrival,burst,priority);
        }
    }
  

};


int main(){

    string menu="1. Enter processes. \n2. Display processes. \n3. Execute processes. \n4. Exit.";
    PriorityNonPreemptive p;
    // cout<<menu;

    while(true){
        cout<<endl<<"Menu :"<<endl<<menu;
        int choice;
        cout<<endl<<"Enter choice : ";
        cin>>choice;

        if(choice==1){
            p.addProcesses();
        }else if(choice==2){
            p.displayProcesses();
        }else if(choice==3){
            p.executeTask();
        }else if(choice==4){
            cout<<"Thank You !"<<endl;
            break;
        }else{
            cout<<"Enter valid choice."<<endl;
        }
    }
    
    // p.addProcess("P1",2,5,1);
    // p.addProcess("P2",0,3,2);
    // p.addProcess("P3",0,5,2);
    // p.addProcess("P4",3,4,3);
    // p.addProcess("P5",0,8,2);
    // p.addProcess("P6",4,4,3);
    // p.displayProcesses();   
    
    // p.executeTask();
    // p.executeTask();


// P1 2 5 1
// P2 0 3 2
// P3 0 5 2
// P4 0 4 3
// P5 0 8 2
// P6 4 4 3
    
    
    return 0;
}
