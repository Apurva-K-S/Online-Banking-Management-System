#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <string.h>
#include "client_util.h"
#define PORT 43458
#define size 1000

int main()
{
	if(findSize()==-1)
	{
		struct customer c1;
		printf("create admin\nenter username:");
		scanf("%s", c1.username);
		printf("enter password:");
		scanf("%s", c1.password);
		c1.accno = 0;
		c1.type = 2;
		c1.user_status = 1;
		addAdmin(c1);
	}

	struct customer c1;
	struct account a1;
	
	struct sockaddr_in server, client;    
	int socket_desc, size_client, client_desc,temp, acno;    
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);    
	server.sin_family = AF_INET;    
	server.sin_addr.s_addr = INADDR_ANY;    
	server.sin_port = htons(PORT);    
	
	char user[size], pass[size], sample[size];
	
	if(bind(socket_desc, (void *)(&server), sizeof(server)) < 0)    
	{        
		perror("Error on binding:");        
		exit(EXIT_FAILURE);    
	}    
	
	listen(socket_desc, 5);    
	while(1)
	{        
		
		size_client = sizeof(client);        
		if((client_desc = accept(socket_desc, (struct sockaddr*)&client,&size_client)) < 0) 
		{            
			perror("Error on accept:");            
			exit(EXIT_FAILURE);        
		}        
		if(fork() == 0)
		{
			while(1)
			{
				
				int choice, buff;
				read(client_desc, &choice, sizeof(choice));
				choice = ntohl(choice); 
				
				read(client_desc, user, sizeof(user));  // reading username
				
				if(choice==0 || choice==2)
					buff = isUserValid(user);   		// username validation.
				else
					buff = isJointUserValid(user);	// for joint user.			
							
				//printf("%d", buff);
				buff=htonl(buff);
				write(client_desc, &buff, sizeof(buff));		// sending client whether the user is valid or not.. valid -> 1 not valid -> 0
				
				read(client_desc, pass, sizeof(pass));	// reading password
				
				if(choice==0 || choice==2)
					buff = isPasswordValid(user,pass);    // password validation and finding the type of user it is. 0 -> not valid, 10-> normal user, 11->joint user, 12->admin
				else
					buff = isJointPasswordValid(user,pass);			
								
				buff=htonl(buff);
				
				write(client_desc, &buff, sizeof(int));	// sending validation information to client.
				//printf("%d\n", buff);
				
				buff=ntohl(buff);
				//printf("%d", buff);				// 12 -> admin...  10 -> single .. 11 -> joint.
				
				struct customer c3;
				struct account a;
				
				if(buff==10)			// single user.
				{
					int flag=0;
					while(1)
					{
						read(client_desc, &buff, sizeof(buff));	// read choice inside admin
						buff=ntohl(buff);
						
						int b, accno, amt, bala;
						switch(buff)
						{
						
							case 1:
								
								a=searchDetailsByName(user);			// search the file for the details.
								//printf("1. server side accountnumber = %d\n", a.accno);
								
								int stat = a.acc_status;
								//printf("2. server acc status = %d\n", stat);
								
								stat = htonl(stat);
								write(client_desc, &stat, sizeof(stat));	// writing account status to the client.
								//perror("socket error: ");
								
								stat = ntohl(stat);
								//printf("status = %d\n", stat);
								
								
								if(stat != 0)
								{
									int accntno=a.accno;
									accntno=htonl(accntno);
									write(client_desc, &accntno, sizeof(accntno));		// write accnt number to client.
									
									
									read(client_desc, &amt, sizeof(amt));			// read amt from client.
									amt = ntohl(amt);
									
									//printf("3. server side accno = %d\namount = %d\n", a.accno, amt);
									
									deposit(a.accno, amt);
								}		
								break;
								
							case 2:
								
								read(client_desc, &b, sizeof(b));
								b=ntohl(b);
								
								a=searchDetailsByName(user);
								
								
								bala=a.balance;
								bala=htonl(bala);
								write(client_desc, &bala, sizeof(bala));		// write balance to client.
								
								bala = ntohl(bala);
								
								if(b<=bala)
								{
									accno = a.accno;
									withdraw(accno,b);				// b => amount to withdraw, bala => present balance in account.
								}
								
								break;
							
							case 3:
								a=searchDetailsByName(user);			// search the file for the details.
								
								int accntno=a.accno;
								accntno=htonl(accntno);
								write(client_desc, &accntno, sizeof(accntno));		// write accnt number to client.
								
								bala=a.balance;
								bala=htonl(bala);
								write(client_desc, &bala, sizeof(bala));		// write balance to client.
								
								break;
							
							case 4:
								
								read(client_desc, sample, sizeof(sample));		// read new password from the user.
								passwordChange(user, sample);
								
								break;
							
							case 5:
								
								c3=viewDetailsByName(user);			// search the file for the details.
								
								write(client_desc, c3.password, strlen(c3.password));	// write password to client.
								
								int acctno=c3.accno;
								
								acctno=htonl(acctno);
								write(client_desc, &acctno, sizeof(acctno));		// write acctno to client.
								
								int type=c3.type;
								type=htonl(type);
								write(client_desc, &type, sizeof(type));		// write type to client.
								
								int ustat=c3.user_status;
								ustat=htonl(ustat);
								write(client_desc, &ustat, sizeof(ustat));		// write type to client.
								
								break;
							case 6:
								
								flag=1;
								break;
						}
						if(flag==1)
							break;
					}
				}
				else if(buff == 11)
				{
					int flag=0;
					while(1)
					{
						read(client_desc, &buff, sizeof(buff));	// read choice inside admin
						buff=ntohl(buff);
						
						int b, accno, amt, bala;
						switch(buff)
						{
						
							case 1:
								
								a=searchDetailsByJointName(user);			// search the file for the details.
								//printf("1. server side accountnumber = %d\n", a.accno);
								
								int stat = a.acc_status;
								//printf("2. server acc status = %d\n", stat);
								
								stat = htonl(stat);
								write(client_desc, &stat, sizeof(stat));	// writing account status to the client.
								//perror("socket error: ");
								
								stat = ntohl(stat);
								//printf("status = %d\n", stat);
								
								
								if(stat != 0)
								{
									int accntno=a.accno;
									accntno=htonl(accntno);
									write(client_desc, &accntno, sizeof(accntno));		// write accnt number to client.
									
									
									read(client_desc, &amt, sizeof(amt));			// read amt from client.
									amt = ntohl(amt);
									
									//accno = ntohl(accno);
									//printf("3. server side accno = %d", a.accno);
									
									deposit(a.accno, amt);
								}		
								break;
								
							case 2:
								
								read(client_desc, &b, sizeof(b));
								b=ntohl(b);
								
								a=searchDetailsByJointName(user);
								
								
								bala=a.balance;
								bala=htonl(bala);
								write(client_desc, &bala, sizeof(bala));		// write balance to client.
								
								bala = ntohl(bala);
								
								if(b<=bala)
								{
									accno = a.accno;
									withdraw(accno,b);				// b => amount to withdraw, bala => present balance in account.
									
								}
								
								break;
							
							case 3:
								a=searchDetailsByJointName(user);			// search the file for the details.
								
								int accntno=a.accno;
								accntno=htonl(accntno);
								write(client_desc, &accntno, sizeof(accntno));		// write accnt number to client.
								
								bala=a.balance;
								bala=htonl(bala);
								write(client_desc, &bala, sizeof(bala));		// write balance to client.
								
								break;
							
							case 4:
								
								read(client_desc, sample, sizeof(sample));		// read new password from the user.
								passwordJointChange(user, sample);
								
								break;
							
							case 5:
								
								c3=viewJointDetailsByName(user);			// search the file for the details.
								
								write(client_desc, c3.password, strlen(c3.password));	// write password to client.
								
								int acctno=c3.accno;
								
								acctno=htonl(acctno);
								write(client_desc, &acctno, sizeof(acctno));		// write acctno to client.
								
								int type=c3.type;
								type=htonl(type);
								write(client_desc, &type, sizeof(type));		// write type to client.
								
								int ustat=c3.user_status;
								ustat=htonl(ustat);
								write(client_desc, &ustat, sizeof(ustat));		// write type to client.
								
								break;
								
							case 6:
								flag=1;
								break;
						}
						if(flag==1)
							break;
					}
				}
				else										// admin.
				{
					int flag=0;
					while(1)
					{
						read(client_desc, &buff, sizeof(buff));	// read choice inside admin
						buff=ntohl(buff);
						
						struct customer c3;
						int b, accno1, accno2, opn, user_type;
						switch(buff)
						{
							case 1:
								// ADD USER
								
								
								read(client_desc, c3.username, sizeof(c3.username)); // username
								read(client_desc, c3.password, sizeof(c3.password)); // password
								
								
								int account_number, type, user_status;
								
								read(client_desc, &account_number, sizeof(account_number)); // accno
								account_number=ntohl(account_number);
								c3.accno=account_number;
								
								read(client_desc, &type, sizeof(type)); // type  0-> single user   1->joint user
								type=ntohl(type); 
								c3.type=type;
								
								
								read(client_desc, &user_status, sizeof(user_status)); // user_status
								user_status=ntohl(user_status);
								c3.user_status=user_status;
								
								//printf("username: %s password: %s accno: %d type: %d status: %d\n", c3.username, c3.password, c3.accno, c3.type, c3.user_status);
								
								
								if(type==0)
									b=addUser(c3);
								else
									b=addJointUser(c3);
								
								
								
								b=htonl(b);				// 0 -> username exists.
								write(client_desc, &b, sizeof(b));
									
								break;
								
							case 2:
								
								// DELETE ACCOUNT DETAILS.
								
								read(client_desc, &user_type, sizeof(user_type));
								user_type=ntohl(user_type);
								
								read(client_desc, &acno, sizeof(acno));
								acno = ntohl(acno);
								//printf("\nAccount number to delete is: %d\n", acno);
								delete_Account(acno, user_type);				// user_type = 0 -> single, 1 -> joint
								
								break;
							
							case 3:
							
								read(client_desc, &user_type, sizeof(user_type));
								user_type=ntohl(user_type);				// user_type 0 -> single  1 -> joint.
							
								read(client_desc, &accno1, sizeof(accno1));		// read original accno from client.
								accno1 = ntohl(accno1);
								
								read(client_desc, &opn, sizeof(opn));		// read opn (0 -> modify account number or 1 -> change status)
							 	opn = ntohl(opn);
							 	
							 	if(opn==0)
							 	{
							 		read(client_desc, &accno2, sizeof(accno2));		// read new accnt number from client.
							 		accno2 = ntohl(accno2);
							 		
							 		modify(accno1, accno2, user_type);
							 	}
							 	else
							 		delete_Account(accno1, user_type);
							 		
								break;
								
							case 4:
								read(client_desc, &b, sizeof(b));  // receive accno from client.
								b=ntohl(b);
								
								//struct account a;
								a=searchDetails(b);			// search the file for the details.
								
								int bala=a.balance;
								bala=htonl(bala);
								write(client_desc, &bala, sizeof(bala));		// write balance to client.
								
								int acc_stat = a.acc_status;
								acc_stat = htonl(acc_stat);
								write(client_desc, &acc_stat, sizeof(acc_stat));	// write account status to client.
								
								break;
							case 5:
								flag=1;
								break;
						}
						
						if(flag==1)
							break;
					
					}
				}	
			}
		}
	}
	
	close(socket_desc);
	return (0);
}
