#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Aesimhei.h"
#include <windows.h>
#include <winver.h>

int getMoney(int amount);
int payMoney(int amount);
void disableCoinChannels(int channels[]);

int main( int argc, char * argv [] ){

    int OpenStatus = OpenMHE ();
    if(OpenStatus != 0){
        printf("ERROR, Paylink device not connected." );
        fflush(stdout);
        return 1;
    }

    //checking whether correct number of arguments
    if(argc <3){
        printf("ERROR, nothing to do." );
        fflush(stdout);
        return 1;
    }

    //calling disableCoinChannels() function
    if(argc == 4){
        if(strlen(argv[3]) == 8){
            int channels[8];
            int j;
            for( j= 0; j < 8; j++){
              if(argv[3][j] - '0' == 1){
                  channels[j] = argv[3][j] - '0';
              }else channels[j] = 0;
            }
            disableCoinChannels(channels);
        }
    }

    //calling getMoney() function
    if(!strncmp(argv[1], "getMoney", 8)){
        getMoney(atoi(argv[2]));
    }

    //calling payMoney() function
    if(!strncmp(argv[1], "payMoney", 8)){
        payMoney(atoi(argv[2]));
    }

    return 0;
}

int getMoney(int amount){

    int LastCurrencyValue = CurrentValue() ;
    int NewCurrencyValue;

    EnableInterface() ;

    while (amount > 0){

        NewCurrencyValue = CurrentValue() ;
        if (NewCurrencyValue != LastCurrencyValue){
            //printf("The user has just inserted %d \n", NewCurrencyValue - LastCurrencyValue) ;
            printf("%d", NewCurrencyValue - LastCurrencyValue) ;
            fflush(stdout);
            LastCurrencyValue = NewCurrencyValue ;
            amount =0;//--amount;
        }
        Sleep(100) ;
    }

    DisableInterface();
  return 0;
}

int payMoney(int amount){

    //long CurrentPayOut;
    long initialValue = CurrentPaid();
    EnableInterface();

    PayOut(amount) ;
    while (LastPayStatus() == PAY_ONGOING){
        if (CurrentPayOut != CurrentPaid()){
            //CurrentPayOut = CurrentPaid() ;
            //printf("CurrentPayOut %ld\n",  CurrentPaid()) ;
        }
        Sleep(100) ;
    }
    printf("%ld",  CurrentPaid() - initialValue) ;

    if (LastPayStatus() != PAY_FINISHED){
        printf("Error %d \n", LastPayStatus()) ;
    }
    //else{
    //    printf("PAY FINISHED\n") ;
    //}
    DisableInterface();

return 0;
}

//Enabel Coin Channels
void disableCoinChannels(int channels[]){

    AcceptorBlock AcceptorDetails ;
    int AcceptorNo ;
    int CoinNo ;

    for (AcceptorNo = 0 ; ReadAcceptorDetails(AcceptorNo, &AcceptorDetails) ; ++ AcceptorNo){

       for (CoinNo = 0 ; CoinNo < AcceptorDetails.NoOfCoins ; ++CoinNo){
            AcceptorDetails.Coin[CoinNo].Inhibit = channels[CoinNo];
            WriteAcceptorDetails(AcceptorNo, &AcceptorDetails );
       }
    }
}
