#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<map>
#include<vector>
#include<sstream>
#include<algorithm>
#include<fstream>
#include<typeinfo>
#include<stack>
#include<queue>
#define SUCCESS true
#define FAILURE false
#define AND_GATE 0
#define NAND_GATE 3
#define NOR_GATE 4
#define OR_GATE 1
#define INV_GATE 2
#define BUF_GATE 7

using namespace std;

struct gate {
	int gateid;
	int gateno;
	int ip1;//netno
	int ip2;
	int op;
	bool ip2n;
	gate* next;
	gate* prev;

};
struct net1{
	int netno;
	int source;
	int dest;
	bool fromgate;
	bool togate;
	vector<int>gateip;
	vector<int>gateop;
	int netype; //0 for input; 1 for output; 2 for wire
	int val;
	net1* next;
	net1* prev;
	int fault;
	int maximumNets;
};

class gatell {
public:
	gatell()
	{
		head = nullptr;
	}
    bool is_single_ip(int gno)
    {
        gate* curr = head;
        while(curr!=nullptr)
        {
            if(curr->gateno == gno)
            {
                return !curr->ip2n;
            }
            curr = curr->next;
        }
		return false;
    }
    int get_gate_type(int gno)
    {
        gate* curr = head;
        while(curr!=nullptr)
        {
            if(curr->gateno == gno)
            {
                return curr->gateid;
            }
            curr = curr->next;
        }
    }
	void insertfront(int gateid,int gateno,int ip1, int ip2,int op,bool ip2n)
	{
		gate* newgate = new gate();
		newgate->gateid = gateid;
		newgate->gateno = gateno;
		newgate->ip1 = ip1;
		newgate->ip2 = ip2;
		newgate->op = op;
		newgate->ip2n = ip2n;
		newgate->next = head;
		head = newgate;
		newgate->prev = head;

	}
	void insert(int gateid,int gateno,int ip1, int ip2,int op,bool ip2n)
	{
		gate* newgate = new gate();
		newgate->gateid = gateid;
		newgate->gateno = gateno;
		newgate->ip1 = ip1;
		newgate->ip2 = ip2;
		newgate->op = op;
		newgate->ip2n = ip2n;
		newgate->next = nullptr;
		gate* curr = head;
		if(curr == nullptr)
			{
				head = newgate;
				newgate->prev = head;
			}
		else
		{
			while (curr->next!= nullptr)
			{
				curr = curr->next;
			}
			curr->next = newgate;
			newgate->prev = curr;
			
		}

	}
	void deletenode(int gateno)
	{
		gate* curr = head;
		while(curr!=nullptr)
		{
			if(curr->gateno == gateno)
				break;
			curr = curr->next;

		}
		if(curr == nullptr)
		{
			cout<<"Gate Not Found"<<endl;
            return;
		}
		if(curr->prev != nullptr)
            curr->prev->next = curr->next;
        else    
            head = curr->next;
        if(curr->next != nullptr)
            curr->next->prev = curr->prev;
        delete curr;
	}
	void search(int gateno)
    {
        gate* curr = head;
        while (curr != nullptr)
        {
            if (curr->gateno == gateno)
            {
                cout << "Found Gate" << endl;
                break;
            }
            curr = curr->next;
        }
        if (curr == nullptr)
            cout << "Gate Not Found" << endl;

    }
	void traverse()
    {
        gate* curr = head;
        while (curr != nullptr)
        {
            cout <<"Gate Number "<<curr->gateno << " " <<" Inputs "<<curr->ip1<<" "<<(curr->ip2n?(curr->ip2):curr->op)<< " output: "<< (curr->ip2n?(curr->op):(-1))<<endl;
            
			curr = curr->next;
        }
        cout << endl;

    }
	vector<pair<int,int>> searchnet(int netno)
	{
		gate* curr = head;
		vector<pair<int,int>> ans;
		while(curr!= nullptr)
		{
			if(curr->ip1==netno || curr->ip2 == netno )
				ans.push_back(make_pair(curr->gateno,1)); // 1 for ip 2 for op
			if(curr->op == netno)
				ans.push_back(make_pair(curr->gateno,2));
			curr = curr->next;
			
		}
		return ans;
	}
	int getop(int gno)
	{
		gate* curr = head;
		while(curr!= nullptr)
		{
			if(curr->gateno ==gno)
			{
				return curr->op;
			}
			curr = curr->next;
			
		}
	}
    vector<int> return_nets(int gno)
    {
        gate* curr = head;
        vector<int> rtn;
        while(curr!=nullptr)
        {
            if(curr->gateno == gno)
            {
                rtn.push_back(curr->ip1);
                rtn.push_back(curr->ip2);
                rtn.push_back(curr->op);
                return rtn;
                
            }
            curr = curr->next;
        }
    }
	void backtrack()
    {
        gate* c = head;
        cout<<"Going Forward ";
        while(c->next != nullptr)
        {
            cout<<c->gateno<<" ";
            c = c->next;
        }
        cout<<c->gateno;
        
        cout<<endl;
        cout<<"BackTracking ";
        while (c!=head)
        {
            cout<<c->gateno<<" ";
            c = c->prev;
        }

        
    cout<<c->gateno<<endl;

    }


	


private:
	gate* head;
};
class netll {
public:
	netll()
	{
		head = nullptr;
	}
	void inserfront(int netno,int source,int dest,bool fromgate,bool togate,int netype,int val, vector<int>gateip,vector<int>gateop)
	{
		net1* newnet = new net1;
		newnet->netno = netno;
		newnet->source = source;
		newnet->dest = dest;
		newnet->fromgate = fromgate;
		newnet->togate = togate;
		newnet->netype = netype;
		newnet->fault =-1;
		newnet->maximumNets = 0;
		for(auto i:gateip)
			newnet->gateip.push_back(i);
		for(auto i:gateop)
			newnet->gateop.push_back(i);
		newnet->val = val;
		
		newnet->next = head;
		head = newnet;
		newnet->prev = head;

	}
	void insert(int netno,int source,int dest,bool fromgate,bool togate,int netype,int val, vector<int>gateip,vector<int>gateop)
	{
		net1* newnet = new net1;
		newnet->netno = netno;
		newnet->source = source;
		newnet->dest = dest;
		newnet->fromgate = fromgate;
		newnet->togate = togate;
		newnet->netype = netype;
		newnet->fault = -1;
		newnet->maximumNets = 0;
		for(auto i:gateip)
			newnet->gateip.push_back(i);
		for(auto i:gateop)
			newnet->gateop.push_back(i);
		newnet->val = val;
		newnet->next = nullptr;
		net1* curr = head;
		if(curr == nullptr)
			{
				head = newnet;
				newnet->prev = head;
			}
		else
		{
			while (curr->next!= nullptr)
			{
				curr = curr->next;
			}
			curr->next = newnet;
			newnet->prev = curr;
			
		}

	}
	void setmaxnet(int mnet)
	{
		net1* curr = head;
		
        while (curr != nullptr)
        {
            curr->maximumNets = mnet;
            curr = curr->next;
        }

	}
	int getmaxnet()
	{
		net1* curr = head;
		return curr->maximumNets;
	}
	void deletenode(int netno)
	{
		net1* curr = head;
		while(curr!=nullptr)
		{
			if(curr->netno == netno)
				break;
			curr = curr->next;

		}
		if(curr == nullptr)
		{
			cout<<"Net Not Found"<<endl;
            return;
		}
		if(curr->prev != nullptr)
            curr->prev->next = curr->next;
        else    
            head = curr->next;
        if(curr->next != nullptr)
            curr->next->prev = curr->prev;
        delete curr;
	}
	void search(int netno)
    {
        net1* curr = head;
        while (curr != nullptr)
        {
            if (curr->netno == netno)
            {
                cout << "Found Net" << endl;
                break;
            }
            curr = curr->next;
        }
        if (curr == nullptr)
            cout << "Net Not Found" << endl;

    }
    void setval(int netno,int data)
    {
        net1* curr = head;
        while (curr != nullptr)
        {
            if (curr->netno == netno)
            {
				//cout<<" setval "<<data<<endl;
                curr->val = data;
                break;
            }
            curr = curr->next;
        }
    }
	void setfault(int netno,int fault)
	{
		net1* curr = head;
        while (curr != nullptr)
        {
            if (curr->netno == netno)
            {
				//cout<<" setval "<<data<<endl;
                curr->fault = fault;
                break;
            }
            curr = curr->next;
        }
	}
	int getfault(int netno)
	{
		net1* curr = head;
        while (curr != nullptr)
        {
            if (curr->netno == netno)
            {
				//cout<<" getval "<<curr->val<<endl; 
                return curr->fault;
            }
            curr = curr->next;
        }
	}

