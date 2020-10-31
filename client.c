#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <netinet/in.h>
#define size 100
#define PORT 43458

int single=1000;
int main()
{
	char buff[size], to_server[size], username[size], password[size];
	int i, q, p,ch, choice, socket_desc; ;
	
	struct sockaddr_in server;    
	   
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);    
	server.sin_family = AF_INET;    
	server.sin_addr.s_addr = INADDR_ANY;    
	server.sin_port = htons(PORT);    
	connect(socket_desc, (void *)(&server), sizeof(server));  
	 
	printf("\n===================Welcome To Online Banking System===================\n");
	
	
	
	printf("You want to login as user(0) or joint(1) or Admin(2): ");
	scanf("%d", &choice);
	choice = htonl(choice);
	write(socket_desc, &choice, sizeof(choice));
	
	
	printf("enter username: ");
	scanf("%s", username);
	 	
	write(socket_desc, username, strlen(username));
	 	
	read(socket_desc, &p, sizeof(p));
	p=ntohl(p);
	 	
	if((p)!=0)
	{
		printf("enter password: ");
		scanf("%s", password);
		 	
		p=htonl(p);
	 	write(socket_desc, password, strlen(password));
	 		
	 	read(socket_desc, &p, sizeof(p));
	 	p=ntohl(p);
	 	
	 	if((p)!=0)
	 	{	
		 	if(p==10)
			{
				printf("\n===================Welcome user: %s===================\n", username);
				
				int flag=0;
				while(1)
				{
					printf("Choose one option:\n1.Deposit\n2.Withdraw\n3.Balance Enquiry\n4.Password change\n5.View details\n6.Exit\n");
					scanf("%d", &ch);
					
					ch=htonl(ch);
					write(socket_desc, &ch, sizeof(ch));			// write choice to server.
					
					ch=ntohl(ch);
					int accno, type, user_status, temp, amt;			
					switch(ch)
					{
						case 1:
							
							printf("\n-----------------DEPOSIT-----------\n");
							int t;
							read(socket_desc, &t, sizeof(t));	// account status 
							//perror("error: ");
							t = ntohl(t);
							//printf("%d\n", t);
							if(t!=0)
							{
								read(socket_desc, &temp, sizeof(temp));	// account number 
								temp = ntohl(temp);
								printf("\nYour Account Number is = %d", temp);
								
								while(1)
								{
									printf("\nEnter amount(>=0) to deposit: ");
									scanf("%d", &amt);
									if(amt>=0)
										break;
								}							
								
								amt = htonl(amt);
								write(socket_desc, &amt, sizeof(amt));	// send amt to deposit to server.
							}
							else
								printf("Your account has been closed.\nYou cannot Deposit!\n");	
							break;
						
						case 2:
							printf("\n-----------------WITHDRAW-----------\n");
							
							while(1)
							{
								printf("Enter amount(>=0) to withdraw: ");
								scanf("%d", &amt);
								if(amt>=0)
									break;
							}
													
							amt=htonl(amt);
							write(socket_desc, &amt, sizeof(amt));	// sending amount to server.
							
							read(socket_desc, &temp, sizeof(temp));	// balance 
							temp = ntohl(temp);
							amt = ntohl(amt);
							if(amt<=temp)
							{
								printf("withdrawn\n");
							}
							else
							{	
								printf("\nInsufficient Funds!!\nYour balance is %d\n", temp);
							}
							break;
						
						case 3: 
							printf("\n-----------------BALANCE ENQUIRY-----------\n");
							printf("%s\n", username);
							
							read(socket_desc, &temp, sizeof(temp));	// account number 
							temp = ntohl(temp);
							printf("Account Number is = %d\n", temp);
							
							read(socket_desc, &temp, sizeof(temp));	// balance 
							temp = ntohl(temp);
							printf("balance is = %d\n", temp);
							
							break;
							
						case 4:
							printf("\n-----------------PASSWORD CHANGE-----------\n");
							printf(" Password Change \n ");
							printf("Enter new password: ");
							scanf("%s", buff);
							
							write(socket_desc, buff, sizeof(buff));	// send new password to server.
							
							break;
							
							
						case 5:
							printf("\n-----------------VIEW DETAILS-----------\n");
							printf("%s's Account Details are\n", username);
							
							read(socket_desc, buff, sizeof(buff));	// Password 
							printf("Password= %s\n", buff);
							
							read(socket_desc, &temp, sizeof(temp));	// account number. 
							temp = ntohl(temp);
							printf("Account number= %d\n", temp);
							
							read(socket_desc, &temp, sizeof(temp));	// type of customer. 
							temp = ntohl(temp);
							//printf("type of customer: %d\n", temp);
							
							if(temp==0)
								printf("Normal user\n");
							else if(temp==1)
								printf("Joint account\n");
								
							read(socket_desc, &temp, sizeof(temp));	// account status. 
							temp = ntohl(temp);
							if(temp==0)
								printf("Account inactive\n");
							else
								printf("Account is active\n");
							
							break;
						
						case 6:
							printf("\n-----------------EXIT-----------\n");
							printf("Exiting.....\n");
							flag=1;
							break;
						
					}
					printf("\n");
					if(flag==1)
						break;
				}
				
			}
			else if(p==11)
			{
				printf("\n===================Welcome Joint user: %s===================\n", username);
				
				int flag=0;
				while(1)
				{
					printf("Choose one option:\n1.Deposit\n2.Withdraw\n3.Balance Enquiry\n4.Password change\n5.View details\n6.Exit\n");
					scanf("%d", &ch);
					
					ch=htonl(ch);
					write(socket_desc, &ch, sizeof(ch));			// write choice to server.
					
					ch=ntohl(ch);
					int accno, type, user_status, temp, amt;			
					switch(ch)
					{
						case 1:
							
							printf("\n-----------------DEPOSIT-----------\n");
							int t;
							read(socket_desc, &t, sizeof(t));	// account status 
							//perror("error: ");
							t = ntohl(t);
							
							if(t!=0)
							{
								read(socket_desc, &temp, sizeof(temp));	// account number 
								temp = ntohl(temp);
								printf("\nYour Account Number is = %d", temp);
								
								while(1)
								{
									printf("\nEnter amount(>=0) to deposit: ");
									scanf("%d", &amt);
									if(amt>=0)
										break;
								}
								amt = htonl(amt);
								write(socket_desc, &amt, sizeof(amt));	// send amt to deposit to server.
							}
							else
								printf("Your account has been closed.\nYou cannot Deposit!\n");	
							break;
						
						case 2:
							printf("\n-----------------WITHDRAW-----------\n");
							
							while(1)
							{
								printf("Enter amount(>=0) to withdraw: ");
								scanf("%d", &amt);
								if(amt>=0)
									break;
							}
							amt=htonl(amt);
							write(socket_desc, &amt, sizeof(amt));	// sending amount to server.
							
							read(socket_desc, &temp, sizeof(temp));	// balance 
							temp = ntohl(temp);
							amt = ntohl(amt);
							if(amt<=temp)
							{
								printf("withdrawn\n");
							}
							else
							{	
								printf("\nInsufficient Funds!!\nYour balance is %d\n", temp);
							}
							break;
						
						case 3: 
							printf("\n-----------------BALANCE ENQUIRY-----------\n");
							printf("%s\n", username);
							
							read(socket_desc, &temp, sizeof(temp));	// account number 
							temp = ntohl(temp);
							printf("Account Number is = %d\n", temp);
							
							read(socket_desc, &temp, sizeof(temp));	// balance 
							temp = ntohl(temp);
							printf("balance is = %d\n", temp);
							
							break;
							
						case 4:
							printf("\n-----------------PASSWORD CHANGE-----------\n");
							printf(" Password Change \n ");
							printf("Enter new password: ");
							scanf("%s", buff);
							
							write(socket_desc, buff, sizeof(buff));	// send new password to server.
							
							break;
							
							
						case 5:
							printf("\n-----------------VIEW DETAILS-----------\n");
							printf("%s's Account Details are\n", username);
							
							read(socket_desc, buff, sizeof(buff));	// Password 
							printf("Password= %s\n", buff);
							
							read(socket_desc, &temp, sizeof(temp));	// account number. 
							temp = ntohl(temp);
							printf("Account number= %d\n", temp);
							
							read(socket_desc, &temp, sizeof(temp));	// type of customer. 
							temp = ntohl(temp);
							//printf("type of customer: %d\n", temp);
							
							if(temp==0)
								printf("Normal user\n");
							else if(temp==1)
								printf("Joint account\n");
								
							read(socket_desc, &temp, sizeof(temp));	// account status. 
							temp = ntohl(temp);
							if(temp==0)
								printf("Account inactive\n");
							else
								printf("Account is active\n");
							
							break;
						
						case 6:
							printf("\n-----------------EXIT-----------\n");
							printf("Exiting.....\n");
							flag=1;
							break;
						
					}
					printf("\n");
					if(flag==1)
						break;
				}
			}
			else 
			{
				printf("\n===================Welcome admin: %s===================\n", username);
				
				int flag=0;
				while(1)
				{
					printf("choose one option:\n1.Add Account Details\n2.Delete Account Details\n3.Modify Account Details\n4.Search Account Details\n5. exit\n");
					scanf("%d", &ch);
					
					ch=htonl(ch);
					write(socket_desc, &ch, sizeof(ch));
					
					ch=ntohl(ch);
					int accno, type, user_status, temp, opn, user_type;
					switch(ch)
					{
						case 1: 
							printf("\n-----------------ADD USER-----------\n");
							
							printf("enter name of user to add: ");
							scanf("%s", username);
							write(socket_desc, username, sizeof(username));
							printf("enter password of user to add: ");
							scanf("%s", password);
							write(socket_desc, password, sizeof(password));
							
							printf("username: %s password: %s\n", username, password);
							
							//printf("enter account number, type(0: normal, 1:joint) and status of user to add: ");
							//scanf("%d %d %d", &accno, &type, &user_status);
							
							printf("\nenter single(0) user or joint user(1): ");
							scanf("%d", &type);
							
							if(type==0)		// single user.
							{
								single++;
								accno = single;
							}
							else				// joint user.
							{
								printf("Please enter account number for Joint Account: ");
								scanf("%d", &accno);
							}						
							
							user_status=1;
							
							accno=htonl(accno);
							write(socket_desc, &accno, sizeof(accno));		// accno to server.
							
							type=htonl(type);
							write(socket_desc, &type, sizeof(type));			// user type to server. 0 -> single 1->joint
							
							user_status=htonl(user_status);
							write(socket_desc, &user_status, sizeof(user_status));   // user status to server.
							
							
							read(socket_desc, &temp, sizeof(temp));		
							temp=ntohl(temp);
							if(temp==0)
								printf("Username already exists\n");
							
							break;
						
						case 2:
							
							printf("\n-----------------DELETE ACCOUNT-----------\n");
							
							printf("Enter single user(0) or joint user(1): ");
							scanf("%d", &user_type);
							user_type=htonl(user_type);
							write(socket_desc, &user_type, sizeof(user_type));	// send user_type to server.
							
							
							printf("enter account number to delete: ");
							scanf("%d", &accno);
							
							accno = htonl(accno);
							write(socket_desc, &accno, sizeof(accno));		// send accno to server.
							
							break;
							
						case 3:
							printf("\n------------------MODIFY ACCOUNT---------------\n");
							
							
							printf("Enter single user(0) or joint user(1): ");
							scanf("%d", &user_type);
							user_type=htonl(user_type);
							write(socket_desc, &user_type, sizeof(user_type));	// send user_type to server.
							
							
							printf("Enter Number of account which you want to modify: ");
							scanf("%d", &temp);
							
							temp = htonl(temp);
							write(socket_desc, &temp, sizeof(temp));			// send account number to server.
							
							printf("You want to modify Account number(0) or\nAccount status(1) (If you choose this then Account will be deleted): ");
							scanf("%d", &opn);
							
							opn = htonl(opn);
							write(socket_desc, &opn, sizeof(opn));			// sending opn to server...> 0 -> modify accnount number 1 -> modify account status.
							
							opn = ntohl(opn);
							if(opn==0)						// change account number to new one.
							{
								printf("enter new account number: ");
								scanf("%d", &accno);
								
								accno=htonl(accno);
								write(socket_desc, &accno, sizeof(accno));		// send this new accno to server.
							}
							
							break;
							
						case 4:
							printf("\n-----------------SEARCH ACCOUNT-----------\n");
							
							printf("enter account number of the user to search the details:");
							scanf("%d", &accno);
							
							accno=htonl(accno);
							write(socket_desc, &accno, sizeof(accno));  // send accno to server 
							
							int bal, as;
							read(socket_desc, &bal, sizeof(bal));	// balance 
							bal = ntohl(bal);
							
							read(socket_desc, &as, sizeof(as));	// account status. 
							as = ntohl(as);
							
							if(as == -10)
							{
								printf("Account Number doesnt exist\n");
							}
							else
							{
								accno = ntohl(accno);
								printf("Account Details\nAccount number = %d\n", accno);
								
								printf("balance= %d\n", bal);
								
								if(as==0)
									printf("Account status= %d(inactive)\n", as);
								else
									printf("Account status= %d(active)\n", as);
							}
							break;
						case 5:
							flag=1;
							break;
					}
					printf("\n");
					if(flag==1)
						break;
				}
			}
		}
		else
		{
			printf("wrong password\nenter details again!!\n");
		}
	 }
	 else
	 {
	 	printf("wrong username\nenter again\n");
	 }
	return (0);
}
