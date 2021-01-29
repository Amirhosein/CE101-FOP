#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void Countdown(){
    for (int i = 3; i > 0; i--) {
        printf("The Game Will start within %d seconds ...", i);
        Sleep(1);
        system("cls");
    }
}



typedef struct MAPindex{
    int x;
    int y;
} mapindex;

typedef struct SHIP{

    mapindex * indexes;
    int size;
    struct SHIP * next;
    struct SHIP * prev;

}ship;

void availableShipsMenu(ship* head){
    ship * curr = NULL;
    int temp[100] = {0}, i = 0, count[100] = {0}, stat = 0;
    for(curr = head->next; curr!=NULL; curr = curr->next){
        stat = 0;
        temp[i] = curr->size;
        for (int j = 0; temp[j] != 0 ; j++) {
            if (temp[i] == temp[j]) {
                stat++;
            }
        }

        if ( stat > 1 )
            continue;
        for(ship* curr1 = head->next; curr1 != NULL; curr1 = curr1->next){
            if(curr1->size == temp[i] && curr1->indexes[0].x == -2)
                count[i]++;
        }
        printf("Size: %d, Count: %d\n", temp[i], count[i]);
        i++;
    }
}

ship* NewShip(int n){
    ship* output = (ship*)malloc(sizeof(ship));
    output->indexes = (mapindex*)malloc(n * sizeof(mapindex));
    output->indexes[0].x = -2;
    output->size = n;
    output->next = NULL;
    output->prev = NULL;
}

void AddShip(ship * head, ship* new){
    ship* curr = NULL;
    for(curr = head; curr->next != NULL; curr = curr->next);
    curr->next = new;
    new->prev = curr;
}

void addressship(int n, ship* head,int map[10][10]){
    ship* curr = NULL;
    int inputx,inputy;
    for(curr = head; curr!=NULL; curr = curr->next){
        if(curr->size == n && curr->indexes[0].x == -2)
            break;

    }
    for(int i = 0;i < n; i++ ){
        scanf("%d,%d", &inputx, &inputy);
        curr->indexes[i].x = inputx -1;
        curr->indexes[i].y = inputy -1;
        map[inputx -1][inputy -1] = 4;
    }
}

typedef struct player{
    char Name[100];
    int Coins;
    struct player * next;
    struct player * prev;
}Player;

Player * NewPlayer(char name[100]){
    Player* output = (Player *)malloc(sizeof(Player));
    strcpy(output->Name, name);
    output->Coins = 0;
    output->next = NULL;
    output->prev = NULL;
}

void  AddPlayer(Player * head, Player * newplayer ){
    Player * curr = NULL;
    for(curr = head; curr->next != NULL; curr = curr->next);
    curr->next = newplayer;
    newplayer->prev = curr;
}

void Menu(){
    printf("1. Play with a Friend\n");
    printf("2. Play with bot\n");
    printf("3. Load game\n");
    printf("4. Load last game\n");
    printf("5. Settings\n");
    printf("6. Score Board\n");
    printf("7. Exit\n");

}

void MAPprint(int MAP[10][10]){
    printf("     ");
    for (int i = 0; i < 10; i++) {
        printf("%d    ",i+1);
    }
    printf("\n");
    for(int i = 0; i < 10; i++){
        printf("   ");
        for(int j = 0; j<10; j++){
            printf("|---|");
        }
        printf("\n");
        if(i+1 >= 10)
            printf("%d ",i+1);
        else
            printf("%d  ",i+1);
        for(int j = 0; j<10; j++){
            if(MAP[j][i] == 2)
                printf("| E |");
            else if(MAP[j][i] == 3)
                printf("| C |");
            else if(MAP[j][i] == 1)
                printf("| W |");
            else if(MAP[j][i] == 4)
                printf("| S |");
            else
                printf("|   |");
        }
        printf("\n");
    }
    printf("   ");
    for(int i = 0; i<10; i++){
        printf("|---|");
    }
    printf("\n");
}

void RemoveShip(ship* DestroyedShip, ship* head){
    ship* curr = NULL;
    DestroyedShip->prev->next = DestroyedShip->next;
    DestroyedShip->next->prev = DestroyedShip->prev;
    free(DestroyedShip);
}

