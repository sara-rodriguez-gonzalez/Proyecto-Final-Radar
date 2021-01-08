//Intentar que primero lea los valores y luego le aplique el filtro. O probar con menos mediciones.
#include <Wire.h>
#include <VL53L0X.h>
#include <Servo.h>  

VL53L0X sensor;
Servo myservo;

int pos=0,dis=0,aren1=0, aren2=0, aren3=0,acol1=0,acol2=0,acol3=0, obj1[10][2]={0},obj2[10][2]={0},obj3[10][2]={0}, cont1=0,cont2=0,cont3=0,flag=0;
int tobjs[90]={0};
int c, d, n=7, med=3;
double lectura, a[7]={0}, aux[7]={0}, chan;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  myservo.attach(9);

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  sensor.setMeasurementTimingBudget(200000);
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  
}

void loop()
{
  Serial.println("Medicion");
  int i=0,c,d, swap1,swap0,band1=0,band2=0,band3=0, c1=0, c2=0, c3=0, cfr=0, ren1=0, ren2=0, ren3=0,col1=0,col2=0,col3=0;
  float gr=PI/180,x1=0, x2=0, x3=0, y1=0, y2=0, y3=0;
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    //Cambia posición
    myservo.write(pos);              

    //Comprueba que no haya error
    if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
    
    //Toma distancia
    lectura=sensor.readRangeSingleMillimeters();
    if(lectura>850){
      lectura=0;
    }
    
    //FILTRO DE MEDIANA
    a[n-1] = lectura;

      for(c=0; c<n; c++)
    {
      aux[c]=a[c];
    }
     
    //Saca la mediana
    for (c = 0 ; c < n - 1; c++)
    {
      for (d = 0 ; d < n - c - 1; d++)
      {
        if (a[d] > a[d+1]) 
        {
          chan       = a[d];
          a[d]   = a[d+1];
          a[d+1] = chan;
        }
      }
    }
  
    Serial.println(a[med]);  
    tobjs[pos]=a[med];
    //Serial.println(lectura);
  
    for(c=0; c<n; c++)
    {
      a[c]=aux[c];
    }
    
    for(c=0; c<(n-1); c++)
    {
      a[c] = a[c+1]; 
    }
    
    /*Impresion de datos
    Serial.print("Ángulo: ");
    Serial.print(pos);*/
    //Serial.print("    Distancia: ");
    /*if(lectura>850){
      Serial.print(0);
    }
    else{
      Serial.print(lectura);
    }
    
    Serial.println();*/

    delay(5);
    
  }//for

  //Asignar objetos
  
      for(pos=0;pos<90;pos++){
        if((band3==0)&&(tobjs[pos]!=0)&&(band2==1)){
        //Serial.println("Entre a objeto 3");
        obj3[c3][0]=pos;
        obj3[c3][1]=tobjs[pos];
       // Serial.print("Pos: ");
        //Serial.print(obj3[c3][0]);
        //Serial.print(" Dis: ");
        //Serial.println(obj3[c3][1]);
        if(tobjs[pos+1]==0){
          band3=1;
        }
        c3++;
      }

      

      if((band2==0)&&(tobjs[pos]!=0)&&(band1==1)){
        //Serial.println("Entre a objeto 2");
        obj2[c2][0]=pos;
        obj2[c2][1]=tobjs[pos];
        //Serial.print("Pos: ");
        //Serial.print(obj2[c2][0]);
        //Serial.print(" Dis: ");
        //Serial.println(obj2[c2][1]);
        if(tobjs[pos+1]==0){
          band2=1;
        }
        c2++;
      }
    
      if((band1==0)&&(tobjs[pos]!=0)){
        obj1[c1][0]=pos;
        obj1[c1][1]=tobjs[pos];
        //Serial.print("Pos: ");
        //Serial.print(obj1[c1][0]);
        //Serial.print(" Dis: ");
        //Serial.println(obj1[c1][1]);
        if(tobjs[pos+1]==0){
          band1=1;
          //Serial.println("Cambié estado bandera 1");
        }
        c1++;
      }
   
  }
  
  //Sacar valor medio de ángulo
  
  if(c1!=0){
    if(c1%2==0){
    c1=c1/2;  
  }
  else{
    c1=(c1-1)/2;
  } 
  }

  if(c2!=0){
    if(c2%2==0){
    c2=c2/2;   
  }
  else{
    c2=(c2-1)/2;
  } 
  }

  if(c3!=0){
    if(c3%2==0){
    c3=c3/2;  
  }
  else{
    c3=(c3-1)/2;
  } 
  }

  Serial.println("*******CONTADORES*******");
  Serial.println(c1);
  Serial.println(c2);
  Serial.println(c3);
  

     //Rectangulares

   x1=obj1[c1][1]*sin(obj1[c1][0]*gr);
   y1=obj1[c1][1]*cos(obj1[c1][0]*gr);
   x2=obj2[c2][1]*sin(obj2[c2][0]*gr);
   y2=obj2[c2][1]*cos(obj2[c2][0]*gr);
   x3=obj3[c3][1]*sin(obj3[c3][0]*gr);
   y3=obj3[c3][1]*cos(obj3[c3][0]*gr);

   if(x1!=0){
    x1=x1/10;
    x1=x1-2;
   }
   if(y1!=0){
    y1=y1/10;
    y1=y1-2;
   }
   if(x2!=0){
    x2=x2/10;
    x2=x2-2;
   }
   if(y2!=0){
    y2=y2/10;
    y2=y2-2;
   }
   if(x3!=0){
    x3=x3/10;
    x3=x3-2;
   }
   if(y3!=0){
    y3=y3/10;
    y3=y3-2;
   }
      
  //Impresion de cuadrante

  Serial.println("******COORDENADAS******");
  Serial.println();

    if(((aren1-2.5)>x1||(aren1+2.5)<x1)&&((acol1-2.5)>y1||(acol1+2.5)<y1))
    {
      cont1=1;
      //Serial.println("Volvio cont1 a 1");
    }
    if((cont1<=3)&&(cont1!=0)&&(c1!=0))    
    {
      Serial.print("Objeto 1: ");
      Serial.print(x1);
      Serial.print(" ,");
      Serial.println(y1);
      Serial.print("Contador actual: ");
      Serial.println(cont1);
      cont1++;
      //Serial.print("Contador1 siguiente (++): ");
      //Serial.println(cont1);
    }    
    aren1=x1;
    acol1=y1;  

     Serial.println();

    if(((aren2-2.5)>x2||(aren2+2.5)<x2)&&((acol2-2.5)>y2||(acol2+2.5)<y2))
    {
      cont2=1;
    }
    if((cont2<=3)&&(cont2!=0)&&(c2!=0))
    { 
    Serial.print("Objeto 2: ");
    Serial.print(x2);
    Serial.print(" ,");
    Serial.println(y2);
    Serial.print("Contador2 actual: ");
    Serial.println(cont2);
    cont2++;
    //Serial.print("Contador2 siguiente (++): ");
    //Serial.println(cont2);
    }
    
    aren2=x2;
    acol2=y2;

     Serial.println();

    if(((aren3-2.5)>x3||(aren3+2.5)<x3)&&((acol3-2.5)>y3||(acol3+2.5)<y3))
    {
      cont3=1;
    }
    if((cont3<=3)&&(cont3!=0)&&(c3!=0))
    {
    Serial.print("Objeto 3: ");
    Serial.print(x3);
    Serial.print(" ,");
    Serial.println(y3);
    Serial.print("Contador3 actual: ");
    Serial.println(cont3);
    cont3++;
    //Serial.print("Contador3 siguiente (++): ");
    //Serial.println(cont3);
    }
    
    aren3=x3;
    acol3=y3;  

    /*if(flag==0){
      cont1++;
      cont2++;
      cont3++;
      flag=100;
    }*/
    
    for(i=0;i<7;i++){
      a[i]=0;
      aux[i]=0;
    }

     Serial.println();
    
    for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    }
    delay(500);
}
