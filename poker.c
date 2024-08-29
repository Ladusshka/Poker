#include <stdio.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define DATAFILE "/var/casino"         //place where balance of user will be saved

typedef struct player{
     int uid;
     char  name[100];
     int credits;
} PLAYER;


int get_player_data();
void register_new_player();
void update_player_data();
void input_name();
void play_game();

PLAYER player;

int main(){

     srand(time(NULL));

     if(get_player_data() == -1) { // no player was found
          register_new_player();
     }

     int choice = 0;
     while (choice != 2){
          printf("-=POKER MENU=-\n");
          printf("0 -- play Pocker\n");
          printf("1 -- get balance back to 500$\n");
          printf("2 -- exit\n\n\n");
          printf("[Name: %s]\n",player.name);
          printf("[You have %u $] ->", player.credits);
          scanf("%d", &choice);

          if((choice < 0) || (choice > 2)){
               printf("\n\n!!! WRONG NUMBER WAS ENTERED !!!\n\n");
          } else if(choice == 0){
               play_game();
          }else if(choice == 1 ){
               player.credits = 500;
          }
          else{
               break;
          }
     }
          

     printf("THANKS FOR THE GAME!!!!\n");
     update_player_data();    
     
     return EXIT_SUCCESS;
}




/*Ja lublu tebya mam */


typedef struct card{ // peredelaj karty
     char colour;
     char number;
     int digitNumber;
     int taken;

}Card;



//odin massiv iz kart igroka i dilera + sort + poisk kombinacii 

typedef struct res{
     int low;
     int high;
}Result;


int highest(Card * cards){ // dangerous
     return cards[0].digitNumber;
}


/*Result Pair(Card *cards, size_t size) {
    int highestPair = 0;

    for (int i = 0; i < size - 1; i++) {
        if (cards[i].digitNumber == cards[i + 1].digitNumber) {
            highestPair = cards[i].digitNumber;
            break;
        }
    }

    return (Result){highestPair, 0};
}*/
Result Pair(Card * cards, size_t size){ // vozvrat 0 esli net, esli est to mladshuju kartu

     int result=0;
     
     for(int i=0; i<size-1;i++){
          int current_number_on_card = cards[i].digitNumber;
          if(current_number_on_card != 0){
               for(int j = i+1;j<size;j++){
               if(current_number_on_card == cards[j].digitNumber && current_number_on_card>result){
                    result = cards[j].digitNumber;
               }
          }
          }
          
     }

     return (Result){result,0};    
}

/*Result twoPairs(Card *cards, size_t size) {
    int firstPair = 0, secondPair = 0;

    for (int i = 0; i < size - 1; i++) {
        if (cards[i].digitNumber == cards[i + 1].digitNumber) {
            if (!firstPair) {
                firstPair = cards[i].digitNumber;
            } else if (cards[i].digitNumber != firstPair) {
                secondPair = cards[i].digitNumber;
                break;
            }
            i++; // Пропускаем следующую карту, так как она является частью пары
        }
    }

    if (firstPair && secondPair) {
        if (firstPair < secondPair) {
            int temp = firstPair;
            firstPair = secondPair;
            secondPair = temp;
        }
        return (Result){secondPair, firstPair};
    }

    return (Result){0, 0};
}*/
Result twoPairs(Card * cards, size_t size){ // zapomnim starshuju i mladshuju paru

     int first_result=0;    // Change to Pair()
     for(int i=0; i<size; i++){
          int current_number_on_card = cards[i].digitNumber;
          if(current_number_on_card != 0){
               for(int j = i+1;j<size;j++){
                    if(current_number_on_card == cards[j].digitNumber && current_number_on_card>first_result){ // najdu sperva maximalnuu paru
                         first_result = cards[j].digitNumber;
                    }
               }
          }
          
     }

     if(!first_result){
          return (Result){0,0};
     }


     int second_result=0;
     for(int i=0; i<size; i++){
          int current_number_on_card = cards[i].digitNumber;
          if(current_number_on_card != 0){
               for(int j = i+1;j<size;j++){
                    if(current_number_on_card == cards[j].digitNumber && current_number_on_card>second_result && current_number_on_card != first_result){ // najdu sperva maximalnuu paru
                          second_result = cards[j].digitNumber;
                    }
               }    
          }
          
     }

     if(!second_result){
          return (Result){0,0};
     }

     return (Result){second_result,first_result};
}

