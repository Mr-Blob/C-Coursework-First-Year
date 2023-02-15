#include <stdio.h>
#include <stdbool.h> // This library is to add support for boolean data types.
#include <stdlib.h>


/*
 * ----------------------------------------------------------------------------------------------------------------- *
 *
 * This code calculates the vOut and gain values for summing, inverting and non-inverting operation amplifier.
 * It allows sets of inputs to be used, allowing multiple different configurations of the same amplifier to be tested.
 * All inputs are value range checked (i.e. between 1 and 3 for example), format checked (i.e. Y or N only),
 * data type checked (i.e. values input must be integers and cannot contain characters).
 *
 * ----------------------------------------------------------------------------------------------------------------- *
 */

// These are the declarations of the functions used in the program.

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
    // This creates a structure to store the values of a circuit. An array of these structures is used to store the values needed for each circuit.

    int id;
    /* This is a simple way to reference which set of inputs are being shown after the array has been sorted.
     * If this was not used, it would not be easy to identify which inputs are being shown after the input array has been
     * sorted. The sort function changes the order of the array, so the normal index cannot be used to tell the user what
     * is being used, but the index is instead used to print out the results in the correct order.
     */

    float resistors[3];
    /* This makes an array of floats to store the resistor values. Using malloc() the array could have been dynamically
     * sized, but since the maximum amount of resistors used is 3, it works for this program.
     */

    float voltages[2];  // This makes an array of floats to store the voltage values.
    float vOut;
};



int main(void) {
    bool repeat; // True for repeating the program again.
    bool repeat_a; // True for repeating the function with the same amplifier.
    int amp_op = 0; // Used to store which amplifier type used.

    do {
        /* Using a do while here is preferable because it allows the program to run fully once before asking if
         * it should be repeated.
         */
        if (amp_op == 0) {
            amp_op = int_range("What type of amp would you like to calculate?\n"
                               "1. Summing Amplifier\n"
                               "2. Inverting Amplifier\n"
                               "3. Non-Inverting Amplifier\n", 1, 3);
            /* This int_range function takes a string input, which is then printed when the function is run.
             * It also takes the min and max values of what should be acceptable inputs. If the value input is over
             * 3 or under 1, in this example, it will state that it's out of the range, and ask again.
             */

            printf("Option: %i selected.\n\n", amp_op);
        }


        float vPlus = int_range("What value is needed for the positive voltage rail of the op amp?\n", 0, 600);
        // This limits the negative rail to be between 0V minimum and 600V maximum.

        float vMinus = int_range("What value is needed for the negative voltage rail of the op amp?\n", -600, 0);
        // This limits the negative rail to be between -600V minimum and 0V maximum.

        float num_inputs = int_range("How many set of inputs do you want to test?\n", 0, 5);


        // This statement is used to decide which amplifier function will be run.
        /* All the amplifiers have the max and min voltage rails, along with the number of circuits to be simulated as
         * variables.
         */

        if (amp_op == 1) {
            summing_amplifier(vMinus, vPlus, num_inputs);

        } else if (amp_op == 2) {
            inverting_amplifier(vMinus, vPlus, num_inputs);

        } else if (amp_op == 3) {
            non_inverting_amplifier(vMinus, vPlus, num_inputs);

        }

        clear_buffer();
        /* This function clears the input buffer. I had problems with scanf continuously reading the input buffer when
         * used in a loop even when using fflush to clear the input buffer.
         * fflush(stdin) has undefined behaviour which, I think, is why it was not clearing the whole buffer.
         * I ended up using getchar() to clear the input buffer until all that was left was a new line.
         */

        char value_repeat_question[] = "\nDo you want to reset the values and try again? (Y/N)\n";
        repeat = yes_no(value_repeat_question);
        // This function only accepts an input of Y or N, and repeat if not. The repeat variable is used to determine if the program should run again.

        if (repeat) {
            clear_buffer();
            char amp_repeat_question[] = "\nDo you want to use the same amplifier again? (Y/N)\n";
            repeat_a = yes_no(amp_repeat_question);
            // The repeat_a variable is used to determined if the same amplifier should be used if when the program is run again.

            if (!repeat_a) {
                amp_op = 0;
                // If they don't want to use the same amplifier, the amplifier choice
            }
        }

    } while (repeat);

    return 0; // This can be used to tell the CLI that's running the program, that the program has been completed successfully.
}


void summing_amplifier(int vMinus, int vPlus, int num_inputs) {
    struct inputs set[num_inputs];
    /* This defines an array that contains multiple "inputs" structures. num_inputs is used to define how many sets of
     * inputs will be use (i.e. how many circuit calculations to do).
     */

    for (int i = 0; i < num_inputs; i++) {
        // This for loop repeats until the correct amount of sets of inputs is reached which is set by "num_inputs".

        char *resistor_questions[] = {
                "What is the value for R1?\n",
                "What is the value for R2?\n",
                "What is the value for Rf?\n"
        };

        /* Both of these character arrays store the questions to be asked to the user. They are stored in 3D arrays.
         * They are 3D arrays because a question (first dimension), is really just a collection of characters
         * (second dimension), and then a group of questions is stored to make the third dimension.
         */

        char *voltage_questions[] = {
                "What is the value for V1?\n",
                "What is the value for V2?\n"
        };

        set[i].id = i; // Sets the ID of a structure so that the inputs can be found later after sorting has happened.

        if (num_inputs != 1) {
            printf("\nCircuit Number %i:\n", i + 1);
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

        if (num_inputs != 1) {
            printf("\nCircuit Number %i:\n", i + 1);
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

        if (num_inputs != 1) {
            printf("\nCircuit Number %i:\n", i + 1);
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
    // This function clears whatever is left from the input.
    fflush(stdin);
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

    while (scanf("%i", &result) != 1) {
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

// Ref: https://stackoverflow.com/questions/13372688/sorting-members-of-structure-array
int compare(const void *a, const void *b) {
    struct inputs *left = (struct inputs *) a;
    struct inputs *right = (struct inputs *) b;

    return left->vOut- right->vOut;
}