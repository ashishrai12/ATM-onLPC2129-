#include<LPC21xx.h>
#include<stdio.h>
#include<math.h>
char PIN[]="Enter Pin:",rx;
char welcome[]="Welcome to MyBank\nOptions:\n1.Balance\n2.Withdraw\n3.Deposit\n4.Previous Transaction\n5.Change PIN";
char ContinueS[]="\nDo you wish to Continue?[y/n] ";
char Insuffice[]="\nInsufficient funds :(";
char NewPINS[]="\nEnter new pin:";
char debit[]="\nDebited: Rs.",credit[]="\nCredited: Rs.";
char Success[]="\nSuccess";
char Amount[]="\nPlease Enter the amount:";
char Current[]="Current Balance: Rs.";
int getAmount(void);
char rxbyte(void);
void PreviousT(void),Clear(void),Balance(void),txbyte(char rx);
void Withdraw(void),ChangePIN(void),Deposit(void);
int checkPIN(void),Continue(void);
double balance=20000,LastW=0,LastD=0,temp_bal=0;
int i=0,temp=0,flag=1,masterPin[]={'1','2','3','4'};
int main()
{
int a=0;
PINSEL0=0X0005; //tx rx
U0LCR=0x9B;
U0DLL=0X61;
U0DLM=0X00;
U0LCR=0X1B;
while(1)
{	
	flag=checkPIN();
	while(flag==0)
	{
	txbyte(8);
	for(a=0;a<sizeof(welcome);a++)
	{
		txbyte(welcome[a]);
	}
	txbyte(8);
	rx=rxbyte();
	
	if(rx=='1')
	{
		Clear();
		Balance();
		flag=Continue();
	}
	else if(rx=='2')
	{
		Clear();
		Withdraw();
		flag=Continue();
	}
	else if(rx=='3')
	{
		Clear();
		Deposit();
		flag=Continue();
	}
	else if(rx=='4')
	{
		Clear();
		PreviousT();
		flag=Continue();
	}
	else if(rx=='5')
	{
		Clear();
		ChangePIN();
		flag=Continue();
	}
	else
	{
		Clear();
	}}}}
void PreviousT(void)
{
int bT=0,go=0;
if(LastW==0 && LastD==0)
{
	txbyte('N');
	txbyte('A');
}		
else if(LastW==0)
{
	for(i=0;i<sizeof(credit);i++)
		txbyte(credit[i]);
	for(i=sizeof(LastD);i>=0;i--)
	{
				bT=(LastD/pow(10,(i)));
				bT=bT%10;				
				if(bT!=0||go==1)
				{
				txbyte(bT+48);
				go=1;
				}}
}
else if(LastD==0)
{
	for(i=0;i<sizeof(debit);i++)
		txbyte(debit[i]);
	for(i=sizeof(LastW);i>=0;i--)
	{
				bT=(LastW/pow(10,(i)));
				bT=bT%10;	
				if(bT!=0||go==1)
				{
				txbyte(bT+48);
				go=1;
				}}}}
void ChangePIN(void)
{
	int check=0,pin;
	check=checkPIN();
	if(check==0){
		
	for(i=0;i<sizeof(NewPINS);i++)
		txbyte(NewPINS[i]);
	for(i=0;i<4;i++)
	{
		pin=rxbyte();
		if(pin<48||pin>58)
			{
				Clear();
				break;
			}
		masterPin[i]=pin;
		txbyte('*');
	}
	for(i=0;i<sizeof(Success);i++)
		txbyte(Success[i]);		
}
}
void Balance(void)
{	
	int go=0;
	for(i=0;i<sizeof(Current);i++)
	txbyte(Current[i]);
	for(i=sizeof(balance);i>=0;i--)
	{
				temp=(balance/pow(10,(i)));
				temp=temp%10;				
				if(temp!=0||go==1)
				{
				txbyte(temp+48);
				go=1;
				}
	}
	rxbyte();
	Clear();
}
void Deposit(void)
{
	LastD=getAmount();
	LastW=0;
	balance=balance+LastD;
	Clear();
}
void Withdraw(void)
{
	temp_bal=getAmount();
	if(temp_bal>balance)
	{
		for(i=0;i<sizeof(Insuffice);i++)
		txbyte(Insuffice[i]);
		rxbyte();
	}
	else
	{
  LastW=temp_bal;
	LastD=0;
	balance=balance-LastW;
	}
	Clear();	
}

int getAmount(void)
{
	int a=0;
  for(a=0;a<sizeof(Amount);a++)
	{
		txbyte(Amount[a]);
	}	
	txbyte(8);
	temp_bal=0;
	for(a=0;(temp!=13);a++)
	{
		if(a<=sizeof(double))
		{
			temp_bal=temp_bal*10;
			temp=rxbyte();
		
			if(temp<48||temp>58)
			{
				if(temp!=13)
					temp_bal=0;
				Clear();
				break;
			}
			if(a!=sizeof(double))
			txbyte(temp);
			temp_bal=temp_bal+(temp-48);
		}
		else
		temp=rxbyte();
	}
	temp=0;
	return temp_bal/10;
}
void Clear(void)
{
	for(i=0;i<7;i++)
	txbyte('\n');
}
int checkPIN(void) //return 0 if correct
{
	int check=0;
	for(i=0;i<sizeof(PIN);i++)
	{
		txbyte(PIN[i]);
	}
	for(i=0;i<4;i++)
	{
		if(rxbyte()!=masterPin[i])
				check=1;
		txbyte('*');
	}
	Clear();
	return check;
}
int Continue(void)
{
	for(i=0;i<sizeof(ContinueS);i++)
		txbyte(ContinueS[i]);
	rx=rxbyte();
	Clear();
	if(rx=='y'||rx=='Y')
	return 0;
	return 1;
}
char rxbyte(void)
{
	while(!(U0LSR&0x01));
	rx=U0RBR;
	return(rx);
}
void txbyte(char rx)
{
  while(!(U0LSR&0x20));
  U0THR=rx;
}