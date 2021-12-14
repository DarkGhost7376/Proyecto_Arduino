#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>
Servo servo1;
LiquidCrystal lcd (A0, A1, A2, A3, A4, A5);

//Matriz
const byte Filas = 4;
const byte Cols = 4;
byte Pins_Filas[] = {9,8,7,6};
byte Pins_Cols[] = {5,4,3,2};

//Definimos el teclado
char Teclas [Filas][Cols]=
{
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

char contraCerr[4]={'9','8','7','6'}; // Contraseña 
char cerrar[1]={'A'}; // Dígito para cerrar la puerta
int posicion=0;
int cursor=5;
int clave=0;       // la clave es incorrecta
int ledVerde=12;   // pin para el LED verde
int ledRojo=13;    // pin para el LED rojo
int buzzer=10;     // pin buzzer
int cont=0;		   // Contador de errores

//teclado
Keypad Teclado = Keypad(makeKeymap(Teclas), Pins_Filas, Pins_Cols, Filas, Cols);

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);       // Inicializamos el LCD.
  servo1.write(0); // Servomotor en posición inicial de 0º
  pinMode (ledVerde,OUTPUT);
  pinMode (ledRojo,OUTPUT);
  servo1.attach(11); //Pin del servomotor
  pinMode (buzzer,OUTPUT);
  digitalWrite(ledRojo,HIGH); // encendemos el LED rojo
  digitalWrite(ledVerde,LOW); // apagamos el LED verde
  lcd.setCursor(3,0);     // situamos el cursor el la posición 1 de la linea 0.
  lcd.print("Bienvenido"); // escribimos en LCD
  delay(2000);
  lcd.setCursor(1,0);     // situamos el cursor el la posición 1 de la linea 0.
  lcd.print("Ingresar Clave"); // escribimos en LCD
  lcd.setCursor(6,1); // cursor en la posicion 6 de la variable, linea 1
  lcd.print("____"); // borramos de la pantalla los numeros  
  lcd.setCursor(6,1);
}