/*Result Set(Card *cards, size_t size) {
    int highestSet = 0;

    for (int i = 0; i < size - 2; i++) {
        if (cards[i].digitNumber == cards[i + 1].digitNumber && cards[i].digitNumber == cards[i + 2].digitNumber) {
            highestSet = cards[i].digitNumber;
            break;
        }
    }

    return (Result){highestSet, 0};
}
*/
Result Set(Card * cards,size_t size){
     int result=0;
     for(int i=0; i<size-1;i++){
          int current_number_on_card = cards[i].digitNumber;
          if(current_number_on_card != 0){
               int counter=0;
               for(int j = i+1;j<size;j++){
                    if(current_number_on_card == cards[j].digitNumber ){
                    counter++;
                    }
               }

               if(counter >= 2 && current_number_on_card > result){
               result = current_number_on_card;
               }
          }
         
     }

     return (Result){result,0}; 
}
/*Result Street(Card *cards, size_t size) {
    int start = cards[0].digitNumber;

    for (int i = 1; i < size; i++) {
        if (cards[i].digitNumber != start + i) {
            if (i == 1 && start == 14 && cards[size - 1].digitNumber == 2) {
                start = 1; // "Колесо"
            } else {
                return (Result){0, 0};
            }
        }
    }

    return (Result){start, start + 4};
}
*/
Result Street(Card * cards,size_t size){


    int hasAce = 0;
    int hasTwo = 0;
    int hasThree = 0;
    int hasFour = 0;
    int hasFive = 0;

    for (int i = 0; i < size; i++) {
        if (cards[i].digitNumber == 14) hasAce = 1;
        if (cards[i].digitNumber == 2) hasTwo = 1;
        if (cards[i].digitNumber == 3) hasThree = 1;
        if (cards[i].digitNumber == 4) hasFour = 1;
        if (cards[i].digitNumber == 5) hasFive = 1;
    }

    if (hasAce && hasTwo && hasThree && hasFour && hasFive) {
        return (Result){1, 5}; // для стрита A-2-3-4-5 старшая карта — 5
    }



     int result=15;
     for(int i=0; i<size; i++){
          int current_number_on_card = cards[i].digitNumber;
          if(current_number_on_card != 0){
               int counter=0;
               for(int j = i+1;j<size;j++){
                    if(cards[j].digitNumber - 1 == cards[(j-1)].digitNumber){
                         counter++;
                    }else{
                         break;
                    }
               }
               if(counter>=4 && result>current_number_on_card){
                    result = current_number_on_card;
               }
          }
          

     }

    if(result == 15){
        return (Result){0,0};
    }
     return (Result){result,result+4};
}
int compareFlashes(Card *flash1, Card *flash2, size_t size) {
  
    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = i + 1; j < size; j++) {
            if (flash1[i].digitNumber < flash1[j].digitNumber) {
                Card temp = flash1[i];
                flash1[i] = flash1[j];
                flash1[j] = temp;
            }
            if (flash2[i].digitNumber < flash2[j].digitNumber) {
                Card temp = flash2[i];
                flash2[i] = flash2[j];
                flash2[j] = temp;
            }
        }
    }

    
    for (size_t i = 0; i < size; i++) {
        if (flash1[i].digitNumber > flash2[i].digitNumber) {
            return 1; 
        } else if (flash1[i].digitNumber < flash2[i].digitNumber) {
            return -1; 
        }
    }

    return 0;
}
Result Flash(Card *cards, size_t size) {
    int maxValueInFlush = 0;
    
    
    for (int i = 0; i < size; i++) {
        int currentValue = cards[i].digitNumber;
        if(currentValue != 0){
          int counter = 1;
          int maxInThisFlush = currentValue;

        
          for (int j = i + 1; j < size; j++) {
            if (cards[j].colour == cards[i].colour) {
                counter++;
                if (cards[j].digitNumber > maxInThisFlush) {
                    maxInThisFlush = cards[j].digitNumber;
                }
            }
          }

        
          if (counter >= 5 && maxInThisFlush > maxValueInFlush) {
            maxValueInFlush = maxInThisFlush;
          }
        }
        
    }

    
    if (maxValueInFlush == 0) {
        return (Result){0, 0};
    }

    return (Result){0, maxValueInFlush};
}

