#include <LedControl.h>

int DIN = 12;
int CS =  11;
int CLK = 10;

byte displayImage1[8][8]=     {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
// considering a 2d co-ordinate system with origin (0,0) at bottom left corner
int ballX=4; //X position
int ballY=4; //Y position - top
int speedX=-1; // no X movement, can be -1, 0 or 1
int speedY=1; // downward Y movement, can be -1, 0 or 1
int firstPaddleY = 4; // X position of center of paddle - can be 1 to 6. Paddle width is 3 
int secondPaddleY = 4;
int scoreLeft = 0;
int scoreRight = 0;

LedControl lc=LedControl(DIN,CLK,CS,0);

void setup(){
  //first paddle
 pinMode (2, INPUT);
 pinMode (3, INPUT);
 //second paddle
 pinMode (4, INPUT);
 pinMode (5, INPUT);
 lc.shutdown(0,false);       // Keep MAX7219 awake
 lc.setIntensity(0,15);      // Set the brightness to maximum value
 lc.clearDisplay(0);         // and clear the display
}

void loop(){

    // update ball position
    ballX=ballX+speedX;
    ballY=ballY+speedY;

    // handling paddle control left
    if(digitalRead(2) && firstPaddleY>=0) {
        firstPaddleY = firstPaddleY-1; // move paddle left
    }
    
    if(digitalRead(3) && firstPaddleY<=7) {
        firstPaddleY = firstPaddleY+1; // move paddle right
    }

    // handling paddle control right
    if(digitalRead(4) && secondPaddleY>=0) {
        secondPaddleY = secondPaddleY-1; // move paddle left
    }
    
    if(digitalRead(5) && secondPaddleY<=7) {
        secondPaddleY = secondPaddleY+1; // move paddle right
    }
 
    // // check for ball interaction with walls
    // if(ballX==0 || ballX==7){
    //     speedX = speedX *-1; // bouncing off walls in horizontal direction
    // }
 
    // bouncing off ceiling
    if(ballY==7 || ballY==0){
        speedY = speedY *-1; 
    }
     
    // bouncing off the paddle FOR FIRST paddle
    if(ballX==0 && (ballY>=(firstPaddleY-1) && ballY<=(firstPaddleY+1))){
        speedX = speedX *-1;
    }

    // bouncing off the paddle FOR SECOND paddle
    if(ballX==7 && (ballY>=(secondPaddleY-1) && ballY<=(secondPaddleY+1))){
        speedX = speedX *-1; 
    }
    
    
    // going past the paddle
    if(ballX==0 && (ballY<(firstPaddleY-1) || ballY>(firstPaddleY+1))){
        // going past the paddle - player is out
        scoreLeft++;
        Serial.println(); Serial.print("Left Score: "); Serial.println(scoreLeft);
        for(int i=0; i<8; i++){
           for(int j=0; j<8; j++){
             displayImage1[i][j] = 0x00;
           }
        } 
        // displayImage1[3] = 0xFF; displayImage1[4] = 0xFF; // show a line
        renderByte();
        ballX=4; 
        ballY=4;
    }

    // going past the paddle
    if(ballX==7 && (ballY<(secondPaddleY-1) || ballY>(secondPaddleY+1))){
        // going past the paddle - player is out
        scoreRight++;
        Serial.println(); Serial.print("Right Score: "); Serial.println(scoreRight);
        for(int i=0; i<8; i++){
           for(int j=0; j<8; j++){
             displayImage1[i][j] = 0x00;
           }
        }  
        // displayImage1[3] = 0xFF; displayImage1[4] = 0xFF; // show a line
        renderByte();
        ballX=4; 
        ballY=4;
    }


    // clearing the image variable
    for(int i=0; i<8; i++){
           for(int j=0; j<8; j++){
             displayImage1[i][j] = 0x00;
           }
        } 

  
    
    // generating new image
    addPixel(ballX, ballY); // adding the ball position to image
    addPixel(0, firstPaddleY - 1); addPixel(0, firstPaddleY); addPixel(0, firstPaddleY + 1);
    addPixel(7, secondPaddleY - 1); addPixel(7, secondPaddleY); addPixel(7, secondPaddleY + 1);
    // adding paddle position to image
    renderByte(); // show the generated image
    
 
    delay(20);
}

void addPixel(int xVal, int yVal){
    // int newRowval = 2^(7-xVal);
    // int rowVal = displayImage1[7-yVal];
    // displayImage1[7-yVal] = rowVal || newRowval;    // inserting a 1 at the required pixel
    displayImage1[yVal][xVal] = 0x01;
} 

void renderByte()
{
  int i = 0;
  // for(i=0;i<8;i++){
  //   lc.setColunm(0,i,image[i]);
  // }
   for(int i=0; i<8; i++){
           for(int j=0; j<8; j++){
             if(displayImage1[i][j]==0){
                lc.setLed(0,i,j,false);
             }
             else{
                lc.setLed(0,i,j,true);
             }
           }
        } 
}
