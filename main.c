#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void summing_amplifier(int vMinus, int vPlus, int num_inputs);

void inverting_amplifier(int vMinus, int vPlus, int num_inputs);

void non_inverting_amplifier(int vMinus, int vPlus, int num_inputs);

void print_questions(float variables_in[], char *question[], int size, bool require_positive);

void print_vOut(float input_sets, int vPlus, int vMinus);

void gain(float *resistors, int length, bool is_invert);

void clear_buffer(void);

int check_real_int(char *question);

float int_range(char *question, float min, float max);

bool yes_no(char *question);

int compare(const void *a, const void *b);


struct inputs {
    int id;
    float resistors[3];
    float voltages[2];
    float vOut;
};

int main(void) {
    bool repeat; // True for repeating the program again.
    bool repeat_a; // True for repeating the function with the same amplifier.
    int amp_op = 0; // Used to choose the amplifier type used.

    do {

        if (amp_op == 0) {
            amp_op = int_range("What type of amp would you like to calculate?\n"
                               "1. Summing Amplifier\n"
                               "2. Inverting Amplifier\n"
                               "3. Non-Inverting Amplifier\n", 1, 3);

            printf("Option: %i selected.\n\n", amp_op);
        }


        float vPlus = int_range("What value is needed for the positive voltage rail of the op amp?\n", 0, 600);
        // This limits the positive rail of the amplifier to 600V max.

        float vMinus = int_range("What value is needed for the negative voltage rail of the op amp?\n", -600, 0);

        float num_inputs = int_range("How many set of inputs do you want to test?\n", 0, 5);


        if (amp_op == 1) {
            summing_amplifier(vMinus, vPlus, num_inputs);
        } else if (amp_op == 2) {
            inverting_amplifier(vMinus, vPlus, num_inputs);
        } else if (amp_op == 3) {
            non_inverting_amplifier(vMinus, vPlus, num_inputs);
        }

        clear_buffer();
        char value_repeat_question[] = "\nDo you want to reset the values and try again? (Y/N)\n";
        repeat = yes_no(value_repeat_question);

        if (repeat) {
            clear_buffer();
            char amp_repeat_question[] = "\nDo you want to use the same amplifier again? (Y/N)\n";
            repeat_a = yes_no(amp_repeat_question);

            if (!repeat_a) {
                amp_op = 0;
            }
        }

    } while (repeat);

    return 0;
}


void summing_amplifier(int vMinus, int vPlus, int num_inputs) {
    struct inputs set[num_inputs];

    for (int i = 0; i < num_inputs; i++) {
        char *resistor_questions[] = {
                "What is the value for R1?\n",
                "What is the value for R2?\n",
                "What is the value for Rf?\n"
        };

        char *voltage_questions[] = {
                "What is the value for V1?\n",
                "What is the value for V2?\n"
        };

        set[i].id = i;

        if (num_inputs != 1){
            printf("\nCircuit Number %i:\n", i+1);
        }

        print_questions(set[i].resistors, resistor_questions, 3, true);
        print_questions(set[i].voltages, voltage_questions, 2, false);

        set[i].vOut = (-set[i].resistors[2] *
                       ((set[i].voltages[0] / set[i].resistors[0]) + (set[i].voltages[1] / set[i].resistors[1])));
    }

    qsort(set, num_inputs, sizeof(struct inputs), compare);

    for (int f = 0; f < num_inputs; f++) {
        printf("\nOutput for circuit %i:\n", set[f].id + 1);
        print_vOut(set[f].vOut, vPlus, vMinus);
        gain(set[f].resistors, 2, true);
    }
}


