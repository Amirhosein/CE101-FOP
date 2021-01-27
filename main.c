#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main() {
    int input, inc = 1;
    char inputname[100];
    Player * head = NewPlayer("head");
    Player * player1 = NULL;
    Player * player2 = NULL;
    Player* curr = NULL;



    Menu();
    while (1)
    {
        scanf("%d",&input);
        if (input == 1){
            // first player asking //
            printf("First PLayer: \n\t");
            printf("Choose User:\n\t1. Choose from available users\n\t2. New User\n");


            scanf("%d", &input);
            if (input == 1){
                if (head->next == NULL)
                    printf("There's no users\n");

                for(curr = head->next; curr!=NULL; curr = curr->next , inc++)

                    printf("%d. %s with %d coins.\n", inc, curr->Name, curr->Coins);

                inc = 0;
                scanf("%d", &input);
                for (curr = head->next; inc < input; inc++, curr = curr->next );
                player1 = curr;
            }

            else if (input == 2) {
                printf("\nPlease enter new user's name: ");
                fflush(stdin);
                gets(inputname);
                AddPlayer(head, NewPlayer(inputname));
                for(curr = head->next; curr->next != NULL; curr = curr->next );
                player1 = curr;
            }

            // second player asking
            printf("Second PLayer: \n\t");
            printf("Choose User:\n\t1. Choose from available users\n\t2. New User\n");


            scanf("%d", &input);
            if (input == 1){
                for(curr = head->next; curr!=NULL; curr = curr->next , inc++)

                    printf("%d. %s with %d coins.\n", inc, curr->Name, curr->Coins);

                inc = 0;
                scanf("%d", &input);
                for (curr = head->next; inc < input; inc++, curr = curr->next );
                player2 = curr;
            }

            else if (input == 2) {
                printf("\nPlease enter new user's name: ");
                fflush(stdin);
                gets(inputname);
                AddPlayer(head, NewPlayer(inputname));
                for(curr = head; curr->next != NULL; curr = curr->next );
                player2 = curr;
            }
            printf("First Player's Arrangement:\n\t1. Auto\n\t2. Manual\n");
            scanf("%d", &input);

            // Arrangement Program will be placed here soon ...

            printf("Second Player's Arrangement:\n\t1. Auto\n\t2. Manual\n");
            scanf("%d", &input);
        }

        else if(input == 2){
            // player asking //
            printf("First PLayer: \n\t");
            printf("Choose User:\n\t1. Choose from available users\n\t2. New User\n");


            scanf("%d", &input);
            if (input == 1){
                for(curr = head; curr!=NULL; curr = curr->next , inc++)

                    printf("%d. %s with %d coins.\n", inc, curr->Name, curr->Coins);

                inc = 0;
                scanf("%d", &input);
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
        }

        else if( input == 6 ){
            // score board program will be placed here
        }


        if (input == 7)
            break;
    }
    return 0;
}
