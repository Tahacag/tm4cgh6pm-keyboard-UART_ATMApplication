#include "TM4C123GH6PM.h"
#include <stdio.h>
#include <TM4C123.h>     
#include <tm4c123gh6pm.h>                       // This header for register definitions .
#include <stdint.h>

#define  RowsSize  4                            // Row and Coloums number. 
#define  ColsSize  4


struct Current_Account;                         // Structure to use bank operations.             
struct Account ;
struct Money_Send_Accoun;


int AccountID_Control(char* ID_Adress , char * Person1_ID , char * Person2_ID ,char *Person3_ID  );
int Password_Control(int Passwordcode ,char * password, char * person1_password , char * person2_password , char *person3_password);
int  Password_Comparison(char * currentpassword , char * personpassword);
void Take_IDNumber(void);
void Take_Menustate(void);
char keypad_getkey(void);
void Delay(unsigned long counter);                                    // Declarations  of funcitons that we need. 
void UART5_Initialization(void);
void Transmit(unsigned char data);     
void Print_As_String(char *strng);
char keypadgetkey(unsigned char);
void keypad_Init(void);
char Take_Cancelcommand(void);
void Take_Number(void);
void Take_Password(void);
int  ClosePassword_Control(char* enterednew_password, char* current_password );
void delay_us(int n);
void delay_ms(int n);

// Variables to control or count states.
int Save_Code;  
int Sending_Control;
int Password_Code =0 ;
int Enter_Code=0;
char menu_state [1];
int second_counter ;
int Password_Counter = 2;
// Variables to take  values.
char ID_Writer[7];
char Cancel_Writer[1];
char Password_Writer[4];
char Number_Writer[6];   

// Variables to store values.
int Given_Moneyint;
char Given_Moneychar;
int x;
int Sending_Value ;
unsigned char money[40];




struct Current_Account{

unsigned char name[5]  ;
unsigned char account_number[6];
unsigned char password[4];
int bill;
};
struct Account {

unsigned char Account_Number[6];
unsigned char Account_Password[4];
unsigned char Account_Name[5];
int Account_Balance ;
};

struct Money_Send_Account {
unsigned char Account_Number[6];
unsigned char Account_Password[4];
unsigned char Account_Name[5];
int Account_Balance ;
};



unsigned  char symbol[RowsSize][ColsSize] = {{ '1', '2',  '3', 'A'},      
                                                { '4', '5',  '6', 'B'},      
                                                { '7', '8',  '9', 'C'},      
                                                { '*', '0',  '#', 'D'}};


unsigned int integer_value;

