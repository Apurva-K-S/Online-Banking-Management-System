#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define size 1000

struct customer{
	char username[size];
	char password[size];
	int accno;
	int type;  // 0 -> normal 1 -> joint 2-> admin
	int user_status;  // 0 -> inactive 1 -> active
};

struct account{

	int accno;
	int balance;
	int acc_status;

};

struct transaction{

	char date[size];
	int accno;
	int amount;
	int balance;		
};

int addUser(struct customer cust) 
{
	
	//printf("username: %s\t password: %s\n", cust.username, cust.password);
	
	int fd=open("customer.txt", O_CREAT | O_RDWR | O_APPEND, 0766);
	struct customer c;
	
	//printf("\n%s\n", cust.username);
	
	int flag1=0;
	while(read(fd, &c, sizeof(c))>0)
	{
		if( strcmp(c.username, cust.username)==0)
		{
			//printf("%s %s\n", c.username, cust.username);
			flag1=1;
		}
	}
	if(flag1!=1)		// username doesnt exist
	{
		struct flock lock;
		lock.l_type = F_WRLCK;	
		lock.l_whence = SEEK_CUR;
		lock.l_start = 0;
		lock.l_len = 0;
		lock.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
		write(fd, &cust, sizeof(cust));
		lock.l_type = F_UNLCK;			// unlocking.
		fcntl(fd, F_SETLK, &lock);		// unlocking.
		
		close(fd);
		
		// -----------locking and writing into customer file-------------------
		
		int fd1=open("Account.txt", O_CREAT | O_RDWR | O_APPEND, 0766);
		struct account a,a1;
		int flag1=0;
		
		while(read(fd1, &a, sizeof(a)) > 0)
		{
			if( (cust.accno-a.accno) == 0)
				flag1=1;
				
		}
		
		if(flag1!=1)
		{
			a1.accno=cust.accno;
			a1.balance = 0;
			a1.acc_status=1;
			write(fd1, &a1, sizeof(a1));	
		}
		
		close(fd1);
		
		return 1;
	}
	else		// username exists.
		return 0;
	close(fd);
}

int addJointUser(struct customer cust) 
{
	
	int fd=open("joint.txt", O_CREAT | O_RDWR | O_APPEND, 0766);
	struct customer c;
	
	//printf("\n%s\n", cust.username);
	
	int flag1=0;
	while(read(fd, &c, sizeof(c))>0)
	{
		if( strcmp(c.username, cust.username)==0)
		{
			//printf("%s %s\n", c.username, cust.username);
			flag1=1;
		}
	}
	if(flag1!=1)		// username doesnt exist
	{
		struct flock lock;
		lock.l_type = F_WRLCK;	
		lock.l_whence = SEEK_CUR;
		lock.l_start = 0;
		lock.l_len = 0;
		lock.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
		write(fd, &cust, sizeof(cust));
		lock.l_type = F_UNLCK;			// unlocking.
		fcntl(fd, F_SETLK, &lock);		// unlocking.
		
		close(fd);
		
		// -----------locking and writing into customer file-------------------
		
		int fd1=open("Account.txt", O_CREAT | O_RDWR | O_APPEND, 0766);
		struct account a,a1;
		int flag1=0;
		
		while(read(fd1, &a, sizeof(a)) > 0)
		{
			if( (cust.accno-a.accno) == 0)
				flag1=1;
		}
		if(flag1!=1)
		{
			a1.accno=cust.accno;
			a1.balance = 0;
			a1.acc_status=1;
			write(fd1, &a1, sizeof(a1));	
		}
		close(fd1);
		
		return 1;
	}
	else
		return 0;
	close(fd);
}


void getUserDetails()
{
	int fd=open("customer.txt", O_RDONLY, 0766);
	
	printf("getUserDetails\n");
	
	struct customer c1;
	
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		printf("\n%s\t%s\t%d\t%d\t%d", c1.username, c1.password, c1.accno, c1.type, c1.user_status);
	}
	close(fd);
}

void getAccountDetails()
{
	int fd=open("Account.txt", O_RDONLY, 0766);
	
	printf("getUserDetails\n");
	struct account a1;
	
	while(read(fd, &a1, sizeof(a1))>0)
	{		
		printf("\n%d %d %d", a1.accno, a1.balance, a1.acc_status);
	}
	close(fd);
}

