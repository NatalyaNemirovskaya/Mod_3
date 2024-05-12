#include <Arduino.h>
#include <string>
#include <vector>

#define led_pin  38 // 38pin вывод на светодиод

int power = 255;

std::string buffer = "";
std::vector <int> vec_cmd(4) ; //вектор ждя хранения 4 команд

bool light_on = false; // признак включения светодиода
int index_vec; // индекс 
unsigned long time_begin;
unsigned long time_end;;
bool em_vec = false; //признак, что массив заполнен
const short max_number = 3; // количество ячеек


void setup() {
  // определяем 38 пин как выход
  pinMode(led_pin, OUTPUT);

  Serial.begin(115200);
  index_vec = 0; //перевая ячейка вектора
   time_begin = millis();//
}

void loop() {
  if (Serial.available() >0) {

    buffer = "";

    while(Serial.available()>0)
    {
      buffer +=std::string(1, char(Serial.read()));
      delay(500);
    }

    if(!em_vec ) //вектор еще пуст
     {
        vec_cmd[index_vec] = (stoi(buffer)); //заполняем
        index_vec++; 
       if(index_vec > max_number) // весь запполнен
       {
        em_vec = true;
        index_vec--; // индекс на последний элемент
       } 
     }
       else //вектор один раз полностью заполнен
     {
      // сдвигаем в массиве 3 ячейки
                   
          for (int i = 0; i<3; i++) 
             {
               vec_cmd[i] = vec_cmd[i+1] ; //сдвигаем данные
             }
              vec_cmd[index_vec] =std::stoi(buffer);
              // записываем в конец новое значение
      //
     }


    power = std::stoi(buffer);

    Serial.print( "Recived power =");
    Serial.println(power);

  }
  
  if (digitalRead(17) == HIGH)
  {
   
      light_on = true; //признак, что нужно включать
   
      
  }
  else
  {

  }

// если признак включения светодиода установлен
  if (light_on)
  {
    light_on = false; // сброс признака
    analogWrite(led_pin, power); // зажигаем светодиод
    delay(1000);
   
  }
  else
  {
    analogWrite(led_pin, 0); //светодиод не светится
  }

  time_end = millis(); //получаем текущее время
  if((time_end - time_begin) >= 20000) //если прошло 20 сек
  {
    //выводим все значения вектора
    for (int i = 0; i < vec_cmd.size(); i++) 
      {  
         Serial.println(vec_cmd[i]) ;
      }

     time_begin = millis(); //пкрезапускаем таймер
  }
  
}
