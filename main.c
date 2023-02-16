

/*
 * ----------------------------------------------------------------------------------------------------------------- *
 *
 * This code calculates the vOut and gain values for summing, inverting and non-inverting operation amplifier.
 * It allows sets of inputs to be used, allowing multiple different configurations of the same amplifier to be tested.
 * All inputs are value range checked (i.e. between 1 and 3 for example), format checked (i.e. Y or N only),
 * data type checked (i.e. values input must be integers and cannot contain characters).
 * The code is written in such a way to be modular and allow future edits to be fairly easy.
 *
 * ----------------------------------------------------------------------------------------------------------------- *
 */

/*
 * ----------------------------------- Optimisations that could be made: ------------------------------------------- *
 *
 * print_question function could dynamically get the length of the arrays used.
 * All the code for each amplifier types could be simplified into a few functions to share between them such as qsort
 * and outputting the vOut and gain. That should be in its own function since it's the same between the amplifier
 * types.
 *
 * ----------------------------------------------------------------------------------------------------------------- *
 */

// Library imports
#include <stdio.h>
#include <stdbool.h> // This library is to add support for boolean data types.
#include <stdlib.h>

// These are the declarations of the functions used in the program.
void summing_amplifier(int vMinus, int vPlus, int num_inputs);

        /* This function calculates output voltages and gain for a summing amplifier.
         * Equation: vOut = -Rf((V1/R1)+(V2/R2))
         *
         * - vMinus/vPlus: This is used to set the maximum and minimum voltage of the power rails for the OP Amp.
         * - num_inputs: This tells the function how many sets of inputs is required by the user.
         *
         */

void inverting_amplifier(int vMinus, int vPlus, int num_inputs);

void non_inverting_amplifier(int vMinus, int vPlus, int num_inputs);

void print_questions(float variables_in[], char *question[], int size, bool require_positive);

        /* This print_questions function takes the resistors/voltage member of "set[i]"(corresponding to which set of values is being used to calculate vOut):
         *
         * - variables_in: This is where it stores the values of the resistors/voltages, for that specific set.
         * - question: This uses the 3D question arrays defined above to ask the users questions on which value they
         *      want to store.
         * - size: This is used to tell the for loop inside the function how long the question and variable array is.
         *      This could be optimised by using the total length of the memory used, and dividing that by the length of one
         *      item in the array.
         * - require_positive: This is used to tell the function if it needs to limit the values the use can enter to
         *      be > 0 i.e. positive.
         */

void print_vOut(float input_sets, int vPlus, int vMinus);

        /* This print_questions function takes the resistors/voltage member of "set[i]"(corresponding to which set of values is being used to calculate vOut):
         *
         * - variables_in: This is where it stores the values of the resistors/voltages, for that specific set.
         * - question: This uses the 3D question arrays defined above to ask the users questions on which value they
         *      want to store.
         * - size: This is used to tell the for loop inside the function how long the question and variable array is.
         *      This could be optimised by using the total length of the memory used, and dividing that by the length of one
         *      item in the array.
         * - require_positive: This is used to tell the function if it needs to limit the values the use can enter to
         *      be > 0 i.e. positive.
         */

void gain(float *resistors, int length, bool is_invert);

        /* This calculates the gain using the resistor values from the set array.
         *
         * - resistors: This is used to pass the values of the resistors that need to be calculated.
         * - length: This defines how many gain calculations/outputs there needs to be. This is because the summing amp,
         *      for example, has 2 gain calculations.
         * - is_invert: This states whether the amplifier configuration is inverting.
         */

void clear_buffer(void);

        /* I had problems with scanf continuously reading the input buffer when used in a loop even when using fflush
         * to clear the input buffer.
         * fflush(stdin) has undefined behaviour which, I think, is why it was not clearing the whole buffer.
         * I ended up using getchar() to clear the input buffer until all that was left was a new line.
         */

int check_real_int(char *question);

        /* This function takes a 3D array as an input, prints each question, takes an input for the question, and
         * returns an integer. This function is used to require the user to input a real integer.
         *
         * - question: Takes an array of questions to be printed in the function.
         */

float int_range(char *question, float min, float max);

        /* This function uses the check_real_int function to check if a int is real, and is within the range specified
         * by min and max.
         *
         * - question: Takes an array of questions to be printed in the function. This gets passed straight onto the
         *      check_real_int function.
         * - min: Takes the minimum value for the integer to be checked against.
         * - max: Takes the maximum value for the integer to be checked against.
         */

bool yes_no(char *question);

        /* This function only accepts an input of Y or N, and repeat if not. The repeat variable is used to determine if the program should run again.
         *
         * - question: Takes an array of questions to be printed in the function.
         *
         * returns boolean value: true for yes, false for no.
         */

int compare(const void *a, const void *b);

        /* This function is used by qsort. It compares two elements of an array. qsort passes these two elements to the function.
         *
         * - a: The a pointer is the first element of the array that is passed to it i.e. set[1].
         * - b: The b pointer is the second element of the array that is passed to it i.e. set[2].
         *
         * Returns the difference between the value of the vOut member of each set, which the qsort function uses to
         * determine where one should be moved in relation to the other.
         * Ref: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/qsort
         */