int isUserValid(char *name)		// choice - 0 -> user, 1 -> admin.
{
	int fd=open("customer.txt", O_RDONLY, 0766);
		
	struct customer c1;
		
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		if(strcmp(c1.username, name) == 0)
			return 1;
	}
	close(fd);
	return (0);
}

int isJointUserValid(char *name)		// choice - 0 -> user, 1 -> admin.
{
	int fd=open("joint.txt", O_RDONLY, 0766);
		
	struct customer c1;
		
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		if(strcmp(c1.username, name) == 0)
			return 1;
	}
	close(fd);
	return (0);
}


int isPasswordValid(char *name, char *pass)
{
	int fd=open("customer.txt", O_RDONLY, 0766);
	int value;
	
	struct customer c1;
	
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		if( strcmp(c1.username,name)==0 && strncmp(c1.password, pass, strlen(c1.password))==0)
		{
			value = c1.type + 10;
			
			//printf("%s\t%d\t%d\n", c1.username, c1.type, value);
			
			return value;
		}
	}
	close(fd);
	return (0);
}

int isJointPasswordValid(char *name, char *pass)
{
	int fd=open("joint.txt", O_RDONLY, 0766);
	int value;
	
	struct customer c1;
	
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		if( strcmp(c1.username,name)==0 && strncmp(c1.password, pass, strlen(c1.password))==0)
		{
			value = c1.type + 10;
			
			//printf("%s\t%d\t%d\n", c1.username, c1.type, value);
			
			return value;
		}
	}
	close(fd);
	return (0);
}

long int findSize()
{
    struct stat st; 
    
    char *file_name = "customer.txt"; 
     
    if(stat(file_name,&st)==0)
        return (st.st_size);
    else
        return -1;
}

int addAdmin(struct customer cust) 
{
	int fd=open("customer.txt", O_CREAT | O_RDWR | O_APPEND, 0766);
	write(fd, &cust, sizeof(cust));
	close(fd);
}

struct account searchDetails(int acc_no)
{
	int fd=open("Account.txt", O_RDONLY, 0766);
	
	struct flock lock;
		
	lock.l_type = F_RDLCK;	
	lock.l_whence = SEEK_CUR;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
		
	fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
	
	
	struct account a1;
	int flag=0;
	while(read(fd, &a1, sizeof(a1))>0)
	{		
		//printf("\n\nI am inside client_util.h and i am able to read from file\n\n %d\t%d\t%d\n", a1.accno, a1.balance, a1.acc_status);
		//printf("\n");
		if(a1.accno == acc_no)
		{
			//lock.l_type = F_UNLCK;			// unlocking.
			//fcntl(fd, F_SETLK, &lock);		// unlocking.
			
			//return a1;
			flag=1;
			break;
		}
	}
	
	lock.l_type = F_UNLCK;			// unlocking.
	fcntl(fd, F_SETLK, &lock);		// unlocking.
	
	if(flag!=0)
		return a1;
	else
	{
		a1.acc_status = -10;
		return a1;	
		
	}

	close(fd);
}

struct account searchDetailsByName(char *name)
{
	
	//printf("\n this is search details by name \n");
	int fd=open("customer.txt", O_RDONLY, 0766);
	
	struct flock lock;
		
	lock.l_type = F_RDLCK;	
	lock.l_whence = SEEK_CUR;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
		
	fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
	
	int accountno=0;
	struct customer c1;
	
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		if(strcmp(c1.username, name) == 0)
		{
			accountno=c1.accno;
			break;
		}
	}
	
	lock.l_type = F_UNLCK;			// unlocking.
	fcntl(fd, F_SETLK, &lock);		// unlocking.
	
	close(fd);  //--------------------------------------------------------------------locking 1--------------------------
	
	//printf("accno = %d\n", accountno);
	
	int fd1=open("Account.txt", O_RDONLY, 0766);
	
	struct account a1;
	
	while(read(fd1, &a1, sizeof(a1))>0)
	{		
		if(a1.accno == accountno)
		{
			//printf("\n%d\n", a1.accno);
			return a1;
		}
	}
	
	return a1;
	close(fd1);
}

struct account searchDetailsByJointName(char *name)
{
	