void loop()
{
  int conteo=10;
  char pulsacion=Teclado.getKey();
  if(pulsacion!=0) //Si el valor es 0 es que no se ha pulsado ninguna tecla
  {
    if(pulsacion!='*' && pulsacion!='#' && clave==0) // Se descarta asterisco y numeral
    {
      lcd.print("*");
      cursor++;             // incrementamos el cursor
      tone(buzzer,800);     // tono de pulsacion
      delay(10);
      noTone(buzzer);
      
      //------ Se comprueba que la contraseña es correcta ------
      
      if(pulsacion==contraCerr[posicion])
        posicion++; // Se aumenta posicion si es correcto el digito
      
      if(posicion==4)
      {
        lcd.setCursor(0,0);
        lcd.print(" Clave Correcta ");// escribimos en LCD     
        delay(200);
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Abriendo...");
        tone(buzzer,500); // tono de clave correcta
        delay(100);       
        noTone(buzzer);  
        tone(buzzer,600);
        delay(100);
        noTone(buzzer);
        tone(buzzer,800);
        delay(200);
        noTone(buzzer);
        clave=1; // indicamos que la clave es correcta
        digitalWrite(ledRojo,LOW); // apagamos el LED rojo
        digitalWrite(ledVerde,HIGH); // encendemos el verde
        servo1.write(90); // Servomotor se posiciona en 90º
        delay(2000);
        lcd.clear();
        lcd.setCursor(1,0); // cursor en la posicion 0, linea 0
        lcd.print("Puerta Abierta");
        lcd.setCursor(5,1);
        lcd.print(">>>>>>");
        delay(2000); 
        lcd.clear();
        lcd.setCursor(0,0); 
        lcd.print("`A`  Para Cerrar");
        lcd.setCursor(6,1); 
        lcd.print("____");  
        lcd.setCursor(7,1);
      }
      
     //--- En el caso de que no se haya acertado ----------
            
     if(cursor>8)        // comprobamos que no pase de la cuarta posicion
       {   
         cursor=5;     // lo volvemos a colocar al inicio
         posicion=0;           // borramos clave introducida
         lcd.setCursor(6,1);
         lcd.print("____");  // borramos la clave de la pantalla	        
         lcd.setCursor(6,1); 
       
         if(clave==0)         // comprobamos que no hemos acertado
         {
           tone(buzzer,70,500); // para generar tono de error
           delay(250); 
           noTone(buzzer);
           cont++;//Aumentamos contador de errores
           lcd.setCursor(0,0);     // situamos el cursor el la posición 0 de la linea 0.
      	   lcd.print("Clave Incorrecta"); // escribimos en LCD
           
           if(cont==3) //acumula la cantidad de errores
           delay(100);
           else
           delay(2000);//tiempo de espera de 2 segundos
            
           lcd.setCursor(6,1); // cursor en la posicion 6, linea 1
           lcd.setCursor(0,0);     // situamos el cursor el la posición 0 de la linea 0.
      	   lcd.print(" Ingrese Clave "); // escribimos en LCD
           lcd.setCursor(6,1); // cursor en la posicion 6, linea 1
         }        
       }
      
     // --- Si la cantidad de errores es igual a 3, se encenderá la alarma---
      
     if(cont==3)
     { 
        while(conteo!=0)
        {
          tone(buzzer,200,500);//Tono de Alarma
          lcd.clear();
          lcd.setCursor(0,0);
      	  lcd.print("Sistema");
      	  lcd.setCursor(0,1);
      	  lcd.print("Bloqueado");
      	  lcd.setCursor(12,1);
          lcd.print(conteo);
          delay(1000);
          conteo--;
    	}
          cont=0;//reseteamos el contador de errores
          lcd.clear();
          pulsacion='*';
          lcd.setCursor(2,0);     
          lcd.print("Reparando"); 
          lcd.setCursor(4,1); 
          lcd.print("Sistema"); 
          delay(2000);
          lcd.clear();
          lcd.setCursor(3,0);
          lcd.print("Espere..."); 
          delay(2000);
          lcd.clear();
          tone(buzzer,500); // tono de clave correcta
          delay(100);       
          noTone(buzzer);  
          tone(buzzer,600);
          delay(100);
          noTone(buzzer);
          tone(buzzer,800);
          delay(200);
          noTone(buzzer);
          lcd.setCursor(0,0);
          lcd.print("---- Listo ----");
          delay(2000);
          lcd.clear();
          lcd.setCursor(1,0);     
          lcd.print("Ingrese Clave"); 
          lcd.setCursor(6,1); 
          lcd.print("____"); 
          lcd.setCursor(6,1);
      }
  }
    
  // --- Condicional para cerrar la puerta--- 

  if(pulsacion=='A' && clave==1)
  {
    lcd.setCursor(7,1);
    lcd.print("*");
    cursor++;             // incrementamos el cursor
    tone(buzzer,800);     // tono de pulsacion
    delay(30);
    noTone(buzzer); 
        
    if(pulsacion==cerrar[posicion]) 
       posicion++;
           
    if(posicion==1)
    {
       delay(100);
       lcd.clear();
       lcd.setCursor(2,0); 
       lcd.print("Cerrando....");
       clave=0; // indicamos que se ha borrado la clave
       cursor=5;
       posicion=0;
       tone(buzzer,800);
       delay(100);
       noTone(buzzer);
       tone(buzzer,600);
       delay(100);
       noTone(buzzer);
       tone(buzzer,500);
       delay(200);
       noTone(buzzer);
       digitalWrite(ledVerde, LOW); // Apagamos el verde         
       digitalWrite(ledRojo,HIGH); // Encendemos el LED rojo
       servo1.write(0);
       delay(2000);
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print(" Puerta Cerrada");
       lcd.setCursor(5,1);
       lcd.print("<<<<<<");
       delay(2000);
       lcd.clear();
       lcd.setCursor(1,0);     // situamos el cursor en la posición 0 de la linea 0.
   	   lcd.print("Ingrese Clave"); // escribimos en LCD
       lcd.setCursor(6,1); // cursor en la posicion 0, linea 1
       lcd.print("____"); // borramos de la pantalla los numeros
       lcd.setCursor(6,1);
       cont=0;//reseteamos el contador de errores      
    }
   } 
 }
  
 //--- Condicionales para resetear clave introducida -------------
  
 if (pulsacion== '*' && clave==0)
 { 
   // se debe presionar asterisco para borra los digitos ingresados
   posicion = 0;
   cursor = 5;
   clave=0;
   posicion=0;
   lcd.setCursor(1,0); // situamos el cursor en la posición 2 de la linea 0.
   lcd.print("Ingrese Clave"); // escribimos en LCD
   lcd.setCursor(6,1);
   lcd.print("____"); // borramos de la pantalla los numeros
   lcd.setCursor(6,1);  
   digitalWrite(ledRojo,HIGH); // encendemos el LED rojo
   digitalWrite(ledVerde, LOW); // apagamos el verde
 }
}