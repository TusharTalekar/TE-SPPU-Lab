#include<iostream>
#include<vector>
#include<cstdlib>
using namespace std;

const int memorySize=200;
const int partitionNumber=10;
const vector<int> partitionSizes={20,30,20,10,20,10,25,60,35,30};



class Memory{
    protected:
    vector<vector<int>> partition;  //each vecotr =>[0]=>total size,[1]=>remain space[2,...]=>memory allocated 
    public:
    Memory(int m=memorySize,int n=partitionNumber,vector<int> partSizes=partitionSizes){
        int remainSize=m;
        for(int i=0;i<n;i++){
            if(remainSize >= partSizes[i]){
                vector<int> part={partSizes[i],partSizes[i],0};
                partition.push_back(part);
                remainSize -= partSizes[i];
            }else{
                vector<int> part={remainSize,remainSize,0};
                partition.push_back(part);
                break;
            }
        }
    }

    void displayMemoryState(){
        int n = partition.size();
        cout.width(15);
        cout<<"   Partition size";
        cout.width(20);
        cout<<"Remaining Space";
        cout.width(15);
        cout<<"Occupied";
        cout<<endl;
        for(int i=0;i<n;i++){
            cout<<i<<".\t"<<partition[i][0]<<"\t";
            cout.width(5);
            cout<<"\t"<<partition[i][1]<<"\t";
            cout.width(10);
            int n1 = partition[i].size();
            for(int j=2;j<n1;j++){
                cout<<partition[i][j]<<"  ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
    
    vector<vector<int>> getInfo(){
        return this->partition;
    }
};


class BestFit:public Memory{
    public:
    int allocatePartition(int size){
        int n=partition.size();
        int idx=-1;
        int minSize=0;
        for(int i=0;i<n;i++){
            if(partition[i][2]==0 && partition[i][1]>=size){
                if(minSize==0 || partition[i][1]<minSize){
                    minSize=partition[i][1];
                    idx=i;
                }
            }
        }
        if(idx<0){
            cout<<"Can't allocate memory."<<endl;
        }else{
            partition[idx][2]=size;
            partition[idx][1]-=size;
        }

        return idx;
    }

    void deallocate(int index){
        if(index<0)return;
        partition[index][2]=0;
        partition[index][1]=partition[index][0];
    }

};




class FirstFit:public Memory{
    public:
    int allocatePartition(int size){
        int n=partition.size();
        int idx=-1;
        for(int i=0;i<n;i++){
            if(partition[i][2]==0 && partition[i][1]>=size){
                idx=i;
                break;
            }
        }
        if(idx<0){
            cout<<"Can't allocate memory."<<endl;
        }else{
            partition[idx][2]=size;
            partition[idx][1]-=size;
        }

        return idx;
    }

    void deallocate(int index){
        if(index<0)return;
        partition[index][2]=0;
        partition[index][1]=partition[index][0];
    }

};




class NextFit:public Memory{
    int prevAlloc;
    public:
    NextFit():prevAlloc(0){}
    int allocatePartition(int size){
        int n=partition.size();
        int idx=-1;
        for(int i=prevAlloc+1;i<n&&i!=prevAlloc;i=(i+1)%n){
            if(partition[i][2]==0 && partition[i][1]>=size){
                idx=i;
                break;
            }
        }
        if(idx<0){
            cout<<"Can't allocate memory."<<endl;
        }else{
            partition[idx][2]=size;
            partition[idx][1]-=size;
        }

        prevAlloc=idx;
        return idx;
    }

    void deallocate(int index){
        if(index<0)return;
        partition[index][2]=0;
        partition[index][1]=partition[index][0];
    }

};




class WorstFit:public Memory{
    public:
    int allocatePartition(int size){
        int n=partition.size();
        int idx=-1;
        int maxSize=0;
        for(int i=0;i<n;i++){
            if(partition[i][2]==0 && partition[i][1]>=size){
                if(maxSize==0 || partition[i][1]>maxSize){
                    maxSize=partition[i][1];
                    idx=i;
                }
            }
        }
        if(idx<0){
            cout<<"Can't allocate memory."<<endl;
        }else{
            partition[idx][2]=size;
            partition[idx][1]-=size;
        }

        return idx;
    }

    void deallocate(int index){
        if(index<0)return;
        partition[index][2]=0;
        partition[index][1]=partition[index][0];
    }

};


class memoryManagement{
    vector<int> processes;
    vector<int> memoryIndex;
    public:

    void clearProcesses(){
        processes.clear();
    }
    
    void getProcesses(){
        clearProcesses();
        int n;
        cout<<"Enter number of processes : ";
        cin>>n;
        for(int i=0;i<n;i++){
            int blockSize;
            cout<<"Enter size for process P"<<i+1<<" : ";
            cin>>blockSize;
            processes.push_back(blockSize);
        }
    }

    void displayProcesses(vector<int>& vect){
        int n=vect.size();
        cout<<"\tProcesses"<<"\t Addresses"<<endl;
        for(int i=0;i<n;i++){
            cout<<"P"<<i<<"\t\t"<<vect[i]<<endl;
        }
    }
    void displayProcesses(){
        displayProcesses(processes);
    }
    
    void addProcessAddress(int idx,vector<int>& memoryIndex){
        memoryIndex.push_back(idx);
    }
    void addProcessAddress(int idx){
        addProcessAddress(idx,memoryIndex);
    }

    void displayAllocation(vector<vector<int>> &partition){
        cout.width(12);
        cout<<"Memory idx";
        cout.width(12);
        cout<<"Process size";
        cout.width(12);
        cout<<"Allocated";
        cout.width(12);
        cout<<"Block size";
        cout.width(12);
        cout<<"Remaining";
        cout<<endl;
        int n=memoryIndex.size();
        for(int i=0;i<n;i++){
            int idx=memoryIndex[i];
            cout.width(12);
            cout<<idx;
            cout.width(12);
            cout<<processes[i];
            if(idx>+0){
                cout.width(12);
                cout<<partition[idx][2];
                cout.width(12);
                cout<<partition[idx][0];
                cout.width(12);
                cout<<partition[idx][1];
            }else{
                cout.width(12);
                cout<<0;
                cout.width(12);
                cout<<0;
                cout.width(12);
                cout<<0;
            }
            cout<<endl;
        }
    }

    void allocate(){
        if(processes.size()==0){
            cout<<"No processes."<<endl;
            return;
        }
        int choice;
        string menu="\t1. Best Fit.\n\t2. First Fit.\n\t3. Next Fit.\n\t4. Worst Fit.\n\t5. Exit.";
        cout<<menu<<endl;
        cout<<"\tEnter choice:";
        cin>>choice;
        switch (choice){
        case 1:
            allocateByBestFit();
            break;
        case 2:
            allocateByFirstFit();
            break;
        case 3:
            allocateByNextFit();
            break;
        case 4:
            allocateByWorstFit();
            break;
        case 5:
            break;
        
        default:
            cout<<"Invalid choice"<<endl;
            break;
        }
        memoryIndex.clear();
    }
    void allocateByFirstFit(){
        FirstFit f;
        for(int processSize:processes){
            int idx=f.allocatePartition(processSize);
            addProcessAddress(idx);
        }
        vector<vector<int>> partition=f.getInfo();
        displayAllocation(partition);
    }
    void allocateByBestFit(){
        BestFit b;
        for(int processSize:processes){
            int idx=b.allocatePartition(processSize);
            addProcessAddress(idx);
        }
        vector<vector<int>> partition=b.getInfo();
        displayAllocation(partition);

    }
    void allocateByWorstFit(){
        WorstFit w;
        for(int processSize:processes){
            int idx=w.allocatePartition(processSize);
            addProcessAddress(idx);
        }
        vector<vector<int>> partition=w.getInfo();
        displayAllocation(partition);

    }
    void allocateByNextFit(){
        NextFit n;
        for(int processSize:processes){
            int idx=n.allocatePartition(processSize);
            addProcessAddress(idx);
        }
        vector<vector<int>> partition=n.getInfo();
        displayAllocation(partition);
    }


};



int main(){

    memoryManagement m;
    string menu="1. Add processes. \n2. Display processes. \n3. Allocate. \n4. Exit.";
    // cout<<menu<<endl;

    while(true){
        int choice;
        cout<<menu<<endl;
        cout<<"Enter choice : ";
        cin>>choice;

        if(choice==1){
            m.getProcesses();
            m.displayProcesses();
            m.allocate();
        }else if(choice==2){
            m.displayProcesses();
        }else if(choice==3){
            m.allocate();
        }else if(choice==4){
            cout<<"Thank You !"<<endl;
            break;
        }else{
            cout<<"Invalid choice."<<endl;
        }

    }


//     partitionSizes={20,30,20,10,20,10,25,60,35,30};
// 1
// 7
// 15
// 25
// 20
// 5
// 70
// 50
// 25


    return 0;
}
