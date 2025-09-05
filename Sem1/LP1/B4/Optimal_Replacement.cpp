#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

const int totalFrames=3;

class OptimalReplacement{
    vector<int> pageFrame;
    int pageFault;
    vector<int> pages;
    public:
    OptimalReplacement():pageFault(0){}

    void enterPage(vector<int>& pages,int i){
        if(pages[i]<0)return;
        if(pageFrame.size()<totalFrames){
            pageFrame.push_back(pages[i]);
            pageFault++;
        }else{
            if(find(pageFrame.begin(),pageFrame.end(),pages[i])==pageFrame.end()){
                int n=pages.size();
                int replaceIdx=0;
                int maxDist=0;
                for(int j=0;j<pageFrame.size();j++){
                    int k=i+1,d=0;
                    while(k<n && pageFrame[j]!=pages[k]){
                        d++;
                        k++;
                    }
                    if(maxDist<d){
                        replaceIdx=j;
                        maxDist=d;
                    }
                }
                pageFrame[replaceIdx] = pages[i];
                pageFault++;
            }
        }
    }

    void displayCurrState(){
        cout<<"Page Frame : |";
        for(int page:pageFrame){
            cout<<"  "<<page<<"  |";
        }
        cout<<endl;
        cout<<"\tPafe Faults : "<<pageFault<<endl;
    }

    void clearFrames(){
        pageFrame.clear();
        pages.clear();
        pageFault=0;
    }

    int pageFaults(vector<int>& pages){
        int n=pages.size();
        if(n==0){
            cout<<"No pages."<<endl;
            return 0;
        }
        for(int i=0;i<n;i++){
            cout<<"Page "<<pages[i]<<"\t";
            enterPage(pages,i);
            displayCurrState();
        }
        int faults=pageFault;
        clearFrames();
        return faults;
    }
    int pageFaults(){
        return pageFaults(this->pages);
    }

    void inputPages(vector<int>& pages){
        pages.clear();
        cout<<"Enter -1 to exit !"<<endl;
        int n;
        cout<<"Enter number of pages : ";
        cin>>n;
        if(n<0)return;
        for(int i=0;i<n;i++){
            int p; 
            cout<<"Enter page "<<i+1<<" : ";
            cin>>p;
            if(p<0){
                pages.clear();
                return;
            }
            pages.push_back(p);
        }
    }
    void inputPages(){
        inputPages(this->pages);
    }


    void displayPages(vector<int>& pages){
        int n=pages.size();
        cout<<"Pages : ";
        if(n==0){
            cout<<"No pages."<<endl;
            return;
        }
        for(int i=0;i<n;i++){
            cout<<pages[i]<<"\t";
        }
        cout<<endl;
    }
    void displayPages(){
        displayPages(this->pages);
    }

};


int main(){

    OptimalReplacement o;
    // o.pageFaults({1,3,4,2,6,5});
    // cout<<endl;
    // o.pageFaults({3,1,2,1,6,5,1,3});
    // o.pageFaults({7,0,1,2,0,3,0,4,2,3,0,3,2,3});
     

    string menu="1. Enter pages. \n2. Display Pages. \n3. Calculate page faults. \n4. Exit.";

    while (true){
        int choice;
        cout<<endl<<menu<<endl;
        cout<<"Enter choice : ";
        cin>>choice;

        if(choice==1){
            o.inputPages();
        }else if(choice==2){
            o.displayPages();
        }else if(choice==3){
            o.pageFaults();
        }else if(choice==4){
            cout<<"Thank You !"<<endl;
            break;
        }else{
            cout<<"Invalid choice."<<endl;
        }

        if (cin.fail()) {
            cout << "Invalid input! Expected an integer."<< endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
        }

    }


    // 7 0 1 2 0 3 0 4 2 3 0 3 2 3
    

    return 0;
}
