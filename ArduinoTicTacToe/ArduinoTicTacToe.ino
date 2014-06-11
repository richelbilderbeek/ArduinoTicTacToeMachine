//Hieronder alles voor de buttons en beurten bijhouden
const int buttonRow[3] = {5, 8, 11};                            //Array met de rijpinnen om stroom op de buttons te zetten (per rij)
const int buttonCol[3] = {A5, A4, A3};                               //Array met de kolompinnen waarmee de buttons uitgelezen worden (per kolom)
int ledState[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};           //Array waarin de definitieve staat van een vakje wordt opgeslagen (na uitlezen en debouncen button(s))
int buttonState[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};        //Array waarin de huidige staat van een button opgeslagen wordt (na uitlezen)
int lastDebounceTime[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};   //Array waarin de tijd wanneer een button als 'aan' uitgelezen wordt wordt opgeslagen (voor debounce vergelijking met debouncetime)
int debounceDelay = 50;                                           //debounceDelay geeft aan hoeveel milliseconden een button 'aan' moet zijn om daadwerkelijk te debouncen
int reading = 0;                                                  //In reading wordt de huidige buttonstatus opgeslagen (voor wegschrijven in buttonState array)
int beurt = 1;                                                    //Beurt houdt wie aan de beurt is bij. 1 is blauw, 2 is rood.
int aantalBeurten = 0;  //Houdt het aantal beurten bij zodat je weet of het spel afgelopen is of niet
int winnaar = 0;

//Hieronder alles voor de leds aansturen
const int ledRed[3] = {7, 10, 13};                                 //Array die de rijen waar stroom op komt voor de rode ledjes weer
const int ledBlue[3] = {6, 9, 12};                                 //Array die de rijen waar stroom op komt voor de blauwe ledjes weer
const int ledCol[3] = {A2, A1, A0};                               //Array die de kolommen weergeeft van de ledjes waar wel of geen stroom op moet komen om te laten branden 

//Setup zet alles klaar om daarna het spel te kunnen spelen
void setup() {                                                    //Begin de setup
  for (int thisPin = 0; thisPin != 3; ++thisPin) {                //Start een loop voor steeds 3 pinnnen (alle arrays zijn 3x3)
    pinMode(buttonRow[thisPin], OUTPUT);                          //Zet de pinnen van de rijen van de buttons waar om de beurt stroom op moet op output
    pinMode(buttonCol[thisPin], INPUT);                           //Zet de pinnen van de rijen die je van de buttons uit wil lezen op input
    digitalWrite(buttonRow[thisPin], LOW);                        //Zet de stroom op de rijen van buttons op laag (zodat je er daarna om de beurt stroom op kan zetten)
    pinMode(ledRed[thisPin], OUTPUT);                             //Zet de pinnen van de rijen van de rode ledjes waar stroom op moet komen of niet op output
    pinMode(ledCol[thisPin], OUTPUT);                             //Zet de pinnen van de kolommen van de ledjes (ground) waar stroom wel/niet op moet komen op output
    pinMode(ledBlue[thisPin], OUTPUT);                            //Zet de pinnen van de rijen van de blauwe ledjes waar stroom op moet komen of niet op output
    digitalWrite(ledCol[thisPin], HIGH);                          //Zet de pinnen van de rijen van de kolommen van de ledjes (ground) op hoog zodat er geen ledje kan branden
    digitalWrite(ledRed[thisPin], LOW);                           //Zet de pinnen van de rijen van de rode ledjes op laag zodat er geen ledje brandt
    digitalWrite(ledBlue[thisPin], LOW);                          //Zet de pinnen van de rijen van de blauwe ledjes op laag zodat er geen ledje brandt
  }
  Serial.begin(9600);                                             //Zet de Serial Monitor klaar om het verloop van de code uit te kunnen lezen (kan er later uit)
}                                                                 //Eindig de setup functie