/*Result FullHouse(Card *cards, size_t size) {
    Result set = Set(cards, size);
    if (set.low == 0) {
        return (Result){0, 0};
    }

    Card newcards[7]; 
    int counter = 0;

    for (int i = 0; i < size; i++) {
        if (cards[i].digitNumber != set.low) {
            newcards[counter++] = cards[i];
        }
    }

    Result pair = Pair(newcards, counter);
    if (pair.low == 0) {
        return (Result){0, 0};
    }

    return (Result){pair.low, set.low};
}
*/
Result FullHouse(Card * cards, const size_t size){
     
      //  dvojka i set razlichit; set = true , kde karta != dvojka = true
     
     Result set = Set(cards,size);
     if(set.low == 0){
          return (Result){0, 0};
     }
     
     Card newcards[size-3] ;
     int counter=0;
     for(int i =0;i<size;i++){
          if(cards[i].digitNumber != set.low){
               newcards[counter++] = cards[i];
          }
     }

     Result pair = Pair(newcards, (size - 3));

     if(pair.low==0){
          return (Result){0,0};
     }


     return (Result){pair.low, set.low};
   
}


Result Kare( Card * cards, size_t size){
     int result=0;
     for(int i=0; i<size-1;i++){
          int current_number_on_card = cards[i].digitNumber;
          if(current_number_on_card != 0){
               int counter=0;
               for(int j = i+1;j<size;j++){
                    if(current_number_on_card == cards[j].digitNumber ){
                    counter++;
                    }
               }

               if(counter >= 3 && current_number_on_card > result){
                    result = current_number_on_card;
               }
          }
          
     }

     return (Result){result,0}; 
}


Result StreetFlash(Card* cards, size_t size) {
    Result street = Street(cards, size);
    if (street.low == 0) {
        return (Result){0, 0}; 
    }

    char flushColour = '\0';
    int streetLength = 0;

    
    for (int i = 0; i < size; i++) {
        if (cards[i].digitNumber == street.low) {
            flushColour = cards[i].colour;
            streetLength = 1;

            for (int j = i + 1; j < size && streetLength < 5; j++) {
                if (cards[j].digitNumber == street.low + streetLength && cards[j].colour == flushColour) {
                    streetLength++;
                }
            }

            if (streetLength == 5) {
                return (Result){street.low, street.low + 4};
            }
        }
    }

   
    if (street.low == 1) { 
        int hasAce = 0, hasTwo = 0, hasThree = 0, hasFour = 0, hasFive = 0;

        for (int i = 0; i < size; i++) {
            if (cards[i].digitNumber == 14 && cards[i].colour == flushColour) hasAce = 1;
            if (cards[i].digitNumber == 2 && cards[i].colour == flushColour) hasTwo = 1;
            if (cards[i].digitNumber == 3 && cards[i].colour == flushColour) hasThree = 1;
            if (cards[i].digitNumber == 4 && cards[i].colour == flushColour) hasFour = 1;
            if (cards[i].digitNumber == 5 && cards[i].colour == flushColour) hasFive = 1;
        }

        if (hasAce && hasTwo && hasThree && hasFour && hasFive) {
            return (Result){1, 5}; 
        }
    }

    return (Result){0, 0}; 
}




Result FlashRoyale(Card * cards, size_t size){

     Result streetflush = StreetFlash(cards,size);
     if(streetflush.low!=10){
          return (Result){0,0};
     }

     return (Result){10,14};
}