	//printf("\n this is search details by name \n");
	int fd=open("joint.txt", O_RDONLY, 0766);
	
	struct flock lock;
		
	lock.l_type = F_RDLCK;	
	lock.l_whence = SEEK_CUR;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
		
	fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
	
	int accountno=0;
	struct customer c1;
	
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		if(strcmp(c1.username, name) == 0)
		{
			accountno=c1.accno;
			break;
		}
	}
	
	lock.l_type = F_UNLCK;			// unlocking.
	fcntl(fd, F_SETLK, &lock);		// unlocking.
	
	close(fd);  //--------------------------------------------------------------------locking 1--------------------------
	
	//printf("accno = %d\n", accountno);
	
	int fd1=open("Account.txt", O_RDONLY, 0766);
	
	struct account a1;
	
	while(read(fd1, &a1, sizeof(a1))>0)
	{		
		if(a1.accno == accountno)
		{
			//printf("\n%d\n", a1.accno);
			return a1;
		}
	}
	
	return a1;
	close(fd1);
}

struct customer viewDetailsByName(char *name)
{
	int fd=open("customer.txt", O_RDONLY, 0766);
	
	struct flock lock;
		
	lock.l_type = F_RDLCK;	
	lock.l_whence = SEEK_CUR;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
		
	fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
	
	
	struct customer c1;
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		//printf("\n\nI am inside client_util.h and i am able to read from file\n\n %s\t%s\t%d\t%d\t%d\n", c1.username, c1.password, c1.accno, c1.type, c1.user_status);
		if(strcmp(c1.username, name) == 0)
		{
			
			lock.l_type = F_UNLCK;			// unlocking.
			fcntl(fd, F_SETLK, &lock);		// unlocking.
			return c1;
		}
	}
	lock.l_type = F_UNLCK;			// unlocking.
	fcntl(fd, F_SETLK, &lock);		// unlocking.
	
	close(fd);
}

struct customer viewJointDetailsByName(char *name)
{
	int fd=open("joint.txt", O_RDONLY, 0766);
	
	struct flock lock;
		
	lock.l_type = F_RDLCK;	
	lock.l_whence = SEEK_CUR;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
		
	fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
	
	
	struct customer c1;
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		//printf("\n\nI am inside client_util.h and i am able to read from file\n\n %s\t%s\t%d\t%d\t%d\n", c1.username, c1.password, c1.accno, c1.type, c1.user_status);
		if(strcmp(c1.username, name) == 0)
		{
			
			lock.l_type = F_UNLCK;			// unlocking.
			fcntl(fd, F_SETLK, &lock);		// unlocking.
			return c1;
		}
	}
	lock.l_type = F_UNLCK;			// unlocking.
	fcntl(fd, F_SETLK, &lock);		// unlocking.
	
	close(fd);
}

//--------------------------------------------Delete functionality in Admin -----------------------------------------------------

void deleteAccount(int acc_num)
{
	//int fd=open("Account.txt", O_RDWR , 0766);
	
	//printf("\nAccount number: %d\n", acc_num);
	
	/*struct account a1, a2;
	int ch=1;
		
	while(read(fd, &a1, sizeof(a1))>0)
	{		
		printf("%d   %d", a1.accno, acc_num);
		if(a1.accno == acc_num)
		{
			
			break;
		}
		ch++;
	}
	*/
	
	int fd=open("Account.txt", O_RDWR, 0766);
	
	struct account a1;
	int ch=1;
	
	while(read(fd, &a1, sizeof(a1))>0)
	{		
		//printf("\nI am inside client_util.h and i am able to read from file\n%d %d %d", a1.accno, a1.balance, a1.acc_status);
		//printf("%d\n", ch);
		if( (a1.accno-acc_num) == 0 )
		{	
			//printf("%d\n", ch);
			break;
		}
		ch++;
	}
	
	struct account a2;
	
	a2.accno = a1.accno;
	a2.balance = a1.balance;
	a2.acc_status = 0;
	
	//printf("ch = %d\naccno = %d\nbalance = %d\nacc_status = %d\n", ch, a2.accno, a2.balance, a2.acc_status);
	
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = ((ch-1)*12)+1;
	lock.l_len = 12;
	lock.l_pid = getpid();
	
		
	fcntl(fd, F_SETLKW, &lock);		// locking.......
		
	lseek(fd, ((ch-1) * 12), SEEK_SET);		// critical section...
		
	write(fd, &a2, 12);
	
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);		// unlocked.
	 
	close(fd);
}