	int getval(int netno)
	{
		net1* curr = head;
        while (curr != nullptr)
        {
            if (curr->netno == netno)
            {
				//cout<<" getval "<<curr->val<<endl; 
                return curr->val;
            }
            curr = curr->next;
        }
	}
	void traverse()
    {
        net1* curr = head;
        while (curr != nullptr)
        {
            cout << curr->netno << " value is "<<curr->val<<endl;
            curr = curr->next;
        }
        cout << endl;

    }
	void traversedeep()
	{
		net1* curr = head;
		while(curr!=nullptr)
		{
			cout<<"NET # "<<curr->netno<<endl;
			cout<<"Input to the following gates: ";
			for(auto i:curr->gateip)
			{
				cout<<i<<", ";
			}
			cout<<endl;
			cout<<"Output for the following gates: ";
			for(auto i:curr->gateop)
			{
				cout<<i<<", ";
			}
			cout<<endl;
			curr = curr->next;
		}
	}
    vector<int> net_dependency(int netno)
    {
        net1* curr = head;
		while(curr!=nullptr)
		{
			if(curr->netno == netno)
            {
                return curr->gateip;
            }
			
			curr = curr->next;
		}
    }
	vector<int> netoutput(int netno)
	{
		net1* curr = head;
		while(curr!=nullptr)
		{
			if(curr->netno == netno)
            {
                return curr->gateop;
            }
			
			curr = curr->next;
		}
	}
    

