ONLINE BANKING MANAGEMENT SYSTEM.

1. Assignment is uploaded in the format MT2020011.zip

2. Download the file and extract the contents.

3. There will be three files: client.c, server.c and client_util.h and README.txt.

4. client.c serves as the client side, server.c serves as the server side of the online banking application and client_util.h contains all the functions required for the application to run.

5. store all the files in a folder.

6. Open 2 terminals one for running server.c and other for running client.c

7. first compile and run server.c and then compile and run client.c

8. To compile Server:
			gcc server.c
   To run Server:
   			./a.out

   	
9. To compile client:
			gcc client.c
   To run client:
   			./a.out


10. client_util.h file need not be compiled or run separately. It is already included in server.c

11. After compiling and running the server side, you will be prompted to created new Admin. There enter Admin name and password.

12. This application is based on single central admin.

13. After creating an Admin, login on client side with Admin. 

14. You will be prompted to choose one among the five options: 
	1. Add 
	2. Delete  
	3. Modify  
	4. Search  
	5. Exit.
	
15. Choose one (Add) to add a new user. Choose Two, to delete an already existing user. Here as there are no prior users you are requested to create a user first (using option 1) and then perform deletion. Choose option three to modify the Account number of an already existing user. Choose option four to search for any account details. Choose option five to exit the session.

16. To terminate the server side session press "contol + c".

17. Once a user has been added (single or joint), run the client side program and login with the username and password.

18. you will be prompted to choose one among the six options:
	1. Deposit
	2. Withdraw
	3. Balance Enquiry
	4. Password Change
	5. View Details
	6. Exit.

19. Choose option 1 to Deposit the amount. Please enter only positive integers. For this your account must be active. 

20. Choose option 2 to withdraw the amount. Please enter only positive integers. For this you must have sufficient funds.

21. Choose option 3 to check your balance.

22. Choose option 4, to change your password. After changin your password, relogin and check if you are able to login with new details or not.

23. Choose option 5, to check your details. The details displayed are: Password, Account Number, Single user or joint user, Account is active or not.

24. Choose option 6, to exit from the application.

25. Files and their contents:

	-> customer.txt => this file stores all the details of single user and central admin.
	-> Transaction.txt => this file stores all the details of transactions of single and joint users.
	-> joint.txt => this file stores the details of joint users.
	-> Account.txt => this files stores the account details of single and joint account users.
	-> server.c   => code for server side of the application.
	-> client.c  => code for client side of the application.
	-> client_util.h => all functions for Add, modify, search, delete etc are included in this file and it will be included in header of server.c
	
26. Joint user is implemented in the similar way as single user. Here username is unique and account numbers for single account holders start from 1000.

27. For concurrency, Fork() is used on server side. Three different structures are used. One for customer, other for Account details and another for Transaction details. Three logins are implemented, one for each single, joint and admin. Client takes details from user and sends them to server. Server then processes the information by calling the respective programs and returns the output to the client. 