void inverting_amplifier(int vMinus, int vPlus, int num_inputs) {
    struct inputs set[num_inputs];
    for (int i = 0; i < num_inputs; i++) {
        char *resistor_questions[] = {
                "What is the value for R1?\n",
                "What is the value for Rf?\n"
        };

        char *voltage_questions[] = {
                "What is the value for Vin?\n"
        };

        set[i].id = i;

        if (num_inputs != 1){
            printf("\nCircuit Number %i:\n", i+1);
        }

        print_questions(set[i].resistors, resistor_questions, 2, true);
        print_questions(set[i].voltages, voltage_questions, 1, false);

        set[i].vOut = (-(set[i].resistors[1] / set[i].resistors[0]) * set[i].voltages[0]);
    }

    qsort(set, num_inputs, sizeof(struct inputs), compare);

    for (int f = 0; f < num_inputs; f++) {
        printf("\nOutput for circuit %i:\n", set[f].id + 1);
        print_vOut(set[f].vOut, vPlus, vMinus);
        gain(set[f].resistors, 1, true);
    }
}

void non_inverting_amplifier(int vMinus, int vPlus, int num_inputs) {
    struct inputs set[num_inputs];
    for (int i = 0; i < num_inputs; i++) {
        char *resistor_questions[] = {
                "What is the value for R1?\n",
                "What is the value for Rf?\n"
        };

        char *voltage_questions[] = {
                "What is the value for Vin?\n"
        };

        set[i].id = i;

        if (num_inputs != 1){
            printf("\nCircuit Number %i:\n", i+1);
        }

        print_questions(set[i].resistors, resistor_questions, 2, true);
        print_questions(set[i].voltages, voltage_questions, 1, false);

        set[i].vOut = (set[i].voltages[0] + (set[i].voltages[0] / set[i].resistors[0]) * set[i].resistors[1]);

    }

    qsort(set, num_inputs, sizeof(struct inputs), compare);

    for (int f = 0; f < num_inputs; f++) {
        printf("\nOutput for circuit %i:\n", set[f].id + 1);
        print_vOut(set[f].vOut, vPlus, vMinus);
        gain(set[f].resistors, 1, false);
    }

}

void print_questions(float variables[], char *question[], int size, bool require_positive) {
    printf("\n");
    for (int i = 0; i < size; i++) {
        float value = 0;
        if (require_positive) {
            while (value <= 0) {
                value = check_real_int(question[i]);
                if (value <= 0) {
                    printf("The value entered is not a positive value, please try again.\n");
                }
            }
        } else {
            value = check_real_int(question[i]);
        }
        variables[i] = value;
    }
}

void print_vOut(float input_sets, int vPlus, int vMinus) {
    if (input_sets > vPlus) {
        input_sets = vPlus;
        printf("\nThe voltage is more than the positive rail! The voltage rail that has been exceeded is: %iV\n\n",
               vPlus);
    } else if (input_sets < vMinus) {
        input_sets = vMinus;
        printf("\nThe voltage is more than the negative rail! The voltage rail that has been exceeded is: %iV\n\n",
               vMinus);
    }

    printf("The voltage output of the  amplifier is: %.2fV\n", input_sets);

}

void gain(float *resistors, int length, bool is_invert) {
    float gain;
    for (int i = 0; i < length; i++) {
        if (is_invert) {
            gain = (-(resistors[length] / resistors[i]));
        } else {
            gain = (1 + (resistors[1] / resistors[0]));
        }
        printf("The gain for input %i is: %.2f.\n", i + 1, gain);
    }
}

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

bool yes_no(char *question) {
    char repeat_amp;
    while (true) {
        printf("%s", question);
        scanf("%c", &repeat_amp);

        if (repeat_amp == 'Y' || repeat_amp == 'y') {
            return true;

        } else if (repeat_amp == 'N' || repeat_amp == 'n') {
            return false;

        } else {
            printf("This is not a valid input. Use Y or N.\n");

        }
    }
}

int compare(const void *a, const void *b) {
    struct inputs *left = (struct inputs *) a;
    struct inputs *right = (struct inputs *) b;

    return right->vOut - left->vOut;
}