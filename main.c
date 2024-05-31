#include "main.h"

typedef unsigned char t_Pit;

void show( int player, t_Pit p[], t_Pit s[], int win ) {
    int a = player, o = !a; // 'a'ctive & 'o'pponent

    int x = a ? 0 : 6;
    int y = a ? 6 : 0;
    char *str = win ? "Wins the game\n\n" : "selects pit (1-6): ";
    printf(
            "\n"
            "----+-----player%d-----+\n"
            "%2d  |%2d|%2d|%2d|%2d|%2d|%2d|\n"
            "----+-----------------+\n"
            "%2d  |%2d|%2d|%2d|%2d|%2d|%2d|\n"
            "----+-----player%d-----+\t"

            "\n%d %s",
            o,  s[o], p[y+5], p[y+4], p[y+3], p[y+2], p[y+1], p[y+0], 
            s[a], p[x+0], p[x+1], p[x+2], p[x+3], p[x+4], p[x+5], a,
            a, str
    );
}

void show_AI(int winner, t_Pit p[], t_Pit s[]) {
    char *str;
    if (winner == 1) {
        str = "AI Wins the game\n\n";
    } else if (winner == 0) {
        str = "You Win the game\n\n";
    } else if (winner == 2) {
        str = "Select pit (1-6): ";
    } else if (winner == 3) {
        str = "Press c to continue: ";
    } else
        str = " ";

    printf(
        "\n"
        "----+-----AI-----+\n"
        "%2d  |%2d|%2d|%2d|%2d|%2d|%2d|\n"
        "----+--------------+\n"
        "%2d  |%2d|%2d|%2d|%2d|%2d|%2d|\n"
        "----+-----Player-----+\t"

        "\n %s",
        s[1], p[11], p[10], p[9], p[8], p[7], p[6],
        s[0], p[0], p[1], p[2], p[3], p[4], p[5],
        str
    );
}

int T_Seeds ( int player, t_Pit p[] ) { //função para calcular o numero de sementes nas pits todas do jogador que queiramos
    int t = 0;
    int i;
    int start_p = ((player * 6) + 6) % 12;
    for ( i = start_p; i < (start_p + 6) ; i++ ) {
        t += p[i];
    }
    return t;
}

void saveFile(int player, t_Pit p[], t_Pit s[]) { //salvar estado
    char fName[26];
    printf("Introduzir nome do ficheiro (incluir .txt):");scanf("%s", fName);
    FILE *save = fopen(fName, "w");
    if (save == NULL) {
        perror("Erro a abrir ficheiro.");
        return;
    }

    int a = player, o = !a; // 'a'ctive & 'o'pponent

    int x = a ? 0 : 6;
    int y = a ? 6 : 0;

    fprintf(save, "%hhu\n", s[o]); //salvar deposito 'o' (opponent)
    fprintf(save, "%hhu %hhu %hhu %hhu %hhu %hhu\n", p[y + 5], p[y + 4], p[y + 3], p[y + 2], p[y + 1], p[y + 0]); //salvar casas 'o' (opponent) 
    fprintf(save, "%hhu\n", s[a]); //salvar deposito 'a' (active)
    fprintf(save, "%hhu %hhu %hhu %hhu %hhu %hhu\n", p[x + 0], p[x + 1], p[x + 2], p[x + 3], p[x + 4], p[x + 5]); //salvar casas 'a' (active) 

    fclose(save);printf("Ficheiro '%s' salvo.\n", fName);
}