void print_cards(Card * players_cards, unsigned  quantity_of_cards){

     for(int i=0;i<quantity_of_cards;i++){
          if (players_cards[i].colour == 'H') {
            printf("\033[1;31m%c♥\033[0m ", players_cards[i].number);  // Красный для черв
        } else if (players_cards[i].colour == 'D') {
            printf("\033[1;31m%c♦\033[0m ", players_cards[i].number);  // Красный для бубен
        } else if (players_cards[i].colour == 'C') {
            printf("\033[1;34m%c♣\033[0m ", players_cards[i].number);  // Синий для треф
        } else if (players_cards[i].colour == 'S') {
            printf("\033[1;34m%c♠\033[0m ", players_cards[i].number);  // Синий для пик
        }
     }
}


int bet (int bet_ammount){ // rewrite comppletetly!!
     char answer='\n';
     while(answer != 'N' && answer != 'n' && answer != 'Y' && answer !='y'){
          printf("Would you like to bet(y/n) %d:", bet_ammount); 
          scanf(" %c", &answer);
          
        
     }


     answer = tolower(answer);

     if(answer == 'y'){
          player.credits-=bet_ammount;
          return 1;
     }

     return 0;


}



int Compare_cards(const void *  CardA , const void *CardB){

     Card first = *(const Card*)CardA;
     Card second = *(const Card*)CardB;
     
     return  (first.digitNumber > second.digitNumber) - (first.digitNumber < second.digitNumber);

}

void combinations_check(Result * combinations, Card * cards, size_t size_of_cards){

     combinations[1] = Pair(cards, size_of_cards);
     combinations[2] = twoPairs(cards, size_of_cards);
     combinations[3] = Set(cards, size_of_cards);
     combinations[4] = Street(cards,size_of_cards);
     combinations[5] = Flash(cards,size_of_cards);
     combinations[6] = FullHouse(cards,size_of_cards);
     combinations[7] = Kare(cards,size_of_cards);
     combinations[8] = StreetFlash(cards,size_of_cards);
     combinations[9] = FlashRoyale(cards,size_of_cards);


}


