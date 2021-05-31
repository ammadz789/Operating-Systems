#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#include <stdio.h>


using namespace std;

int Seats[2][50];   //the 2D array of seats in the plane
bool Full = false;
bool TA1_again = false, TA2_again = false;
int turn = 0;

// This function checks if there are remaining seats in the plane
// initiated by the main thread
void *seatCheck(void *)
{
	int count = 0;
			
	for (int i=0; i<2; i++)
	{
		for (int j =0; j<50; j++)
		{
			if (Seats[i][j] != 0)
				count++;			
			
		}
	}
	if (count == 100){                   //count == 100 indicates that all seats occupied thus boolean Full is set as true
		Full = true;
		cout<<"Plane is full:"<<endl;
	}
	else
		Full = false;
}


void *A1Booking(void* )
{
	
	int seatNum = 0, count = 0;
	int randRow1 = rand()%2;    //gives random value between 0-1
	int randCol1 = rand()%50;   //gives random value between 0-49
	
	if (randRow1 == 0)
		seatNum = randCol1;
	else
		seatNum = randCol1 + 50;   //randRow =1 means we are in the second row of the array so each coloumn number is added by 50 to obtain the seat number
	
	bool isFull = false;
    
	 // the thread checks for availability of seats in the plane
	 
	for (int i=0; i<2; i++)
	{
		for (int j =0; j<50; j++)
		{
			if (Seats[i][j] != 0)
				count++;
		}
	}
	if (count == 100)
		bool isFull = true;   //all seats are ocuupied
								
			
			
	
	if (Seats[randRow1][randCol1] == 0)               //if seat is available, the agency will book it
	{
		cout<<"Agency 1 Entered Critical Region"<<endl;
		Seats[randRow1][randCol1] = 1;
		cout<<"Seat number "<<seatNum<<" is reserved by Agency 1"<<endl;
		TA1_again = false;
		cout<<"Agency 1 Exit Critical Region"<<endl<<endl;
	}
	else
	{
		TA1_again = true;    //if seat not available, TA_again is set as false
	}                        //the thread will ener the function again and again untill an available seat is found
	
	if (isFull)
	{
		cout<<"Agency 1 Exit Critical Region"<<endl;
		cout<<"No Seats Left"<<endl<<endl;
	}
}

//replica of A1Booking function, initiated by thread 2
void *A2Booking(void* )
{
	
	int seatNum = 0, count = 0;
	int randRow2 = rand()%2;
	int randCol2 = rand()%50;
	
	
	if (randRow2 == 0)
		seatNum = randCol2;
	else
		seatNum = randCol2 + 50;
	
	bool isFull = false;

	for (int i=0; i<2; i++)
	{
		for (int j =0; j<50; j++)
		{
			if (Seats[i][j] != 0)
				count++;
		}
	}
	if (count == 100)
		bool isFull = true;

	if (Seats[randRow2][randCol2] == 0)
	{
		cout<<"Agency 2 Entered Critical Region"<<endl;			
		Seats[randRow2][randCol2] = 2;
		cout<<"Seat number "<<seatNum<<" is reserved by Agency 2"<<endl;
		TA2_again = false;
		cout<<"Agency 2 Exit Critical Region"<<endl<<endl;
	}
	else
	{
		TA2_again = true;
	}
	
	if (isFull)
	{
		cout<<"Agency 2 Exit Critical Region"<<endl;
		cout<<"No Seats Left"<<endl<<endl;
	}
}



int main()
{
	pthread_t main_thread, TravelAgency1, TravelAgency2;
	srand(time(NULL));      //for obtaining random integers for seat numbers
	
	for (int i=0; i<2; i++)        
	{
		for (int j=0;j<50; j++)     //variables in all indexes of the array initialized as 0
		{
			Seats[i][j] = 0;
		}
	}	
	
	while(!Full)  //condition verified in seatCheck function initiated by main thread
	{
			
		pthread_create(&main_thread, NULL, seatCheck, NULL); //main thread, checks if all seats filled in each loop
		pthread_join(main_thread, NULL);
		
		if (turn == 0)  // for busy waiting
		{
			pthread_create(&TravelAgency1, NULL, A1Booking, NULL);
			pthread_join(TravelAgency1, NULL);
			if (TA1_again)
				turn = 0;  //not successful booking-->we repeat the booking process for this thread
			else
				turn = 1;  //succesful booking-->turn set as 1, allowing second thread to access the shared memory 
		}
		
		else
		{
			pthread_create(&TravelAgency2, NULL, A2Booking, NULL);
			pthread_join(TravelAgency2, NULL);
			if (TA2_again)
				turn = 1;
			else
				turn = 0;			
		}		
		
		
		
	}
	
	//printing the 2D array
	for(int i=0; i<2; i++)
	{
		cout<<endl;
		for(int j=0; j<50; j++)
		{
			cout<<Seats[i][j]<<" ";
		}
	}	
	pthread_exit(NULL);
	
	return 0;
}