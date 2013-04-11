/*
  POV3DLED
  Display a 3d image using tehe persistance of view
 */
 

// Pins
int matriceAllumer[13][6];
int matriceCouleur[13][6];
const int luminosityPin = A0;



int leds [13]={1,2,3,4,5,6,7,8,9,10,11,12,13};
int r = 14;
int b = 16;
int g = 15;

// the setup
void setup() {                
  // initialize the digital pin as an output.
  Serial.begin(9600);
for (int i=0;i<13;i++) {pinMode( leds[i], OUTPUT );};
 pinMode(r, OUTPUT); 
 pinMode(b, OUTPUT); 
 pinMode(g, OUTPUT);   
 
}

void actualiserColonne()
{ 
  int i,j ;
for (j=0;j<6;j++)
{for (i=0;i<13;i++) {if (j==0)  {matriceAllumer[i][6]=matriceAllumer[i][0];matriceAllumer[i][j]=matriceAllumer[i][j+1];}
                     else if (!(j==0))  {matriceAllumer[i][j]=matriceAllumer[i][j+1];}
                     ;};
}
}


void choixLedAllumees ()
{

int i=0;
int j=0;
  
// initialisation des matrices à 0
for (i=0;i<13;i++) 
{for (j=0;j<5;j++) 

    {matriceAllumer[i][j]=0;
     matriceCouleur[i][j]=0;} ; 
} 


// Choix des leds allumées
for (i=0;i<13;i++) 
{for (j=0;j<5;j++) 

    {matriceAllumer[i][j]=1;};  
}
// Choix des couleurs
for (i=0;i<13;i++) 
{for (j=0;j<5;j++) 

    {matriceCouleur[i][j]=1;if (j==1) matriceCouleur[i][j]=2;if (j==2) matriceCouleur[i][j]=0;};  
};
matriceCouleur[7][0]=3;
matriceCouleur[9][0]=0;
matriceCouleur[8][0]=2;
matriceCouleur[10][0]=4;
matriceCouleur[11][0]=5;



}


  
void allumerUneLed(int i,int j)
{  //initialisation
    for (int i=0;i<13;i++) {digitalWrite( leds[i], LOW);}; 
    //Allumage des leds concernees
     if (matriceAllumer[i][j]==1) {digitalWrite( leds[i], HIGH);}
 //initialisation des couleurs
digitalWrite( r, HIGH);
digitalWrite( g, HIGH);
digitalWrite( b, HIGH);
//Allumage des couleurs
 if (matriceCouleur[i][j]==0) {digitalWrite( r, LOW);}
 else if (matriceCouleur[i][j]==1) {digitalWrite( b, LOW);}
 else if (matriceCouleur[i][j]==2) {digitalWrite( g, LOW);}
 else if (matriceCouleur[i][j]==3) {digitalWrite( g, LOW);digitalWrite( b, LOW);}
 else if (matriceCouleur[i][j]==4) {digitalWrite( g, LOW);digitalWrite( r, LOW);}
 else if (matriceCouleur[i][j]==5) {digitalWrite( r, LOW);digitalWrite( b, LOW);}
 else  {digitalWrite( r, LOW);}


}

void allumerColonne(int j)
{
  

for (int i=0;i<13;i++) {allumerUneLed(i,j);};
}



boolean testCapteur(int sensorValue,boolean b){

  if (sensorValue>630) {b=false;}
  else if (sensorValue<=630) {b= true;}
  return b;
}

// the loop routine runs over and over again forever:
void loop() {
    
  choixLedAllumees ();
  while (true){
  boolean b=true;
  int j=0;  
    while (j<5){
     
        
//    int waitTime = map(sensorValue, 200, 800, 20, 1000);
//  Serial.print("Capteur = " );
//  Serial.print(sensorValue);
//  Serial.print("\tDelai de clignotement (millisecondes) = ");
//  Serial.println(waitTime);
    int i=0; 
        while (i<400) //la valeur de i controle le temps d'allumage de chaque colonne
          {
            int sensorValue = analogRead(luminosityPin);
            Serial.println(sensorValue);
            b=testCapteur(sensorValue,b);
            if (b==false) {break;}
            allumerColonne(j);
            i++;
          }
     if (b==false) {break;}
    j++;  
    }
  
  }
  //delay(100);
    
   //or (int j=0;j<3;j++) {allumerColonne(j);delay(1000);};
    //allumerColonne(1);
    //digitalWrite( b, LOW);
    //for (int i=0;i<13;i++) {digitalWrite( leds[i], HIGH);delay(1);  };
}
