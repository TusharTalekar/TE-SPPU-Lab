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
    public:
    Process(string n="",int a=0,int b=0,int t=0,int w=0){
        name=n;
        arrivalTime=a;
        burstTime=b;
        turnaroundTime=t;
        waitTime=w;
        remainingTime=b;  ///when object is created remaing execution is same as burst time
    }
    void calculateWait(){
        waitTime=turnaroundTime-burstTime;
    }
    friend class FCFS;
};



class FCFS{
    queue<Process*> readyQ;
    vector<Process> tasks;


    void sortTasks(vector<Process>& tasks){
        sort(tasks.begin(), tasks.end(), [](const Process &a, const Process &b) {
            return a.arrivalTime < b.arrivalTime;
        });
    }


    void displayCurrState(){
        if(readyQ.empty())  {
            cout<<"Running : No Process Running"<<endl;
            return;
        }
        queue<Process*> temp=readyQ;
        cout<<"Running : "<< temp.front()->name<<"\t"<<temp.front()->remainingTime<<endl;
        cout<<"Read queue : "<<endl;
        int i=1;
        while(!temp.empty()){
            cout.width(10);
            cout<<"\t"<<temp.front()->name;
            cout.width(5);
            cout<<temp.front()->remainingTime<<endl;
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

    void displayAnalysis(vector<Process>& tasks){
        cout.width(10);
        cout<<"Name";
        cout.width(14);
        cout<<"Arrival Time";
        cout.width(12);
        cout<<"Burst Time";
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
        sortTasks(tasks);
        auto it=tasks.begin();
        int time=0;
        
        while(!readyQ.empty() || it != tasks.end()){
            while (it != tasks.end() &&  (*it).arrivalTime==time){
                readyQ.push(&(*it));
                it++;
            }
            
            if(!readyQ.empty() && !readyQ.front()->remainingTime){
                readyQ.front()->turnaroundTime=time-readyQ.front()->arrivalTime;
                readyQ.front()->calculateWait();
                readyQ.pop();
            }
            
            cout<<endl<<"Time :\t "<<time<<" : "<<endl;
            displayCurrState();
            
            time++;
            if(!readyQ.empty()) readyQ.front()->remainingTime--;
            this_thread::sleep_for(chrono::seconds(1));
        }
        displayAnalysis(tasks);
        clearProcesses();
        
    }
    
    void addProcess(string name="",int arrival=0,int burst=0){
        if(!burst)return;
        Process p(name,arrival,burst);
        tasks.push_back(p);
    }

    void addProcesses(){
        clearProcesses();
        int n;
        cout<<"Enrer number of processes : ";
        cin>>n;
        for(int i=0;i<n;i++){
            string name;
            int arrival,burst=0;
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
            cout<<endl;
            addProcess(name,arrival,burst);
        }
    }
  

};


int main(){

    string menu="1. Enter processes. \n2. Display processes. \n3. Execute processes. \n4. Exit.";
    FCFS f;
    // cout<<menu;

    while(true){
        cout<<endl<<"Menu :"<<endl<<menu;
        int choice;
        cout<<endl<<"Enter choice : ";
        cin>>choice;

        if(choice==1){
            f.addProcesses();
        }else if(choice==2){
            f.displayProcesses();
        }else if(choice==3){
            f.executeTask();
        }else if(choice==4){
            cout<<"Thank You !"<<endl;
            break;
        }else{
            cout<<"Enter valid choice."<<endl;
        }
    }
    
    // f.addProcess("P1",0,5);
    // f.addProcess("P2",1,3);
    // f.addProcess("P3",3,5);
    // f.addProcess("P4",0,4);
    // f.addProcess("P5",5,8);
    // f.addProcess("P6",6,4);
    // f.displayProcesses();
    
    // f.executeTask();
    // f.executeTask();
    
    return 0;
}
