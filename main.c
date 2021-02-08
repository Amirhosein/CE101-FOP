#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dir.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void Countdown(){
    for (int i = 3; i > 0; i--) {
        printf("The Game Will start within %d seconds ...", i);
        Sleep(1000);
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

Player * NewPlayer(char name[100],int coin){
    Player* output = (Player *)malloc(sizeof(Player));
    strcpy(output->Name, name);
    output->Coins = coin;
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
    if(DestroyedShip->next != NULL){
        DestroyedShip->next->prev = DestroyedShip->prev;
    }
    free(DestroyedShip);
}

void AutomaticArrangement(int MAP[10][10], ship* head){
    int direction = 0;
    int xstart,ystart;
    ship* curr = NULL;
    for (curr = head->next; curr != NULL; curr = curr->next) {
        direction = rand() % 2 + 1;
        if( direction == 1) {
            while (1) {
                srand(time(NULL));
                int stat = 0;
                xstart = rand() % 10;
                ystart = rand() % 10;
                if (xstart != 0){
                    for (int i = -1; i < 2; i++) {
                        if (MAP[xstart - 1][ystart + i] == 4) {
                            stat = -1;
                            break;
                        }
                    }
                }
                if( stat == -1)
                    continue;
                if (xstart + curr->size > 9) {
                    stat = -1;
                    continue;
                }
                for (int i = 0; i < curr->size; i++) {
                    for (int j = -1; j < 2; j++) {
                        //printf("%d ",MAP[xstart][ystart + i]);
                        if (MAP[xstart + i][ystart + j] == 4){
                            stat = -1;
                            break;
                        }
                    }
                    //printf("\n");
                    if( stat == -1)
                        break;
                }
                if( stat == -1)
                    continue;
                if (xstart + curr->size - 1 != 9) {
                    for (int i = -1; i < 2; i++) {
                        if (MAP[xstart + curr->size][ystart + i] == 4) {
                            stat = -1;
                            break;
                        }
                    }
                }
                if( stat == -1)
                    continue;
                else {
                    for (int i = 0; i < curr->size; i++) {
                        curr->indexes[i].y = ystart;
                        curr->indexes[i].x = xstart + i;
                        MAP[xstart + i][ystart] = 4;
                    }
                    break;
                }
            }

        }
        else{
            while (1) {
                srand(time(NULL));
                int stat = 0;
                xstart = rand() % 10;
                ystart = rand() % 10;
                if (ystart != 0){
                    for (int i = -1; i < 2; i++) {
                        if (MAP[xstart + i][ystart-1] == 4) {
                            stat = -1;
                            break;
                        }
                    }
                }
                if (ystart + curr->size > 9) {
                    stat = -1;
                    continue;
                }
                for (int i = 0; i < curr->size; i++) {
                    for (int j = -1; j < 2; j++) {
                        if (MAP[xstart + j][ystart + i] == 4){
                            stat = -1;
                            break;
                        }
                    }
                    if( stat == -1)
                        break;
                }
                if( stat == -1)
                    continue;
                if (ystart + curr->size - 1 != 9) {
                    for (int i = -1; i < 2; i++) {
                        if (MAP[xstart + i][ystart + curr->size] == 4) {
                            stat = -1;
                            break;
                        }
                    }
                }
                if( stat == -1)
                    continue;
                else {
                    for (int i = 0; i < curr->size; i++) {
                        curr->indexes[i].y = ystart + i;
                        curr->indexes[i].x = xstart;
                        MAP[xstart][ystart + i] = 4;
                    }
                    break;
                }
            }
        }
    }
}

void CheckDir(){

    struct stat st = {0};
    if (stat("data", &st) == -1) {
        mkdir("data");
    }
    FILE * fp = fopen("data/lastgame.txt","r");
    FILE * fp1 = fopen("data/saves.txt","r");
    FILE * fp2 = fopen("data/users.txt","r");

    if(fp == NULL) {
        fclose(fp);
        fp = fopen("data/lastgame.txt", "w");
        fclose(fp);
    }
    if(fp1 == NULL) {
        fclose(fp1);
        fp1 = fopen("data/saves.txt", "w");
        fclose(fp1);
    }
    if(fp2 == NULL) {
        fclose(fp2);
        fp2 = fopen("data/users.txt", "w");
        fclose(fp2);
    }
    if (stat("data/users", &st) == -1) {
        mkdir("data/users");
    }
    if (stat("data/saves", &st) == -1) {
        mkdir("data/saves");
    }

}

void playerfile(char inputname[100]){
    FILE * users = fopen("data/users.txt","a");
    fprintf(users,"%s\n",inputname);
    fclose(users);
    char *filename = calloc(100, 1);
    sprintf(filename, "data/users/%s.coin", inputname);
    FILE *playerfile = fopen(filename, "wb");
    fprintf(playerfile, "0");
    fclose(playerfile);
}

void importplayers(Player * head){
    int c;
    char name[100];
    FILE * playerlist = fopen("data/users.txt" ,"r");
    while(1){
        if (feof(playerlist)) break;
        int coin;
        fscanf(playerlist,"%s\n",name);
        char *filename = calloc(100, 1);
        sprintf(filename, "data/users/%s.coin", name);
        FILE * player = fopen(filename, "rb");
        fscanf(player,"%d",&coin);
        AddPlayer(head, NewPlayer(name,coin));
    }
}

void save (int MAP1[10][10],int MAP2[10][10], ship* headship1, ship* headship2, Player* player1, Player* player2,int turn,char name[100]){

}

void saveplayer(Player * target){
    char* filename = calloc(100, 1);
    sprintf(filename, "data/users/%s.coin", target->Name);
    FILE * player = fopen(filename, "wb");
    fprintf(player,"%d",target->Coins);
    fclose(player);
}

void TheGame(int MAP1[10][10],int MAP2[10][10], ship* headship1, ship* headship2, Player* player1, Player* player2,int turn){
    int stat = 1, inputx, inputy;
    int max1 = 0, max2 = 0;
    ship* curr;
    // Finding Largest size player1's ships
    for(curr = headship1->next; curr!=NULL;curr = curr->next){
        if(curr->size > max1)
            max1 = curr->size;
    }
    // Finding Largest size player2's ships
    for(curr = headship2->next; curr!=NULL;curr = curr->next){
        if(curr->size > max2)
            max2 = curr->size;
    }

    while(1) {
        if(turn == 1) {
            if (headship1->next == NULL || headship2->next == NULL) {
                if (headship1->next == NULL) {
                    printf("Player %s wins with total coins: %d", player2->Name, player2->Coins);
                    break;
                } else {
                    printf("Player %s wins with total coins: %d", player1->Name, player1->Coins);
                    break;
                }
            }
            system("cls");
            MAPprint(MAP2);
            printf("This is Player %s's turn.\nPlease enter the coordinates to be fired.\n", player1->Name);
            scanf("%d,%d", &inputx, &inputy);

            for (curr = headship2->next; curr != NULL; curr = curr->next) {
                for (int i = 0; i < curr->size; i++) {
                    if ((curr->indexes[i].x == (inputx - 1)) && (curr->indexes[i].y == (inputy - 1))) {
                        MAP2[inputx - 1][inputy - 1] = 2;
                        stat = -2;
                        player1->Coins++;

                        //Checking if all indexes of a ship have been destroyed.
                        for (int j = 0; j < curr->size; j++) {
                            if (MAP2[curr->indexes[j].x][curr->indexes[j].y] == 2) {
                                stat = -1;
                            } else {
                                stat = -2;
                                break;
                            }
                        }
                    } else {
                        MAP2[inputx - 1][inputy - 1] = 1;
                    }
                    if (stat == -1) {
                        // Applying destruction of ship on map
                        for (int j = 0; j < curr->size; j++)
                            MAP2[curr->indexes[j].x][curr->indexes[j].y] = 3;
                        if (curr->size != 1 && (curr->indexes[1].x - curr->indexes[0].x == 1)) {
                            if ( curr->indexes[0].y != 0 ) {
                                if (curr->indexes[0].x != 0)
                                    MAP2[curr->indexes[0].x - 1][curr->indexes[0].y - 1] = 1;

                                if (curr->indexes[0].x + curr->size <= 9)
                                    MAP2[curr->indexes[0].x + curr->size][curr->indexes[0].y - 1] = 1;

                                for (int j = curr->indexes[0].x; j < curr->indexes[0].x + curr->size; j++)
                                    MAP2[j][curr->indexes[0].y - 1] = 1;
                            }
                            if (curr->indexes[0].x != 0)
                                MAP2[curr->indexes[0].x - 1][curr->indexes[0].y] = 1;

                            if (curr->indexes[0].x != 9)
                                MAP2[curr->indexes[0].x + curr->size][curr->indexes[0].y] = 1;

                            if(curr->indexes[0].y != 9) {
                                if (curr->indexes[0].x != 0)
                                    MAP2[curr->indexes[0].x - 1][curr->indexes[0].y + 1] = 1;

                                if (curr->indexes[0].x + curr->size <= 9)
                                    MAP2[curr->indexes[0].x + curr->size][curr->indexes[0].y + 1] = 1;

                                for (int j = curr->indexes[0].x; j < curr->indexes[0].x + curr->size; j++) {
                                    MAP2[j][curr->indexes[0].y + 1] = 1;
                                }
                            }
                        } else {
                            if (curr->indexes[0].x != 9) {
                                if (curr->indexes[0].y != 0)
                                    MAP2[curr->indexes[0].x + 1][curr->indexes[0].y - 1] = 1;

                                if (curr->indexes[0].y + curr->size <= 9)
                                    MAP2[curr->indexes[0].x + 1][curr->indexes[0].y + curr->size] = 1;

                                for (int j = curr->indexes[0].y; j < curr->indexes[0].y + curr->size; j++)
                                    MAP2[curr->indexes[0].x + 1][j] = 1;
                            }

                            if (curr->indexes[0].y != 0) {
                                MAP2[curr->indexes[0].x][curr->indexes[0].y - 1] = 1;
                            }

                            if (curr->indexes[0].y != 9) {
                                MAP2[curr->indexes[0].x][curr->indexes[0].y + curr->size] = 1;
                            }

                            if (curr->indexes[0].x != 0) {
                                if (curr->indexes[0].y != 0)
                                    MAP2[curr->indexes[0].x - 1][curr->indexes[0].y - 1] = 1;

                                if (curr->indexes[0].y + curr->size <= 9)
                                    MAP2[curr->indexes[0].x - 1][curr->indexes[0].y + curr->size] = 1;

                                for (int j = curr->indexes[0].y; j < curr->indexes[0].y + curr->size; j++)
                                    MAP2[curr->indexes[0].x - 1][j] = 1;
                            }
                        }
                        player1->Coins += (5 * (max1))/(curr->size);
                        saveplayer(player1);
                        RemoveShip(curr, headship2);
                        break;

                    } else if (stat == -2) {
                        break;
                    }

                }
                if (stat == -2 || stat == -1)
                    break;
            }
            if (headship1->next == NULL || headship2->next == NULL) {
                if (headship1->next == NULL) {
                    printf("Player %s wins with total coins: %d", player2->Name, player2->Coins);
                    break;
                } else {
                    printf("Player %s wins with total coins: %d", player1->Name, player1->Coins);
                    break;
                }
            }
            system("cls");
            MAPprint(MAP2);
            Sleep(5000);
            turn++;
            if(stat == -1 || stat == -2)
                turn--;
            stat = 1;
        }
        else {
            if (headship1->next == NULL || headship2->next == NULL) {
                if (headship1->next == NULL) {
                    printf("Player %s wins with total coins: %d", player2->Name, player2->Coins);
                    break;
                } else {
                    printf("Player %s wins with total coins: %d", player1->Name, player1->Coins);
                    break;
                }
            }
            system("cls");
            MAPprint(MAP1);
            printf("This is Player %s's turn.\nPlease enter the coordinates to be fired.\n", player2->Name);
            scanf("%d,%d", &inputx, &inputy);
            for (curr = headship1->next; curr != NULL; curr = curr->next) {
                for (int i = 0; i < curr->size; i++) {
                    if ((curr->indexes[i].x == (inputx - 1)) && (curr->indexes[i].y == (inputy - 1))) {
                        MAP1[inputx - 1][inputy - 1] = 2;
                        stat = -2;
                        player2->Coins++;

                        //Checking if all indexes of a ship have been destroyed.
                        for (int j = 0; j < curr->size; j++) {
                            if (MAP1[curr->indexes[j].x][curr->indexes[j].y] == 2) {
                                stat = -1;
                            } else {
                                stat = -2;
                                break;
                            }
                        }
                    } else {
                        MAP1[inputx - 1][inputy - 1] = 1;
                    }
                    if (stat == -1) {
                        // Applying destruction of ship on map
                        for (int j = 0; j < curr->size; j++)
                            MAP1[curr->indexes[j].x][curr->indexes[j].y] = 3;
                        if (curr->size != 1 && (curr->indexes[1].x - curr->indexes[0].x == 1)) {
                            if ( curr->indexes[0].y != 0 ) {
                                if (curr->indexes[0].x != 0)
                                    MAP1[curr->indexes[0].x - 1][curr->indexes[0].y - 1] = 1;

                                if (curr->indexes[0].x + curr->size <= 9)
                                    MAP1[curr->indexes[0].x + curr->size][curr->indexes[0].y - 1] = 1;

                                for (int j = curr->indexes[0].x; j < curr->indexes[0].x + curr->size; j++)
                                    MAP1[j][curr->indexes[0].y - 1] = 1;
                            }
                            if (curr->indexes[0].x != 0)
                                MAP1[curr->indexes[0].x - 1][curr->indexes[0].y] = 1;

                            if (curr->indexes[0].x != 9)
                                MAP1[curr->indexes[0].x + curr->size][curr->indexes[0].y] = 1;

                            if(curr->indexes[0].y != 9) {
                                if (curr->indexes[0].x != 0)
                                    MAP1[curr->indexes[0].x - 1][curr->indexes[0].y + 1] = 1;

                                if (curr->indexes[0].x + curr->size <= 9)
                                    MAP1[curr->indexes[0].x + curr->size][curr->indexes[0].y + 1] = 1;

                                for (int j = curr->indexes[0].x; j < curr->indexes[0].x + curr->size; j++) {
                                    MAP1[j][curr->indexes[0].y + 1] = 1;
                                }
                            }
                        } else {
                            if (curr->indexes[0].x != 9) {
                                if (curr->indexes[0].y != 0)
                                    MAP1[curr->indexes[0].x + 1][curr->indexes[0].y - 1] = 1;

                                if (curr->indexes[0].y + curr->size <= 9)
                                    MAP1[curr->indexes[0].x + 1][curr->indexes[0].y + curr->size] = 1;

                                for (int j = curr->indexes[0].y; j < curr->indexes[0].y + curr->size; j++)
                                    MAP1[curr->indexes[0].x + 1][j] = 1;
                            }

                            if (curr->indexes[0].y != 0) {
                                MAP1[curr->indexes[0].x][curr->indexes[0].y - 1] = 1;
                            }

                            if (curr->indexes[0].y != 9) {
                                MAP1[curr->indexes[0].x][curr->indexes[0].y + curr->size] = 1;
                            }

                            if (curr->indexes[0].x != 0) {
                                if (curr->indexes[0].y != 0)
                                    MAP1[curr->indexes[0].x - 1][curr->indexes[0].y - 1] = 1;

                                if (curr->indexes[0].y + curr->size <= 9)
                                    MAP1[curr->indexes[0].x - 1][curr->indexes[0].y + curr->size] = 1;

                                for (int j = curr->indexes[0].y; j < curr->indexes[0].y + curr->size; j++)
                                    MAP1[curr->indexes[0].x - 1][j] = 1;
                            }
                        }
                        player2->Coins += (5 * (max1))/(curr->size);
                        saveplayer(player2);
                        RemoveShip(curr, headship1);
                        break;

                    } else if (stat == -2) {
                        break;
                    }

                }
                if (stat == -2 || stat == -1)
                    break;
            }
            if (headship1->next == NULL || headship2->next == NULL) {
                if (headship1->next == NULL) {
                    printf("Player %s wins with total coins: %d", player2->Name, player2->Coins);
                    break;
                } else {
                    printf("Player %s wins with total coins: %d", player1->Name, player1->Coins);
                    break;
                }
            }
            system("cls");
            MAPprint(MAP1);
            if(stat != -1 && stat != -2)
                turn = 1;
            stat = 1;
            Sleep(5000);
        }
    }
}

void BotGame(int MAP1[10][10],int MAP2[10][10], ship* headship1, ship* headship2, Player* player1, Player* player2){
    int stat = 1, inputx, inputy;
    int max1 = 0, max2 = 0;
    ship* curr;
    // Finding Largest size player1's ships
    for(curr = headship1->next; curr!=NULL;curr = curr->next){
        if(curr->size > max1)
            max1 = curr->size;
    }
    // Finding Largest size player2's ships
    for(curr = headship2->next; curr!=NULL;curr = curr->next){
        if(curr->size > max2)
            max2 = curr->size;
    }

    while(1) {
        if (headship1->next == NULL || headship2->next == NULL) {
            if (headship1->next == NULL) {
                printf("Bot wins.");
                break;
            } else {
                printf("Player %s wins with total coins: %d", player1->Name, player1->Coins);
                break;
            }
        }
        MAPprint(MAP2);
        system("cls");
        printf("This is Player %s 's turn.\nPlease enter the coordinates to be fired.\n", player1->Name);
        scanf("%d,%d", &inputx, &inputy);
        for (curr = headship2->next; curr != NULL; curr = curr->next) {
            for (int i = 0; i < curr->size; i++) {
                if ((curr->indexes[i].x == (inputx - 1)) && (curr->indexes[i].y == (inputy - 1))) {
                    MAP2[inputx - 1][inputy - 1] = 2;
                    stat = -2;
                    player1->Coins++;

                    //Checking if all indexes of a ship have been destroyed.
                    for (int j = 0; j < curr->size; j++) {
                        if (MAP2[curr->indexes[j].x][curr->indexes[j].y] == 2) {
                            stat = -1;
                        } else {
                            stat = -2;
                            break;
                        }
                    }
                } else {
                    MAP2[inputx - 1][inputy - 1] = 1;
                }
                if (stat == -1) {
                    // Applying destruction of ship on map
                    for (int j = 0; j < curr->size; j++)
                        MAP2[curr->indexes[j].x][curr->indexes[j].y] = 3;
                    if (curr->size != 1 && (curr->indexes[1].x - curr->indexes[0].x == 1)) {
                        if ( curr->indexes[0].y != 0 ) {
                            if (curr->indexes[0].x != 0)
                                MAP2[curr->indexes[0].x - 1][curr->indexes[0].y - 1] = 1;

                            if (curr->indexes[0].x + curr->size <= 9)
                                MAP2[curr->indexes[0].x + curr->size][curr->indexes[0].y - 1] = 1;

                            for (int j = curr->indexes[0].x; j < curr->indexes[0].x + curr->size; j++)
                                MAP2[j][curr->indexes[0].y - 1] = 1;
                        }
                        if (curr->indexes[0].x != 0)
                            MAP2[curr->indexes[0].x - 1][curr->indexes[0].y] = 1;

                        if (curr->indexes[0].x != 9)
                            MAP2[curr->indexes[0].x + curr->size][curr->indexes[0].y] = 1;

                        if(curr->indexes[0].y != 9) {
                            if (curr->indexes[0].x != 0)
                                MAP2[curr->indexes[0].x - 1][curr->indexes[0].y + 1] = 1;

                            if (curr->indexes[0].x + curr->size <= 9)
                                MAP2[curr->indexes[0].x + curr->size][curr->indexes[0].y + 1] = 1;

                            for (int j = curr->indexes[0].x; j < curr->indexes[0].x + curr->size; j++) {
                                MAP2[j][curr->indexes[0].y + 1] = 1;
                            }
                        }
                    } else {
                        if (curr->indexes[0].x != 9) {
                            if (curr->indexes[0].y != 0)
                                MAP2[curr->indexes[0].x + 1][curr->indexes[0].y - 1] = 1;

                            if (curr->indexes[0].y + curr->size <= 9)
                                MAP2[curr->indexes[0].x + 1][curr->indexes[0].y + curr->size] = 1;

                            for (int j = curr->indexes[0].y; j < curr->indexes[0].y + curr->size; j++)
                                MAP2[curr->indexes[0].x + 1][j] = 1;
                        }

                        if (curr->indexes[0].y != 0) {
                            MAP2[curr->indexes[0].x][curr->indexes[0].y - 1] = 1;
                        }

                        if (curr->indexes[0].y != 9) {
                            MAP2[curr->indexes[0].x][curr->indexes[0].y + curr->size] = 1;
                        }

                        if (curr->indexes[0].x != 0) {
                            if (curr->indexes[0].y != 0)
                                MAP2[curr->indexes[0].x - 1][curr->indexes[0].y - 1] = 1;

                            if (curr->indexes[0].y + curr->size <= 9)
                                MAP2[curr->indexes[0].x - 1][curr->indexes[0].y + curr->size] = 1;

                            for (int j = curr->indexes[0].y; j < curr->indexes[0].y + curr->size; j++)
                                MAP2[curr->indexes[0].x - 1][j] = 1;
                        }
                    }
                    player1->Coins += (5 * (max1))/(curr->size);
                    saveplayer(player1);
                    RemoveShip(curr, headship2);
                    break;

                } else if (stat == -2) {
                    break;
                }

            }
            if (stat == -2 || stat == -1)
                break;
        }
        if (headship1->next == NULL || headship2->next == NULL) {
            if (headship1->next == NULL) {
                printf("Bot wins.");
                break;
            } else {
                printf("Player %s wins with total coins: %d", player1->Name, player1->Coins);
                break;
            }
        }
        MAPprint(MAP2);
        Sleep(3);
        stat = 1;
        if (headship1->next == NULL || headship2->next == NULL) {
            if (headship1->next == NULL) {
                printf("Bot wins.");
                break;
            } else {
                printf("Player %s wins with total coins: %d", player1->Name, player1->Coins);
                break;
            }
        }
        while(1) {
            srand(time(NULL));
            inputx = rand() % 10 + 1;
            inputy = rand() % 10 + 1;
            if (MAP1[inputx][inputy] == 1 || MAP1[inputx][inputy] == 2 || MAP1[inputx][inputy] == 3)
                continue;
            else
                break;
        }
        for (curr = headship1->next; curr != NULL; curr = curr->next) {
            for (int i = 0; i < curr->size; i++) {
                if ((curr->indexes[i].x == (inputx - 1)) && (curr->indexes[i].y == (inputy - 1))) {
                    MAP1[inputx - 1][inputy - 1] = 2;
                    stat = -2;

                    //Checking if all indexes of a ship have been destroyed.
                    for (int j = 0; j < curr->size; j++) {
                        if (MAP1[curr->indexes[j].x][curr->indexes[j].y] == 2) {
                            stat = -1;
                        } else {
                            stat = -2;
                            break;
                        }
                    }
                } else {
                    MAP1[inputx - 1][inputy - 1] = 1;
                }
                if (stat == -1) {
                    // Applying destruction of ship on map
                    for (int j = 0; j < curr->size; j++)
                        MAP1[curr->indexes[j].x][curr->indexes[j].y] = 3;
                    if (curr->size != 1 && (curr->indexes[1].x - curr->indexes[0].x == 1)) {
                        if ( curr->indexes[0].y != 0 ) {
                            if (curr->indexes[0].x != 0)
                                MAP1[curr->indexes[0].x - 1][curr->indexes[0].y - 1] = 1;

                            if (curr->indexes[0].x + curr->size <= 9)
                                MAP1[curr->indexes[0].x + curr->size][curr->indexes[0].y - 1] = 1;

                            for (int j = curr->indexes[0].x; j < curr->indexes[0].x + curr->size; j++)
                                MAP1[j][curr->indexes[0].y - 1] = 1;
                        }
                        if (curr->indexes[0].x != 0)
                            MAP1[curr->indexes[0].x - 1][curr->indexes[0].y] = 1;

                        if (curr->indexes[0].x != 9)
                            MAP1[curr->indexes[0].x + curr->size][curr->indexes[0].y] = 1;

                        if(curr->indexes[0].y != 9) {
                            if (curr->indexes[0].x != 0)
                                MAP1[curr->indexes[0].x - 1][curr->indexes[0].y + 1] = 1;

                            if (curr->indexes[0].x + curr->size <= 9)
                                MAP1[curr->indexes[0].x + curr->size][curr->indexes[0].y + 1] = 1;

                            for (int j = curr->indexes[0].x; j < curr->indexes[0].x + curr->size; j++) {
                                MAP1[j][curr->indexes[0].y + 1] = 1;
                            }
                        }
                    } else {
                        if (curr->indexes[0].x != 9) {
                            if (curr->indexes[0].y != 0)
                                MAP1[curr->indexes[0].x + 1][curr->indexes[0].y - 1] = 1;

                            if (curr->indexes[0].y + curr->size <= 9)
                                MAP1[curr->indexes[0].x + 1][curr->indexes[0].y + curr->size] = 1;

                            for (int j = curr->indexes[0].y; j < curr->indexes[0].y + curr->size; j++)
                                MAP1[curr->indexes[0].x + 1][j] = 1;
                        }

                        if (curr->indexes[0].y != 0) {
                            MAP1[curr->indexes[0].x][curr->indexes[0].y - 1] = 1;
                        }

                        if (curr->indexes[0].y != 9) {
                            MAP1[curr->indexes[0].x][curr->indexes[0].y + curr->size] = 1;
                        }

                        if (curr->indexes[0].x != 0) {
                            if (curr->indexes[0].y != 0)
                                MAP1[curr->indexes[0].x - 1][curr->indexes[0].y - 1] = 1;

                            if (curr->indexes[0].y + curr->size <= 9)
                                MAP1[curr->indexes[0].x - 1][curr->indexes[0].y + curr->size] = 1;

                            for (int j = curr->indexes[0].y; j < curr->indexes[0].y + curr->size; j++)
                                MAP1[curr->indexes[0].x - 1][j] = 1;
                        }
                    }
                    RemoveShip(curr, headship1);
                    break;

                } else if (stat == -2) {
                    break;
                }

            }
            if (stat == -2 || stat == -1)
                break;
        }
        if (headship1->next == NULL || headship2->next == NULL) {
            if (headship1->next == NULL) {
                printf("Bot wins.");
                break;
            } else {
                printf("Player %s wins with total coins: %d", player1->Name, player1->Coins);
                break;
            }
        }
        MAPprint(MAP1);
        stat = 1;
    }
}

int main() {
    int input, inc = 1, shipscount = 10;
    int MAP1[10][10] = { 0 };
    int MAP2[10][10] = { 0 };
    int tempmap[10][10] = { 0 };
    char inputname[100];
    char gamename[100];
    CheckDir();
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

    Player * head = NewPlayer("head", -1);
    Player * player1 = NULL;
    Player * player2 = NULL;
    Player * curr = NULL;
    Player * bot = NULL;
    importplayers(head);
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

                    inc = 1;
                    scanf("%d", &input);
                    system("cls");
                    for (curr = head->next; inc < input; inc++, curr = curr->next);
                    player1 = curr;
                    break;
                } else if (input == 2) {
                    printf("\nPlease enter new user's name: ");
                    fflush(stdin);
                    gets(inputname);
                    AddPlayer(head, NewPlayer(inputname, 0));
                    playerfile(inputname);
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

                    inc = 1;
                    scanf("%d", &input);
                    system("cls");
                    for (curr = head->next; inc < input; inc++, curr = curr->next);
                    player2 = curr;
                    break;
                } else if (input == 2) {
                    printf("\nPlease enter new user's name: ");
                    fflush(stdin);
                    gets(inputname);
                    AddPlayer(head, NewPlayer(inputname , 0));
                    playerfile(inputname);
                    for (curr = head; curr->next != NULL; curr = curr->next);
                    player2 = curr;
                    break;
                }
            }
            printf("First Player's Arrangement:\n\t1. Auto\n\t2. Manual\n");
            scanf("%d", &input);
            system("cls");
            if(input == 1){
                printf("Arranging Player's Map ...\n\n\nPlease wait.");
                AutomaticArrangement(tempmap, headship1);
                system("cls");
                MAPprint(tempmap);
                Sleep(10000);
                getchar();
                system("cls");
            }

            if(input == 2){
                MAPprint(MAP1);
                availableShipsMenu(headship1);
                printf("Please select a ship to be addressed.\n");
                for (int i = 0; i < 10; i++) {
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

            if(input == 1){
                printf("Arranging Player's Map ...\n\n\nPlease wait.");
                AutomaticArrangement(tempmap, headship2);
                system("cls");
                MAPprint(tempmap);
                Sleep(3000);
                getchar();
                system("cls");
            }
            if(input == 2){
                MAPprint(MAP2);
                availableShipsMenu(headship2);
                printf("Please select a ship to be addressed.\n");
                for (int i = 0; i < 10; i++) {
                    scanf("%d", &input);
                    printf("Addressing The ship with size: %d\n", input);
                    addressship(input, headship2, tempmap);
                    printf("Done! Please select another ship.\n");
                    availableShipsMenu(headship2);
                }
                system("cls");
            }
            Countdown();
            TheGame(MAP1,MAP2,headship1,headship2,player1,player2,1);


        }

        else if(input == 2) {
            while (1) {
                // player asking //
                printf("PLayer: \n\t");
                printf("Choose User:\n\t1. Choose from available users\n\t2. New User\n");


                scanf("%d", &input);
                system("cls");
                if (input == 1) {
                    if(head->next == NULL){
                        printf("There's no users.");
                        continue;
                    }
                    for (curr = head; curr != NULL; curr = curr->next, inc++)

                        printf("%d. %s with %d coins.\n", inc, curr->Name, curr->Coins);

                    inc = 1;
                    scanf("%d", &input);
                    system("cls");
                    for (curr = head; inc < input; inc++, curr = curr->next);
                    player1 = curr;
                } else if (input == 2) {
                    printf("\nPlease enter new user's name: ");
                    fflush(stdin);
                    gets(inputname);
                    AddPlayer(head, NewPlayer(inputname, 0));
                    playerfile(inputname);
                    for (curr = head; curr->next != NULL; curr = curr->next);
                    player1 = curr;
                }
                printf("Player's Arrangement:\n\t1. Auto\n\t2. Manual\n");
                scanf("%d", &input);
                system("cls");

                if(input == 1){
                    printf("Arranging Player's Map ...\n\n\nPlease wait.");
                    AutomaticArrangement(tempmap, headship1);
                    system("cls");
                    MAPprint(tempmap);
                    Sleep(5000);
                    system("cls");
                }

                if(input == 2){
                    MAPprint(MAP1);
                    availableShipsMenu(headship1);
                    printf("Please select a ship to be addressed.\n");
                    for (int i = 0; i < 10; i++) {
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
                printf("Arranging Bot's Map ...\n\n\nPlease wait.");
                AutomaticArrangement(tempmap, headship2);
                system("cls");
                BotGame(MAP1,MAP2,headship1,headship2,player1,bot);

            }
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
