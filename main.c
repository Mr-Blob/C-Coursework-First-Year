#include <stdio.h>
#include <stdbool.h>

/* If
 *
 */
void summingAmplifier(int vMinus, int vPlus);

void invertingAmplifier(int vMinus, int vPlus);

void nonInvertingAmplifier(int vMinus, int vPlus);

void printQuestions(float variable[], char *question[], int size, bool require_positive);

void print_vOut(float vOut, int vPlus, int vMinus);

void clear_buffer(void) {
    // This function clears whatever# is left from the input.
    int clear;
    do {
        clear = getchar();
        // getchar() reads a character sequentially from the input buffer, but it also removes the character from the input buffer at the same time.
    } while (clear != EOF && clear != '\n');
    // This while loop will repeat until the last thing in the buffer is a new line, which is given once the user presses enter to submit an input.
}


int check_real_int(char *question) {
    // This function checks if the value input is actually an integer, this prevents words being input, for example.
    int result;
    printf("%s", question);

    while (scanf("%d", &result) != 1) {
        // Scanf will check try and read the input as a decimal, if it reads correctly it will output a 1. If it can't cast it to a decimal it will return a 0.
        clear_buffer();
        printf("Input was not an integer. Try again.\n");
        printf("%s", question);
    }
    return result;
}

float int_range(char *question, float min, float max) {
    float result = check_real_int(question);

    while (result < min || result > max) {
        printf("Input must be between %.0f and %.0f. Try again.\n", min, max);
        result = check_real_int(question);
    }
    return result;
}

int main(void) {
    bool repeat;
    int amp_op = 0;
    do {
        char repeat_q;
        if(amp_op == 0) {
            amp_op = int_range("What type of amp would you like to calculate?\n"
                               "1. Summing Amplifier\n"
                               "2. Inverting Amplifier\n"
                               "3. Non-Inverting Amplifier\n", 1, 3);

            printf("Option: %i selected.\n\n", amp_op);
        }

        float vPlus = int_range("What value is needed for the positive voltage rail of the op amp?\n", 0, 600);
        // This limits the positive rail of the amplifier to 600V max.

        float vMinus = int_range("What value is needed for the negative voltage rail of the op amp?\n", -600, 0);

        if (amp_op == 1) {
            summingAmplifier(vMinus, vPlus);
        } else if (amp_op == 2) {
            invertingAmplifier(vMinus, vPlus);
        } else if (amp_op == 3) {
            nonInvertingAmplifier(vMinus, vPlus);
        }

        clear_buffer();
        printf("\nDo you want to reset the values and try again? (Y/N)\n");
        scanf("%c", &repeat_q);

        if (repeat_q == 'Y' || repeat_q == 'y') {
            repeat = true;
        } else if (repeat_q == 'N' || repeat_q == 'n') {
            repeat = false;
        } else {
            printf("This is not a valid input. Use Y or N.\n");
        }
    } while (repeat == true);


    return 0;
}

void summingAmplifier(int vMinus, int vPlus) {
    float resistors[3];
    float voltages[2];
    float vOut;

    char *resistor_questions[] = {
            "What is the value for R1?\n",
            "What is the value for R2?\n",
            "What is the value for Rf?\n"
    };
    char *voltage_questions[] = {
            "What is the value for V1?\n",
            "What is the value for V2?\n"
    };

    printQuestions(resistors, resistor_questions, 3, true);
    printQuestions(voltages, voltage_questions, 2, false);

    vOut = (-resistors[2] *
            ((voltages[0] / resistors[0]) + (voltages[1] / resistors[1])));

    print_vOut(vOut, vPlus, vMinus);
}

void invertingAmplifier(int vMinus, int vPlus) {

}

void nonInvertingAmplifier(int vMinus, int vPlus) {
    /*float resistors[3];
    float voltages[2];
    float vOut;

    char resistor_questions[3][30] = {
            "What is the value for R1?\n",
            "What is the value for R2?\n",
            "What is the value for Rf?\n"
    };
    char voltage_questions[2][30] = {
            "What is the value for V1?\n",
            "What is the value for V2?\n"
    };

    printQuestions(resistors, resistor_questions, 3, 1);
    printQuestions(voltages, voltage_questions, 2, 0);

    vOut = (-resistors[2] *
            ((voltages[0] / resistors[0]) + (voltages[1] / resistors[1])));

    print_vOut(vOut, vPlus, vMinus);*/
}

void printQuestions(float variable[], char *question[], int size, bool require_positive) {
    for (int i = 0; i < size; i++) {
        float value = 0;
        if (require_positive == true) {
            while (value <= 0) {
                value = check_real_int(question[i]);
                if (value <= 0) {
                    printf("The value entered is not a positive value, please try again.\n");
                }
            }
        } else if (require_positive == false) {
            value = check_real_int(question[i]);
        }
        variable[i] = value;

    }
}

void print_vOut(float vOut, int vPlus, int vMinus) {
    vPlus = 12;
    vMinus = -12;

    if (vOut > vPlus) {
        vOut = vPlus;
        printf("The voltage is more than the positive rail! The voltage rail that has been exceeded is: %iV\n",
               vPlus);
    } else if (vOut < vMinus) {
        vOut = vMinus;
        printf("The voltage is more than the negative rail! The voltage rail that has been exceeded is: %iV\n",
               vMinus);
    }

    fflush(stdout);
    printf("The voltage output of the  amplifier is: %.2fV\n", vOut);
}