// Main.
int main(void)
{
// Thease are accunts definitions using structure
struct Current_Account  Customer ;
struct Account Person1 ,Person2 ,Person3;
struct Money_Send_Account Receiver_Customer ;
  strcpy(Person1.Account_Name,"Roberto");
	strcpy(Person1.Account_Number,"1234567");
	strcpy(Person1.Account_Password,"7894");
	Person1.Account_Balance =1000;
	 strcpy(Person2.Account_Name,"Gonzales");
	strcpy(Person2.Account_Number,"4554568");
	strcpy(Person2.Account_Password,"3666");
	Person2.Account_Balance =1000;
	 strcpy(Person3.Account_Name,"Ricardo");
	strcpy(Person3.Account_Number,"7894544");
	strcpy(Person3.Account_Password,"1594");
	Person3.Account_Balance =1000;


 // Uart5 module and keypad conncetions initializations functions.   
UART5_Initialization();
keypad_Init();
	  	
	
    while(1)
    {	
		LOOP1:                                             // In order to return the begining after exit from a accunt.
		Password_Code = 0;
		Enter_Code=0;	
    Print_As_String("\n\r *****************TAHA Bank*****************");
	  Print_As_String("\n\r ******************WELCOME*****************");
	  while(Password_Code == 0 || Enter_Code==0){

		// This places take ID number ,then, it controls using a funciton which accunt has this number or not.After ,
		// it identify and control their password using another funciton taking password.
		// Also,the counter is counting false password numbers.
		// Using while for break tools , loops and continuities provied.
	  Print_As_String("\n\r***Please Enter your Account Number:");
		Take_IDNumber();
    Password_Code =AccountID_Control(ID_Writer , Person1.Account_Number ,Person2.Account_Number ,Person3.Account_Number  );
		if(Password_Code==0){
		Print_As_String("\n\r This is not valid account number.Try again...");
		continue ;}
		for(Password_Counter=3;Password_Counter>0;Password_Counter--){
		Print_As_String(" \n\r Please enter the Password ... ");
		Take_Password();
    Enter_Code=Password_Control(Password_Code , Password_Writer,Person1.Account_Password,Person2.Account_Password,Person3.Account_Password);	
		
		// After controls , this state loading the entered accunt to another structure to realize operations on it .
    if(Enter_Code!=0){break;}
		if(Password_Counter==1){
		Print_As_String("\n\r The Bank account is bloked by the system...");
		}
		}}
		if(Password_Code==1){
	  	
	  strcpy(Customer.name,Person1.Account_Name);
	  Customer.bill=Person1.Account_Balance;
		strcpy(Customer.password ,Person1.Account_Password);
		strcpy(Customer.account_number,Person1.Account_Number);
		Save_Code = 1 ;
		}
		else if(Password_Code==2){
	
	  strcpy(Customer.name,Person2.Account_Name);
	  Customer.bill=Person2.Account_Balance;
		strcpy(Customer.password ,Person2.Account_Password);
		strcpy(Customer.account_number,Person2.Account_Number);	
		Save_Code=2;
		}
		else if(Password_Code==3){
	
	  strcpy(Customer.name,Person3.Account_Name);
	  Customer.bill=Person3.Account_Balance;
		strcpy(Customer.password ,Person3.Account_Password);
		strcpy(Customer.account_number,Person3.Account_Number);
		Save_Code=3;
		}
		
		else{Print_As_String("\n\r The system error...");}
	  
		// This state is begining of the menu process.
    Print_As_String("\n\r Welcome to TAHA Bank Mr/Mrs");
		Print_As_String("\n");
		
		while(1){
			LOOP2:                               // This is to return main menu when a case is done continuousluy.
		Print_As_String("\n\r Menu :\r\n1)Withdraw to Account \r\n2)Depozit from Account \r\n3)Change Password \r\n4)Money Transfer \r\n5)Exit");
		Take_Menustate();
    Delay(1000);
		switch (menu_state[0]){
			
			case '0':
				goto LOOP2;
			break;
			
		
			case '1' :
			while(1){
			// Withraw operaitons .
			Print_As_String("\n\r The value must be multiples of 10");
			Print_As_String("\n\r Please enter the value that you want to get:");
			  Take_Number();
			 x = atoi(Number_Writer);
			if(x%10!=0){
			Print_As_String("\n\r Please enter the valid value .It must be multiples of 10.");
			continue ;
			}
			else {
			// Control cases.
			if(x>=Customer.bill){
			Print_As_String("\n\r There is no requested money.It will given possible value by the system.\n");	
			Customer.bill=0;
			Print_As_String("\n\r All money in account is giving");
			sprintf(money,"\n\r The Current balance : %d",Customer.bill);	
			Print_As_String(money);
			
			}
			else if (x<Customer.bill){
			Print_As_String("\n\r Please wait....:\r\n");
			Customer.bill=Customer.bill-(x) ;
			Print_As_String("\n\r Money was given...\r\n");
			sprintf(money,"\n\r The Current balance : %d",Customer.bill);
			Print_As_String(money);
			}
			 break ;}}
			// To return main menu , menu state is taken as 0.
			menu_state[0]=0;
			  break ;
			
			case '2' :
				// This case to depozit money to accunt.
			  // Depozit number is taken then it is coverted to integer value .The last , it is added to balance .
				Print_As_String("/n/r Please... Enter the money value to depozit to our accunt...");
			  Take_Number();
			  x = atoi(Number_Writer);
			  Delay(3000);
			  Print_As_String("/n/r Counter is counting the money .Please wait...");
			  Delay(10000);
			  Print_As_String("\n\r Depozited value is ");
        Print_As_String(Number_Writer);
			  // Current balance is updating.
        Customer.bill=Customer.bill + x;
			  sprintf(money,"\n\r :The current value :%d",Customer.bill);	
			  Print_As_String("\n\r");
    		Print_As_String(money);	
			  // To return main menu , menu state is taken as 0.
				menu_state[0]=0;
			  break ;
			case '3' :
				 // This satate in order to change current accunt password .
			   // The case control the current password again also.
			   // After controlling , customer can determine their new password .
				 Print_As_String("\n\r Please ...Enter the current password.");
			   while(1){
				 Take_Password();
					if(Password_Comparison(Password_Writer,Customer.password)){
					Print_As_String("\n\r The current password is true ....\n ");
					Delay(2000);
				  while(1){
					Print_As_String("\n\r  Enter the new password : ");
					Take_Password();
					if(ClosePassword_Control(Password_Writer,Customer.password)>=2){
					Print_As_String("\n\r The entered new password is so close to the current password.");
					Print_As_String("\n\r Please ,Enter more specific password number.");}
					else {break;}}
					Delay(2000);
					strcpy(Customer.password,Password_Writer);
					Print_As_String("\n\r Password is changed....\n/r ");
					Print_As_String("\n\r New password is ");
					Print_As_String(Customer.password);
					
					break ;
					}
				  else{
					Print_As_String(" \n\r  False password...Please try again...");
					}}
				 // To return main menu , menu state is taken as 0.
				 menu_state[0]=0;
			    break;
			case '4':
				
			while(1){
				// This satate to send money from current accunt to another accunt that is endrolled in system.
				Print_As_String("\n\r Please enter account number of the person to whom the money will be sent:");
			  Take_IDNumber();
				// This is to control which accunt is got money or not .
				Sending_Control=AccountID_Control(ID_Writer , Person1.Account_Number , Person2.Account_Number ,Person3.Account_Number);
			  if(atoi(Customer.account_number)==atoi(ID_Writer)){
				Print_As_String("\n\r This is your account number .Please enter different account number");}
		
				else {
				if(Sending_Control==0){
				Print_As_String("\n\r False account number.. Try again... ");
				continue ;
				}	
					
		// It sends recevier accunt information like balance to make operation .
    // Also , it conrol which customer account will take the sended value.				
		else if(Sending_Control==1){
	  	
	  strcpy(Receiver_Customer.Account_Name,Person1.Account_Name);
	  Receiver_Customer.Account_Balance=Person1.Account_Balance;
		strcpy(Receiver_Customer.Account_Password ,Person1.Account_Password);
		strcpy(Receiver_Customer.Account_Number,Person1.Account_Number);
		
		}
		else if(Sending_Control==2){
	
	   strcpy(Receiver_Customer.Account_Name,Person2.Account_Name);
	  Receiver_Customer.Account_Balance=Person2.Account_Balance;
		strcpy(Receiver_Customer.Account_Password ,Person2.Account_Password);
		strcpy(Receiver_Customer.Account_Number,Person2.Account_Number);}
		else if(Sending_Control==3){
	
	  strcpy(Receiver_Customer.Account_Name,Person3.Account_Name);
	  Receiver_Customer.Account_Balance=Person3.Account_Balance;
		strcpy(Receiver_Customer.Account_Password ,Person3.Account_Password);
		strcpy(Receiver_Customer.Account_Number,Person3.Account_Number);
		}
			break ;	}}
	
		while(1){
		// Value is given in this state.
		Print_As_String("\n \r Please .. Enter the value that you want to send:");
		Take_Number();
		// Current account is controlled in case of balance .
		if(atoi(Number_Writer)>Customer.bill){
		Print_As_String("\n \r There is no money enough to send ..Try again ...");
		}
		else{
		Sending_Value=atoi(Number_Writer);
		Print_As_String("\n\r Transaction is process...");
		Print_As_String("\n\r End of 30 second , opetaiton will be done . ");
		Print_As_String("\n\r if you want to cancel this process please push long  the '*' button in end of the 30 seconds . ");
		Print_As_String("\n\r if you are ready, Please press the '#' button..");
		// This states to count down.
		for (second_counter = 0; second_counter<30 ; second_counter++){
	  	
		sprintf(money,"\r \n %d second left",(30-second_counter));
		Print_As_String(money);
		
		Delay(2000);
		}
		// Decision satate.
		if(Take_Cancelcommand()=='*'){
		
		Print_As_String("\n\r The process is cancelled .....");
		break;
		}
	  }
	
		Receiver_Customer.Account_Balance =Receiver_Customer.Account_Balance+Sending_Value ;
		Customer.bill = Customer.bill-Sending_Value;
	
		// This state sends the money really make copy to recevier structor data to real person structor. 
		if(Sending_Control==1){
	  Person1.Account_Balance=Receiver_Customer.Account_Balance;
		}
		else if(Sending_Control==2){
    Person2.Account_Balance=Receiver_Customer.Account_Balance;}
		else if(Sending_Control==3){
	  Person3.Account_Balance=Receiver_Customer.Account_Balance;
		}
		Print_As_String("\n\r The operation is done... ");
	
		break;
		}
		// This is to return main menu.
		menu_state[0]=0;
		break;
			case '5' :
		// Exit state.
				Print_As_String("\n\r Exit..... Thanks for choosing TAHA Bank...");
			  Print_As_String("\n\r Data is savingg...");
			  Delay(5000);
		// All information is coppied the real acc1unt variables this state.
    // Also , it is controlled which accunt is updated.			
		if(Save_Code==1){
	  	
	  strcpy(Person1.Account_Name,Customer.name);
	  Person1.Account_Balance=Customer.bill;
		strcpy(Person1.Account_Password,Customer.password );
		strcpy(Person1.Account_Number,Customer.account_number);
		
		}
		else if(Save_Code==2){
	
	  strcpy(Person2.Account_Name,Customer.name);
	  Person2.Account_Balance=Customer.bill;
		strcpy(Person2.Account_Password,Customer.password );
		strcpy(Person2.Account_Number,Customer.account_number);}
		else if(Save_Code==3){
	
	  strcpy(Person3.Account_Name,Customer.name);
	  Person3.Account_Balance=Customer.bill;
		strcpy(Person3.Account_Password,Customer.password );
		strcpy(Person3.Account_Number,Customer.account_number);
		}
		goto LOOP1;    // This is to take all process to begining.
			  break;
			default:
				break;
		}
			
		
		
		}
                                                          
    }
}


