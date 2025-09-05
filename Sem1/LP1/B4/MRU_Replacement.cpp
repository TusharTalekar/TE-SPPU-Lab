#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<unordered_map>
#include <climits>
using namespace std;

const int totalFrames=4;

class MRU_Replacement{
    vector<int> pageFrame;
    unordered_map<int,int> mp;   
    int pageFault;
    vector<int> pages;
    public:
    MRU_Replacement():pageFault(0){}

    void enterPage(int page,int i){
        if(page<0)return;
        if(pageFrame.size()<totalFrames){
            pageFrame.push_back(page);
            // q.push(pageFrame.size()-1);
            mp[page]=i;
            pageFault++;
        }else{
            if(find(pageFrame.begin(),pageFrame.end(),page)==pageFrame.end()){
                // pageFrame[q.front()]=page;
                // q.push(q.front());
                // q.pop();
                int dist=0;
                vector<int>::iterator changeIt;
                for(auto it=pageFrame.begin();it!=pageFrame.end();it++){
                    if(mp[*it]>dist){
                        changeIt=it;
                        dist=mp[*it];
                    }
                }
                *changeIt=page;

                pageFault++;
            }
            mp[page]=i;

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
        mp.clear();
        pages.clear();
        pageFault=0;
    }

    int pageFaults(vector<int>& pages){
        if(pages.size()==0){
            cout<<"No pages."<<endl;
            return 0;
        }
        for(int i=0;i<pages.size();i++){
            cout<<"Page "<<pages[i]<<"\t";
            enterPage(pages[i],i);
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

    MRU_Replacement M;
    // M.pageFaults({1,3,4,2,6,5});
    // cout<<endl;
    // vector<int> temp={3,1,2,1,6,5,1,3};
    // vector<int> temp={7,0,1,2,0,3,0,4,2,3,0,3,2,3};
    // M.pageFaults(temp);

    string menu="1. Enter pages. \n2. Display Pages. \n3. Calculate page faults. \n4. Exit.";

    while (true){
        int choice;
        cout<<endl<<menu<<endl;
        cout<<"Enter choice : ";
        cin>>choice;

        if(choice==1){
            M.inputPages();
        }else if(choice==2){
            M.displayPages();
        }else if(choice==3){
            M.pageFaults();
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
    
    // 7 0 1 2 0 3 0 4 2 3 0 3 2 2
    
    return 0;
}