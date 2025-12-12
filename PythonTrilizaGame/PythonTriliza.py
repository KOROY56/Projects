from contextlib import nullcontext
import re as error

print("Welcome to TIC TAC\n")


#build Class game
class Game:
    def __init__ (self):
        self.rows = 3
        self.table = [['  ','  ','  '],
                      ['  ','  ','  '],
                      ['  ','  ','  ']
                      ]
    
    def game_table(self):
        print(' -- '*self.rows)
        for i in range(0,self.rows):
            table =  ' |'.join(self.table[i])
            print('|'+table+'|')
            print(' -- '*self.rows)
    
    def play(self,x,y,player):
        if (x < '0' or x > '3' or y < '0' or y > '3'):
            return True
        if (int(x)<1 or int(x)>3 or int(y)<1 or int(y)>3 ):
            return True
        
        self.row = int(x)-1
        self.column = int(y)-1
        if (player == 'P1'):
            if (self.table[self.row][self.column] == '  '  ):
                self.table[self.row][self.column] = ' X'
                return False
            else:
                return True
        else :
            if (self.table[self.row][self.column] == '  '  ):
                self.table[self.row][self.column] = ' O'
                return False
            else : 
                return True
        
    
    def table_check(self):  
        if (self.table[0][0] == ' X' and self.table[1][1] == ' X' and self.table[2][2] == ' X'):
            return 'X'
        elif (self.table[0][0] == ' O' and self.table[1][1] == ' O' and self.table[2][2] == ' O'):
            return 'O'
        elif (self.table[0][2] == ' X' and self.table[1][1] == ' X' and self.table[2][0] == ' X'):
            return 'X'
        elif (self.table[0][2] == ' O' and self.table[1][1] == ' O' and self.table[2][0] == ' O'):
            return 'O'
        
        for i in range (0,self.rows):
            countX = 0
            countO = 0
            for j in range(0,self.rows):
                if (self.table[i][j] == ' X') :
                    countX += 1
                    if (countX == 3):
                        winner = 'X'
                        return winner        
                if (self.table[i][j] == ' O'):
                    countO += 1
                    if (countO == 3):
                        winner = 'O'
                        return winner
                        

        
        for i in range(0,self.rows):
            countX = 0
            countO = 0
            for j in range (0,self.rows):
                if (self.table[j][i] == ' X'):
                    countX += 1
                    if (countX == 3):
                        winner = 'X'
                        return winner
                if (self.table[j][i] == ' O'):
                    countO += 1
                    if (countO == 3):
                        winner = 'O'
                        return winner
                                             

    def table_reset(self):
        self.table = [['  ','  ','  '],
                      ['  ','  ','  '],
                      ['  ','  ','  ']
                      ]


game1 = Game()
#game1.game_table()


msg = True
run = True
while (run==True):

    
    champ  = False
   
    print("Player X type the Dimensions :")
    if (msg):
        print("(Example :1 3 )")
        msg = False
    
    run1 = True
    run2 = True
    
    #player = game1.table_check() # player is a string which indicates which player has won
    
    while(run1==True):
        
        dimensions = input() 
        x,y = dimensions.split()
        if (game1.play(x,y,'P1')):
            print("Wrong input please try again")
        else :
            run1=False
    
    if (champ != True):
        game1.game_table()
    
    player = game1.table_check() # player is a string which indicates which player has won
    
    #checks if any player has won 
    if (player == 'X' ):
        if(champ!=True):
            print("Player X wins !")
            #print("The End .")
            run1 = False
            run2 = False
            champ = True
    
    elif(player == 'O' ):
        if (champ!=True):
            print("Player O wins !")
            #print("The End .")
            run1 = False 
            run2 = False
            champ = True

    if run2!=False:  
        print("Player O type the Dimensions :")
    
    while(run2==True):
        
        dimensions = input()
        x,y = dimensions.split() 
    
        if (game1.play(x,y,'P2')):
            print("Wrong input please try again")
        else :
            run2=False
    if (run != False):
        game1.game_table()

    player = game1.table_check() # player is a string which indicates which player has won
    
    #checks if any player has won 
    if (player == 'X' ):
        if (champ!=True):
            print("Player X wins !")
            #print("The End .")
            run1 = False
            run2 = False
            champ = True
    
    elif(player == 'O' ):
        if (champ!=True):
            print("Player O wins !")
            #print("The End .")
            run1 = False 
            run2 = False
            champ = True


    if(champ == True):
        run3 = True
        print("Do you want to play , again ?")
        while run3:
            run4 = True
            amswer = False
            decision = input("Type either  yes  or  no :").lower()
            while run4:
                if (decision == 'yes' or decision == ' yes' or decision == ' yes ' or decision == 'yes '):
                    run = True
                    run4 = False
                    run3 = False
                    game1.table_reset()
                elif (decision == 'no' or decision == ' no ' or decision == ' no' or decision == 'no '):
                    print("GOODBYE !!! ")
                    run4 = False
                    run3 = False
                    run = False
                else:
                    print("Wrong data input , try again !")
                    run4 = False