void UART5_Initialization(void)
{   SYSCTL->RCGCUART |= 0x20;  /* enable clock to UART5 */
    SYSCTL->RCGCGPIO |= 0x10;  /* enable clock to PORTE for PE4/Rx and RE5/Tx */
                            
    UART5->CTL = 0;         /* UART5 module disbable */
    UART5->IBRD = 104;      /* for 9600 baud rate, integer = 104 */
    UART5->FBRD = 11;       /* for 9600 baud rate, fractional = 11*/
    UART5->CC = 0;          /*select system clock*/
    UART5->LCRH = 0x60;     /* data lenght 8-bit, not parity bit, no FIFO */
    UART5->CTL = 0x301;     /* Enable UART5 module, Rx and Tx */

    /* UART5 TX5 and RX5 use PE4 and PE5. Configure them digital and enable alternate function */
    GPIOE->DEN = 0x30;      /* set PE4 and PE5 as digital */
    GPIOE->AFSEL = 0x30;    /* Use PE4,PE5 alternate function */
    GPIOE->AMSEL = 0;    /* Turn off analg function*/
    GPIOE->PCTL = 0x00110000;     /* configure PE4 and PE5 for UART */
}
void Transmit(unsigned char Data)  
{
    while((UART5->FR & (1<<5)) != 0);           //wait until Tx buffer not full *           
    UART5_DR_R= Data;                                           
}

