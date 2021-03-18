
char d1[]="ABCDEFGHIJKLMNOPQR";
char d2[]="ABCDEFGHIJKLMNOPQRSTUVWX";
char locator[9];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  //char first = d1[2];

  //char* first = (char *) d1[2];
  
  //strcat(first,d1[2]);
   
  strncat(locator,&d1[2],1);

  //Serial.print("first:");
  //Serial.println(first);
  
  Serial.print("locator:");
  Serial.println(locator);


  delay(1000);
  
  
}
