#include<iostream>
#include<vector>
#include<cstdlib>
using namespace std;

const int memorySize=200;
const int partitionNumber=10;
const vector<int> partitionSizes={20,30,20,10,20,10,25,60,35,30};

class NextFit{
    vector<vector<int>> partition;  //each vecotr =>[0]=>total size,[1]=>remain space[2,...]=>memory allocated 
    int prevAlloc;
    public:
    NextFit(int m=memorySize,int n=partitionNumber,vector<int> partSizes=partitionSizes):prevAlloc(0){
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


void displayMemoryIndex(vector<int>& vect){
    int n=vect.size();
    cout<<"Processes"<<"\t Addresses"<<endl;
    for(int i=0;i<n;i++){
        cout<<i<<"\t\t"<<vect[i]<<endl;
    }
}
void addProcess(int idx,vector<int>& memoryIndex){
    if(idx>=0) memoryIndex.push_back(idx);
}


int main(){
    
    // int n,size;
    // cout<<"Enter total size of memeory: ";
    // cin>>size;
    // cout<<"Enter number of partitions: ";
    // cin>>n;
    // vector<int> partitionSize(n);
    // cout<<"Enter size of each partition: "<<endl;
    // for(int i=0;i<n;i++){
    //     cout<<"Partition "<<i+1<<" : ";
    //     cin>>partitionSize[i];
    // }

    // NextFit nf(size,n,partitionSize);
    NextFit nf;
    vector<int> memoryIndex;

    string menu="1. Show memory state. \n2. Display processes. \n3. Allocate memory. \n4. Deallocate memory. \n5. Exit.";

    while (true){
        cout<<endl<<menu;
        int choice;
        cout<<endl<<"Enter choice : ";
        cin>>choice;

        if(choice==1){
            nf.displayMemoryState();
        }else if(choice==2){
            displayMemoryIndex(memoryIndex);
        }else if(choice==3){
            int size;
            cout<<"Enter memory size to allocate : ";
            cin>>size;
            addProcess(nf.allocatePartition(size),memoryIndex);
        }else if(choice==4){
            if(memoryIndex.size()==0){
                cout<<"No processes"<<endl;
                continue;
            }
            displayMemoryIndex(memoryIndex);
            int n;
            cout<<"Enter process number to deallocate : ";
            cin>>n;
            if(n<0 || n>=memoryIndex.size()){
                cout<<"Invalid index"<<endl;
                continue;
            }
            nf.deallocate(memoryIndex[n]);
            memoryIndex.erase(memoryIndex.begin()+n);
        }else if(choice==5){
            cout<<"Thank You!"<<endl;
            break;
        }else{
            cout<<"Invalid choice."<<endl;
        }
    }
    

    





    // nf.displayMemoryState();
    // addProcess(nf.allocatePartition(21),memoryIndex);
    // addProcess(nf.allocatePartition(19),memoryIndex);
    // addProcess(nf.allocatePartition(25),memoryIndex);
    // addProcess(nf.allocatePartition(31),memoryIndex);
    // addProcess(nf.allocatePartition(4),memoryIndex);
    // addProcess(nf.allocatePartition(25),memoryIndex);
    // displayMemoryIndex(memoryIndex);
    // nf.displayMemoryState();
    // nf.deallocate(memoryIndex[0]);
    // nf.displayMemoryState();


    return 0;
}