void deleteCustomer(int accountnumber)
{
	int fd=open("customer.txt", O_RDWR, 0766);	
	//perror("open error: ");
	int i=1;
	
	struct customer c1, c;
	
	//printf("\nHello .. inside delete customer\n");
	
	//read(fd, &c1, sizeof(c1));
	//perror("read error: ");
	
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		//printf("%d\n", c1.accno);
		if( (c1.accno - accountnumber) == 0)
		{
			//printf("inside while and inside if\n");
			
			strcpy(c.username, c1.username);		// copying from c1.username to c.username
			strcpy(c.password, c1.password);
			c.accno = c1.accno;
			c.type = c1.type;
			c.user_status=0;
				
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*2012)+1;
			lock.l_len = 2012;
			lock.l_pid = getpid();
		
			fcntl(fd, F_SETLKW, &lock);
				
			lseek(fd, ((i-1) * 2012), SEEK_SET);
				
			write(fd, &c, 2012);
				
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);

		}
		i++;
	}
	
	close(fd);
}

void deleteJointCustomer(int accountnumber)
{
	int fd=open("joint.txt", O_RDWR, 0766);	
	//perror("open error: ");
	int i=1;
	
	struct customer c1, c;
	
	//printf("\nHello .. inside delete customer\n");
	
	//read(fd, &c1, sizeof(c1));
	//perror("read error: ");
	
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		//printf("%d\n", c1.accno);
		if( (c1.accno - accountnumber) == 0)
		{
			//printf("inside while and inside if\n");
			
			strcpy(c.username, c1.username);		// copying from c1.username to c.username
			strcpy(c.password, c1.password);
			c.accno = c1.accno;
			c.type = c1.type;
			c.user_status=0;
				
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*2012)+1;
			lock.l_len = 2012;
			lock.l_pid = getpid();
		
			fcntl(fd, F_SETLKW, &lock);
				
			lseek(fd, ((i-1) * 2012), SEEK_SET);
				
			write(fd, &c, 2012);
				
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);

		}
		i++;
	}
	
	close(fd);
}

void delete_Account(int accountnumber, int user_type)
{
	deleteAccount(accountnumber);
	
	if(user_type==0)				// user_type = 0 : single user   ++ user_type = 1 : joint user
		deleteCustomer(accountnumber);
	else
		deleteJointCustomer(accountnumber);
}

//-------------------------------------------------MODIFY OF ADMIN----------------------------------------------------------------------


void modifyAccount(int accno1, int accno2)
{
	// --------------------------------- modifying Account.txt file--------------------------------------------------
	int fd=open("Account.txt", O_RDWR, 0766);
	
	struct account a1;
	int ch=1;
	
	while(read(fd, &a1, sizeof(a1))>0)
	{		
		if( (a1.accno-accno1) == 0 )
		{	
			break;
		}
		ch++;
	}
	
	struct account a2;
	
	a2.accno = accno2;
	a2.balance = a1.balance;
	a2.acc_status = a1.acc_status;
	
	//printf("ch = %d\naccno = %d\nbalance = %d\nacc_status = %d\n", ch, a2.accno, a2.balance, a2.acc_status);
	
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = ((ch-1)*12)+1;
	lock.l_len = 12;
	lock.l_pid = getpid();
	
		
	fcntl(fd, F_SETLKW, &lock);		// locking.......
		
	lseek(fd, ((ch-1) * 12), SEEK_SET);		// critical section...
		
	write(fd, &a2, 12);
	
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);		// unlocked.
	
	close(fd);
}