struct inputs {
    /* This creates a structure to store the values of a circuit. An array of these structures is used to store the values needed for each circuit:
     * - id: This is a simple way to reference which set of inputs are being shown after the array has been sorted.
     *      If this was not used, it would not be easy to identify which inputs are being shown after the input array has been
     *      sorted. The sort function changes the order of the array, so the normal index cannot be used to tell the user what
     *      is being used, but the index is instead used to print out the results in the correct order.
     * - resistors: This makes an array of floats to store the resistor values. Using malloc() the array could have been dynamically
     *      sized, but since the maximum amount of resistors used is 3, it works for this program.
     * - voltages: ^^
     */

    int id;
    float resistors[3];
    float voltages[2];
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


        // This below statement is used to decide which amplifier function will be run.

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
        // This function clears the input buffer.


        char value_repeat_question[] = "\nDo you want to reset the values and try again? (Y/N)\n";
        repeat = yes_no(value_repeat_question);
        /

        if (repeat) {
            clear_buffer();
            char amp_repeat_question[] = "\nDo you want to use the same amplifier again? (Y/N)\n";
            repeat_a = yes_no(amp_repeat_question);
            // The repeat_a variable is used to determined if the same amplifier should be used if when the program is run again.

            if (!repeat_a) {
                amp_op = 0;
                // If they don't want to use the same amplifier, the amplifier choice will be reset to zero, so when it's repeated it will ask the amplifier question again.
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
        // i is used in the loop to determine which set of inputs is being used.

        char *resistor_questions[] = {
                "What is the value for R1?\n",
                "What is the value for R2?\n",
                "What is the value for Rf?\n"
        };

        /* Both of these character arrays store the questions to be asked to the user. They are stored in 3D arrays.
         * They are 3D arrays because a question (first dimension), is really just a co- question: llection of characters
         * (second dimension), and then a group of questions is stored to make the third dimension.
         */

        char *voltage_questions[] = {
                "What is the value for V1?\n",
                "What is the value for V2?\n"
        };

        set[i].id = i; // Sets the ID of a structure so that the inputs can be found later after sorting has happened.

        if (num_inputs != 1) {
            printf("\nCircuit Number %i:\n", i + 1);
            // This tells the user which circuit values are being input, it doesn't print if the user asks for only one set of data to be calculated, since its pointless info.

        }

        print_questions(set[i].resistors, resistor_questions, 3, true);
        // Prints the questions for storing the resistor/voltage values. More info at the function declaration.

        print_questions(set[i].voltages, voltage_questions, 2, false);

        set[i].vOut = (-set[i].resistors[2] *
                       ((set[i].voltages[0] / set[i].resistors[0]) + (set[i].voltages[1] / set[i].resistors[1])));

        /* This is the equation to calculate vOut for a summing amplifier.
         * Equation: vOut = -Rf((V1/R1)+(V2/R2))
         * It takes the values from each member of the currently used set, i, and calculates vOut, and stores it in said
         * structure.
         */
    }

    qsort(set, num_inputs, sizeof(struct inputs), compare);

    /* qsort is a function inside the C Standard Library: stdlib.h, to sort an array:
     *
     * - base: What set of values should be used, in this case, ALL of the "set" structures, which contains all members.
     * - nmemb: The number of values in the array.
     * - size: Number of bytes that each "inputs" structure takes up in memory.
     * - compar: The function used to compare. In this program, that is the compare function. The compar field passes
     *      two values of the array to the specified function: compare.
     *
     * Ref: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/qsort
     */


    for (int f = 0; f < num_inputs; f++) {
        printf("\n\nOutput for circuit %i:\n", set[f].id + 1);
        // When outputting the vOut values at the end, the id member is used so that the user can identify which input values correspond to the output.

        print_vOut(set[f].vOut, vPlus, vMinus);
        // Prints vOut
        gain(set[f].resistors, 2, true);
        // Uses the gain function to output the gain of the circuit
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
        printf("\n\nOutput for circuit %i:\n", set[f].id + 1);
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
        printf("\n\nOutput for circuit %i:\n", set[f].id + 1);
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
        printf("\nThe voltage is more than the positive rail! The voltage rail that has been exceeded is: %iV\n",
               vPlus);
    } else if (input_sets < vMinus) {
        input_sets = vMinus;
        printf("\nThe voltage is more than the negative rail! The voltage rail that has been exceeded is: %iV\n",
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
        printf("Input must be between %.0f and %.0f. Try again.\n", min, max); // %.0f rounds a float to be zero decimal places.
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


// Ref: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/qsort
int compare(const void *a, const void *b) {
    struct inputs *left = (struct inputs *) a;
    // This defines the first element of the array as having the same structure as the "inputs" structure. This means members can be referenced.
    struct inputs *right = (struct inputs *) b;
    // This defines the second element of the array as having the same structure as the "inputs" structure.

    return left->vOut - right->vOut; // This returns the difference between the value of the vOut member of each set.
}