void play_game(){
     
    Card deck[52] = {
    {'H', 'A', 14, 0}, {'H', '2', 2, 0}, {'H', '3', 3, 0}, {'H', '4', 4, 0}, {'H', '5', 5, 0}, {'H', '6', 6, 0}, {'H', '7', 7, 0}, {'H', '8', 8, 0}, {'H', '9', 9, 0}, {'H', '1', 10, 0}, {'H', 'J', 11, 0}, {'H', 'Q', 12, 0}, {'H', 'K', 13, 0},
    {'D', 'A', 14, 0}, {'D', '2', 2, 0}, {'D', '3', 3, 0}, {'D', '4', 4, 0}, {'D', '5', 5, 0}, {'D', '6', 6, 0}, {'D', '7', 7, 0}, {'D', '8', 8, 0}, {'D', '9', 9, 0}, {'D', '1', 10, 0}, {'D', 'J', 11, 0}, {'D', 'Q', 12, 0}, {'D', 'K', 13, 0},
    {'C', 'A', 14, 0}, {'C', '2', 2, 0}, {'C', '3', 3, 0}, {'C', '4', 4, 0}, {'C', '5', 5, 0}, {'C', '6', 6, 0}, {'C', '7', 7, 0}, {'C', '8', 8, 0}, {'C', '9', 9, 0}, {'C', '1', 10, 0}, {'C', 'J', 11, 0}, {'C', 'Q', 12, 0}, {'C', 'K', 13, 0},
    {'S', 'A', 14, 0}, {'S', '2', 2, 0}, {'S', '3', 3, 0}, {'S', '4', 4, 0}, {'S', '5', 5, 0}, {'S', '6', 6, 0}, {'S', '7', 7, 0}, {'S', '8', 8, 0}, {'S', '9', 9, 0}, {'S', '1', 10, 0}, {'S', 'J', 11, 0}, {'S', 'Q', 12, 0}, {'S', 'K', 13, 0}
};

     // bet for player and PC. Give first three cards to player and to PC. 
     // database for each combination and how to find it for the PC
     // 


     printf("The starting bet is 100$\n");
     if(!bet(100)){
          return;
     }

     int bank = 200;

     Card  players_cards[7];
     Card  computer_cards[7];
     Card  dealer_cards[5];

     for(int i=0;i<7;i++){         //obnulenie cart
          if(i<5){
               dealer_cards[i] = (Card){0, 0, 0, 0};
          }
        players_cards[i] = (Card){0, 0, 0, 0};
        computer_cards[i] = (Card){0, 0, 0, 0};
        
    }
     

     

     printf("\t!!! First cards on a table !!!\n");

     
     int counter_cards =0;
     int iterpl=0,itercomp=0;     
     while(counter_cards<4){
          int random_number = rand() % 53;
          if(!deck[random_number].taken){
               
               deck[random_number].taken  =1;
               if(counter_cards%2==1){
                    players_cards[iterpl] = deck[random_number];
                    iterpl++;
               }else{
                    computer_cards[itercomp] = deck[random_number];
                    itercomp++;
               }
               counter_cards++;
          }
     }


     int counter_dealer=0;
     while(counter_dealer<3){
          int random_number = rand() % 53;
          if(!deck[random_number].taken){
               deck[random_number].taken  =1;
               dealer_cards[counter_dealer++] = deck[random_number];
          }
     }

     printf("Player's cards: ");
     print_cards(players_cards,2);

     printf("\nComputer's cards: ");
     print_cards(computer_cards,2);

     printf("\nDealer's cards : ");
     print_cards(dealer_cards,3);
     printf("\n");
     

     int counter =2;
     for(int i=0;i<3;i++){
          players_cards[(counter+i)] = dealer_cards[i];
          computer_cards[(counter+i)] = dealer_cards[i];
     }
     

     Result computer_combinations[10] = {0};
     Result player_combinations[10] = {0};


     size_t cards_size = sizeof(players_cards) / sizeof(players_cards[0]);
     size_t cards_size2 = sizeof(computer_cards) / sizeof(computer_cards[0]);
     qsort(players_cards, cards_size, sizeof(Card), Compare_cards);
     qsort(computer_cards, cards_size2, sizeof(Card), Compare_cards);
     qsort(dealer_cards,cards_size,sizeof(Card), Compare_cards);

     
     print_cards(players_cards,7);
     printf("\n");
     print_cards(computer_cards,7);
     printf("\n");
     
     combinations_check(player_combinations,players_cards, 7);
     combinations_check(computer_combinations, computer_cards, 7);

/*
     printf("Player's combinations:");
     for(int i=1;i<10;i++){
          printf("%d %d, ", player_combinations[i].low, player_combinations[i].high);
     }
     printf("\n");
     printf("Computer's combinations:");
     for(int i=1;i<10;i++){
          printf("%d %d, ", computer_combinations[i].low, computer_combinations[i].high);
     }
     printf("\n");
     
     */

     int computer_credits = 1000;
     int players_bet =0;
     printf("Enter your bet:");
     scanf(" %d", &players_bet);


     if(!bet(players_bet)){  
          return;
     }


     bank += players_bet;


     //count computers bet
     int computers_bet = 20;
     for(int i= 6;i>=0;i-- ){
          if(computer_combinations[i].low !=0 && i!=0){
               computers_bet*=i;
               break;
          }
     }

     printf("\n\n");
     //no combination on computer 
     if(computers_bet == 20){
          int randomka = rand()%3;
          if(randomka % 2 == 0){
               computers_bet  *= rand()%6;
               computer_credits -= computers_bet;
               bank += computers_bet;
          }
          else{

               printf("YOU WON %d\n", bank);
               player.credits += bank;
               return;
          }
     }

     computer_credits -= computers_bet;
     bank += computers_bet;

     if(players_bet < computers_bet){
          printf("Computer's bet is %d\n", computers_bet);
          if(!bet(computers_bet - players_bet)){  
               return;
          }
     }

     if(computers_bet < players_bet){
          printf("--Computer EQUALS--\n");
          computer_credits -= (players_bet-computers_bet);
          if(computer_credits<0){
               printf("YOU WON %d\n", bank);
               player.credits += bank;
               return;
          }
          bank += (players_bet-computers_bet);
     }

/*
     int counter_dealer=0;
     while(counter_dealer<3){
          int random_number = rand() % 53;
          if(!deck[random_number].taken){
               deck[random_number].taken  =1;
               dealer_cards[counter_dealer++] = deck[random_number];
          }
     }
*/

     int random_number = rand() % 53;
     while(deck[random_number].taken){
          random_number = rand() % 53;
     }

     deck[random_number].taken  =1;
     dealer_cards[1] = deck[random_number];

     computer_cards[1] = deck[random_number];
     players_cards[1] = deck[random_number];

    
     qsort(players_cards, cards_size, sizeof(Card), Compare_cards);
     qsort(computer_cards, cards_size2, sizeof(Card), Compare_cards);
     qsort(dealer_cards,cards_size,sizeof(Card), Compare_cards);
     combinations_check(player_combinations,players_cards, 7);
     combinations_check(computer_combinations, computer_cards, 7);
     print_cards(players_cards,7);
     printf("\n");
     print_cards(computer_cards,7);
     printf("\n");

     computers_bet = 20;
     for(int i= 6;i>=0;i-- ){
          if(computer_combinations[i].low !=0 && i!=0){
               computers_bet*=i;
               break;
          }
     }

     printf("\n\n");
     //no combination on computer 
     if(computers_bet == 20){
          int randomka = rand()%3;
          if(randomka % 2 == 0){
               computers_bet  *= rand()%6;
               computer_credits -= computers_bet;
               bank += computers_bet;
          }
          else{

               printf("YOU WON %d\n", bank);
               player.credits += bank;
               return;
          }
     }

     computer_credits -= computers_bet;
     bank += computers_bet;
     printf("COMPUTER's bet is %d\n", computers_bet);
     if(!bet(computers_bet)){
          printf("YOU LOST!!!!");
          return;
     }


     random_number = rand() % 53;
     while(deck[random_number].taken){
          random_number = rand() % 53;
     }

     deck[random_number].taken  =1;
     dealer_cards[0] = deck[random_number];

     computer_cards[0] = deck[random_number];
     players_cards[0] = deck[random_number];
     qsort(players_cards, cards_size, sizeof(Card), Compare_cards);
     qsort(computer_cards, cards_size2, sizeof(Card), Compare_cards);
     qsort(dealer_cards,cards_size,sizeof(Card), Compare_cards);
     combinations_check(player_combinations,players_cards, 7);
     combinations_check(computer_combinations, computer_cards, 7);

     print_cards(players_cards,7);
     printf("\n");
     print_cards(computer_cards,7);
     printf("\n");


       printf("Enter your bet:");
     scanf(" %d", &players_bet);


     if(!bet(players_bet)){  
          return;
     }


     bank += players_bet;


     //count computers bet
     computers_bet = 20;
     for(int i= 6;i>=0;i-- ){
          if(computer_combinations[i].low !=0 && i!=0){
               computers_bet*=i;
               break;
          }
     }

     printf("\n\n");
     //no combination on computer 
     if(computers_bet == 20){
          int randomka = rand()%3;
          if(randomka % 2 == 0){
               computers_bet  *= rand()%6;
               computer_credits -= computers_bet;
               bank += computers_bet;
          }
          else{

               printf("YOU WON %d\n", bank);
               player.credits += bank;
               return;
          }
     }

     computer_credits -= computers_bet;
     bank += computers_bet;

     if(players_bet < computers_bet){
          printf("Computer's bet is %d\n", computers_bet);
          if(!bet(computers_bet - players_bet)){  
               return;
          }
     }

     if(computers_bet < players_bet){
          printf("--Computer EQUALS--\n");
          computer_credits -= (players_bet-computers_bet);
          if(computer_credits<0){
               printf("YOU WON %d\n", bank);
               player.credits += bank;
               return;
          }
          bank += (players_bet-computers_bet);
     }

     int max_index_comp=-1;
     int max_index_player=-1;

     for(int i=0;i<7;i++){
          if(computer_combinations[i].low!=0 ){
               max_index_comp = i;
          }
          if(player_combinations[i].low != 0 ){
               max_index_player =i;
          }
     }

     if(max_index_player > max_index_comp){
          printf("YOU WON %d\n", bank);
          player.credits += bank;
          return;
     }
     else if(max_index_player < max_index_comp){
          printf("YOU LOST\n\n");
          return;
     }



     // flush - 5
     int res =0;
     if(max_index_comp == 5){
          res = compareFlashes(players_cards,computer_cards,7);
          
     }else{
          if(player_combinations[max_index_player].low < computer_combinations[max_index_comp].low){
               res = -1;
          }else if(player_combinations[max_index_player].low > computer_combinations[max_index_comp].low){
               res =1;
          }
          else{
               res =0;
          }
     }


     
     if(res > 0){
          printf("YOU WON %d\n", bank);
          player.credits += bank;
          return;
     }
     else if(res<0){
          printf("YOU LOST\n\n");
          return;
     }
     else{
          printf("---DRAW---\n YOU WON %d\n", bank/2);
          player.credits += bank/2;
          return;
     }


   printf("\n\n\n\n");



     
}