	void backtrack()
    {
        net1* c = head;
        cout<<"Going Forward ";
        while(c->next != nullptr)
        {
            cout<<c->netno<<" ";
            c = c->next;
        }
        cout<<c->netno;
        
        cout<<endl;
        cout<<"BackTracking ";
        while (c!=head)
        {
            cout<<c->netno<<" ";
            c = c->prev;
        }

        
    cout<<c->netno<<endl;

    }


	


private:
	net1* head;

};

vector<vector<int>> gmap = {
	{0,0,1},//and
	{0,1,1,1},//or
	{1,0},//inv
	{1,1,0,0},//nand
	{1,0,0,0},//nor
	{0,1,0},//xor
	{1,0,1},//xnor
	{0,1}//buf

};
//and-0, or-1, inv-2, nand-3, nor-4, xor-5, xnor-6,buf 7
vector<pair<int,int>>faultsinjected;
map<string,vector<int>>netlist;
netll n1;
gatell g1;
netll n2;
netll n3;
int gno=1;
int check_gate(string gate)
{
	if(gate=="AND")
		return 0;
	if(gate =="OR")
		return 1;
	if(gate=="NAND")
		return 3;
	if(gate =="NOR")
		return 4;
	if(gate=="INV")
		return 2;
	if(gate =="XOR")
		return 5;
	if(gate=="XNOR")
		return 6;
	if(gate == "BUF")
		return 7;
	if(gate=="INPUT")
		return 8;
	if(gate =="OUTPUT")
		return 9;
}
void buildgatelist(vector<int>g,string gt)
{
	if(gt == "INV" || gt == "BUF")
	{

		g1.insert(check_gate(gt),gno,g[0],-1,g[1],false);
		
		gno++;
	}
	else
	{
		g1.insert(check_gate(gt),gno,g[0],g[1],g[2],true);
		gno++;
	}
}
void buildnetlist(int maxn)
{
	for(int i=1;i<=maxn;i++)
	{
		
		vector<int>gi,go;
		vector<pair<int,int>>a;
		int src=0,dst=0;
		
		a = g1.searchnet(i);
		
		for(auto j:a)
		{
			if(j.second == 1)
				gi.push_back(j.first);
			else
				go.push_back(j.first);
		}
		for(auto j:netlist["INPUT"])
		{
			if(j == i)
				src =1;
		}
		for(auto j:netlist["OUTPUT"])
		{
			if(j == i)
				dst =1;
		}

		n1.insert(i,src,dst,0,0,0,-1,gi,go);
		
	}

}
vector<string> break_string_by_spaces(string input_string) {
  vector<string> output_list;
  string current_word = "";
  for (char c : input_string) {
    if (c == ' ') {
      if (current_word != "") {
        output_list.push_back(current_word);
        current_word = "";
      }
    } else {
      current_word += c;
    }
  }
  if (current_word != "") {
    output_list.push_back(current_word);
  }
  return output_list;
}
int eval_test(int gno)
{
	vector<int>r = g1.return_nets(gno);
	if((g1.is_single_ip(gno))&&((n1.getval(r[0]) == -1)))
		return -1;
	else
	{
		if((n1.getval(r[0]) == -1) || (n1.getval(r[1])==-1))
			return -1;
	}
	return 0;
	
}

