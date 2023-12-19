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
#define D 2
#define Dbar 3
#define X -1
using namespace std;
class Gate{
    public: 
    int gateno;
    int gatetype;
    int ip1;
    int ip2;
    int op;
    bool isSingleOutput;
    Gate()
    {
        gateno = -1;
        gatetype = -1;
        op = -1;
        ip1 = -1;
        ip2 = -1;
        isSingleOutput = true;

    }
    void insert(int gno, int gt, int i1, int i2, int o) 
    {
        gateno = gno;
        gatetype = gt;
        ip1 = i1;
        ip2 = i2;
        op = o;
        isSingleOutput = gt == INV_GATE || gt == BUF_GATE;
    }
    
};
class Net{
    public:
        int netno;
        bool isPO;
        bool isPI;
        vector<int>inputToGates;
        vector<int>outputFromGates;
        int val;
        int fault;
        int error;
    Net()
    {
        netno = -1;
        isPO = false;
        isPI = false;
        val = -1;
        fault = -1;
        error = -1;
    }

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
map<string,vector<int>>netlist;
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
vector<Gate>gates;
vector<Net>nets;
vector<Gate>gatefaults;
vector<Net>netfaults;
vector<Net>stucknets;
vector<pair<int,int>>faultinj;
int ReturnInversion(int gatetype)
{
	int ans = 0;
	if(gatetype>=2 && gatetype<=4)
		return 1;
	return ans;
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
int faultVal(int SAtfault,int value)
{
    switch(SAtfault)
    {
        case 0:
                if(value==0)
                    return 0;
                if(value == 1)
                    return D;
                if(value==D)
                    return D;
                if(value == Dbar)
                    return 0;
                break;
        case 1:
                if(value==0)
                    return Dbar;
                if(value == 1)
                    return 1;
                if(value==D)
                    return 1;
                if(value == Dbar)
                    return Dbar;
                break;
    }
}
int eval_test(int gno)
{
	int i1 = gates[gno-1].ip1;
    int i2 = gates[gno-1].ip2;
    int o = gates[gno-1].op;
    int gt = gates[gno-1].gatetype;
    int ival1 = nets[i1-1].val;
    int ival2 = nets[i2-1].val;
    if(gates[gno-1].isSingleOutput)
    {
        if(nets[i1-1].val == -1)
            return -1;
        
    }
	
	else
	{   
        int cv = ReturnControllingValue(gt);
		if(ival1==-1 && ival2 ==-1)
			return -1;
        if((ival1==-1)||(ival2==-1))
        {
            if(ival1!=cv && ival2!=cv)
                return -1;
        }
	}
	return 0;
	
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
int DCalculus_Sim(int ip1,int ip2,int gatetype)
{
    int i1v,i2v;
    if(gatetype==INV_GATE || gatetype==BUF_GATE)
    {
        i1v = nets[ip1-1].val;
        i2v = -1;
    }
    else
    {
        i1v = nets[ip1-1].val;
        i2v = nets[ip2-1].val;
    }
    switch (gatetype)
	{
		case INV_GATE:
				switch(i1v)
                {
                    case 0: return 1;
                            break;
                    case 1: return 0;
                            break;
                    case D: return Dbar;
                            break;
                    case Dbar: return D;
                            break;
                    case X: return X;
                            break;
                }
				break;
		case BUF_GATE:
                return i1v;
				break;
		case AND_GATE: 
				if(i1v ==X && i2v == X)
                    return X;
                if(i1v==0 || i2v == 0)
                    return 0;
                if(i1v==1)
                    return i2v;
                if(i2v == 1)
                    return i1v;
                if(i1v==X)
                    return X;
                if(i2v == X)
                    return X;
                if(i1v==D)
                {
                    if(i2v==Dbar)
                        return 0;
                    if(i2v==D)
                        return D;
                }
                if(i1v==Dbar)
                {
                    if(i2v==Dbar)
                        return Dbar;
                    if(i2v==D)
                        return 0;
                }
                if(i2v==D)
                {
                    if(i1v==Dbar)
                        return 0;
                    if(i1v==D)
                        return D;
                }
                if(i2v==Dbar)
                {
                    if(i1v==Dbar)
                        return Dbar;
                    if(i1v==D)
                        return 0;
                }
				
				break;
		case NAND_GATE:
				
				if(i1v ==X && i2v == X)
                    return X;
                if(i1v==0 || i2v == 0)
                    return 1;
                if(i1v==1)
                {
                    switch(i2v)
                    {
                        case 0: return 1;
                                break;
                        case 1: return 0;
                                break;
                        case D: return Dbar;
                                break;
                        case Dbar: return D;
                                break;
                        case X: return X;
                                break;
                    }
                }    
                if(i2v == 1)
                {
                    switch(i1v)
                    {
                        case 0: return 1;
                                break;
                        case 1: return 0;
                                break;
                        case D: return Dbar;
                                break;
                        case Dbar: return D;
                                break;
                        case X: return X;
                                break;
                    }
                }
                if(i1v==X)
                    return X;
                if(i2v == X)
                    return X;
                if(i1v==D)
                {
                    if(i2v==Dbar)
                        return 1;
                    if(i2v==D)
                        return Dbar;
                }
                if(i1v==Dbar)
                {
                    if(i2v==Dbar)
                        return D;
                    if(i2v==D)
                        return 1;
                }
                if(i2v==D)
                {
                    if(i1v==Dbar)
                        return 1;
                    if(i1v==D)
                        return Dbar;
                }
                if(i2v==Dbar)
                {
                    if(i1v==Dbar)
                        return D;
                    if(i1v==D)
                        return 1;
                }
				
				
				break;
		case OR_GATE: 
				
				if(i1v ==X && i2v == X)
                    return X;
                if(i1v==1 || i2v == 1)
                    return 1;
                if(i1v==0)
                    return i2v;
                if(i2v == 0)
                    return i1v;
                if(i1v==X)
                    return X;
                if(i2v == X)
                    return X;
                if(i1v==D)
                {
                    if(i2v==Dbar)
                        return 1;
                    if(i2v==D)
                        return D;
                }
                if(i1v==Dbar)
                {
                    if(i2v==Dbar)
                        return Dbar;
                    if(i2v==D)
                        return 1;
                }
                if(i2v==D)
                {
                    if(i1v==Dbar)
                        return 1;
                    if(i1v==D)
                        return D;
                }
                if(i2v==Dbar)
                {
                    if(i1v==Dbar)
                        return Dbar;
                    if(i1v==D)
                        return 1;
                }
				
				

				break;
		case NOR_GATE: 
				
				if(i1v ==X && i2v == X)
                    return X;
                if(i1v==1 || i2v == 1)
                    return 0;
                if(i1v==0)
                {
                    switch(i2v)
                    {
                        case 0: return 1;
                                break;
                        case 1: return 0;
                                break;
                        case D: return Dbar;
                                break;
                        case Dbar: return D;
                                break;
                        case X: return X;
                                break;
                    }
                }    
                if(i2v == 0)
                {
                    switch(i1v)
                    {
                        case 0: return 1;
                                break;
                        case 1: return 0;
                                break;
                        case D: return Dbar;
                                break;
                        case Dbar: return D;
                                break;
                        case X: return X;
                                break;
                    }
                }
                if(i1v==X)
                    return X;
                if(i2v == X)
                    return X;
                if(i1v==D)
                {
                    if(i2v==Dbar)
                        return 0;
                    if(i2v==D)
                        return Dbar;
                }
                if(i1v==Dbar)
                {
                    if(i2v==Dbar)
                        return D;
                    if(i2v==D)
                        return 0;
                }
                if(i2v==D)
                {
                    if(i1v==Dbar)
                        return 0;
                    if(i1v==D)
                        return Dbar;
                }
                if(i2v==Dbar)
                {
                    if(i1v==Dbar)
                        return D;
                    if(i1v==D)
                        return 0;
                }
				break;
	default:
		break;
	}

}
int DCalculus(int ip1,int ip2,int gatetype)
{
    
    
    int i1v,i2v;
    if(gatetype==INV_GATE || gatetype==BUF_GATE)
    {
        i1v = netfaults[ip1-1].val;
        i2v = -1;
    }
    else
    {
        i1v = netfaults[ip1-1].val;
        i2v = netfaults[ip2-1].val;
    }
    for(auto j:faultinj)
    {
        if(j.first==ip1)
        {
            netfaults[ip1-1].val= faultVal(j.second,i1v);
           
            i1v = netfaults[ip1-1].val;
        }
        if(j.first==ip2)
        {
            netfaults[ip2-1].val= faultVal(j.second,i2v);
            i2v = netfaults[ip2-1].val;
        }
    }
   
	switch (gatetype)
	{
		case INV_GATE:
				switch(i1v)
                {
                    case 0: return 1;
                            break;
                    case 1: return 0;
                            break;
                    case D: return Dbar;
                            break;
                    case Dbar: return D;
                            break;
                    case X: return X;
                            break;
                }
				break;
		case BUF_GATE:
                return i1v;
				break;
		case AND_GATE: 
				if(i1v ==X && i2v == X)
                    return X;
                if(i1v==0 || i2v == 0)
                    return 0;
                if(i1v==1)
                    return i2v;
                if(i2v == 1)
                    return i1v;
                if(i1v==X)
                    return X;
                if(i2v == X)
                    return X;
                if(i1v==D)
                {
                    if(i2v==Dbar)
                        return 0;
                    if(i2v==D)
                        return D;
                }
                if(i1v==Dbar)
                {
                    if(i2v==Dbar)
                        return Dbar;
                    if(i2v==D)
                        return 0;
                }
                if(i2v==D)
                {
                    if(i1v==Dbar)
                        return 0;
                    if(i1v==D)
                        return D;
                }
                if(i2v==Dbar)
                {
                    if(i1v==Dbar)
                        return Dbar;
                    if(i1v==D)
                        return 0;
                }
				
				break;
		case NAND_GATE:
				
				if(i1v ==X && i2v == X)
                    return X;
                if(i1v==0 || i2v == 0)
                    return 1;
                if(i1v==1)
                {
                    switch(i2v)
                    {
                        case 0: return 1;
                                break;
                        case 1: return 0;
                                break;
                        case D: return Dbar;
                                break;
                        case Dbar: return D;
                                break;
                        case X: return X;
                                break;
                    }
                }    
                if(i2v == 1)
                {
                    switch(i1v)
                    {
                        case 0: return 1;
                                break;
                        case 1: return 0;
                                break;
                        case D: return Dbar;
                                break;
                        case Dbar: return D;
                                break;
                        case X: return X;
                                break;
                    }
                }
                if(i1v==X)
                    return X;
                if(i2v == X)
                    return X;
                if(i1v==D)
                {
                    if(i2v==Dbar)
                        return 1;
                    if(i2v==D)
                        return Dbar;
                }
                if(i1v==Dbar)
                {
                    if(i2v==Dbar)
                        return D;
                    if(i2v==D)
                        return 1;
                }
                if(i2v==D)
                {
                    if(i1v==Dbar)
                        return 1;
                    if(i1v==D)
                        return Dbar;
                }
                if(i2v==Dbar)
                {
                    if(i1v==Dbar)
                        return D;
                    if(i1v==D)
                        return 1;
                }
				
				
				break;
		case OR_GATE: 
				
				if(i1v ==X && i2v == X)
                    return X;
                if(i1v==1 || i2v == 1)
                    return 1;
                if(i1v==0)
                    return i2v;
                if(i2v == 0)
                    return i1v;
                if(i1v==X)
                    return X;
                if(i2v == X)
                    return X;
                if(i1v==D)
                {
                    if(i2v==Dbar)
                        return 1;
                    if(i2v==D)
                        return D;
                }
                if(i1v==Dbar)
                {
                    if(i2v==Dbar)
                        return Dbar;
                    if(i2v==D)
                        return 1;
                }
                if(i2v==D)
                {
                    if(i1v==Dbar)
                        return 1;
                    if(i1v==D)
                        return D;
                }
                if(i2v==Dbar)
                {
                    if(i1v==Dbar)
                        return Dbar;
                    if(i1v==D)
                        return 1;
                }
				
				

				break;
		case NOR_GATE: 
				
				if(i1v ==X && i2v == X)
                    return X;
                if(i1v==1 || i2v == 1)
                    return 0;
                if(i1v==0)
                {
                    switch(i2v)
                    {
                        case 0: return 1;
                                break;
                        case 1: return 0;
                                break;
                        case D: return Dbar;
                                break;
                        case Dbar: return D;
                                break;
                        case X: return X;
                                break;
                    }
                }    
                if(i2v == 0)
                {
                    switch(i1v)
                    {
                        case 0: return 1;
                                break;
                        case 1: return 0;
                                break;
                        case D: return Dbar;
                                break;
                        case Dbar: return D;
                                break;
                        case X: return X;
                                break;
                    }
                }
                if(i1v==X)
                    return X;
                if(i2v == X)
                    return X;
                if(i1v==D)
                {
                    if(i2v==Dbar)
                        return 0;
                    if(i2v==D)
                        return Dbar;
                }
                if(i1v==Dbar)
                {
                    if(i2v==Dbar)
                        return D;
                    if(i2v==D)
                        return 0;
                }
                if(i2v==D)
                {
                    if(i1v==Dbar)
                        return 0;
                    if(i1v==D)
                        return Dbar;
                }
                if(i2v==Dbar)
                {
                    if(i1v==Dbar)
                        return D;
                    if(i1v==D)
                        return 0;
                }
				break;
	default:
		break;
	}
}
void evaluateFault(int gno)
{
    int i1 = gatefaults[gno-1].ip1;
    int i2 = gatefaults[gno-1].ip2;
    int o = gatefaults[gno-1].op;
    int gt = gatefaults[gno-1].gatetype;
    int ival1 = netfaults[i1-1].val;
    int ival2 = netfaults[i2-1].val;
    if(gatefaults[gno-1].isSingleOutput)
    {
        if(ival1 == X)
            return;
        
		
        
        
		
		int res = DCalculus(i1,-1,gt);
        bool flag = false;
        for(auto f:faultinj)
        {
            if(f.first==o && res!=X)
            {
                netfaults[o-1].val = faultVal(f.second,res);
                flag = true;
            }
        }
		
        if(!flag)
            netfaults[o-1].val = res;
       
    }
    else
    {
        bool flag= false;
		if(ival1==X && ival2 ==X)
			return;
        

        
		int res = DCalculus(i1,i2,gt);
        for(auto f:faultinj)
        {
            if(f.first==o && res!=X)
            {
                netfaults[o-1].val = faultVal(f.second,res);
               
                flag = true;
            }
        }
		
        if(!flag)
            netfaults[o-1].val = res;
		
    }
	return;
}
void evaluate(int gno)
{
    int i1 = gatefaults[gno-1].ip1;
    int i2 = gatefaults[gno-1].ip2;
    int o = gatefaults[gno-1].op;
    int gt = gatefaults[gno-1].gatetype;
    int ival1 = nets[i1-1].val;
    int ival2 = nets[i2-1].val;
    int x;
    if(gatefaults[gno-1].isSingleOutput)
    {
        if(ival1 == -1)
            return;
        
		
        
       
		
		int res = DCalculus_Sim(i1,-1,gt);
        
		
        nets[o-1].val = res;
        
    }
    else
    {
		if(ival1==-1 && ival2 ==-1)
			return;
        int cv = ReturnControllingValue(gt);

        
		int res = DCalculus_Sim(i1,i2,gt);
		
        nets[o-1].val = res;
		
    }
    
	return;
}

bool CircuitSim()
{
    int cnt = 0;
    vector<int>gi;
    stack<int>st;
	queue<int>q;
	int ar[1000]={0};
	
    for(auto it:netlist["INPUT"])
    {
        
        

        
        gi = nets[it-1].inputToGates;
        
        for(auto i:gi)
		{
            
			if(ar[i]==0)
			{
				
				st.push(i);
				ar[i]++;
			}
		}
            
    }
    vector<int>qq;
	vector<int>qqi;
	stack<int>st2 = st;
    while(st.size()>0 && cnt<1000)
	{
        
		
		vector<int>gop;
		
		if(eval_test(st.top())==X)
		{
            
			
			ar[st.top()]=0;// make eval =0 or not ready
			qq.push_back(st.top());
			
			st.pop();
		}
		else
		{
            
			
			
			evaluate(st.top());// evaluate the gate
            
            int currgate = st.top();
            int x = gates[st.top()-1].op;
            int i1 = gates[currgate-1].ip1;
            int i2 = gates[currgate-1].ip2;
            int o = gates[currgate-1].op;
            int gt = gates[currgate-1].gatetype;
            int ival1 = nets[i1-1].val;
            int ival2 = nets[i2-1].val;
			
			
			
			
			
				
			
			st.pop();
			gop = nets[x-1].inputToGates;
			
			for(auto i:gop)
			{
				
				auto it = find(qq.begin(),qq.end(),i);
				if((ar[i]==0) && (it == qq.end()))
				{
					
					st.push(i);
					ar[i]++;
				}
			}

		}
		if(st.size()==0 && qq.size()>0)
		{
            
			
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
        cnt++;
	}
    if(cnt>100000)
        return false;
    return true;
}

vector<int>DFrontier;
int Fvals[10000]={-1};
pair<int,int> backtrace(pair<int,int>p)
{
	pair<int,int> ans = make_pair(-1,-1);
	
    
    
	int v = p.second;
	int k = p.first;
	if(k==-1)
		return ans;
	vector<int>output = netfaults[k-1].outputFromGates;
	int itr = 0;
   
    
    
       
    
	while(output.size()>0 && itr<output.size() && !CheckPI(k))
	{
        int currgate = output[itr];
        int i1 = gates[currgate-1].ip1;
        int i2 = gates[currgate-1].ip2;
        int o = gates[currgate-1].op;
        int gt = gates[currgate-1].gatetype;
        int ival1 = netfaults[i1-1].val;
        int ival2 = netfaults[i2-1].val;
        bool flag = false;
       

		
		int i = ReturnInversion(gt);
		
		if(gt == 2 ||gt ==7) 
		{

            
			if(ival1==X)
			{
				v = v^i;
				k = i1;
                flag = true;
				
			}	
            
		}
		else
		{
			
			if(ival1==X)
			{
				v = v^i;
				k = i1;
                flag = true;
				
			}
			else if(ival2==X)
			{
				v = v^i;
				k = i2; 
                flag = true;
				
			}		
            
		}
		if(flag)
		{
			
            output = netfaults[k-1].outputFromGates;
			
			itr =0;
			
		}
        else
            itr++;


	
	}
	if(k!=-1 && CheckPI(k))
	{
		return make_pair(k,v);
	}
    return ans;
}

queue<int>DFrontier_Done;
bool simulateCircuitFaulty() 
{
    int cnt = 0;
    vector<int> pin = netlist["INPUT"];
    int gr[10000] = {0};
    int netready[10000] = {0};
    stack<int> gatepush;
    vector<int> gateQ;

   
   
    
    

    // Initialize the stack with ready input pins
    for(auto i:pin)
	{
		
		vector<int>gi1;
        netready[i]=1;
        gi1 = netfaults[i-1].inputToGates;
		
		for(auto j:gi1)
		{
			if(gr[j]==0)
			{
				gatepush.push(j);
				gr[j]++;
			}
			
		}
	}
    
    bool reenterLoop = false;  // Flag to control re-entering the loop

    do {
        while (!gatepush.empty()&&cnt<1000) {
            int currentGate = gatepush.top();
           
            gatepush.pop();
            bool canprocess = false;
            int i1 = gatefaults[currentGate-1].ip1;
            int i2 = gatefaults[currentGate-1].ip2;
            int o = gatefaults[currentGate-1].op;
            int gt = gatefaults[currentGate-1].gatetype;
            int ival1 = netfaults[i1-1].val;
            int ival2 = netfaults[i2-1].val;
            
            

            if (gatefaults[currentGate-1].isSingleOutput) 
            {
                if(ival1!=X)
                    canprocess = true;
                
            } 
            else
            {
                int CV = ReturnControllingValue(gt);
                if((ival1!=X)&&(ival2!=X))
                {
                    canprocess = true;
                   
                }
                    
                else
                {
                    canprocess = ((ival1 == CV )|| (ival2 == CV));
                }
                
                
            }
            if (canprocess) {
               
                evaluateFault(currentGate);

                int outputNet = o;
                netready[outputNet] = 1;

               
                vector<int> gop = netfaults[outputNet-1].inputToGates;
                for (auto i : gop) {
                    if (gr[i] == 0) {
                        gatepush.push(i);
                        gr[i]++;
                    }
                }
            } else {
                
                gr[currentGate] = 0;
                gateQ.push_back(currentGate);
            }
            cnt++;
        }

        // Process gates in the queue (if any) without re-entering the loop
        if (!reenterLoop && !gateQ.empty()) {
            reenterLoop = true;  // Set the flag to prevent re-entering the loop

            // Reverse the queue to process gates in the correct order
            reverse(gateQ.begin(), gateQ.end());

            // Push gates from the queue to the stack if they are ready
            for (auto it : gateQ) {
                if (gr[it] == 0) {
                    gatepush.push(it);
                    gr[it]++;
                }
            }

            // Clear the queue for the next iteration
            gateQ.clear();
        } else {
            reenterLoop = false;  // Reset the flag if the loop is not re-entered
        }
    } while (reenterLoop);
    if(cnt>1000)
        return false;
    return true;

    // Process gates in the queue (if any) without re-entering the loop
    
    
}

bool imply(int pi, int piv)
{
   
    bool fl = false;
    for(auto j:faultinj)
    {
        if(j.first==pi)
        {
            netfaults[pi-1].val = faultVal(j.second,piv);
            fl = true;
        }
    }
    if(!fl)
    {
        netfaults[pi-1].val = piv;
    }
	
	bool op = simulateCircuitFaulty();
    if(!op)
        return false;
   
    
    
	
	
	return true;


}

void clearInternalNets()
{
	vector<int>inputs = netlist["INPUT"];
	for(auto i:netfaults)
    {
        auto it = find(inputs.begin(),inputs.end(),i.netno);
        if(it==inputs.end())
        {
            i.val = X;
        }
    }
	return;
}





bool errorAtPO()
{
    

	
    
    
    for(auto i:netlist["OUTPUT"])
    {

       
        if(netfaults[i-1].val==D || netfaults[i-1].val==Dbar)
        {
            cout<<"Error Found at output net: "<<i<<endl;
            return true;
        }

        
        
        
    }
   
   
	return false;
}
pair<bool,pair<int,int>>objective()
{
    
    int k,vk;
    for(auto i:faultinj)
    {
        if(netfaults[i.first-1].val==X)
        {
            k = i.first;
            vk = 1-i.second;
            return make_pair(true,make_pair(k,vk));
        }
    }
    bool faultexcited = false;
    for(auto i:faultinj)
    {

        if(netfaults[i.first-1].val!=X )
        {
            if(netfaults[i.first-1].val==D ||netfaults[i.first-1].val==Dbar)
                faultexcited = true;
            
        }
    }
   
    if(!faultexcited)
        return make_pair(false,make_pair(-1,X));
    DFrontier.clear();
    //Updateing the DFrontier with new gates
    for(auto i:gates)
    {
        if(netfaults[i.ip1-1].val==D ||netfaults[i.ip1-1].val==Dbar)
        {
            if(netfaults[i.ip2-1].val==X)
            {
                cout<<"IP1: "<<netfaults[i.ip1-1].val<<" IP2: "<<netfaults[i.ip2-1].val<<endl;
                DFrontier.push_back(i.gateno);
            }
        }
        else if(netfaults[i.ip2-1].val==D ||netfaults[i.ip2-1].val==Dbar)
        {
            if(netfaults[i.ip1-1].val==X)
            {
                cout<<"IP1: "<<netfaults[i.ip1-1].val<<" IP2: "<<netfaults[i.ip2-1].val<<endl;
                DFrontier.push_back(i.gateno);
            }
        }

    }
    for(auto j:DFrontier)
        cout<<j<<" ";
        cout<<endl;
    if(DFrontier.empty())
    {
		
        return make_pair(false,make_pair(-1,-1));
    }
    int currgate = DFrontier[0];
    int ip1v = netfaults[gates[currgate-1].ip1-1].val;
    int ip2v = netfaults[gates[currgate-1].ip2-1].val;
    int cv = ReturnControllingValue(gates[currgate-1].gatetype);
    if(ip1v==X)
    {
        return make_pair(true,make_pair(gates[currgate-1].ip1,1-cv));

    }
    if(ip2v==X)
    {
        return make_pair(true,make_pair(gates[currgate-1].ip2,1-cv));
    }
    
    return make_pair(false,make_pair(-1,-1));
    
}



bool PODEM()
{
    
	vector<int>PIs = netlist["INPUT"];
    
    
    
    
    if(errorAtPO())
        return SUCCESS;
    
    pair<bool,pair<int,int>>g = objective();
    if(g.first==false)
        return false;
    
        


    
    pair<int,int>kvk;
    
    kvk.first = g.second.first;
    kvk.second = g.second.second;
    
    

    
    
    pair<int,int> jvj = backtrace(kvk);
    
    if(jvj.first==-1 && jvj.second == -1)
        return false;
	
    imply(jvj.first,jvj.second);
	
    
    if(PODEM()==SUCCESS)
        return SUCCESS;
	clearInternalNets();
    
  
    imply(jvj.first,1-jvj.second);
	
    
    if(PODEM()==SUCCESS)
        return SUCCESS;
	clearInternalNets();
    //clearerrors();
    imply(jvj.first,-1);
	clearInternalNets();
    //clearerrors();
    
    return FAILURE;
    
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
	
	int n1val,n2val,opval;
	
	vector<int>netdep;
	int nid1 = gates[gateno-1].ip1;
    int nid2 = gates[gateno-1].ip2;
    int opid = gates[gateno-1].op;
    
	
	n1val =nets[nid1-1].val; 
	if(gatetype!=2&&gatetype!=7)
		n2val =nets[nid2-1].val;
	opval =(1- nets[opid-1].val);
    
	
	
	
    
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
						
						
					
						vector<pair<int,int>>temp = singlefaultlist(make_pair(opid,opval));
						
						for(auto j:temp)
						{
							
						}
						
						vector<pair<int,int>>temp2 = listunion(flist[nid1],flist[nid2]);
						
						
						
						flist[opid] = listunion(temp,temp2);
						
						
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
	
        
	return flist;
}
int main()
{
    
    int maxnetno =0;

	ifstream input_file("inputckt.txt");
    int gno=1;
    string line;
    while (getline(input_file, line)) 
    {
        
        
        vector<int>t;
        vector<string> words = break_string_by_spaces(line);
        
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
            string gt = words[0];
            Gate g;
            if(gt == "INV" || gt == "BUF")
            {

                g.insert(gno,check_gate(gt),t[0],-1,t[1]);
                gates.push_back(g);
                
                gno++;
            }
            else
            {
                g.insert(gno,check_gate(gt),t[0],t[1],t[2]);
                gates.push_back(g);
                gno++;
            }
            
            no_of_gates++;
        }
        if(words[0]=="OUTPUT")
            break;
            
        
    }
   
  for(int i=1;i<=maxnetno;i++)
	{
		
		vector<int>gi,go;
		vector<pair<int,int>>a;
		int src=0,dst=0;
		
		for(auto j:gates)
        {
            if(j.ip1==i || j.ip2 == i)
                gi.push_back(j.gateno);
            if(j.op == i)
                go.push_back(j.gateno);
        }
        Net n;
        n.inputToGates = gi;
        n.outputFromGates = go;
        
        for(auto j:netlist["INPUT"])
        {
            if(j==i)
                n.isPI=true;
        }
        for(auto j:netlist["OUTPUT"])
        {
            if(j==i)
                n.isPO = true;
        }
        n.netno = i;
        n.fault = -1;
        n.val = X;
        nets.push_back(n);
		
		

		
    }
    netfaults = nets;
    stucknets = nets;

    gatefaults = gates;
    ifstream test("test.txt");
    vector<int>tst;
    string t;
    getline(test,t);
    
    for(int i=0;t[i]!='\0';i++)
    {
       
        if(t[i]=='-')
        {
            tst.push_back(-1);
            i++;
        }
        else
            tst.push_back(t[i]-'0');
        
    }
    
    
    
    

    
    
    ifstream faults("faults.txt");
	if (!faults.is_open()) 
    {
        std::cerr << "Error: Could not open the fault list file." << std::endl;
        return 1;
    }
	int fault_net, fault_value;
	string line1;
	vector<pair<int,int>>faultsinjected;
    int faultyvals[10000]={-1};
    int fcheck[1000]={0};
    while (getline(faults, line1)) 
    {
        
		int n=0,v=0,x=0;
       	int i=0;
	   	while(i<line1.size())
	   	{
			
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
			
			
			x++;
			i++;
	  	}
		faultsinjected.push_back(make_pair(n,v));
    }
    for(auto i:faultsinjected)
	{	
		int d;
		
        Fvals[i.first]=i.second;
		if(i.second ==0)
			d = 2;
		else
			d = 3;
        
        vector<int>ginputs = nets[i.first-1].inputToGates;
		
		for(auto j:ginputs)
		{
			if(fcheck[j]==0)
			{
				DFrontier.push_back(j);
				fcheck[j]++;
			}
			
		}
    }
    for(auto i:faultsinjected)
    {
        faultinj.push_back(i);
        
    }
    for(auto i:stucknets)
    {
        if(i.fault!=1)
            i.fault = 0;
    }
   
   
    
	bool ans = PODEM();
    std::cout<<"PODEM RESULTS: "<<endl;
    string a = ans?"SUCCESS":"FAILURE";
    std::cout<<a<<endl;
    
    
    vector<pair<int,int>>podinputs;
    for(auto i:netlist["INPUT"])
    {
        int v;
        
        if(netfaults[i-1].val==D ||netfaults[i-1].val==Dbar )
        {
            
            v = 3 - netfaults[i-1].val;
        }
        else
            v = netfaults[i-1].val;
        podinputs.push_back(make_pair(i,v));
    }
   std::cout<<"PODEM INPS: \n";
    
    
    cout<<endl;
	cout<<"Net Value\n";
    for(auto i:podinputs)
    {
        if(i.second!=X)
        {
            nets[i.first-1].val = i.second;
        }
        else
            nets[i.first-1].val = 0;
        
    }

    CircuitSim();
    for(auto i:podinputs)
    {
        cout<<i.first<<"    ";
        if(i.second==X)
            cout<<'X'<<endl;
        else    
            cout<<i.second<<endl;

    }
    vector<vector<pair<int,int>>>flist(maxnetno+1); // net no, stuck at fault 2 s-a-0 as 2,0
    vector<int>pin  = netlist["INPUT"];
    cout<<"Test Vector: \n";
	for(auto i:podinputs)
    {
       
        if(i.second==X)
            cout<<'X';
        else    
            cout<<i.second;

    }
    cout<<endl;
	for(int i =0;i<pin.size();i++)
	{
		
		
		vector<pair<int,int>>x;
		x.push_back(make_pair(pin[i],1-nets[pin[i]-1].val));
		flist[pin[i]] = x;

		

		
	}
	
	int netready[10000]={0};
	int gr[10000]={0};
	
	for(auto i:pin)
	{
		netready[i] =1;
	}
	stack<int>gatepush;
	for(auto i:pin)
	{
		
		vector<int>gi1;
		gi1 = nets[i-1].inputToGates;
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
		
		stack<int>t = gatepush;
		while(t.size()>0)
		{
			
			t.pop();
		}
		
		vector<int>netss;
		
        int i1 = gates[gatepush.top()-1].ip1;
        int i2 = gates[gatepush.top()-1].ip2;
        
		vector<int>gop;
		int gid1 = gates[gatepush.top()-1].gatetype;
		
		if(((gid1==2||gid1==7)&&(netready[i1]==0)) || ((gid1!=2 && gid1!=7)&&(((netready[i1]==0))||((netready[i2]==0)))))
		{
			
			gr[gatepush.top()]=0;
			gateQ.push_back(gatepush.top());
			
			gatepush.pop();
		}
		else
		{
			
			flist = evalaute_fault_list(gid1,gatepush.top(),flist);
			int x = gates[gatepush.top()-1].op;
			netready[x]=1;
			gatepush.pop();
			
			gop = nets[x-1].inputToGates;
			for(auto i:gop)
			{
				
				auto it = find(gateQ.begin(),gateQ.end(),i);
				if((gr[i]==0) && (it == gateQ.end()))
				{
					
					gatepush.push(i);
					gr[i]++;
				}
			}

		}
		if(gatepush.size()==0 && gateQ.size()>0)
		{
			
			reverse(gateQ.begin(),gateQ.end());
			
			
			
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
	
	
	int itr = 0;
	vector<vector<pair<int,int>>>outputFaultlist;
	for(auto it:netlist["OUTPUT"])
	{
		
		vector<pair<int,int>>x;
        
		for(auto j:flist[it])
		{
           
			for(auto k:faultsinjected)
			{
				
				if(j.first == k.first && k.second == j.second )
				{
					x.push_back(j);
				}
			}
		}
       
		outputFaultlist.push_back(x);
    
    	
	}
    
    
	cout<<"\nPrinting Output faults\n";
	bool flag = false;
	for(int i=0;i<outputFaultlist.size();i++)
	{
		cout<<"fault list for output: "<<netlist["OUTPUT"][i]<<endl;
        if(!outputFaultlist[i].empty())
            flag = true;
		for(auto j:outputFaultlist[i])
		{
			cout<<j.first<<"/"<<j.second<<" ";
		}
		cout<<endl;
	}
    if(!flag)
        cout<<"Undetectable!"<<endl;
return 0;
}
