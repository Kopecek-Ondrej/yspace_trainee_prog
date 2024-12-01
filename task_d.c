#include <stdio.h>
#include <stdlib.h>
#include <time.h>   //for clock()...determine processor time
#include <unistd.h> //for usleep()...to suspend execution of the calling

#define DEBOUNCE_DELAY 50 //after 50 milliseconds the button is considered stable


enum ButtonState{       //enum to list button states
    STATE_WAIT_FOR_PRESS,
    STATE_DEBOUNCE,
    STATE_PRESSED,
};

//clock() measures cpu-time 
unsigned long millis()
{  
    unsigned long time;
    time = (clock() * CLOCKS_PER_SEC / 1000);// / CLOCKS_PER_SEC;
    return time;
}

int read_button()
{
    static int button_press = 0;    //static variable is stored in data segment, not on stack like local variables
    //it holds its value across other function calls; its lifetime is program execution.
    //if uninitialized ...initialized to zero 
    button_press = !button_press;   //this toggles the pin
    return button_press;
}



void button_state_machine()
{
    static unsigned long start_time = 0;
    static int current_state = STATE_WAIT_FOR_PRESS;

    int button_input = read_button();   //takes value of the toggled button

    switch(current_state)
    {
        case STATE_WAIT_FOR_PRESS:
        {
            if(button_input == 1)   //button has been pressed
            {
                start_time = millis();  //start measuring if its not a mechanical schum (disturbance)
                current_state = STATE_DEBOUNCE;  
                fprintf(stdout, "Button Press Detected\n");
            }
            break;

        }
        case STATE_DEBOUNCE:
        {
            unsigned long current_time = millis();
            if((current_time - start_time )>= DEBOUNCE_DELAY)   // ensure the button state is stable
            {
                if(button_input == 1)
                {
                    current_state = STATE_PRESSED;
                }
                else
                {
                    current_state = STATE_WAIT_FOR_PRESS;
                }
            }
            break;
        }
        case STATE_PRESSED:
        {
            if(button_input == 0)   //button has been released
            {
                current_state = STATE_WAIT_FOR_PRESS;
                fprintf(stdout,"Button Released\n");
            }
            break;
        }
        //default state would be to wait for press, but since all values of current_state are discrete, default value is irrelevant
    }      
}

int main()
{

    for(int i = 0; i < 10; i++)
    {
        button_state_machine();
        usleep(50000);
    }
    return 0;
}