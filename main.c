#include <stdio.h>
#include <string.h>

void summingAmplifier(int vMinus, int vPlus);

void invertingAmplifier(int vMinus, int vPlus);

void nonInvertingAmplifier(int vMinus, int vPlus);

void printQuestions(float *variable, char question[][30], int size, int positive);

void print_vOut(float vOut, int vPlus, int vMinus);

int amp_question(char *question) {
    int result;
    printf("%s: ", question);
    while (scanf("%d", &result) != 1) {
        printf("Input was not an integer. Try again.\n");
        printf("%s: ", question);
    }
    return result;
}

/*int read_int_range(char *prompt, int min, int max) {
    int result = read_int(prompt);

    while (result < min || result > max) {
        printf("Input must be between %d and %d. Try again.\n", min, max);
        result = read_int(prompt);
    }
    return result;
}
*/

int is_positive(float input) {
    if (input >= 0) {
        return 1;
    }
    if (input < 0) {
        printf("The value entered is not a positive value, please try again.\n");
        return 0;
    }
}

int main(void) {
    int vPlus = 0;
    int vMinus = 0;

    int amp_op = amp_question("What type of amp would you like to calculate?\n"
                              "1. Summing Amplifier\n"
                              "2. Inverting Amplifier\n"
                              "3. Non-Inverting Amplifier\n");


    printf("Option: %c selected.\n", amp_op);
    printf("\n");


    printf("Option: %i selected.\n", amp_op);
    printf("\n");

    printf("What value is needed for the positive voltage rail of the op amp?\n");
    scanf("%d", &vPlus);

    printf("What value is needed for the positive voltage rail of the op amp?\n");
    scanf("%i", &vMinus);

    if (amp_op == 1) {
        summingAmplifier(vMinus, vPlus);
    } else if (amp_op == 2) {
        invertingAmplifier(vMinus, vPlus);
    } else if (amp_op == 3) {
        nonInvertingAmplifier(vMinus, vPlus);
    }

}

void summingAmplifier(int vMinus, int vPlus) {
    float resistors[3];
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

void printQuestions(float *variable, char question[][30], int size, int positive) {
    for (int i = 0; i < size; i++) {
        float value = 0;
        if (positive == 1) {
            while (is_positive(value) == 0) {
                fflush(stdin);
                printf("%s", question[i]);
                scanf("%f", &value);
            }
        } else if (positive == 0) {
            fflush(stdin);
            printf("%s", question[i]);
            scanf("%f", &value);
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
