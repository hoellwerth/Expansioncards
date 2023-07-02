#include <Wire.h>
#include "tcpm_driver.h"
#include "usb_pd.h"

const int usb_pd_int_pin = D3;
// const int debug_led_pin  = D5;
// const int pushbutton_pin = D4;
// int pushbutton_last_state, pushbutton_current_state, pushbutton_last_time, pushbutton_current_time;
// int debug_led_current_state = 0;
int pd_source_cap_current_index = 0, pd_source_cap_max_index = 0;

// USB-C Specific - TCPM start 1
const struct tcpc_config_t tcpc_config[CONFIG_USB_PD_PORT_COUNT] = {
  {0, fusb302_I2C_SLAVE_ADDR, &fusb302_tcpm_drv},
};
// USB-C Specific - TCPM end 1

void setup() {
  pinMode(usb_pd_int_pin, INPUT);
  // pinMode(debug_led_pin, OUTPUT);
  // pinMode(pushbutton_pin, INPUT_PULLUP);
  // pushbutton_last_state = 1;
  // pushbutton_last_time = 0;
  digitalWrite(debug_led_pin, LOW);
  
  Wire.begin();
  Wire.setClock(400000);
  
  tcpm_init(0);
  delay(50);
  pd_init(0);
  delay(50);
}

void loop() {
  int reset = 0;
  pushbutton_current_state = digitalRead(pushbutton_pin);
  pushbutton_current_time = millis();
  if ((pushbutton_current_state == 0) && (pushbutton_last_state == 1) && ((pushbutton_current_time - pushbutton_last_time) < 100)) {
    /*if (debug_led_current_state) {
      digitalWrite(debug_led_pin, LOW);
      debug_led_current_state = 0;
    } else {
      digitalWrite(debug_led_pin, HIGH);
      debug_led_current_state = 1;
    }*/
    
    if (pd_source_cap_current_index < pd_source_cap_max_index) {
      pd_source_cap_current_index++;

      reset = 1;

      return;
    }
    
    pd_source_cap_current_index = 0;
    
    reset = 1;
    
    //pd[0].task_state = PD_STATE_SOFT_RESET);
  }

  // pushbutton_last_state = pushbutton_current_state;
  // pushbutton_last_time = pushbutton_current_time;

  
  if (LOW == digitalRead(usb_pd_int_pin)) {
      tcpc_alert(0);
  }
  
  pd_run_state_machine(0, reset);
  
  delay(4);
}

void pd_process_source_cap_callback(int port, int cnt, uint32_t *src_caps) {
  //digitalWrite(debug_led_pin, HIGH);
  pd_source_cap_max_index = cnt - 1;
}