int get_player_data(){
     int fd, uid, read_bytes;

     PLAYER entry;
     
     uid = getuid();

     fd = open(DATAFILE,  O_RDONLY );
     if(fd == -1){ // could not open the file

          printf("Could not open the file\n\n");
          exit(-1);
          return -1;
     }

     read_bytes = read(fd, &entry, sizeof(struct player));

     while(entry.uid != uid && read_bytes>0){ // if something was read and userids are similar
          read_bytes = read(fd, &entry, sizeof(struct player));
     }


     close(fd); // closing the file

     if(read_bytes < sizeof(struct player)){ // EOF
          return -1;
     }
     else{
          player = entry;
     }

     return 1;


}

void register_new_player(){
     int fd;
     printf("-=REGISTRATION OF NEW PALYER=-\n");

     printf("Enter your name:");
     input_name();

     player.uid = getuid();
     player.credits = 500;

     fd = open(DATAFILE, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR|S_IWUSR);

     if(fd == -1){
          printf("ERROR IN OPENING THE FILE WHILE REGISTRATION!!!!!!\n\n");
          exit(-1);
     }

     write(fd, &player, sizeof(struct player));
     close(fd);



     printf("WELCOME TO THE POKER GAME\n");
     printf("Your balance is %u credits\n", player.credits);


}



void input_name(){ //rewrite it on your way

     char * name_ptr, input_char='\n';
     while(input_char == '\n'){ //clean all enters
          scanf("%c", &input_char);
     }

     name_ptr = (char*) &(player.name);

     int counter = 1;
     while(input_char != '\n' && counter<100){
          *name_ptr = input_char;
          scanf("%c", &input_char);
          name_ptr++;
          counter++;

     }

     *name_ptr = '\0';


}

void update_player_data(){

     int fd;
     int read_uid;

     char burned_byte;

     fd = open(DATAFILE, O_RDWR);
     if(fd == -1){
          printf("ERROR IN OPENING FILE WHILE UPDATING PLAYER'S DATA");
     }

     read(fd,&read_uid,4);


     while(read_uid!=player.uid){
          for(int i=0;i<sizeof(struct player) - 4; i++){
               read(fd,&burned_byte,1);//reading the rest of player by byte
          }

          read(fd,&read_uid,4);
     }


     write(fd,&(player.name),100);
     write(fd,&(player.credits),4);

     close(fd);


}

