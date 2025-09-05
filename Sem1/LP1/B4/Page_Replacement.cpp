#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<unordered_map>
#include<climits>
using namespace std;

const int totalFrames=3;







class Frames{
    protected:
    vector<int> pageFrame;
    int pageFault;

    public:
    Frames():pageFault(0){}

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
        pageFault=0;
    }

};




class FIFOReplacement:public Frames{
    queue<int> q;   // Stores index of frames
    public:

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

    void clearFrames(){
        pageFrame.clear();
        while(!q.empty()){
            q.pop();
        }
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
    
};


class LRU_Replacement:public Frames{
    unordered_map<int,int> mp;   
    public:

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
                int dist=INT_MAX;
                vector<int>::iterator changeIt;
                for(auto it=pageFrame.begin();it!=pageFrame.end();it++){
                    if(mp[*it]<dist){
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

    void clearFrames(){
        pageFrame.clear();
        mp.clear();
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
    
};



class MRU_Replacement:public Frames{
    unordered_map<int,int> mp;   
    public:

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

    void clearFrames(){
        pageFrame.clear();
        mp.clear();
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
    
};




class OptimalReplacement:public Frames{
    public:
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

};



class PageReplacement{
    vector<int> pages;
    
    public:

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

    void pageFault(){
        int choice;
        string menu="\t1. FIFO.\n\t2. LRU.\n\t3. MRU.\n\t4. Optimal.\n\t5. Exit.";
        cout<<menu<<endl;
        cout<<"\tEnter choicee : ";
        cin>>choice;

        switch (choice){
            case 1:
                pageFaultByFIFO();
                break;
            case 2:
                pageFaultByLRU();
                break;
            case 3:
                pageFaultByMRU();
                break;
            case 4:
                pageFaultByOptimal();
                break;
            case 5:
                break;
            default:
                cout<<"Invalid choice"<<endl;
                break;
        }
        // if (cin.fail()) {
        //     cout << "Invalid input! Expected an integer."<< endl;
        //     cin.clear();
        //     cin.ignore(numeric_limits<streamsize>::max(),'\n');
        // }
    }

    int pageFaultByFIFO(){
        FIFOReplacement f;
        return f.pageFaults(pages);
    }
    int pageFaultByLRU(){
        LRU_Replacement l;
        return l.pageFaults(pages);
    }
    int pageFaultByMRU(){
        MRU_Replacement M;
        return M.pageFaults(pages);
    }
    int pageFaultByOptimal(){
        OptimalReplacement o;
        return o.pageFaults(pages);
    }
};


int main(){

    PageReplacement p;

    string menu="1. Enter pages. \n2. Display Pages. \n3. Calculate page faults. \n4. Exit.";

    while (true){
        int choice;
        cout<<endl<<menu<<endl;
        cout<<"Enter choice : ";
        cin>>choice;

        if(choice==1){
            p.inputPages();
        }else if(choice==2){
            p.displayPages();
        }else if(choice==3){
            p.pageFault();
        }else if(choice==4){
            cout<<"Thank You !"<<endl;
            break;
        }else{
            cout<<"Invalid choice."<<endl;
        }

        // if (cin.fail()) {
        //     cout << "Invalid input! Expected an integer."<< endl;
        //     cin.clear();
        //     cin.ignore(numeric_limits<streamsize>::max(),'\n');
        // }

    }


    // 7 0 1 2 0 3 0 4 2 3 0 3 2 2
    

    return 0;
}
