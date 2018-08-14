#include<iostream>
#include<cmath>
#include<sstream>
#include<cstdio>
using namespace std;
template <class T>
class Stack{
private :
    T data[1000];
    int t;
public :
    Stack(){t=0;}
    void clear(){t=0;}
    T pop(){
        if (t<=0) {
            cout<<"illegal expression"<<endl;
            exit(0);
        }
        t--;
        //cout<<"pop "<<data[t]<<endl;
        return data[t];
    }
    void push(T x){
        if(t>=1000){
            cout<<"illegal expression"<<endl;
            exit(0);
        }
        //cout<<"push "<<x<<endl;
        data[t++]=x;
    }
    T top(){
        if(is_empty()){
            cout<<"illegal expression"<<endl;
            exit(0);
        }
        return data[t-1];
    }
    bool is_empty(){
        return t <= 0 ;
    }
};

class Calculator{
private :
    string postfixExp;
    string infixExp;
    double ans;
public :
    Calculator(string s=""){
        infixExp=s;
        ans=0;
    }
    void display();
    double calculate(string infixExp);
    bool infix_to_postfix();
    double  cal_postfix();
    double cal(double ,double ,char);
    string change(double);
    int get_priority(char);
    double read_num(string s,int *pos);
};
void Calculator::display()
{
//    cout<<"last expression is :"<<postfixExp<<endl;
    cout<<"the value of expression is :";
    printf("%.2f\n",ans);
}
double Calculator::calculate(string infixExp){
    this->infixExp= infixExp;
    if (infix_to_postfix()){
        return ans=cal_postfix();
    }else{
        cout<<"error,illegal expression"<<endl;
//        return 0.0;

        exit(0);
    }
}
double Calculator::read_num(string s,int *pos)
{
    double x=0;
    int flag=0,w=0,h=1,i;
    if(s[(*pos)]=='-') {(*pos)++;h=-1;}
    for ( i=(*pos);i<s.length();i++)
        if (s[i]!='#')
            if(s[i]=='.'){
                flag=1;
            }else {
                if(flag) w++;
                x=x*10+s[i]-'0';
            }else  break;
    (*pos)=i;
    return x/pow(10,w)*h;
}
string Calculator::change(double x){
    int y = (int)(x*10000),yy = y%10000;
    stringstream ss;
    ss<<y/10000<<'.'<<yy/1000<<(yy%1000)/100<<(yy%100)/10<<(yy%10);
    string r;
    ss>>r;
    int i;
    for (i=r.length()-1;i>=0;i--)
        if(r[i]!='0') break;
    if (r[i]=='.') i--;
    return  r.assign(r,0,i+1);
}
double Calculator::cal(double num1,double num2,char op)
{
    switch(op){
        case '+':
            return num1+num2;
        case '-':
            return num1-num2;
        case '*':
            return num1*num2;
        case '/':
            if (num2 == 0) {
                cout << "0 can't be devider" << endl;
                exit(0);
            }
            return num1/num2;
        default:
            return 0.0;
    }
}

int Calculator::get_priority(char c){
    switch(c){
        case '+':case '-': return 1;
        case '*':case '/': return 2;
        case '(':return 0;
        case ')':return 0;
        case '#':return -2;
        default:
            return -1;
    }
}
bool Calculator::infix_to_postfix()
{
    Stack<char> stk;
    int a_num_end=0,is_negative=0;
    string result="#";
    stk.push('#');
    for (int i=0;i<infixExp.length();i++)
    {
        char c = infixExp[i];
        if(c=='-'&&(i==0||infixExp[i-1]=='(')) {result+='-'; is_negative=1;continue;}
        if((c<='9'&&c>='0')||c=='.') {
            if(a_num_end&&!is_negative) {result+='#'; }
            is_negative=0;
            result+=c;
            a_num_end=0;
        }
        else{a_num_end=1;
            if(c=='(') stk.push('(');
            else if (c==')') {
                while (stk.top()!='(')
                {
                    result+='#';
                    result+=stk.pop();
                }
                stk.pop();
            }
            else {
                int p = get_priority(c);
                if (p==-1) return false;
                while (p<=get_priority(stk.top())){
                    result+='#';
                    result+=stk.pop();
                }
                stk.push(c);
            }
        }
    }
    while (stk.top()!='#'){
        result+='#';
        result+=stk.pop();
    }
    result+='#';
//    cout<<result<<endl;
    postfixExp = result;
    return true;
}
double Calculator::cal_postfix()
{
    int l = postfixExp.length();
    Stack<double> stk1;
    for (int i=1;i<l;i++){
        char c = postfixExp[i];
        if (c=='#') continue;
        if((c=='-'&&postfixExp[i+1]!='#')||(c>='0'&&c<='9')) stk1.push(read_num(postfixExp,&i));
        else {
            double y  = stk1.pop();
            double x = stk1.pop();
            stk1.push(cal(x,y,c));
        }
    }
    return stk1.pop();
}
int main()
{
    Calculator c;
    string ini;
    while (true){
        cout<<"please input the expression"<<endl;
        cin>>ini;
        if(ini.length()==1&&ini[0]=='Q') break;
        c.calculate(ini);
        c.display();
    }
    //c.calculate("(-5)*10+20/20-2*3*6/36");
    //c.display();
    return 0;
}