void loadState(int player, t_Pit p[], t_Pit s[], const char *fName){ //carregar estado
    FILE *save = fopen(fName, "r");
    if (save == NULL) {
        perror("Erro a abrir ficheiro.");
        return;
    }

    int a = player, o = !a; // 'a'ctive & 'o'pponent
    int x = a ? 0 : 6;
    int y = a ? 6 : 0;

    fscanf(save, "%hhu", &s[o]); //carregar deposito 'o' (opponent)
    fscanf(save, "%hhu %hhu %hhu %hhu %hhu %hhu", &p[y + 5], &p[y + 4], &p[y + 3], &p[y + 2], &p[y + 1], &p[y + 0]); //carregar casas 'o' (opponent)
    fscanf(save, "%hhu", &s[a]); //carregar deposito 'a' (active)
    fscanf(save, "%hhu %hhu %hhu %hhu %hhu %hhu", &p[x + 0], &p[x + 1], &p[x + 2], &p[x + 3], &p[x + 4], &p[x + 5]); //carregar casas 'a' (active)

    fclose(save);printf("Ficheiro '%s' carregado.\n", fName);
}

int main( int argc, char *argv[] ) {
    enum { nPlayer = 2, nOne = 6, nPit = (2*nOne), nSeed = 4, nWin = 25 }; // ease of code

    t_Pit score[ nPlayer ] = { 0 };
    t_Pit pit[ nPit ] = { 0 };
    int counter = 0;
    int counter1 = 0;
    int i;
    int actv = 0;
    int end_con = 0;

    if(argc == 1){
        //pointer to game's beginning
        for (i = 0; i < nPit; ++i) {
            pit[i] = nSeed;
        }
        goto Game;
    }
    if(argc == 2){
        //load save
        loadState(actv, pit, score, argv[1]);
    }
    else{
        //handing undefined behaviour
        printf("Utilizacao:%s ou %s [filename]\n", argv[0], argv[0]);
        return 1;
    }


    int GameMode;
    int repeat = 0;
    Game:
    while (repeat == 0){
        puts ( "Selecionar modo de jogo (0 para Player VS Player, 1 para Player VS AI): " );
        if ( scanf( "%d", &GameMode ) != 1 || GameMode > 1 ){
            puts ( "scanf falhado, por favor tente de novo\n" );
        } else {
            repeat = 1;
        }
    }

    if (GameMode == 0){ //PvP

        for( ; score[0] < nWin && score[1] < nWin; actv = !actv ) {
            show( actv, pit, score, 0 );
            
            int fix_p = ((actv * nOne) + 6) % 12; //como foi desenhado que o jogador 0 tenha as pits [5 a 11] e o 1 as pits [0 a 5], tive que arranjar uma variavel fix para identificar as pits correctas dependendo do jogador activo
            int fix_o = (actv * nOne); //o mesmo que a linha anterior mas agora para saber as pits do oponente.
            int temp = pit[ fix_p ]; //criação de uma variável para armazenar o numero da pit mais baixa da faixa do jogador (se 0 então é pit [6], se é 1 então pit [0])
            for (i = fix_p; i < (fix_p + 6); i++) { 
                if (temp < pit[i]){
                    temp = pit[i]; //verificação da pit com mais sementes dependendo do jogador que está a jogar.
                }
            }
            
            int temp1 = pit[ fix_o ]; //criação de uma variavel temporaria para armazenas a quantidade de sementes da primeira pit do oponente
            for (i = actv * nOne; i < (fix_o + 6); i++) { //vai da primeira pit do oponente à ultima
                if (temp1 < pit[i]){
                    temp1 = pit[i]; //verifica qual a pit do oponente com mais sementes
                }
            }

            int total_p = T_Seeds ( actv, pit ); //calcular total de seeds do jogador ativo 
            int total_o = T_Seeds ( !actv, pit ); //calcular total de seeds do oponente 
            
            for ( i = 0; i < nOne; i++ ) {
                if ( total_p == 1 && total_o == 1 && pit[i] == 1 && pit[i] == pit[i+6]) { //se o ambos os jogadores só tém uma semente e se a pit de cada um está exactamente a 6 posições de distancia, então existe loop infinito e por isso o jogo deve terminar empatado.
                    puts ( "Loop infinito, Empate!\n");
                    return 0;
                }
            }
            

            if ( temp == 0 ) { //se é o próprio player a não ter sementes então tem que passar o turno
                printf (" \nPlayer %d nao tem sementes, passar turno \n", actv);
                continue; //proximo loop do for inicial
            } 
            if ( temp1 == 0 ) { //se o oponente não tem sementes então o contador1 é igual a zero para que se aplique a regra especial
                counter1 = 0;
            } else counter1 = 1;  //se o oponente tiver sementes entao o counter1 = 1 para que tenha restrições mais leves do que a regra
    
            int from, to;
            while (counter == 0) { //counter para que caso o jogador se engane no input, repita o processo
                if (counter1 == 1) {  //se counter1 == 1 então não existe restrições adicionais
                    if (scanf("%d", &from) != 1) {
                        // input was not a number
                        int c;
                        printf("Input invalido\n");
                        while ((c = getchar()) != EOF && c != '\n')
                            continue;
                        if (c == EOF)
                            exit(1);
                    }
                    if(from == 8){
                        //exit
                        return 1;
                    }
                    if (from == 9) {
                        //load state
                        loadState(actv, pit, score, NULL);
                        actv = !actv;
                    }
                    if (from == 0) {
                        //save state
                        saveFile(actv, pit, score);
                        actv = !actv;
                        
                    }
                    if (from < 1 || from > 6
                        || (pit[(from - 1) + fix_p] <= 1 && temp > 1)
                        || (pit[(from - 1) + fix_p] == 0 && temp == 1)) {
                        puts("Input invalido, selecionar casa (1-6): ");

                    }
                    if (from == 0 || from == 9 || from == 8) { break; }
                    else {
                        break;
                    }
                }

                if (counter1 == 0) { //se counter1 == 0 então temos que impor a certeza de que sementes serão colocadas na faixa do oponente
                    for (i = fix_p; i < (fix_p + 6); i++) { //primeiro verificamos se existem pits em que é possivel o player fazer uma jogada em que dá sementes ao oponente. 
                        if ( (pit[i] >= (fix_p + 6) - i && i != (fix_p + 6)) || (temp == 1 && pit[fix_p + 6] == 1) ) { //esta condição deve verificar cada pit da faixa dele e se tem menos sementes que as necessarias para passar ao oponente desde que não seja a ultima ou então desde que todas as suas casas não tenham mais que uma seed então a ultima pit pode ser escolhida se tiver uma seed para passar ao oponente.
                            end_con = 1; //se houver pelo menos uma pit que satisfaça essa condição, damos o counter ending condition o valor de 1, quebramos o for e assim ignora o proximo passo.
                            break; 
                        }
                    }
                    
                    if (end_con == 0) { // Se end_con = 0 então o oponente não pode fazer jogadas que dê sementes ao oponente e o jogo tem que terminar.
                        puts( "\n\nAdversario nao tem sementes e"
                                " nao podem ser dadas sementes ao adversario!");
                        for (i = fix_p; i < (fix_p + 6); i++) {
                            score[ actv ] += pit[ i ]; 
                            pit [ i ] = 0; //e por isso deve de esvaziar as suas pits para o seu score.
                        }

                    //como só há 48 sementes, existe hipotese de neste caso haver empate a 24 sementes
                        if ( score [actv] == 24) {
                            puts ("Empate!");
                            return 0;
                        } else {
                            actv = !actv; //como o show no fim é com !actv, eu tenho que o trocar aqui primeiro para depois dispor o jogador vencedor correto
                            goto Ending; //senão for empate entao quebra o for todo e mostra o show com o vencedor.
                        }
                    }

                    if ( scanf( "%d", &from ) != 1 || from < 1 || from > 6 || pit[(from - 1) + fix_p] < (nOne + 1) - from || (pit[(from - 1) + fix_p] <= 1 && temp > 1) || (pit[(from - 1) + fix_p] == 0 && temp == 1)) { 
                        //não foi testado ainda mas basicamente obriga ao jogador escolher uma pit que tenha o numero de sementes minimas necessarias para colocar pelo menos uma semente na faixa de pits do oponente
                        puts( "\n\nAdversario nao tem sementes!\n" ); 
                        puts( "scanf falhado, selecionar casa (1-6): " ); //a sequencia que te dei não permite dar sementes ao oponente por isso tem que ser outra
                    } else {
                        counter = 1;
                    }
                }
            }

            counter = 0; //quando os ifs terminam o counter e counter1 tem que passar a 0 outra vez, senão ficarão sempre 1, ou seja, sempre false
            counter1 = 0;
            from -= 1; // index para 0
            from += fix_p;
            to = from;
            
            int nSow = pit[ from ];
            pit[ from ] = 0;

            while( nSow ) { //se houver mais de 12 sementes, saltar a casa originária
                if( ( to = (to+1)%nPit ) != from ) {
                    ++pit[ to ];
                    --nSow;
                }
            }
                

            //condição aplica-se nas casas do adversário
            for( ; to/nOne != from/nOne; to = (to+(nPit-1))%nPit ) // modulo -1
                if( pit[ to ] == 2 || pit[ to ] == 3 ) {
                    score[ actv ] += pit[ to ]; //adicionar a depósito 
                    pit[ to ] = 0; //esvaziar a casa
                } else break;
        }
        Ending:
        show( !actv, pit, score, 1 ); //requerer vencedor
    }

    if (GameMode == 1){ //PvAI
        int y = 13;
        int winner = 2;

        counter = 0; //quando os ifs terminam o counter e counter1 tem que passar a 0 outra vez, senão ficarão sempre 1, ou seja, sempre false
        counter1 = 0;
        end_con = 0;

        for( ; score[0] < nWin && score[1] < nWin; actv = 0 ) {
            
           if (y < 13)
                printf (" \nAI escolheu casa: %d", ((y + 1) % nOne));
            
            show_AI( winner, pit, score);
            
            
            int temp = pit[ actv ]; //criação de uma variável para armazenar o numero da pit mais baixa da faixa do jogador
            for (i = actv; i < nOne; i++) { 
                if (temp < pit[i]){
                    temp = pit[i]; //verificação da pit com mais sementes do jogador
                }
            }
            
            int temp1 = pit[ nOne ]; //criação de uma variavel temporaria para armazenar a quantidade de sementes da primeira pit do oponente
            for (i = nOne; i < nPit; i++) { //vai da primeira pit do oponente à ultima
                if (temp1 < pit[i]){
                    temp1 = pit[i]; //verifica qual a pit do oponente com mais sementes
                }
            }

            int total_p = T_Seeds ( actv, pit ); //calcular total de seeds do jogador ativo 
            int total_o = T_Seeds ( !actv, pit ); //calcular total de seeds do oponente 
            
            for ( i = 0; i < nOne; i++ ) {
                if ( total_p == 1 && total_o == 1 && pit[i] == 1 && pit[i] == pit[i+6]) { //se o ambos os jogadores só tém uma semente e se a pit de cada um está exactamente a 6 posições de distancia, então existe loop infinito e por isso o jogo deve terminar empatado.
                    puts ( "Loop infinto, Empate!\n");
                    return 0;
                }
            }
            

            if ( temp == 0 ) { //se é o próprio player a não ter sementes então tem que passar o turno
                puts ("Player nao tem sementes!\n");
                goto AIturn; //turno do oponente
            } 
            if ( temp1 == 0 ) { //se o oponente não tem sementes então o contador1 é igual a zero para que se aplique a regra especial
                counter1 = 0;
            } else counter1 = 1;  //se o oponente tiver sementes entao o counter1 = 1 para que tenha restrições mais leves do que a regra
    
            int from, to;
            while (counter == 0) { //counter para que caso o jogador se engane no input, repita o processo
                if (counter1 == 1) {  //se counter1 == 1 então não existe restrições adicionais
                    if (scanf("%d", &from) != 1) {
                        //input não é um número
                        int c;
                        printf("Input invalido\n");
                        while ((c = getchar()) != EOF && c != '\n')
                            continue;
                        if (c == EOF)
                            exit(1);
                    }
                    if (from == 0) {
                        //salvar estado do tabuleiro
                        saveFile(actv, pit, score);
                        actv = !actv;

                    }
                    if (from < 1 || from > 6
                    || (pit[from - 1] <= 1 && temp > 1)
                    || (pit[from - 1] == 0 && temp == 1)) { //verificação de input de utilizador
                        puts("Input invalido, selecionar casa (1-6): ");
                        continue;
                    }
                    if (from == 0) { break; } //depois de salvar, dar break do ciclo
                    else {
                        break;
                    }
                }

                if (counter1 == 0) { //se counter1 == 0 então temos que impor a certeza de que sementes serão colocadas na faixa do oponente
                    for (i = actv; i < nOne; i++) { //primeiro verificamos se existem pits em que é possivel o player fazer uma jogada em que dá sementes ao oponente. 
                        if ( (pit[i] >= (nOne - i) && (i != 5)) || (temp == 1 && pit[ 5 ] == 1) ) { //esta condição deve verificar cada pit da faixa dele e se tem menos sementes que as necessarias para passar ao oponente desde que não seja a ultima ou então desde que todas as suas casas não tenham mais que uma seed então a ultima pit pode ser escolhida se tiver uma seed para passar ao oponente.
                            end_con = 1; //se houver pelo menos uma pit que satisfaça essa condição, damos o counter ending condition o valor de 1, quebramos o for e assim ignora o proximo passo.
                            break; 
                        }
                    }
                    
                    if (end_con == 0) { // Se end_con = 0 então o oponente não pode fazer jogadas que dê sementes ao oponente e o jogo tem que terminar.
                        puts( "\n\nAI nao tem sementes e"
                                " nao podem ser dadas sementes!");
                        for (i = actv; i < (nOne); i++) {
                            score[ actv ] += pit[ i ]; 
                            pit [ i ] = 0; //e por isso deve de esvaziar as suas pits para o seu score.
                        }

                    //como só há 48 sementes, existe hipotese de neste caso haver empate a 24 sementes
                        if ( score [actv] == 24) {
                            puts ("Empate!");
                            return 0;
                        } else {
                            winner = 0; //como o show no fim é com !actv, eu tenho que o trocar aqui primeiro para depois dispor o jogador vencedor correto
                            goto Ending2; //senão for empate entao quebra o for todo e mostra o show com o vencedor.
                        }
                    }

                    if ( scanf( "%d", &from ) != 1 || from < 1 || from > 6 || pit[from - 1] < (nOne + 1) - from || (pit[from - 1] <= 1 && temp > 1) || (pit[from - 1] == 0 && temp == 1)) { 
                        //não foi testado ainda mas basicamente obriga ao jogador escolher uma pit que tenha o numero de sementes minimas necessarias para colocar pelo menos uma semente na faixa de pits do oponente
                        puts( "\n\nAI nao tem sementes!\n" ); //tenta testar sff, senão terei que arranjar forma amanha
                        puts( "scanf falhado, selecionar casa  (1-6): " ); //a sequencia que te dei não permite dar sementes ao oponente por isso tem que ser outra
                    } else {
                        counter = 1;
                    }
                }
            }

            from -= 1; //index para 0
            to = from;

            int nSow = pit[ from ];
            pit[ from ] = 0;

            while( nSow ) { //se houver mais de 12 sementes, saltar a casa originária
                if( ( to = (to+1)%nPit ) != from ) {
                    ++pit[ to ];
                    --nSow;
                }
            }   

            //condição aplica-se nas casas do adversário
            for( ; to/nOne != from/nOne; to = (to+(nPit-1))%nPit ) // modulo -1
                if( pit[ to ] == 2 || pit[ to ] == 3 ) {
                    score[ actv ] += pit[ to ];
                    pit[ to ] = 0;
                } else break;

            if (winner == 2)
                winner = 3;

            show_AI( winner, pit, score);
            
            while (1){
                char input;
                scanf (" %c", &input);
                if (input == 'c')
                    break;
                else
                    printf ("Input invalido, pressione c para continuar: ");
            }

            if (winner == 3)
                winner = 2;

            AIturn:

            counter = 0; //quando os ifs terminam o counter e counter1 tem que passar a 0 outra vez, senão ficarão sempre 1, ou seja, sempre false
            counter1 = 0;
            end_con = 0;

            temp = pit[ actv ]; //criação de uma variável para armazenar o numero da pit mais baixa da faixa do jogador
            for (i = actv; i < nOne; i++) { 
                if (temp < pit[i]){
                    temp = pit[i]; //verificação da pit com mais sementes do jogador
                }
            }

            temp1 = pit[ nOne ]; //criação de uma variavel temporaria para armazenar a quantidade de sementes da primeira pit do oponente
            for (i = nOne; i < nPit; i++) { //vai da primeira pit do oponente à ultima
                if (temp1 < pit[i]){
                    temp1 = pit[i]; //verifica qual a pit do oponente com mais sementes
                }
            }

            total_p = T_Seeds ( actv, pit ); //calcular total de seeds do jogador ativo 
            total_o = T_Seeds ( !actv, pit ); //calcular total de seeds do AI
            
            for ( i = 0; i < nOne; i++ ) {
                if ( total_p == 1 && total_o == 1 && pit[i] == 1 && pit[i] == pit[i+6]) { //se o ambos os jogadores só tém uma semente e se a pit de cada um está exactamente a 6 posições de distancia, então existe loop infinito e por isso o jogo deve terminar empatado.
                    puts ( "Loop infinito, Empate!\n");
                    return 0;
                }
            }
            
            if ( temp > 0 ) { //se e o player não tem sementes então counter1 = 0,
                 counter1 = 1;
            }

            if ( temp1 == 0 ) { //se o AI não tem sementes então o contador1 é igual a zero para que se aplique a regra especial
                puts ("AI nao tem sementes!");
                continue;
            }

            int found = 0;
            if (counter1 == 1) {  //se counter1 == 1 então não existe restrições adicionais
                for(i = actv; i < nOne && !found; i++){
                    if (pit [i] == 2 || pit [i] == 1){
                        for (y = nOne; y < nPit; y++){
                            if ( ( pit[y] > 1 || ( pit[y] == 1 && temp1 == 1) ) && ( ( pit[y] % nPit ) == ( ( nPit - y ) + i) ) ){
                                nSow = pit[y];
                                pit[y] = 0;
                                to = y;
                                
                                while( nSow ) { 
                                    if( ( to = (to+1)%nPit ) != y ) {
                                        ++pit[ to ];
                                        --nSow;
                                    }
                                }

                                for( ; to/nOne != y/nOne; to = (to+(nPit-1))%nPit )
                                    if( pit[ to ] == 2 || pit[ to ] == 3 ) {
                                        score[ !actv ] += pit[ to ];
                                        pit[ to ] = 0;
                                    } else break;
                                
                                counter = 1;
                                found = 1;
                                break;
                            }
                        }
                    }
                }
                
                if (counter == 0){
                    for (y = (nPit - 1); y > (nOne - 1); y--) {
                        if (pit[y] >= 2){
                            nSow = pit[y];
                            pit[y] = 0;
                            to = y;
                            
                            while( nSow ) { 
                                if( ( to = (to+1)%nPit ) != y ) {
                                    ++pit[ to ];
                                    --nSow;
                                    }
                                }

                            for( ; to/nOne != y/nOne; to = (to+(nPit-1))%nPit )
                                if( pit[ to ] == 2 || pit[ to ] == 3 ) {
                                    score[ !actv ] += pit[ to ];
                                    pit[ to ] = 0;
                                } else break;
                            break;
                        }

                        if (pit[y] == 1 && temp1 == 1){
                            nSow = pit[y];
                            pit[y] = 0;
                            to = y;
                            while( nSow ) { 
                                if( ( to = (to+1)%nPit ) != y ) {
                                    ++pit[ to ];
                                    --nSow;
                                    }
                                }
                            
                            for( ; to/nOne != y/nOne; to = (to+(nPit-1))%nPit )
                                if( pit[ to ] == 2 || pit[ to ] == 3 ) {
                                    score[ !actv ] += pit[ to ];
                                    pit[ to ] = 0;
                                } else break;
                            break;
                        }
                    }
                }
            }

            if (counter1 == 0) { //se counter1 == 0 então temos que impor a certeza de que sementes serão colocadas na faixa do oponente
                for (i = nOne; i < nPit; i++) { //primeiro verificamos se existem pits em que é possivel o player fazer uma jogada em que dá sementes ao oponente. 
                    if ( (pit[i] >= (nPit - i) && (i != 11)) || (temp == 1 && pit[ 11 ] == 1) ) { //esta condição deve verificar cada pit da faixa dele e se tem menos sementes que as necessarias para passar ao oponente desde que não seja a ultima ou então desde que todas as suas casas não tenham mais que uma seed então a ultima pit pode ser escolhida se tiver uma seed para passar ao oponente.
                        end_con = 1; //se houver pelo menos uma pit que satisfaça essa condição, damos o counter ending condition o valor de 1, quebramos o for e assim ignora o proximo passo.
                        break; 
                    }
                }
                    
                if (end_con == 0) { // Se end_con = 0 então o oponente não pode fazer jogadas que dê sementes ao oponente e o jogo tem que terminar.
                    puts( "\n\nPlayer nao tem sementes e"
                            " nao podem ser dadas sementes!");
                    for (i = nOne; i < nPit; i++) {
                        score[ !actv ] += pit[ i ]; 
                        pit [ i ] = 0; //e por isso deve de esvaziar as suas pits para o seu score.
                    }

                    //como só há 48 sementes, existe hipotese de neste caso haver empate a 24 sementes
                    if ( score [actv] == 24) {
                        puts ("Empate!");
                        return 0;
                    } else {
                        winner = 1; //como o show no fim é com !actv, eu tenho que o trocar aqui primeiro para depois dispor o jogador vencedor correto
                        goto Ending2; //senão for empate entao quebra o for todo e mostra o show com o vencedor.
                    }
                }    

                if (end_con == 1) { 
                    for (y = (nPit - 1); y > (nOne - 1); y--){
                        if (pit[y] >= (nPit - y) && pit[y] > 1) {
                            nSow = pit[y];
                            pit[y] = 0;
                            to = y;
                            while( nSow ) { 
                                if( ( to = (to+1)%nPit ) != y ) {
                                    ++pit[ to ];
                                    --nSow;
                                    }
                                }

                            for( ; to/nOne != y/nOne; to = (to+(nPit-1))%nPit )
                                if( pit[ to ] == 2 || pit[ to ] == 3 ) {
                                    score[ !actv ] += pit[ to ];
                                    pit[ to ] = 0;
                                } else break;
                            
                            counter = 1;
                            break;
                        }
                    
                        if (pit[y] == 1 && temp1 == 1 && counter == 0){
                            nSow = pit[y];
                            pit[y] = 0;
                            to = y;

                            while( nSow ) { 
                                if( ( to = (to+1)%nPit ) != y ) {
                                    ++pit[ to ];
                                    --nSow;
                                    }
                                }

                            for( ; to/nOne != y/nOne; to = (to+(nPit-1))%nPit )
                                if( pit[ to ] == 2 || pit[ to ] == 3 ) {
                                    score[ !actv ] += pit[ to ];
                                    pit[ to ] = 0;
                                } else break;
                            break;
                        }
                    } 
                }
            }

            if (score[0] >= nWin) {
                winner = 0;
                break;
            } else if (score[1] >= nWin) {
                winner = 1;
                break;
            }


            counter = 0;
            counter1 = 0;
            end_con = 0;
        }

        Ending2:
        show_AI( winner, pit, score);

    }
    return 0;
}