void modifyCustomer(int accno1, int accno2)
{
	int fd=open("customer.txt", O_RDWR, 0766);	
	//perror("open error: ");
	int i=1;
	
	struct customer c1, c;
	
	//printf("\nHello .. inside delete customer\n");
	
	//read(fd, &c1, sizeof(c1));
	//perror("read error: ");
	
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		//printf("%d\n", c1.accno);
		if( (c1.accno - accno1) == 0)
		{
			//printf("inside while and inside if\n");
			
			strcpy(c.username, c1.username);		// copying from c1.username to c.username
			strcpy(c.password, c1.password);
			c.accno = accno2;				// changing account number to new account number 
			c.type = c1.type;
			c.user_status=c1.user_status;
				
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*2012)+1;
			lock.l_len = 2012;
			lock.l_pid = getpid();
		
			fcntl(fd, F_SETLKW, &lock);
				
			lseek(fd, ((i-1) * 2012), SEEK_SET);
				
			write(fd, &c, 2012);
				
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);
		}
		i++;
	}	
	close(fd);	
}

void modifyJointCustomer(int accno1, int accno2)
{
	int fd=open("joint.txt", O_RDWR, 0766);	
	//perror("open error: ");
	int i=1;
	
	struct customer c1, c;
	
	//printf("\nHello .. inside delete customer\n");
	
	//read(fd, &c1, sizeof(c1));
	//perror("read error: ");
	
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		//printf("%d\n", c1.accno);
		if( (c1.accno - accno1) == 0)
		{
			//printf("inside while and inside if\n");
			
			strcpy(c.username, c1.username);		// copying from c1.username to c.username
			strcpy(c.password, c1.password);
			c.accno = accno2;				// changing account number to new account number 
			c.type = c1.type;
			c.user_status=c1.user_status;
				
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*2012)+1;
			lock.l_len = 2012;
			lock.l_pid = getpid();
		
			fcntl(fd, F_SETLKW, &lock);
				
			lseek(fd, ((i-1) * 2012), SEEK_SET);
				
			write(fd, &c, 2012);
				
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);
		}
		i++;
	}	
	close(fd);	
}

void modify(int accno1, int accno2, int user_type)
{

	// --------------------------------- modifying Account.txt file--------------------------------------------------
	modifyAccount(accno1, accno2);
	
	//--------------------------------------Modifying customer.txt file-------------------------------------------
	if(user_type == 0)					// single user.
		modifyCustomer(accno1, accno2);
	else
		modifyJointCustomer(accno1, accno2);
}

//----------------------------------------------------- PASSWORD CHANGE ---------------------------------------------------

void passwordChange(char *username, char *newpass)
{
	int fd=open("customer.txt", O_RDWR, 0766);	
	//perror("open error: ");
	int i=1;
	
	struct customer c1, c;
	
	//printf("\nHello .. inside delete customer\n");
	
	//read(fd, &c1, sizeof(c1));
	//perror("read error: ");
	
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		//printf("%d\n", c1.accno);
		if( strcmp(c1.username, username)==0 )
		{
			//printf("inside while and inside if\n");
			
			strcpy(c.username, c1.username);		// copying from c1.username to c.username
			strcpy(c.password, newpass);
			c.accno = c1.accno;				// changing account number to new account number 
			c.type = c1.type;
			c.user_status=c1.user_status;
				
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*2012)+1;
			lock.l_len = 2012;
			lock.l_pid = getpid();
		
			fcntl(fd, F_SETLKW, &lock);
				
			lseek(fd, ((i-1) * 2012), SEEK_SET);
				
			write(fd, &c, 2012);
				
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);
		}
		i++;
	}	
	close(fd);	
}

void passwordJointChange(char *username, char *newpass)
{
	int fd=open("joint.txt", O_RDWR, 0766);	
	//perror("open error: ");
	int i=1;
	
	struct customer c1, c;
	
	//printf("\nHello .. inside delete customer\n");
	
	//read(fd, &c1, sizeof(c1));
	//perror("read error: ");
	
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		//printf("%d\n", c1.accno);
		if( strcmp(c1.username, username)==0 )
		{
			//printf("inside while and inside if\n");
			
			strcpy(c.username, c1.username);		// copying from c1.username to c.username
			strcpy(c.password, newpass);
			c.accno = c1.accno;				// changing account number to new account number 
			c.type = c1.type;
			c.user_status=c1.user_status;
				
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*2012)+1;
			lock.l_len = 2012;
			lock.l_pid = getpid();
		
			fcntl(fd, F_SETLKW, &lock);
				
			lseek(fd, ((i-1) * 2012), SEEK_SET);
				
			write(fd, &c, 2012);
				
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);
		}
		i++;
	}	
	close(fd);	
}

