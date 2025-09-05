#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

const int totalFrames=3;

class FIFOReplacement{
    vector<int> pageFrame;
    queue<int> q;   // Stores index of frames
    int pageFault;
    vector<int> pages;
    public:
    FIFOReplacement():pageFault(0){}

    void enterPage(int page){
        if(page<0)return;
        if(pageFrame.size()<totalFrames){
            pageFrame.push_back(page);
            q.push(pageFrame.size()-1);
            pageFault++;
        }else{
            if(find(pageFrame.begin(),pageFrame.end(),page)==pageFrame.end()){
                pageFrame[q.front()]=page;
                q.push(q.front());
                q.pop();
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
        while(!q.empty()){
            q.pop();
        }
        pages.clear();
        pageFault=0;
    }

    int pageFaults(vector<int>& pages){
        if(pages.size()==0){
            cout<<"No pages."<<endl;
            return 0;
        }
        for(int page:pages){
            cout<<"Page "<<page<<"\t";
            enterPage(page);
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

    FIFOReplacement f;
    // f.pageFaults({1,3,4,2,6,5});
    // cout<<endl;
    // f.pageFaults({3,1,2,1,6,5,1,3});

    string menu="1. Enter pages. \n2. Display Pages. \n3. Calculate page faults. \n4. Exit.";

    while (true){
        int choice;
        cout<<endl<<menu<<endl;
        cout<<"Enter choice : ";
        cin>>choice;

        if(choice==1){
            f.inputPages();
        }else if(choice==2){
            f.displayPages();
        }else if(choice==3){
            f.pageFaults();
        }else if(choice==4){
            cout<<"Thank You !"<<endl;
            break;;
        }else{
            cout<<"Invalid choice."<<endl;
        }

        if (cin.fail()) {
            cout << "Invalid input! Expected an integer."<< endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
        }

    }
    

    return 0;
}