void Print_As_String(char *strg)
{
  while(*strg)                                  // This is to say begining adress to trasmitt
	{
		Transmit(*(strg++));
}
}
 

void Delay(unsigned long counter)
{                                               // Normal  Delay .
	unsigned long i = 0;
	for(i=0; i< counter*1000; i++);
}


void keypad_Init(void)
{
  SYSCTL->RCGCGPIO |= 0x09;             //Enable clock to PORTA and PORTD  .
  while ((SYSCTL->RCGCGPIO&0x09)==0);   //Wait for clock to be setted
  GPIO_PORTA_CR_R  |= 0xF0;             //Allow settings for all pins of PORTA  
  GPIO_PORTD_CR_R  |= 0xFF;             //Allow settings for all pins of PORTD
  GPIO_PORTD_DIR_R |= 0x00;
  GPIO_PORTA_DIR_R |= 0xFF;	            //PA4-PA7 are used with row and set them as digital output pins
  GPIO_PORTD_PDR_R |= 0xFF;          
  GPIO_PORTA_DEN_R |= 0xF0;             //Set PORTA as digital pins
  GPIO_PORTD_DEN_R |= 0xFF;             //Set PORTD as digital pins
}

char keypad_getkey(void)
{while(1)
  {
		int i =0;
	  int j =0;
    for( i = 0; i < 4; i++)             //Scan columns loop
    {
      GPIOA->DATA = (1U << i+4);
      delay_us(2);
      for( j = 0; j < 4; j++)           //Scan rows
      {
        if((GPIOD->DATA & 0x0F) & (1U << j))
				{ return symbol[j][i];}
      }}}}