void TheGame(int MAP1[10][10],int MAP2[10][10], ship* headship1, ship* headship2, Player* player1, Player* player2 ){
    int stat = 1, inputx, inputy;
    ship* curr;


    while(1) {
        int turn = 1;
        if (headship1->next == NULL || headship2->next == NULL) {
            if (headship1->next == NULL) {
                printf("Player %s wins with total coins: %d", player1->Name, player1->Coins);
                break;
            } else {
                printf("Player %s wins with total coins: %d", player2->Name, player2->Coins);
                break;
            }
        }
        printf("This is Player%d's turn.\nPlease enter the coordinates to be fired.\n", turn);
        scanf("%d,%d",&inputx , &inputy);
        for(curr = headship2; curr != NULL;curr = curr->next){
            for (int i = 0; i < curr->size; i++) {
                if( curr->indexes[i].x == inputx && curr->indexes[i].y == inputy){
                    curr->indexes[i].x = -10;
                    curr->indexes[i].y = -10;
                    MAP2[inputx][inputy] = 2;
                }

                // Checking if all indexes of a ship have been destroyed.
                for (int j = 0; j < curr->size; j++) {
                    if(curr->indexes[j].x != -10 && curr->indexes[j].y != -10) {
                        stat = -1;
                        break;
                    }
                }
                if(stat == 1){

                    // Applying destruction of ship on map
                    for (int j = 0; j < curr->size; j++)
                        MAP2[curr->indexes[j].x][curr->indexes[j].y] = 3;
                    RemoveShip(curr, headship2);

                }
            }
        }
        if (headship1->next == NULL || headship2->next == NULL) {
            if (headship1->next == NULL) {
                printf("Player %s wins with total coins: %d", player1->Name, player1->Coins);
                break;
            } else {
                printf("Player %s wins with total coins: %d", player2->Name, player2->Coins);
                break;
            }
        }
        MAPprint(MAP2);
        Sleep(3);
        turn++;
        printf("This is Player%d's turn.\nPlease enter the coordinates to be fired.\n", turn);
        scanf("%d,%d",&inputx , &inputy);
        for(curr = headship1; curr != NULL;curr = curr->next){
            for (int i = 0; i < curr->size; i++) {
                if( curr->indexes[i].x == inputx && curr->indexes[i].y == inputy){
                    curr->indexes[i].x = -10;
                    curr->indexes[i].y = -10;
                    MAP1[inputx][inputy] = 2;
                }

                // Checking if all indexes of a ship have been destroyed.
                for (int j = 0; j < curr->size; j++) {
                    if(curr->indexes[j].x != -10 && curr->indexes[j].y != -10) {
                        stat = -1;
                        break;
                    }
                }
                if(stat == 1){

                    // Applying destruction of ship on map
                    for (int j = 0; j < curr->size; j++)
                        MAP2[curr->indexes[j].x][curr->indexes[j].y] = 3;
                    RemoveShip(curr, headship1);

                }
            }
        }
        MAPprint(MAP1);
        Sleep(3);
    }
}