//---------------------------------------------- DEPOSIT --------------------------------------------------------------------

void getTransactions()
{
	int fd=open("Transaction.txt", O_RDONLY, 0766);
	
	//printf("Transaction Details\n");
	
	struct transaction t1;
	
	while(read(fd, &t1, sizeof(t1))>0)
	{		
		printf("\n%s\t%d\t%d\t%d", t1.date, t1.accno, t1.amount, t1.balance);
	}
	close(fd);
}


void updateTransactionFile(int accno, int amount, int total_bal)
{
	/* date part */
	
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
  	
  	int year = tm.tm_year + 1900;
  	int month = tm.tm_mon + 1;
  	int day = tm.tm_mday;
  
  	char y[10], m[10], d[10];
  
  	sprintf(y, "%d", year);
  	sprintf(m, "%d", month);
  	sprintf(d, "%d", day);
  
  	char date[size];
  	snprintf(date, sizeof(date), "%s/%s/%s", d, m, y);
	
	// date part end--------------------------------------
	
	int fd = open("Transaction.txt", O_CREAT | O_RDWR | O_APPEND, 0766);

	struct transaction txn;
	
	strcpy(txn.date, date);
	txn.accno = accno;
	txn.amount = amount;
	txn.balance = total_bal;
	
	struct flock lock;
		
	lock.l_type = F_WRLCK;	
	lock.l_whence = SEEK_CUR;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
		
	fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
	
	write(fd, &txn, sizeof(txn));
	//printf("%s %d %d %d\n", txn.date, txn.accno, txn.amount, txn.balance);
	
	lock.l_type = F_UNLCK;			// unlocking.
	fcntl(fd, F_SETLK, &lock);		// unlocking.
	close(fd);
}

void deposit(int accno, int amount)
{
	
	//printf("\n%d %d\n", accno, amount);
	
	int fd=open("Account.txt", O_RDWR, 0766);
	
	struct account a1;
	int ch=1, total_bal;
	
	while(read(fd, &a1, sizeof(a1))>0)
	{		
		if( (a1.accno-accno) == 0 )
		{	
			break;
		}
		ch++;
	}
	
	struct account a2;
	
	a2.accno = accno;
	a2.balance = a1.balance + amount;
	a2.acc_status = a1.acc_status;
	
	total_bal = a2.balance;
	//printf("ch = %d\naccno = %d\nbalance = %d\nacc_status = %d\n", ch, a2.accno, a2.balance, a2.acc_status);
	
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = ((ch-1)*12)+1;
	lock.l_len = 12;
	lock.l_pid = getpid();
	
		
	fcntl(fd, F_SETLKW, &lock);		// locking.......
		
	lseek(fd, ((ch-1) * 12), SEEK_SET);		// critical section...
		
	write(fd, &a2, 12);
	
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);		// unlocked.
	
	close(fd);
	
	//printf("\n%d %d\n", amount, total_bal);
	updateTransactionFile(accno, amount, total_bal);
}

//-------------------------------------------------------------------- WITHDRAW ----------------------------------



void withdraw(int accno, int amount)
{
	
	//printf("\n%d %d\n", accno, amount);
	
	int fd=open("Account.txt", O_RDWR, 0766);
	
	struct account a1;
	int ch=1, total_bal;
	
	while(read(fd, &a1, sizeof(a1))>0)
	{		
		if( (a1.accno-accno) == 0 )
		{	
			break;
		}
		ch++;
	}
	
	struct account a2;
	
	a2.accno = accno;
	a2.balance = a1.balance - amount;
	a2.acc_status = a1.acc_status;
	
	total_bal = a2.balance;
	//printf("ch = %d\naccno = %d\nbalance = %d\nacc_status = %d\n", ch, a2.accno, a2.balance, a2.acc_status);
	
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = ((ch-1)*12)+1;
	lock.l_len = 12;
	lock.l_pid = getpid();
	
		
	fcntl(fd, F_SETLKW, &lock);			// locking.......
		
	lseek(fd, ((ch-1) * 12), SEEK_SET);		// critical section...
		
	write(fd, &a2, 12);
	
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);			// unlocked.
	
	close(fd);
	
	//printf("\n%d %d\n", amount, total_bal);
	amount=-amount;
	updateTransactionFile(accno, amount, total_bal);
}