// Function can take password and store to password variables.
void Take_Password(void){                
	int i ;
for(i=0 ; i<4;i++){
Delay(1000);
Transmit(keypad_getkey());
Password_Writer[i]= keypad_getkey();
}}

// Funciton take menu state and store to menu state variable.
void Take_Menustate(void){
	int i ;
for(i=0 ; i<1;i++){
Delay(1000);
Transmit(keypad_getkey());
menu_state[i]= keypad_getkey();
}}

// Function can take cancel comman and store their variable.
char Take_Cancelcommand(void){
	Delay(1000);
Cancel_Writer[0]= keypad_getkey();	
return Cancel_Writer[0];
}

// Function can take ID number and store their variables.
void Take_IDNumber(void){
	int i ;
for(i=0 ; i<7;i++){
Delay(1000);
Transmit(keypad_getkey());
ID_Writer[i]= keypad_getkey();
}}

// Function can take any numbet to use and store their variables.
void Take_Number(void){
	int i ;
for(i=0 ; i<5;i++){
Delay(1000);
Transmit(keypad_getkey());
Number_Writer[i]= keypad_getkey();
}}


// Function can control whivh ID adress is entered to access or not.
int AccountID_Control(char* ID_Adress , char * Person1_ID , char * Person2_ID ,char *Person3_ID ){ //777777
int j , w=0 ;
// There are some controllin parts for each adress index to index.
for(j=0;j<6;j++){
if(ID_Adress[j]!= Person1_ID[j]){
w=0;
break ; }
else{w =1;}
}
for(j=0;j<6;j++){
if(ID_Adress[j]!= Person2_ID[j]){
if(w==1){break ;}
else {w=0; break ; }}      
else{w =2;}
}
for(j=0;j<6;j++){
if(ID_Adress[j]!= Person3_ID[j]){
if(w==2 ||w== 1){break;}
else {w=0; break ; }}
else{w=3;}
}
if(w==0){
Print_As_String("\n\rFalse Account Number ....Please try again.");
}
return w ;
}

// Function can control what entered password of entered ID number is true or false.
int Password_Control(int Passwordcode ,char * password, char * person1_password , char * person2_password , char *person3_password){
int i , w;
if(Passwordcode == 1){
for(i=0;i<4;i++){
if(password[i]!=person1_password[i]){
w=0;
break ;}
else{w=1;}
	}}
if(Passwordcode==2){
for(i=0;i<4;i++){
if(password[i]!=person2_password[i]){
if(w==1){break ;}
else{w=0; break ;}}
else{w=2;}
	}}
if(Passwordcode==3){
for(i=0;i<4;i++){
if(password[i]!=person3_password[i]){
if(w==2||w==1){break ;}
else{w=0;break ;}}
else{w=3;}
	}}
if(w==0){
Print_As_String("\n\rFalse Password ... Please try again..");
}
return w ;
}

/* Mili seconds delay function */
void delay_ms(int n)
{
 int i,j;
 for(i=0;i<n;i++)
 for(j=0;j<3180;j++)
 {}
}

/* Micro seconds delay function */
void delay_us(int n)
{
 int i,j;
 for(i=0;i<n;i++)
 for(j=0;j<3;j++)
 {}
}
// This is for comparison between current password and new password.
int  Password_Comparison(char * currentpassword , char * personpassword){
	int i , w;
for(i=0;i<4;i++){
if(currentpassword[i]!=personpassword[i]){
w=0;
break ;}
else{w=1;} 
}
return w ;
}
int  ClosePassword_Control(char* enterednew_password, char* current_password ){
int i ,n=0 ;
for(i=0;i<4;i++){
if(enterednew_password[i]==current_password[i]){n+=1;}
else {}}
return n ;
}








void SystemInit(void)
{  
	__disable_irq(); 
  SCB->CPACR |= 0x00F00000;
	
}