int evaluate(int gateno)
{
    vector<int>r = g1.return_nets(gateno);
    if(g1.is_single_ip(gateno))
    {
        if(n1.getval(r[0]) == -1)
            return -1;
        int typ = g1.get_gate_type(gateno);
		
        //cout<<"ip1 is "<<r[0]<<endl;
		//cout<<" gno: "<<gateno<<" typ is "<<typ<<endl;
        n1.setval(r[2],gmap[typ][n1.getval(r[0])]);
    }
    else
    {
		
        if((n1.getval(r[0]) == -1) || (n1.getval(r[1])==-1))
            return -1;
		
        int typ = g1.get_gate_type(gateno);
        
		int ip1,ip2;
		
		ip1 = n1.getval(r[0]);
		ip2 = n1.getval(r[1]);
		
        //cout<<"here "<<ip1<<" "<<ip2<<endl;
		n1.setval(r[2],gmap[typ][ip1+ip2]);
		
		//cout<<n1.getval(r[2])<<endl;
    }
	return 0;
}
int evaluateN2(int gateno)
{
    vector<int>r = g1.return_nets(gateno);
    if(g1.is_single_ip(gateno))
    {
        if(n2.getval(r[0]) == -1)
            return -1;
        int typ = g1.get_gate_type(gateno);
		
        //cout<<"ip1 is "<<r[0]<<endl;
		//cout<<" gno: "<<gateno<<" typ is "<<typ<<endl;
        n2.setval(r[2],gmap[typ][n2.getval(r[0])]);
    }
    else
    {
		
        if((n2.getval(r[0]) == -1) || (n2.getval(r[1])==-1))
            return -1;
		
        int typ = g1.get_gate_type(gateno);
        
		int ip1,ip2;
		
		ip1 = n2.getval(r[0]);
		ip2 = n2.getval(r[1]);
		
        //cout<<"here "<<ip1<<" "<<ip2<<endl;
		n2.setval(r[2],gmap[typ][ip1+ip2]);
		
		//cout<<n1.getval(r[2])<<endl;
    }
	return 0;
}
vector<pair<int,int>>ListSubtraction(vector<pair<int,int>>l1, vector<pair<int,int>>l2)
{   
    vector<pair<int,int>> ans;
    for(auto i:l1)
    {
        int flag =0;
        for(auto j:l2)
        {
            if((i.first == j.first) &&(i.second == j.second) )
                flag=1;

        }
        if(flag ==0)
        {
            ans.push_back(i);
        }
    }
    return ans;
}
vector<pair<int,int>>listunion(vector<pair<int,int>>l1, vector<pair<int,int>>l2)
{
    vector<pair<int,int>>ans;
    int ar[10000] = {0};
	int val[10000] = {-1};
	int samear[10000] = {0};
	int sameval[10000] = {-1};
    for(auto i:l1)
    {
        if(ar[i.first]==0)
		{
			ar[i.first] = 1;
			val[i.first] = i.second;
			ans.push_back(i);
		}
		else
		{
			if(i.second!=val[i.first])
			{
				if(samear[i.first]==0)
				{
					samear[i.first]=1;
					sameval[i.first] = i.second;
					ans.push_back(i);
				}
				
			}
		}
		
        
    }
        
    for(auto j:l2)
    {
		if(ar[j.first]==0)
		{
			ar[j.first]=1;
			val[j.first] = j.second;
			ans.push_back(j);
		}
		else
		{
			if((val[j.first]!=j.second)&&(samear[j.first]==0))
			{
				samear[j.first]=1;
				sameval[j.first] = j.second;
				ans.push_back(j);	
			}
		}
    }
    return ans;

}
vector<pair<int,int>>listinstersection(vector<pair<int,int>>l1, vector<pair<int,int>>l2)
{
    vector<pair<int,int>>ans;
    int ar1[10000] = {0};
    int ar2[10000] = {0};
	int val1[10000] = {-1};
	int val2[10000] = {-1};
	int samear1[10000] = {0};
    int samear2[10000] = {0};
	int sameval1[10000] = {-1};
	int sameval2[10000] = {-1};
    for(auto i:l1)
    {
		if(ar1[i.first]==0)
        {
			ar1[i.first] = 1;
			val1[i.first] = i.second;
		}
		else
		{
			if((val1[i.first]!=i.second) && (samear1[i.first]==0))
			{
				samear1[i.first]=1;
				sameval1[i.first] = i.second;
			}
		}
    }
    for(auto j:l2)
    {
		if(ar2[j.first]==0)
        {
			ar2[j.first] = 1;
			val2[j.first] = j.second;
		}
		else
		{
			if((val2[j.first]!=j.second)&&(samear2[j.first]==0))
			{
				samear2[j.first]=1;
				sameval2[j.first] = j.second;
			}
		}
	}
    for(auto i:l1)
    {
        if(ar2[i.first]==1)
        {
			if(samear2[i.first]==0)
			{
				if(i.second == val2[i.first])
				{
					ans.push_back(i);
				}
			}
			else
			{
				if((i.second == val2[i.first])||(sameval2[i.first] == i.second))
					ans.push_back(i);
				
			}
        }
    }

    return ans;
}   
vector<pair<int,int>>XORlist(vector<pair<int,int>>l1, vector<pair<int,int>>l2)
{
	vector<pair<int,int>>x,y,z;
	x = ListSubtraction(l1,l2);
	y = ListSubtraction(l2,l1);
	z = listunion(x,y);
	return z;
}
vector<pair<int,int>> singlefaultlist(pair<int,int>x)
{
	vector<pair<int,int>>ans;
	ans.push_back(x);
	return ans;
}
vector<vector<pair<int,int>>>evalaute_fault_list(int gatetype,int gateno,vector<vector<pair<int,int>>>flist1)
{
	vector<vector<pair<int,int>>>flist = flist1;
	int nid1,nid2,opid;
	int n1val,n2val,opval;
	
	vector<int>netdep;
	netdep = g1.return_nets(gateno);
	if(gatetype==2||gatetype==7)
	{
		nid1 = netdep[0];
		opid = netdep[2];
		
	}
	else
	{
		nid1 = netdep[0];
		nid2 = netdep[1];
		opid = netdep[2];
		
	
	}
	n1val =n1.getval(nid1);
	if(gatetype!=2&&gatetype!=7)
		n2val =n1.getval(nid2);
	opval =(1- n1.getval(opid));
	if(gatetype==2||gatetype==7)
	{
		//cout<<"Fault list for "<<nid1<<endl;
		for(auto j:flist[nid1])
		{
			//cout<<j.first<<"/"<<j.second<<" ";
		}
		//cout<<endl;
	}
	else
	{
		//cout<<"Fault list for "<<nid1<<endl;
		for(auto j:flist[nid1])
		{
			//cout<<j.first<<"/"<<j.second<<" ";
		}
		//cout<<endl;
		//cout<<"Fault list for "<<nid2<<endl;
		for(auto j:flist[nid2])
		{
			//cout<<j.first<<"/"<<j.second<<" ";
		}
		//cout<<endl;
	}
	//cout<<"N1 val: "<<n1val<<" N2 val:  "<<n2val<<" OPVAL:  "<<opval<<" Gate type is: "<<gatetype<<endl;
	
	switch(gatetype)
	{
		case 0: if(n1val==1)
				{
					if(n2val==1)
					{
						flist[opid] = listunion(listunion(flist[nid1],flist[nid2]),singlefaultlist(make_pair(opid,opval)));
					}
					else
					{
						flist[opid] = listunion(ListSubtraction(flist[nid2],flist[nid1]),singlefaultlist(make_pair(opid,opval)));
					}
				}
				else
				{
					if(n2val==1)
					{
						flist[opid] = listunion(ListSubtraction(flist[nid1],flist[nid2]),singlefaultlist(make_pair(opid,opval)));
					}
					else
					{
						flist[opid] = listunion(listinstersection(flist[nid2],flist[nid1]),singlefaultlist(make_pair(opid,opval)));
					}
				}
				break;
		case 1: if(n1val==0)
				{
					if(n2val==0)
					{
						flist[opid] = listunion(listunion(flist[nid1],flist[nid2]),singlefaultlist(make_pair(opid,opval)));
					}
					else
					{
						flist[opid] = listunion(ListSubtraction(flist[nid2],flist[nid1]),singlefaultlist(make_pair(opid,opval)));
					}
				}
				else
				{
					if(n2val==0)
					{
						flist[opid] = listunion(ListSubtraction(flist[nid1],flist[nid2]),singlefaultlist(make_pair(opid,opval)));
					}
					else
					{
						flist[opid] = listunion(listinstersection(flist[nid2],flist[nid1]),singlefaultlist(make_pair(opid,opval)));
					}
				}
				break;
		case 2: flist[opid] = listunion(flist[nid1],singlefaultlist(make_pair(opid,opval)));
				break;
		case 3: if(n1val==1)
				{
					if(n2val==1)
					{
						flist[opid] = listunion(listunion(flist[nid1],flist[nid2]),singlefaultlist(make_pair(opid,opval)));
					}
					else
					{
						flist[opid] = listunion(ListSubtraction(flist[nid2],flist[nid1]),singlefaultlist(make_pair(opid,opval)));
					}
				}
				else
				{
					if(n2val==1)
					{
						flist[opid] = listunion(ListSubtraction(flist[nid1],flist[nid2]),singlefaultlist(make_pair(opid,opval)));
					}
					else
					{
						flist[opid] = listunion(listinstersection(flist[nid2],flist[nid1]),singlefaultlist(make_pair(opid,opval)));
					}
				}
				break;
		case 4: if(n1val==0)
				{
					if(n2val==0)
					{
						//cout<<"gate no "<<gateno<<" "<<nid1<<" "<<nid2<<endl;
						
					
						vector<pair<int,int>>temp = singlefaultlist(make_pair(opid,opval));
						//cout<<"Fault list for temp "<<endl;
						for(auto j:temp)
						{
							//cout<<j.first<<"/"<<j.second<<" ";
						}
						//cout<<endl;
						vector<pair<int,int>>temp2 = listunion(flist[nid1],flist[nid2]);
						//cout<<"Fault list for temp 2"<<endl;
						for(auto j:temp2)
						{
							//cout<<j.first<<"/"<<j.second<<" ";
						}
						//cout<<endl;
						flist[opid] = listunion(temp,temp2);
						//cout<<"Fault list for opid"<<endl;
						for(auto j:flist[opid])
						{
							//cout<<j.first<<"/"<<j.second<<" ";
						}
						//cout<<endl;
					}
					else
					{
						flist[opid] = listunion(ListSubtraction(flist[nid2],flist[nid1]),singlefaultlist(make_pair(opid,opval)));
					}
				}
				else
				{
					if(n2val==0)
					{
						flist[opid] = listunion(ListSubtraction(flist[nid1],flist[nid2]),singlefaultlist(make_pair(opid,opval)));
					}
					else
					{
						flist[opid] = listunion(listinstersection(flist[nid2],flist[nid1]),singlefaultlist(make_pair(opid,opval)));
					}
				}
				break;
		case 5: flist[opid] = listunion(XORlist(flist[nid1],flist[nid2]),singlefaultlist(make_pair(opid,opval)));
				break;
		case 6: flist[opid] = listunion(XORlist(flist[nid1],flist[nid2]),singlefaultlist(make_pair(opid,opval)));
				break;
		case 7: flist[opid] = listunion(flist[nid1],singlefaultlist(make_pair(opid,opval)));
				break;
	
	}
	//cout<<"Fault list for "<<opid<<endl;
		for(auto j:flist[opid])
		{
			//cout<<j.first<<"/"<<j.second<<" ";
		}
		///cout<<endl;
	return flist;
}
int ReturnControllingValue(int gatetype)
{
	int ans = -1;
	if(gatetype==0 || gatetype ==3)
		ans = 0;
	else if(gatetype == 1 || gatetype ==4)
		ans = 1;
	return ans;
}
int ReturnInversion(int gatetype)
{
	int ans = 0;
	if(gatetype>=2 && gatetype<=4)
		return 1;
	return ans;
}
bool CheckPI(int netno)
{
	vector<int>pis = netlist["INPUT"];
	for(auto i:pis)
	{
		if(netno==i)
			return true;
	}
	return false;

}
vector<int>DFrontier;



