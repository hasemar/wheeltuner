
/*The sketch reads the I2C output of a Pittsburgh dial indicator. The output will be a decimal 
 * number of the double data type. Be sure the dial indicator is in mm mode. Wire the data 
 * out of the indicator to pin 11 and the clock to pin 12. This fist draft takes 117ms for the 
 * main loop.
 * 
 * The 3 byte data stream from indicator is stored in the dataBIN array. The 
 * strream is in the following format:
 * ?,X,X,X, X,X,X,X, X,X,X,X, X,X,X,X, ?,?,?,?, ?,Y,?,?
 * ? = Not sure
 * X's for a binary number with LSB (least significant bit) at the start of the string. The binary number is the distance in mm times 100.
 * Y = Sign bit if Y = 1 number is negative if Y = 0 number is positive.
 * 
 * Code adapted by Blake Wiley from http://www.instructables.com/id/Reading-Digital-Callipers-with-an-Arduino-USB/ example sketch.
 */


// Pin Declarations
int dataIn = 11; 
int clockIn = 12;

// Variables
int clock = 1;
int lastClock = 1;
unsigned long time = 0;
unsigned long timeStart = 0;
int out = 0;
int dataBIN[24]; // Array to store the 3 byte stream from the dial indicator
int number[15]; // Array to store just the binary number
double num; // Converted decimal number
double sign; // Number to store the sign of the output

//*****
unsigned long previousMillis = 0; // Used if the dataBIN array is to be printed to serial monitor
const long interval = 1000;  // Interval in ms for the data to be sent to serial monitor
//*****


void setup() {
  // Pin Set Up
  pinMode(dataIn, INPUT);     
  pinMode(clockIn, INPUT);  
  Serial.begin(115200);
  Serial.println("Ready: ");
}


void loop(){

  unsigned long currentMillis = millis(); // Used if the dataBIN array is to be printed to serial monitor

  int i=0; // Reset the while loop counter
 while ( i<24) // Collects data from dial indicator
   {
  lastClock = clock;
  clock = digitalRead(clockIn);
      if (lastClock == 1 && clock == 0){
        out = digitalRead(dataIn)+digitalRead(dataIn)+digitalRead(dataIn); // Tripple sampling to remove glitches
        if((micros() - time) > 800){ // New data stream
          //Serial.print("   "); 
          Serial.println(" ");
        }
        else if((micros() - time) > 400){
         // Serial.print("  ");
        }
        if (out > 1){
         //Serial.print("1");
          dataBIN[i] = 1;
          //Serial.print(dataBIN[i]);
          i = i + 1;
        }
        else{
         //Serial.print("0");
          dataBIN[i] = 0;
          i = i + 1;
          //Serial.print(dataBIN[i]);
        }
        //Serial.print(",");
        time = micros();
      }
 
 
    }
    // Remove "/*" amd "*/" if you want to see the whole byte stream
    /*   
    if(currentMillis - previousMillis >= interval)  // Delays the 24 bit stream output for easier reading. Only needed if whole bit stream need to be read;
     { 
       previousMillis = currentMillis;   
         for(int i=0; i<24; i++) // Print the dataBIN
         {
          Serial.print(dataBIN[i]);
         }
        Serial.println();
 
      }
    
    */
    
    
    for(int i=1; i<16; i++){ // Select binary stream containing measurement data
      number[i-1] = dataBIN[i];
    }
    
    if (dataBIN[21] == 1) // This controls the sign of the output number
    {
      sign = -1;
    }
    else if (dataBIN[21] == 0)
    {
      sign = 1;
    }
    
    
    
  num = BINtoDEC(number,15)*sign/100; // num is the the decimal equivalent of the 15 bit binary number
  Serial.print(num);
  Serial.println();

}

//******* Functions *******

double BINtoDEC(int bin[],int num) // This function converts a binary array to decimal. LSB first. Note that it returns the decimal number as a double.
{                                  // bin[] is the array containing the binary number as an int data type, and num is the lenght of the array.
int i; 
double sum=0;
    for (i=0; i<num; i++)
    {
    sum = sum+bin[i]*pow(2.0,i) ;
    }
  return(sum);
}