//Start het daadwerkelijke programma  
void loop() {                                                                   //Start de functie voor Tic Tac Toe   

/* 

Onderstaande code loop eerst door de button array om te kijken of er veranderingen in de buttonmatrix zijn geweest. 
Deze veranderingen worden afhankelijk van de beurt opgeslagen in de ledarray om aan te geven wat de actuele status van het spel is. 

*/
  for (int thisRow = 0; thisRow != 3; ++thisRow) {                              //Loop de 3 rijen van een array om de beurt bij langs en stop bij de derde rij
    digitalWrite(buttonRow[thisRow], HIGH);                                     //Zet stroom op de rij van de arraay die aan de beurt is
    for (int thisCol = 0; thisCol != 3; ++thisCol) {                            //Loop de 3 kolommen van een array om de beurt bij langs en stop bij de derde kolom
      reading = digitalRead(buttonCol[thisCol]);                                //Lees de kolom van burrons uit en sla de waarde op in reading (zorgt dat er een specifieke button waarde in reading staat)
      if (reading == 1 && buttonState[thisRow][thisCol] == 0) {                 //Als er een 'aan' wordt uitgelezen voor een button en dit is niet eerder gebeurd (buttonState) dan... 
        buttonState[thisRow][thisCol] = 1;                                      //Sla dan 1 op in buttonState array op de plek van de uitgelezen button
        lastDebounceTime[thisRow][thisCol] = millis();                          //Sla het tijdstip van uitlezen op in lastDebounceTime array van de uitgelezen button
      }                                                                         //Stop de if statement 
      else if (reading == 1 && buttonState[thisRow][thisCol] == 1) {            //Als er 'aan' is uigelezen van een specifieke button en dit is al eerder gebeurd dan...
        if ((millis() - lastDebounceTime[thisRow][thisCol]) > debounceDelay) {  //Check of de debounceDelay overschreden is, als dat zo is dan...
          buttonState[thisRow][thisCol] = 0;                                    //Zet buttonsState weer op 0 zodat er later opnieuw een button gedetecteerd kan worden
          if (ledState[thisRow][thisCol] == 0) {                                //Check of een ledje aan of uit gezet moet worden, afhankelijk van de beurt
            switch(beurt) {                                                     //Controleer welke kleur er aan de beurt is 
              case 1:                                                           //Als 1 aan de beurt is moet er een 1 (blauw) opgeslagen worden in de ledarray 
                ledState[thisRow][thisCol] = 1;                                 //Sla 1 (blauw) op in de ledarray
                beurt = 2;                                                      //En zet beurt op 2 (rood)
                ++aantalBeurten;                                                //Hoog aantal beurten met 1 op
                break;                                                          //Break uit de switch statement van beurt checken voor een ledje aanzetten
              case 2:                                                           //Als 2 aan de beurt is moet er een 2 (rood) opgeslagen worden in de ledarray 
                ledState[thisRow][thisCol] = 2;                                 //Sla 2 (rood) op in de ledarray
                beurt = 1;                                                      //En zet beurt op 1 (blauw)
                ++aantalBeurten;                                                //Hoog aantal beurten met 1 op
                break;                                                          //Break uit de switch statement van beurt checken voor een ledje aanzetten
             }                                                                  //Stop de switch statement voor beurt checken
           }                                                                    //Stop de if statement voor ledjes aanzetten
        }                                                                       //Stop de debounce check
      }                                                                         //Stop de check op of er daadwerkelijk een ledje aangezet moet worden aan de hand van debouncen  
    }                                                                           //Stop de for-loop van het door de kolommen lopen  
    digitalWrite(buttonRow[thisRow], LOW);                                      //Haal de stroom van de actieve rij van de array zodat de volgende straks aangezet kan worden (op de buttons)                               
  }                                                                             //Stop de for-loop van het door de rijen lopen als je alle rijen hebt gehad 



/*
Onderstaande code zorgt ervoor de de ledmatrix op de juiste manier gaat branden (afhankelijk van de huidige status van het spel)
*/
  
  if (winnaar == 0) {
  for (int thisRow = 0; thisRow != 3; ++thisRow) {              //Loop door de rijen
    for (int thisCol = 0; thisCol != 3; ++thisCol) {            //Loop door de kolommen
      if (ledState[thisRow][thisCol] == 1) {                    //Als de status van een ledje 1 is dan...
        digitalWrite(ledCol[thisCol], LOW);                     //Zet de ledkolom op LOW zodat er stroom door kan gaan lopen
        digitalWrite(ledBlue[thisRow], HIGH);                   //Zet de ledrij waarin het desbetreffende blauwe ledje zit op hoog zodat hij aan gaat
        delay(1);                                               //Vertraag 1 milliseconde zodat het ledje regematig lijkt te branden
        digitalWrite(ledCol[thisCol], HIGH);                    //Zet de stroom op desbetreffende kolom weer op hoog (kan geen ledje meer branden)
        digitalWrite(ledBlue[thisRow], LOW);                    //Zet de stroom op desbetreffende blauwe rij weer laag (kan geen ledje meer branden
      }                                                         //Sluit de if statement
      if (ledState[thisRow][thisCol] == 2) {                    //Als de status van het ledje 2 is dan...
        digitalWrite(ledCol[thisCol], LOW);                     //Zet de ledkolom op LOW zodat er stroom door kan gaan lopen
        digitalWrite(ledRed[thisRow], HIGH);                    //Zet de ledrij waarin het desbetreffende rode ledje zit op hoog zodat hij aan gaat
        delay(1);                                               //Vertraag 1 milliseconde zodat het ledje regelmatig lijkt te branden
        digitalWrite(ledCol[thisCol], HIGH);                    //Zet de stroom op desbetreffende kolom weer op hoog (kan gene ledje meer branden)
        digitalWrite(ledRed[thisRow], LOW);                     //Zet de stroom op desbetreffend rode rij weer laag (kan geen ledje meer branden)
      }                                                         //Sluit de if statement
      digitalWrite(ledCol[thisCol], HIGH);                      //Zet de kolom weer op HIGH zodat er geen ledjes branden (is maar heel kort dus geen effect op of ledjes lijken te branden of niet 
     }                                                           //Sluit de for loop voor door de kolommen lopen
    }     //Sluit de for loop voor door de rijen lopen 
  }

  
  if (winnaar != 0) {
   aantalBeurten = 0;
   for (int teller = 0; teller != 5; ++teller) {
    for (int thisRow = 0; thisRow != 3; ++thisRow) {
      for (int thisCol = 0; thisCol != 3; ++thisCol) {
        if (ledState[thisRow][thisCol] == 1) {
          digitalWrite(ledCol[thisCol], LOW); 
          digitalWrite(ledBlue[thisRow], HIGH);
          delay(250); 
          digitalWrite(ledCol[thisCol], HIGH);
          digitalWrite(ledBlue[thisRow], LOW);
        }
        if (ledState[thisRow][thisCol] == 2) {
          digitalWrite(ledCol[thisCol], LOW); 
          digitalWrite(ledRed[thisRow], HIGH);
          delay(250); 
          digitalWrite(ledCol[thisCol], HIGH);
          digitalWrite(ledRed[thisRow], LOW);
        }
      }
    }
    delay(250);
   }
   winnaar = 0; 
   beurt = 1;
   reading = 0;
   for (int thisRow = 0; thisRow != 3; ++thisRow) {
    for (int thisCol = 0; thisCol != 3; ++thisCol) {
      ledState[thisRow][thisCol] = 0;           //Array waarin de definitieve staat van een vakje wordt opgeslagen (na uitlezen en debouncen button(s))
      buttonState[thisRow][thisCol] = 0;
      lastDebounceTime[thisRow][thisCol] = 0;
    }
   }
  }    
  
  if (aantalBeurten == 9) {
   aantalBeurten = 0;
   for (int teller = 0; teller != 5; ++teller) {
    for (int thisCol = 0; thisCol != 3; ++thisCol) {            //Loop door de kolommen
        digitalWrite(ledCol[thisCol], LOW);                     //Zet de ledkolom op LOW zodat er stroom door kan gaan lopen
    }
    for (int thisRow = 0; thisRow != 3; ++thisRow) {              //Loop door de rijen
      digitalWrite(ledBlue[thisRow], HIGH);          //Zet de ledrij waarin het desbetreffende blauwe ledje zit op hoog zodat hij aan gaat
    }
    delay(500);       //Vertraag 1 milliseconde zodat het ledje regematig lijkt te branden
    for (int thisRow = 0; thisRow != 3; ++thisRow) {              //Loop door de rijen
      digitalWrite(ledBlue[thisRow], LOW);          //Zet de ledrij waarin het desbetreffende blauwe ledje zit op hoog zodat hij aan gaat
    }
    delay(500);
    for (int thisRow = 0; thisRow != 3; ++thisRow) {              //Loop door de rijen
        digitalWrite(ledRed[thisRow], HIGH);                    //Zet de ledrij waarin het desbetreffende rode ledje zit op hoog zodat hij aan gaat
    }
    delay(500);
    for (int thisCol = 0; thisCol != 3; ++thisCol) {            //Loop door de kolommen
        digitalWrite(ledCol[thisCol], HIGH);                     //Zet de ledkolom op LOW zodat er stroom door kan gaan lopen
        digitalWrite(ledRed[thisCol], LOW);
    }
    delay(500);                                               //Vertraag 1 milliseconde zodat het ledje regelmatig lijkt te branden
                                                             //Sluit de if statement
//      digitalWrite(ledCol[thisCol], HIGH);                      //Zet de kolom weer op HIGH zodat er geen ledjes branden (is maar heel kort dus geen effect op of ledjes lijken te branden of niet 
     }     //Sluit de for loop voor door de kolommen lopen
      winnaar = 0; 
   beurt = 1;
   reading = 0;
   for (int thisRow = 0; thisRow != 3; ++thisRow) {
    for (int thisCol = 0; thisCol != 3; ++thisCol) {
      ledState[thisRow][thisCol] = 0;           //Array waarin de definitieve staat van een vakje wordt opgeslagen (na uitlezen en debouncen button(s))
      buttonState[thisRow][thisCol] = 0;
      lastDebounceTime[thisRow][thisCol] = 0;
    }     
     
     
    }     //Sluit de for loop voor door de rijen lopen 


   }
          
  
  
  /*
Onderstaande code checked of er een winnaar is of niet. 
- Als het aantalBeurten gelijk is aan 8 (omdat we vanaf 0 tellen) dan is het einde van het spel bereikt en moet er gechecked worden of er een winnaar is of niet. 
  Een winnaar heeft 3 op 1 rij dus dit checken in de ledState array. 
*/

  for (int thisRow = 0; thisRow != 3; ++thisRow) {
    if (ledState[thisRow][0] != 0) {
      if ((ledState[thisRow][0] == ledState[thisRow][1]) && (ledState[thisRow][1] == ledState[thisRow][2])) {
        winnaar = ledState[thisRow][0];
        for (int thisRow = 0; thisRow != 3; ++thisRow) {
        for (int thisCol = 0; thisCol != 3; ++ thisCol) {
          ledState[thisRow][thisCol] = 0;
        }
      }
      ledState[thisRow][0] = winnaar;
      ledState[thisRow][1] = winnaar;
      ledState[thisRow][2] = winnaar;
      }
    }
  }
  for (int thisCol = 0; thisCol != 3; ++thisCol) {
    if (ledState[0][thisCol] != 0) {
      if ((ledState[0][thisCol] == ledState[1][thisCol]) && (ledState[1][thisCol] == ledState[2][thisCol])) {
        winnaar = ledState[0][thisCol];
        for (int thisRow = 0; thisRow != 3; ++thisRow) {
        for (int thisCol = 0; thisCol != 3; ++ thisCol) {
          ledState[thisRow][thisCol] = 0;
        }
      }
      ledState[0][thisCol] = winnaar;
      ledState[1][thisCol] = winnaar;
      ledState[2][thisCol] = winnaar;
      }
    }
  }
  if (ledState[1][1] != 0) {
    if ((ledState[1][1] == ledState[0][0]) && (ledState[1][1] == ledState[2][2])) {
      winnaar = ledState[1][1];
        for (int thisRow = 0; thisRow != 3; ++thisRow) {
        for (int thisCol = 0; thisCol != 3; ++ thisCol) {
          ledState[thisRow][thisCol] = 0;
        }
      }
      ledState[0][0] = winnaar;
      ledState[1][1] = winnaar;
      ledState[2][2] = winnaar;
    }
    if ((ledState[1][1] == ledState[0][2]) && (ledState[1][1] == ledState[2][0])) {
      winnaar = ledState[1][1];
        for (int thisRow = 0; thisRow != 3; ++thisRow) {
        for (int thisCol = 0; thisCol != 3; ++ thisCol) {
          ledState[thisRow][thisCol] = 0;
        }
      }
      ledState[0][2] = winnaar;
      ledState[1][1] = winnaar;
      ledState[2][0] = winnaar;
    }
  }
      
}                                                               //Begin de void loop hierna opnieuw 

 

