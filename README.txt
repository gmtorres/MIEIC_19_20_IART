*****       EXIMO       *****

-> EXECUÇÃO:
    Para executar o jogo é necessário executar make no diretório contendo o código (src), seguido de "./Eximo".

-> MENUS DE JOGO:
    Necessita de escolher as opções de jogo para os dois jogadores, de forma sequencial:
        0 -> Humano;
        1 -> AI com minimax de profundidade 1, sendo o tempo máximo de jogada AI de 1 segundo;
        2 -> AI com minimax de profundidade 3 a 5, sendo o tempo máximo de jogada AI de 1.5 segundos;
        3 -> AI com minimax de profundidade 6 a 8, sendo o tempo máximo de jogada AI de 15 segundos.

    No caso de ser escolhida a opção AI, será pedido para escolher o método de avaliação do jogo, podendo ser escolhida uma de três opções:
        1 -> Avaliação baseada no número de peças;
        2 -> Avaliação que inclui o ponto 1, adicionando a consideração do desenvolvimento nas pontas do board e as peças que podem ser convertidas
             em 2 peças do próprio jogador;
        3 -> Avaliação que inclui o ponto 2, adicionando o desenvolvimento de peças no meio, de modo a obter uma formação vantajosa, 
             e na primeira linha para questões de defesa.

    A qualquer momento, inserindo -1, o programa é encerrado.

-> COMO JOGAR:
    Existem 4 tipos de movimentos:
        -> Ordinary Move: Movimento para a frente ou na diagonal nesse sentido;
        -> Capture Move: Salto por cima de uma peça do adversário, capturando-a. Esta manobra pode ser feita para a frente, diagonal nesse sentido
                         e para os lados. No caso de existir um destes movimentos, o jogador é obrigado a executá-lo;
        -> Jump Move: Salto por cima de uma peça do próprio jogador, para a frente ou nessa diagonal.
        -> Drop Move: Chegando à ultima fila do tabuleiro, a peça é removida e o jogador escolhe duas posições livres na sua Drop Zone (duas primeiras
                      filas do tabuleiro, excetuando os quatro quadrados laterais) para colocar duas peças suas.
                      
        Realizando um Capture ou Jump Move, no caso de existirem mais movimentos do mesmo tipo, o jogador é obrigado a executá-los, 
        até ao fim dessa possibilidade.
        A qualquer momento, um jogador pode pedir ajuda, sendo-lhe sugerida uma jogada, ao escrever "help" ou "HELP".

Bom jogo! :)

Gustavo Torres
Joaquim Rodrigues
Miguel Rosa