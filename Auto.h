#include <DRV8833.h>
extern DRV8833 motor; // motor b e a VESPA pinout


#define l_sen 33 // sensor esquerdo S4 VESPA pinout
#define r_sen 32 // sensor direito S3

// ESTADOS DO AUTONOMO //
enum states_auto {
  start,
  search,
  atc,
  l_atc,
  r_atc
};

enum states_auto state_at = start; // estado atual

// ESTADOS DA PROCURA //
enum states_search {
  no_search,
  search_forward,
  search_curve_right,
  search_curve_left,
};

enum states_search search_state = no_search; // estado atual

unsigned long t_antes = 0;
const unsigned long tempo_amostragem = 10;

// LEITURA DOS SENSORES //
bool l_dtc() {return !digitalRead(l_sen);}
bool r_dtc() {return !digitalRead(r_sen);}

// DETECÇAO DOS SENSORES //
enum states_auto furi_state() {
  enum states_auto state_at;

  if(l_dtc() && r_dtc()) {
    state_at = atc;
    search_state = no_search;
    state_at = l_atc;
    search_state = no_search;
  }else if(r_dtc()) {
    state_at = r_atc;
    search_state = no_search;
  }else {
    state_at = search;
    // if (state_at == search) {
    //   state_search = search_curve_right;
    // } etc...
  }
  return state_at;
}

// ESTRATEGIAS //

/*void search_japan(){

      motor.move(-1023, 1023);
      delay(200);
      motor.move(1023, 1023);
      delay(500);
      motor.move(1023, -1023);
      delay(200);
      motor.move(1023, 1023);
      delay(700);
      motor.move(1023, -1023);
      delay(200);
      motor.move(1023, 1023);
      delay(500);
      motor.move(1023, -1023);
      delay(200);
      motor.move(-800, 800);
}*/


/*void search_greatwallofchina(){

      motor.move(800, 0);
      delay(200);
      motor.move(-800, 0);
      delay(200);
      motor.move(0, 800);
      delay(200);
      motor.move(0, -800);
      delay(200);
      motor.move(800, 0);
      delay(200);
      motor.move(-800, 0);
      delay(200);
      motor.move(0, 800);
      delay(200);
      motor.move(0, -800);
      delay(200);
      motor.move(800, 0);
      delay(200);
      motor.move(-800, 0);
      delay(200);
      motor.move(0, 800);
      delay(200);
      motor.move(0, -800);
      delay(200);
}*/


void furi_japan() {
 
  switch(state_at){

    case start:
    motor.move(-700, 700);
      delay(600);

      motor.move(700, 700);
      delay(600);

      motor.move(700, -700);
      delay(600);

      motor.move(700, 700);
      delay(600);

      motor.move(700, -700);
      delay(600);

      motor.move(700, 700);
      delay(600);

      motor.move(700, 700);
      delay(600);

      motor.move(-700, 700);
      delay(600);
    break;

    case search:
    motor.move(-900, 900);
      break;

    case atc:
      motor.move(1023, 1023);
      break;

    case r_atc:
      motor.move(800, 1023);
      break;

    case l_atc:
      motor.move(1023, 800);
      break;
  }

  enum states_auto e_atual;
  unsigned long    t_atual;
  for (;;) {
    e_atual = furi_state();
    if (e_atual != state_at) {
      state_at = e_atual; return;
    }
    t_atual = millis();
    if (t_antes - t_atual > tempo_amostragem) {
      t_antes = t_atual;  return;
    }
  }
}


void furi_greatwallofchina() {

  switch(state_at){

    case start:
    break;
    
    case search:
    motor.move(800, 0);
    delay(200);
    motor.move(-800, 0);
    delay(200);
    motor.move(0, 800);
    delay(200);
    motor.move(0, -800);
    delay(200);
    motor.move(800, 0);
    delay(200);
    motor.move(-800, 0);
    delay(200);
    motor.move(0, 800);
    delay(200);
    motor.move(0, -800);
    delay(200);
    motor.move(800, 0);
    delay(200);
    motor.move(-800, 0);
    delay(200);
    motor.move(0, 800);
    delay(200);
    motor.move(0, -800);
    delay(200);
    break;

    case atc:
    motor.move(1023, 1023);
    break;

    case r_atc:
    motor.move(800, 1023);
    break;

    case l_atc:
    motor.move(1023, 800);
    break;
  }

  enum states_auto e_atual;
  unsigned long    t_atual;
  for (;;) {
    e_atual = furi_state();
    if (e_atual != state_at) {
      state_at = e_atual; return;
    }
    t_atual = millis();
    if (t_antes - t_atual > tempo_amostragem) {
      t_antes = t_atual;  return;
    }
  }
}


void furi_pat() {
  switch(state_at) {
    case start:

    case search:
      motor.move(-800, 800);
      break;

    case atc:
      motor.move(1023, 1023);
      break;

    case l_atc:
      motor.move(-800, 1023);
      break;

    case r_atc:
      motor.move(-1023, 800);
      break;
  }
  enum states_auto e_atual;
  unsigned long    t_atual;
  for (;;) {
    e_atual = furi_state();
    if (e_atual != state_at) {
      state_at = e_atual; return;
    }
    t_atual = millis();
    if (t_antes - t_atual > tempo_amostragem) {
      t_antes = t_atual;  return;
    }
  }
}


void furi_madmax() {
  motor.move(1023, 1023);
}


void furi_ghoul(){

    switch(state_at){

      case start:
     motor.move(700, 700);
      delay(30);
      break;
      
      case search:
      
      break;

      case atc:
      motor.move(1023, 1023);
      break;

      case r_atc:
      motor.move(800, 1023);
      break;

      case l_atc:
      motor.move(1023, 800);
      break;
    }

  enum states_auto e_atual;
  unsigned long    t_atual;
  for (;;) {
    e_atual = furi_state();
    if (e_atual != state_at) {
      state_at = e_atual; return;
    }
    t_atual = millis();
    if (t_antes - t_atual > tempo_amostragem) {
      t_antes = t_atual;  return;
    }
  }
}