int main()
{
	
    int maxnetno =0;

	ifstream input_file("inputckt.txt");

    string line;
    while (getline(input_file, line)) {
    //cout<<line<<endl;
	
    vector<int>t;
    vector<string> words = break_string_by_spaces(line);
	//cout<<words[0]<<endl;
	int no_of_gates = 0;
    
    
      if(words[0] =="INV" || words[0]=="BUF")
		{
			int i,o;
			
			i = stoi(words[1]);
            o = stoi(words[2]);
			if(i>maxnetno)
				maxnetno = i;
			if(o>maxnetno)
				maxnetno = o;
			t.push_back(i);
			t.push_back(o);
		}
        else if(words[0] =="INPUT" || words[0]=="OUTPUT")
        {
            for(int i=1;i<words.size();i++)
            {
                if(stoi(words[i])!=-1)
                    t.push_back(stoi(words[i]));
            }
            netlist[words[0]] = t;
        }
		else
		{
			int i1,i2,o;
            i1 = stoi(words[1]);
            i2 = stoi(words[2]);
            o = stoi(words[3]);
			if(i1>maxnetno)
				maxnetno = i1;
			if(i2>maxnetno)
				maxnetno = i2;
			if(o>maxnetno)
				maxnetno = o;
			t.push_back(i1);
			t.push_back(i2);
			t.push_back(o);
		}
        if(words[0]!="INPUT" && words[0]!="OUTPUT")
		{
			buildgatelist(t,words[0]);
			no_of_gates++;
		}
		if(words[0]=="OUTPUT")
			break;
            
    
  }
    
	buildnetlist(maxnetno);
	n1.setmaxnet(maxnetno);
	n2 = n1;
	//n1.traversedeep();
	//cout<<"Traversing N2: \n";
	//n2.traverse();
	
    ifstream test("test.txt");
    vector<int>tst;
    string t;
    getline(test,t);
    
    for(int i=0;t[i]!='\0';i++)
    {
        
        tst.push_back(t[i]-'0');
    }
    
    

    for(int i=0;i<netlist["INPUT"].size();i++)
    {
        n1.setval(netlist["INPUT"][i],tst[i]);
    }
	//g1.traverse();
    //n1.traverse();
	//n1.traversedeep();
    vector<int>gi;
    stack<int>st;
	queue<int>q;
	int ar[1000]={0};
	//push all gates which have inputs as PI in the stack
    for(auto it:netlist["INPUT"])
    {
        gi = n1.net_dependency(it);
        for(auto i:gi)
		{
			if(ar[i]==0)
			{
				//q.push(i);
				st.push(i);
				ar[i]++;
			}
		}
            
    }
	//n1.traverse();
	vector<int>qq;
	vector<int>qqi;
	stack<int>st2 = st;
	//print all gates whose ips are PIs
	while(st2.size()>0)
	{
		//cout<<st2.top()<<" ";
		st2.pop();
	}
	//cout<<endl;
	while(st.size()>0)
	{
		
		vector<int>gop;
		//eval test checks if ip val is -1, if true, push in
		//Queue, it's not ready for evaluation.
		if(eval_test(st.top())==-1)
		{
			
			ar[st.top()]=0;// make eval =0 or not ready
			qq.push_back(st.top());
			
			st.pop();
		}
		else
		{
			
			evaluate(st.top());// evaluate the gate
			int x = g1.getop(st.top());
			//cout<<"gate is "<<st.top()<<" ";
			vector<int>gg1;
			gg1 = g1.return_nets(st.top());
			//cout<<"gate type is: "<<g1.get_gate_type(st.top())<<endl;
			//if((g1.get_gate_type(st.top())==2)||(g1.get_gate_type(st.top())==7))
				//cout<<" gate ip is: "<<gg1[0]<<endl;
			//else
				//cout<<" gate ips are "<<gg1[0]<<" "<<gg1[1]<<endl;
			//cout<<" output is "<<n1.getval(x)<<endl;
			st.pop();
			
			gop = n1.net_dependency(x);
			for(auto i:gop)
			{
				//cout<<i<<endl;
				auto it = find(qq.begin(),qq.end(),i);
				if((ar[i]==0) && (it == qq.end()))
				{
					//cout<<i<<endl;
					st.push(i);
					ar[i]++;
				}
			}

		}
		if(st.size()==0 && qq.size()>0)
		{
			//cout<<"here"<<endl;
			reverse(qq.begin(),qq.end());
			for(auto it:qq)
			{
				if(ar[it]==0)
				{
					st.push(it);
					ar[it]++;
				}
			}
		}
	}


    //n1.traverse();
	
	for(auto i:netlist["OUTPUT"])
	{
		cout<<"Output for net# "<<i<<" is "<<n1.getval(i)<<endl;
	}
	for(auto i:netlist["OUTPUT"])
	{
		cout<<n1.getval(i);
	}
	cout<<endl;
	ifstream fault_file("faults.txt");
	if (!fault_file.is_open()) {
        std::cerr << "Error opening the file.\n";
        return 1; // Return an error code
    }
	
	int netNumber, faultValue;
	string line1;
	while (getline(fault_file, line1)) 
    {
        // Replace g1 and n1 with your gate and net data structures
		int n=0,v=0,x=0;
       	int i=0;
	   	while(i<line1.size())
	   	{
			//std::cout<<"i before x%2 : "<<i<<"  x is: "<<x<<endl;
			if(x%2==0)
			{
				int j =i;
				string num = "";
				while(line1[j]!=' ')
				{
					num+=line1[j];
					j++;
					n = stoi(num);
					
				}
				i = j;
			}
			
			else
			{
				
				v = line1[i] - '0';
				i+=1;

			}
			//std::cout<<"i after else : "<<i<<"  x is: "<<x<<endl;
			
			x++;
			i++;
	  	}
		faultsinjected.push_back(make_pair(n,v));
    }
//Generate Faults: if net n has value v, make error ~v and see if f changes.
	vector<vector<pair<int,int>>>flist(maxnetno+1); // net no, stuck at fault 2 s-a-0 as 2,0
    vector<int>pin  = netlist["INPUT"];
	
	for(int i =0;i<pin.size();i++)
	{
		
		//n1.setval(pin[i],1-n1.getval(pin[i]));
		vector<pair<int,int>>x;
		x.push_back(make_pair(pin[i],1-n1.getval(pin[i])));
		flist[pin[i]] = x;

		

		//printf("Net no: %d val: %d \n",pin[i],n1.getval(pin[i]));
	}
	//go over all the gates which have bith inputs ready: first iterate over gates which have PIs as ips and evaluate their falut list
	//n1.traverse();
	int netready[10000]={0};
	int gr[10000]={0};
	for(auto i:pin)
	{
		//cout<<"fault list for net "<<i<<endl;
		
		for(auto j:flist[i])
		{
			//cout<<j.first<<"/"<<j.second<<" ";
		}
		//cout<<endl;
	}
	for(auto i:pin)
	{
		netready[i] =1;
	}
	stack<int>gatepush;
	for(auto i:pin)
	{
		
		vector<int>gi1;
		gi1 = n1.net_dependency(i);
		for(auto j:gi1)
		{
			if(gr[j]==0)
			{
				gatepush.push(j);
				gr[j]++;
			}
			
		}


	}
	
	vector<int>gateQ;
	while(gatepush.size()>0)
	{
		//cout<<"STACK: ";
		stack<int>t = gatepush;
		while(t.size()>0)
		{
			//cout<<t.top()<<" ";
			t.pop();
		}
		//cout<<endl;
		vector<int>netss;
		netss = g1.return_nets(gatepush.top());
		vector<int>gop;
		int gid1 = g1.get_gate_type(gatepush.top());
		//cout<<"GATE NO IS : "<<gatepush.top()<<endl;
		//cout<<"     gate ips are "<<netss[0]<<" "<<netss[1]<<" O/P is "<<netss[2]<<endl;
		//cout<<"     ip1 ready: "<<netready[netss[0]]<<" ip2 ready: "<<netready[netss[1]]<<endl;
		if(((gid1==2||gid1==7)&&(netready[netss[0]]==0)) || ((gid1!=2 && gid1!=7)&&(((netready[netss[0]]==0))||((netready[netss[1]]==0)))))
		{
			
			gr[gatepush.top()]=0;
			gateQ.push_back(gatepush.top());
			
			gatepush.pop();
		}
		else
		{
			
			flist = evalaute_fault_list(gid1,gatepush.top(),flist);
			int x = g1.getop(gatepush.top());
			netready[x]=1;
			gatepush.pop();
			
			gop = n1.net_dependency(x);
			for(auto i:gop)
			{
				//cout<<i<<endl;
				auto it = find(gateQ.begin(),gateQ.end(),i);
				if((gr[i]==0) && (it == gateQ.end()))
				{
					//cout<<i<<endl;
					gatepush.push(i);
					gr[i]++;
				}
			}

		}
		if(gatepush.size()==0 && gateQ.size()>0)
		{
			//cout<<"here"<<endl;
			reverse(gateQ.begin(),gateQ.end());
			
			//cout<<"QUEUE: ";
			for(auto j:gateQ)
			{
				//cout<<j<<" ";
			}
			//cout<<endl;
			for(auto it:gateQ)
			{
				if(gr[it]==0)
				{
					gatepush.push(it);
					gr[it]++;
				}
			}
		}
	}
	
	int cnt =0;
	for(auto i:flist)
	{
		if(cnt!=0)
		{
		cout<<"fault list for net "<<cnt<<endl;
		
		for(auto j:i)
		{
			cout<<j.first<<"/"<<j.second<<" ";
		}
		cout<<endl;
		}
		cnt++;
		
	}
	int itr = 0;
	vector<vector<pair<int,int>>>outputFaultlist;
	for(auto it:netlist["OUTPUT"])
	{
		//cout<<"op "<<it<<endl;
		vector<pair<int,int>>x;
		for(auto j:flist[it])
		{

			//cout<<"J VAL: "<<endl;
			//cout<<j.first<<"/"<<j.second<<endl;
			//cout<<"FAULTS INJECTED "<<endl;
			for(auto k:faultsinjected)
			{
				//cout<<k.first<<"/"<<k.second<<endl;
				if(j.first == k.first && k.second == j.second )
				{
					x.push_back(j);
				}
			}
		}
		outputFaultlist.push_back(x);
    
    	
	}
	cout<<"\nPrinting Output faults\n";
	cnt =0;
	for(int i=0;i<outputFaultlist.size();i++)
	{
		cout<<"fault list for output: "<<netlist["OUTPUT"][i]<<endl;
		for(auto j:outputFaultlist[i])
		{
			cout<<j.first<<"/"<<j.second<<" ";
		}
		cout<<endl;
	}
	
	
	
	
	
	
	


    test.close();
    input_file.close();

	
	return 0;
}