int main() {
    int input, inc = 1, shipscount = 10;
    int MAP1[10][10] = { 0 };
    int MAP2[10][10] = { 0 };
    int tempmap[10][10] = { 0 };
    char inputname[100];

    // Default Ships for each player declaration
    ship* headship1 = NewShip(0);
    AddShip(headship1, NewShip(5));
    AddShip(headship1, NewShip(3));
    AddShip(headship1, NewShip(3));
    AddShip(headship1, NewShip(2));
    AddShip(headship1, NewShip(2));
    AddShip(headship1, NewShip(2));
    AddShip(headship1, NewShip(1));
    AddShip(headship1, NewShip(1));
    AddShip(headship1, NewShip(1));
    AddShip(headship1, NewShip(1));

    ship* headship2 = NewShip(0);
    AddShip(headship2, NewShip(5));
    AddShip(headship2, NewShip(3));
    AddShip(headship2, NewShip(3));
    AddShip(headship2, NewShip(2));
    AddShip(headship2, NewShip(2));
    AddShip(headship2, NewShip(2));
    AddShip(headship2, NewShip(1));
    AddShip(headship2, NewShip(1));
    AddShip(headship2, NewShip(1));
    AddShip(headship2, NewShip(1));
    // Players declaration

    Player * head = NewPlayer("head");
    Player * player1 = NULL;
    Player * player2 = NULL;
    Player* curr = NULL;



    Menu();
    while (1)
    {
        scanf("%d",&input);
        system("cls");
        if (input == 1){
            while(1) {
                // first player asking //
                printf("First PLayer: \n\t");
                printf("Choose User:\n\t1. Choose from available users\n\t2. New User\n");


                scanf("%d", &input);
                system("cls");
                if (input == 1) {
                    if (head->next == NULL) {
                        printf("There's no users\n");
                        continue;
                    }

                    for (curr = head->next; curr != NULL; curr = curr->next, inc++)

                        printf("%d. %s with %d coins.\n", inc, curr->Name, curr->Coins);

                    inc = 0;
                    scanf("%d", &input);
                    system("cls");
                    for (curr = head->next; inc < input; inc++, curr = curr->next);
                    player1 = curr;
                    break;
                } else if (input == 2) {
                    printf("\nPlease enter new user's name: ");
                    fflush(stdin);
                    gets(inputname);
                    AddPlayer(head, NewPlayer(inputname));
                    for (curr = head->next; curr->next != NULL; curr = curr->next);
                    player1 = curr;
                    break;
                }
            }
            while(1) {
                // second player asking
                printf("Second PLayer: \n\t");
                printf("Choose User:\n\t1. Choose from available users\n\t2. New User\n");


                scanf("%d", &input);
                system("cls");
                if (input == 1) {
                    if(head->next == NULL) {
                        printf("There's no user.\n");
                        continue;
                    }
                    for (curr = head->next; curr != NULL; curr = curr->next, inc++)

                        printf("%d. %s with %d coins.\n", inc, curr->Name, curr->Coins);

                    inc = 0;
                    scanf("%d", &input);
                    system("cls");
                    for (curr = head->next; inc < input; inc++, curr = curr->next);
                    player2 = curr;
                    break;
                } else if (input == 2) {
                    printf("\nPlease enter new user's name: ");
                    fflush(stdin);
                    gets(inputname);
                    AddPlayer(head, NewPlayer(inputname));
                    for (curr = head; curr->next != NULL; curr = curr->next);
                    player2 = curr;
                    break;
                }
            }
            printf("First Player's Arrangement:\n\t1. Auto\n\t2. Manual\n");
            scanf("%d", &input);
            system("cls");

            if(input == 2){
                MAPprint(MAP1);
                availableShipsMenu(headship1);
                printf("Please select a ship to be addressed.\n");
                for (int i = 0; i < shipscount; i++) {
                    scanf("%d", &input);
                    printf("Addressing The ship with size: %d\n", input);
                    addressship(input, headship1, tempmap);
                    system("cls");
                    MAPprint(tempmap);
                    printf("Done! Please select another ship.\n");
                    availableShipsMenu(headship1);
                }
                system("cls");
            }
            int tempmap[10][10] = { 0 };
            printf("Second Player's Arrangement:\n\t1. Auto\n\t2. Manual\n");
            scanf("%d", &input);
            system("cls");
            if(input == 2){
                MAPprint(MAP2);
                availableShipsMenu(headship2);
                printf("Please select a ship to be addressed.\n");
                for (int i = 0; i < shipscount; i++) {
                    scanf("%d", &input);
                    printf("Addressing The ship with size: %d\n", input);
                    addressship(input, headship2, tempmap);
                    printf("Done! Please select another ship.\n");
                    availableShipsMenu(headship2);
                }
                system("cls");
            }
            Countdown();


        }

        else if(input == 2){
            // player asking //
            printf("First PLayer: \n\t");
            printf("Choose User:\n\t1. Choose from available users\n\t2. New User\n");


            scanf("%d", &input);
            system("cls");
            if (input == 1){
                for(curr = head; curr!=NULL; curr = curr->next , inc++)

                    printf("%d. %s with %d coins.\n", inc, curr->Name, curr->Coins);

                inc = 0;
                scanf("%d", &input);
                system("cls");
                for (curr = head; inc < input; inc++, curr = curr->next );
                player1 = curr;
            }

            else if (input == 2) {
                printf("\nPlease enter new user's name: ");
                fflush(stdin);
                gets(inputname);
                AddPlayer(head, NewPlayer(inputname));
                for(curr = head; curr->next != NULL; curr = curr->next );
                player1 = curr;
            }
            printf("Player's Arrangement:\n\t1. Auto\n\t2. Manual\n");
            scanf("%d", &input);
            system("cls");

            // Arrangement Program will be placed here soon ...

        }

        else if (input == 3){
            // Load & save program will be placed here
        }

        else if(input == 4){
            // Load last game program will be placed here
        }

        else if(input == 5){
            printf("1. Ships\n2. Map Size\n3. Theme\n");
            scanf("%d", &input);
            system("cls");
        }

        else if( input == 6 ){
            // score board program will be placed here
        }


        if (input == 7)
            break;
    }
    return 0;
}
