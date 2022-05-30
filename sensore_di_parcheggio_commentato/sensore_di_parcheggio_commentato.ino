#include <LiquidCrystal.h>     //inclusione della libreria contenente i comandi necessari per l'uso del display LCD

#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 13;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

const int triggerPort = 10;            //assegnazione delle porte
const int echoPort = 9;
const int led = 1;
const int button = 7;
const int ledRetromarcia = 6;
const int buzzer = 8;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  //assegnazione delle porte del display LCD

static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}
 
void setup(){ 
  pinMode(triggerPort, OUTPUT);         //inizializzazione delle porte
  pinMode(echoPort, INPUT);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  Serial.begin(9600);
}

unsigned long t1, dt;                    //variabili che servono per misurare il tempo
int tblink = 100;                        //variabile che serve a definire il tempo di lampeggio del led
int statoled = LOW;                      //variabile che serve a modificare lo stato del led
int freq = 220;                          //variabile che serve per generare il tono iniziale del buzzer
int stato = LOW;                         //variabile che indica se la retromarcia (pressione del bottone) è stata inserita



void loop(){
  float temperature;
  float humidity;
  long velocita;
  delay(500);
  if( measure_environment( &temperature, &humidity ) == true )
    {
      Serial.print( "T = " );
      Serial.print( temperature, 1 );
      Serial.print( " deg. C, H = " );
      velocita = (331.45+(0.62*temperature));
      Serial.println(velocita);
    } else {
      Serial.println("no");
      velocita = 340;
    }

    
  if(digitalRead(button) == HIGH){        // controllo se in bottone è stato premuto o meno
    stato=!stato;                         //cambio lo stato del pulsante
    delay(100);
  }
  
  if(stato == HIGH){                      //se il bottone è stato premuto...
      digitalWrite(ledRetromarcia,HIGH);  // accendo i led della retromarcia
      long distanza = misura(velocita);           //richiamo la funzione misura() che restituisce il valore misurato dal sensore in cm
      lcd.setCursor(0,1);                 //stampiamo i valori sul display LCD
      lcd.clear();
      lcd.print("distanza cm:");
      lcd.print(distanza);
    
    if(distanza < 30){                    // se la distanza rilevata è inferiore a 30 cm...
      digitalWrite(led, HIGH);            // il led non lampeggia ma resta accesso fisso
      tone(buzzer, 880);                  //il buzzer suona in modo continuo
      lcd.setCursor(0,1);
      lcd.print("WARNING!");              //stampiamo il messaggio di pericolo sul display LCD
      
    }else if (distanza > 70){             //se la distanza è maggiore di 70 cm...
       digitalWrite(led, LOW);            //il led resta spento
       noTone(buzzer);                    //il buzzer non suona
       
    }else{
      tblink = map(distanza, 30, 70, 30, 300); //mappo il tempo di lampeggiamento del led in funzione della distanza rilevata
      freq = map(distanza, 30, 70, 880, 220);  //mappo la frequenza di risonanza del buzzer in funzione della distanza rilevata
      dt = millis() - t1;                 //uso la funzione millis() per far lampeggiare il led dato che millis() sarà sempre maggiore di t1
      
      if(dt >= tblink){                   //una volta raggiunto il tempo di lampeggiamento...
        statoled = !statoled;             //cambio lo stato del led e lo stampo
        digitalWrite(led, statoled);
        t1 = millis();                    //pongo t1 pari a millis()
         
        if(statoled){                     //se il led è acceso...
          tone(buzzer, freq, 100);        //il buzzer suona
          
        }else{
          noTone(buzzer);                 //spengo il buzzer
        }
      }     
    }  
    delay(100);
  }
   else{                               //se la retormarcia viene disattivata...
    digitalWrite(ledRetromarcia,LOW);  //i led si spengono
    digitalWrite(led,LOW);
    noTone(buzzer);                    //il buzzer non suona
    lcd.setCursor(0,1);
    lcd.print("E' DISATTIVATO");       //viene stampato il seguente messaggio
  }
}


long misura(long velocita) {
  digitalWrite(triggerPort,LOW);       //porta a LOW il pin del trigger
  digitalWrite(triggerPort,HIGH);      //invia un impulso di 10 microsecondi sul pin del trigger
  delayMicroseconds(10);
  digitalWrite(triggerPort, LOW);

  //long velocita;
  /*
  if( measure_environment( &temperature, &humidity ) == true )
  {
    Serial.print( "T = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C, H = " );
    velocita = (331.45+(0.62*temperature));
  }else{
    Serial.println("NO");
    velocita = 340;
  }*/
   
  long durata = pulseIn(echoPort,HIGH); //la funzione pulseIn() misura il tempo che una porta impiega a passare da uno stato LOW a uno HIGH o viceversa. Restituisce il tempo in microsecondi
  long distanza = (velocita*0.0001)*durata/2;       //calcolo la distanza misurata facendo la velocità del suono espressa in cm/s moltiplicata per il tempo che il suono ha impiegato a ritornare al sensore diviso 2 dato che il suono percorre la stessa distanza sia all'andata che al ritorno 
  Serial.println(distanza);
  return distanza;                      //restituisco la distnaza in cm
    
}
