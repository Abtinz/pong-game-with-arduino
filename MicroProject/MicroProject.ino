#include <LedControl.h>
//MAX7219 IO pins
int DIN = 12;
int CS =  11;
int CLK = 10;
// 2d cordinate map for the game
byte boardMap[8][8] =  {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};

int ballX=4; //ball X position
int ballY=4; //ball Y position

// ball first speed configuration (-1 , 1)
int speedX=-1;
int speedY=1;

int firstPaddleY = 4; //first paddel initial vertical position
int secondPaddleY = 4;//second paddel initial vertical position

LedControl lc=LedControl(DIN,CLK,CS,0);

void setup(){
  
  //first paddle IO pins
 pinMode (2, INPUT);
 pinMode (3, INPUT);
 //second paddle  IO pins
 pinMode (4, INPUT);
 pinMode (5, INPUT);
 lc.shutdown(0,false);
 lc.setIntensity(0,15);
 lc.clearDisplay(0);
  
}

void loop(){

    // update ball position with its speed
    ballX=ballX+speedX;
    ballY=ballY+speedY;

    // first paddle movement control(down)
    if(digitalRead(2) && firstPaddleY>=0) {
        firstPaddleY = firstPaddleY-1;
    }
    
    // first paddle movement control(top)
    if(digitalRead(3) && firstPaddleY<=7) {
        firstPaddleY = firstPaddleY+1;
    }

     // second paddle movement control(down)
    if(digitalRead(4) && secondPaddleY>=0) {
        secondPaddleY = secondPaddleY-1;
    }
    
    // second paddle movement control(top)
    if(digitalRead(5) && secondPaddleY<=7) {
        secondPaddleY = secondPaddleY+1; // move paddle right
    }
 
    // the impact of the ball on the edges of the board should reflect its speed in the vertical axis
    if(ballY==7 || ballY==0){
        speedY = speedY *-1; 
    }
     
    // the impact of the ball on the paddles should reflect its speed in the horizontal axis
    if(ballX==0 && (ballY>=(firstPaddleY-1) && ballY<=(firstPaddleY+1))){
        speedX = speedX *-1;
    }

    if(ballX==7 && (ballY>=(secondPaddleY-1) && ballY<=(secondPaddleY+1))){
        speedX = speedX *-1; 
    }
    
    
    // passing the ball from first paddle
    if(ballX==0 && (ballY<(firstPaddleY-1) || ballY>(firstPaddleY+1))){
        //restarting board
        for(int i=0; i<8; i++){
           for(int j=0; j<8; j++){
             boardMap[i][j] = 0x00;
           }
        } 
        ledUpdate();
        //start the ball from center
        ballX=4; 
        ballY=4;
    }

    // passing the ball from first paddle
    if(ballX==7 && (ballY<(secondPaddleY-1) || ballY>(secondPaddleY+1))){
        //restarting board
        for(int i=0; i<8; i++){
           for(int j=0; j<8; j++){
             boardMap[i][j] = 0x00;
           }
        }  
        ledUpdate();
        //start the ball from center
        ballX=4; 
        ballY=4;
    }


    // clearing the image variable
    for(int i=0; i<8; i++){
           for(int j=0; j<8; j++){
             boardMap[i][j] = 0x00;
           }
        } 
  
    // updating board
    // setting the new position of the ball
    updateBoardMap(ballX, ballY); 
    // setting the new position of the firstPaddleY
    updateBoardMap(0, firstPaddleY - 1); 
    updateBoardMap(0, firstPaddleY); 
    updateBoardMap(0, firstPaddleY + 1);
    // setting the new position of the secondPaddleY
    updateBoardMap(7, secondPaddleY - 1); 
    updateBoardMap(7, secondPaddleY); 
    updateBoardMap(7, secondPaddleY + 1);
    //here we will show the displayImage1
    ledUpdate();
    
    delay(20);
  
}

/*
  in this function we will updated the game boardMap based on its (x,y) coordinate
*/
void updateBoardMap(int xVal, int yVal){
    boardMap[yVal][xVal] = 0x01;
} 

/*
  in this function we will updated the game led with setLed(false -> off , true -> on) based on boardMap
*/
void ledUpdate()
{
  int i = 0;
   for(int i=0; i<8; i++){
           for(int j=0; j<8; j++){
             if(boardMap[i][j]==0){
                lc.setLed(0,i,j,false);
             }
             else{
                lc.setLed(0,i,j,true);
             }
        }
    } 
}
