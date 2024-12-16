#define echoPin 11 // Se define el pin que se utilizará para el Echo del sensor ultrasónico
#define trigPin 12 // Se define el pin que se utilizará para el Trigger del sensor ultrasónic
int MAX_DISTANCE = 200; // Se da la distancia que debe mantener el carrito de un objeto (en este caso en centímetros)
float duracion, pid, salida, h, distancia, tk, tk_1=0, error=0, error_anterior=0, derivative=0, integral=0, kp=0.5, kd=0.1, ki=.03;
char c;
volatile int vel;

void setup() {
  Serial.begin (9600);
 
  pinMode(trigPin, OUTPUT);//pines del sensor de ultrasonido
  pinMode(echoPin, INPUT);
  pinMode(4,OUTPUT);//Salidas del motor
  pinMode(2,OUTPUT);
  

}

void loop() {
  digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 digitalWrite(trigPin, LOW);
 duracion = pulseIn(echoPin, HIGH);// datos del sensor de ultrasonido
 distancia = duracion/58.4;//se calcula la distancia en cm con respecto a la velocidad del sonido con el sensor de ultrasonido
 distancia=round(distancia);//Se redondea el valor obtenido (esto es opcional)
 tk = millis(); //guardamos en tk el tiempo que a transcurido desde que el arduino comenzó a trabajar
 h=tk-tk_1;//guardamos en h el perdiodo de ciclo
 error = MAX_DISTANCE - distancia;//calculamos el error
 integral = integral + (error*h);//se calcula el integral del error
 derivative = (error - error_anterior)/h;//se calcula la derivada del error
 pid = (error*kp) + (integral*ki) + (derivative*kd);// ecuaciones del pid
 error_anterior = error;//se manda el error actual al error anterior
 tk_1=tk;//se manda el tiempo actual al tiempo anterior
 
  // Leer datos del puerto serie
  while (Serial.available() > 0) {
    c = Serial.read();
    if (c == 'v') {
      vel = Serial.parseInt();
    }
  }

 if (vel >= 0 && vel <= 255) {
    if (pid > 85)     {pid  = 85;}// Se pone un límite en el valor máximo que puede alcanzar el valor de pid (esto es opcional, en mi caso ayudo a tener una mejor respuesta)
     else if (pid < -100) { pid = -100;}//Se pone un límite en el valor mínimo que puede alcanzar el valor de pid (esto es opcional, en mi caso ayudo a tener una mejor respuesta)
  
    
    if (pid < 0)
    {
      digitalWrite(4,LOW);
      digitalWrite(2,HIGH);// el carro se mueve hacia atrás 
      
      salida=map(pid,-100,0,255,0);// se mapea PID para que el pulso pwm se maneje dentro del rango 
      analogWrite(11,pid);

      
    }
    
    else
    {
       digitalWrite(4,HIGH);//el carro se mueve hacia enfrente
       digitalWrite(2,LOW);
      
       salida=map(pid,0,85,0,255);
       analogWrite(11,pid);
      
      
    }
    // Estos comandos son para vizualizar los datos obtenidos en el monitor serie (esta en la pestaña de herramientas :v ) 
 Serial.print("distancia:");
 Serial.print(distancia);
 Serial.print("Salida:");
 Serial.print(salida);
 Serial.print("PID:");
 Serial.println(pid);
 
   }

}
