void setup() {

  pinMode(6, OUTPUT);    /////////// *       , ///////////
  pinMode(7, OUTPUT);    /////////// ,       , ///////////

  pinMode(8, OUTPUT);    /////////// ,       * ///////////
  pinMode(9, OUTPUT);    /////////// ,       , ///////////

  pinMode(10, OUTPUT);   /////////// ,       , ///////////
  pinMode(11, OUTPUT);   /////////// *       , ///////////

  pinMode(12, OUTPUT);   /////////// ,       , ///////////
  pinMode(13, OUTPUT);   /////////// ,       * ///////////


  pinMode(2, OUTPUT);   ///////////   ///////////
  pinMode(3, OUTPUT);   ///////////  ///////////


  Serial.begin(115200);
  Serial.setTimeout(50);

}

void loop() {

  if (Serial.available() > 0) {

    String  data = Serial.readString();

    Serial.print(data);


    if (data == "f") {        //Forward

      digitalWrite(6,  HIGH);
      digitalWrite(7,  LOW);

      digitalWrite(8,  HIGH);
      digitalWrite(9,  LOW);

      digitalWrite(10, HIGH);
      digitalWrite(11, LOW);

      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);

    } else if (data == "b") {  //Backward

      digitalWrite(6,  LOW);
      digitalWrite(7,  HIGH);

      digitalWrite(8,  LOW);
      digitalWrite(9,  HIGH);

      digitalWrite(10, LOW);
      digitalWrite(11, HIGH);

      digitalWrite(12, LOW);
      digitalWrite(13, HIGH);

    } else if (data == "r") {   //Left

      digitalWrite(6,  HIGH);
      digitalWrite(7,  LOW);

      digitalWrite(8,  LOW);
      digitalWrite(9,  HIGH);

      digitalWrite(10, HIGH);
      digitalWrite(11, LOW);

      digitalWrite(12, LOW);
      digitalWrite(13, HIGH);


    } else if (data == "l") {   //Right

      digitalWrite(6,  LOW);
      digitalWrite(7,  HIGH);

      digitalWrite(8,  HIGH);
      digitalWrite(9,  LOW);

      digitalWrite(10, LOW);
      digitalWrite(11, HIGH);

      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);


    } else if (data == "s") {   //Stop

      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);

      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);

    } else if (data == "o") {   //Open Pneumatic Suspension

      digitalWrite(2, HIGH);
      delay(200);
      digitalWrite(2, LOW);


    } else if (data == "c") {   //Close Pneumatic Suspension

      digitalWrite(3, HIGH);
      delay(200);
      digitalWrite(3, LOW);

    } else {   //invalid string recived

      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);

      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);

    }
  }
}
