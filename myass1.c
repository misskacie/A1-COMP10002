/* Program to perform multi-digit integer arithmetic.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2023, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2023

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Kacie Beckett 1452651
   Dated:     9/9/2023

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>

/* All necessary #defines provided as part of the initial skeleton */
#define DEBUG 0
#define INTSIZE	500	/* max number of digits per integer value */
#define LINELEN	999	/* maximum length of any input line */
#define NVARS	26	/* number of different variables */

#define CH_A     'a'    /* character 'a', first variable name */
#define ERROR	(-1)	/* error return value from some functions */
#define PROMPT	"> "	/* the prompt string for interactive input */

#define PRINT	'?'	/* the print operator */
#define ASSIGN	'='	/* the assignment operator */
#define PLUS	'+'	/* the addition operator */
#define MULT	'*'	/* the multiplication operator */
#define POWR	'^'	/* the power-of operator */
#define DIVS	'/'	/* the division operator */
#define ALLOPS  "?=+*^/"

#define CH_ZERO  '0'    /* character zero */
#define CH_ONE   '1'    /* character one */
#define CH_NINE  '9'    /* character nine */
#define CH_COM   ','    /* character ',' */
#define PUT_COMMAS 3    /* interval between commas in output values */

#define INT_ZERO 0	/* integer 0 */
#define INT_TEN  10	/* integer 10 */
#define LONG_ZERO {.num = {0}, .length = 1};
#define LONG_ONE  {.num = {1}, .length = 1};

/* largest exponent that can arise within 500 digit numbers is 2^1660 */
#define MAX_EXPONENT 1660 

/* status return values for is_greater_than function */
#define VARSEQUAL 0 
#define VAR1GREATER 1
#define VAR2GREATER 2

/* define the structure for longint_t */
typedef struct{
	int length;
	int num[INTSIZE];
} longint_t;

/****************************************************************/

/* A "magic" additional function needing explicit declaration */
int fileno(FILE *);

// Function prototypes 
int  read_line(char *line, int maxlen);
void process_line(longint_t vars[], char *line);
int  get_second_value(longint_t vars[], char *rhsarg, 
		longint_t *second_value);
void zero_vars(longint_t vars[], int numvars);
int parse_num(char rhs);
int  to_varnum(char ident);
void do_assign(longint_t *var1, longint_t *var2);
void reverse_longint(longint_t *var);
void remove_leading_zeros(longint_t *var);
void print_prompt(void);
void print_tadaa();
void print_error(char *message);
void do_print(int varnum, longint_t *var);
void debug_print(char message[], longint_t *var);
void error_and_exit(char message[]);
void do_plus(longint_t *var1, longint_t *var2);
void do_multiplication(longint_t *var1, longint_t *var2);
void do_exponentiation(longint_t *var1, longint_t *var2);
int is_greater_than(longint_t *var1, longint_t *var2);
void do_subtraction(longint_t *var1, longint_t *var2);
void shiftup(longint_t *var1);
void pop(longint_t *var1);
void do_division(longint_t *dividend, longint_t *divisor);
/****************************************************************/

/* Main program controls all the action
*/
int
main(int argc, char *argv[]) {
	char line[LINELEN+1] = {0};
	longint_t vars[NVARS];
	zero_vars(vars,NVARS);

	print_prompt();
	while (read_line(line, LINELEN)) {
		if (strlen(line) > 0) {
			/* non empty line, so process it */
			process_line(vars, line);
		}
		print_prompt();
	}

	print_tadaa();
	return 0;
}

/****************************************************************/

/* Reads a line of input into the array passed as argument,
   returns false if there is no input available.
   All whitespace characters are removed on the way through.
*/
int
read_line(char *line, int maxlen) {
	int i=0, c;
	while (((c=getchar())!=EOF) && (c!='\n')) {
		if (i<maxlen && !isspace(c)) {
			line[i++] = c;
		}
	}
	line[i] = '\0';
	/* then, if the input is coming from a file or the output
	   is going to a file, it is helpful to echo the input line
	   and record what the command was */
	if (!isatty(fileno(stdin)) || !isatty(fileno(stdout))) {
		printf("%s%s\n", PROMPT, line);
	}
	return ((i>0) || (c!=EOF));
}


/* Process a command by parsing the input line into parts
*/
void
process_line(longint_t vars[], char *line) {
	int varnum, optype, status;
	longint_t second_value;
	/* determine the LHS variable, it
	   must be first character in compacted line
	*/
	varnum = to_varnum(line[0]);
	if (varnum==ERROR) {
		print_error("invalid LHS variable");
		return;
	}
	/* more testing for validity 
	*/
	if (strlen(line)<2) {
		print_error("no operator supplied");
		return;
	}
	/* determine the operation to be performed, it
	   must be second character of compacted line
	*/
	optype = line[1];
	if (strchr(ALLOPS, optype) == NULL) {
		print_error("unknown operator\n");
		return;
	}
	/* determine the RHS argument (if one is required),
	   it must start in the third character of compacted line
	*/
	if (optype != PRINT) {
		if (strlen(line)<3) {
			print_error("no RHS supplied");
			return;
		}
		status = get_second_value(vars, line+2, &second_value);
		
		if (status==ERROR) {
			print_error("RHS argument is invalid");
			return;
		}
		if (second_value.length > INTSIZE){
			print_error("RHS number is greater than maximum intsize");
			return;
		}
	}
	/* finally, do the actual operation
	*/
	if (optype == PRINT) {
		do_print(varnum, &vars[varnum]);
	} else if (optype == ASSIGN) {
		do_assign(&vars[varnum], &second_value);
	} else if (optype == PLUS) {
		do_plus(&vars[varnum], &second_value);
	} else if (optype == MULT){
		do_multiplication(&vars[varnum], &second_value);
	} else if (optype == POWR){
		do_exponentiation(&vars[varnum], &second_value);
	} else if (optype == DIVS){
		do_division(&vars[varnum], &second_value);
	} else {
		print_error("operation not available yet");
		return;
	}
	return;
}

/* Process the input line to extract the RHS argument, which
   should start at the pointer that is passed
*/
int
get_second_value(longint_t vars[], char *rhsarg,
			longint_t *second_value) {
	char *p;
	int varnum2;
	if (isdigit(*rhsarg)) {
		/* first character is a digit, so RHS is a number
		   now check the rest of RHS for validity */

		int i = 0, j = 0, flag = 0;
		for (p=rhsarg; *p; p++) {
			if (!isdigit(*p)) {
				/* nope, found an illegal character */
				return ERROR;
			} 
			second_value->num[j] = parse_num(*(rhsarg+i));
			i++;
			/* remove leading zeros from the parsed integer
			   without using the remove_leading_zeros function as input could 
               give numbers > INTSIZE with leading zeros.
			   */
			if (flag == 1 || second_value->num[j] != 0){
                /* incremement the j value which is length of the parsed number
                   and set the flag so future values are part of the array
                */
				j++; 
				flag = 1;
			}
		}
		/* if second_value is only zeros make the value of second_value 0*/
		if (j == 0){
			j++;
		}
		second_value->length = j;
		/* store in reverse order to make operations easier */
		reverse_longint(second_value); 
		return !ERROR;
	} else {
		/* argument is not a number, so should be a variable */
		varnum2 = to_varnum(*rhsarg);
		if (varnum2==ERROR || strlen(rhsarg)!=1) {
			/* nope, not a variable either */
			return ERROR;
		}
		/* and finally, get that variable's value */
		do_assign(second_value, &vars[varnum2]);
		return !ERROR;
	}
	return ERROR;
}

/* Set an array of long int of length numvars to have the array
	inside each struct have all indexes equal to zero
*/
void
zero_vars(longint_t vars[], int numvars) {
	// traverse to each longint_t in array
	for (int i=0; i< numvars; i++) {
		vars[i].length = 1;
		// traverse to each element in the array in longint_t
		for(int j=0; j<INTSIZE;j++){
			vars[i].num[j] = 0;
		}
	}
}

/* Convert the character representation of rhs to an int
*/
int
parse_num(char rhs) {
	return (rhs - CH_ZERO);
}

/* Convert a character variable identifier to a variable number
*/
int
to_varnum(char ident) {
	int varnum;
	varnum = ident - CH_A;
	if (0<=varnum && varnum<NVARS) {
		return varnum;
	} else {
		return ERROR;
	}
}

/* Assign a longint value to another by copying only the elements
   currently in use
*/
void
do_assign(longint_t *var1, longint_t *var2) {
	var1->length = var2->length;
	for (int i = 0; i < var2->length; i++){
		var1->num[i]= var2->num[i];
	}
}

/* Reverse the integer array inside longint_t
*/
void 
reverse_longint(longint_t *var) {
	longint_t tmp;
	tmp = *var;
	int i = 0;
	for (int j = var->length - 1; j >= 0; j--){
		var->num[i] = tmp.num[j];
		i++;
	}
}

/* Some operations will generate leading zeroes in a longint 
   array so remove them by decreasing the length of the number
*/
void
remove_leading_zeros(longint_t *var) {
	while(var->length > 1 && var->num[var->length - 1] == 0){
        var->length -=1;
    }
}

/****************************************************************/

/* Prints the prompt indicating ready for input, but only if it
   can be confirmed that the input is coming from a terminal.
   Plus, output might be going to a file, that's why the prompt,
   if required, is written to stderr and not stdout
*/
void
print_prompt(void) {
	if (isatty(fileno(stdin))) {
		fprintf(stderr, "> ");
		fflush(stderr);
	}
}

/* all done, so pack up bat and ball and head home,
   getting the exact final lines right is a bit tedious,
   because input might be coming from a file and output
   might be going to a file */
void
print_tadaa() {
	
	if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
		printf("\n");
	}
	printf("ta daa!!!\n");
	if (isatty(fileno(stdin)) && !isatty(fileno(stdout))) {
		fprintf(stderr, "\n");
	}
}

/* need to write an error message to the right place(s)
	*/
void
print_error(char *message) {
	
	if (isatty(fileno(stdin)) || isatty(fileno(stdout))) {
		fprintf(stderr, "%s\n", message);
		fflush(stderr);
	}
	if (!isatty(fileno(stdout))) {
		printf("%s\n", message);
	}
}

/* Print out a longint value with a comma every third value from the end
	eg. 123,456,789
*/
void
do_print(int varnum, longint_t *var) {
	printf("register %c: ", varnum+CH_A);
	// generate a reference value for when to place commas
	int ref = var->length % PUT_COMMAS; 
    ref = (ref == 0) ? PUT_COMMAS : 0; 

	// number is stored in reverse order so iterate from end of array
    for (int i = var->length - 1; i >= 0; i--){
		printf("%d", var->num[i]);
        if (i != 0 && (ref + i) % PUT_COMMAS == 0){
            printf("%c",CH_COM);
        }
	}
	printf("\n");
}

/* function used for debugging by printing out a message and each 
	element in a longint array in the forward order as opposed
	to do_print which reverses the direction.
*/
void
debug_print(char message[], longint_t *var) {
	printf("%s ", message);
    for (int i = 0; i < var->length; i++){
		printf("%d ", var->num[i]);
	}
	printf("\n");
}

/* prints an error message and then exits the program */
void
error_and_exit(char message[]) {
	print_error(message);
	exit(EXIT_FAILURE);
}

/****************************************************************/

/* add the two longint_t values using long addition method
*/
void
do_plus(longint_t *var1, longint_t *var2) {
    longint_t output;
	int max = var1->length > var2->length ? var1->length : var2->length;
	int i, out, carryover = 0;

	/* add individual indexes in the array and if it is larger than 10
	   carry over the remaining component to the next addition operation
	*/
    for(i = 0; i < max; i++){
		if (i < var1->length && i < var2->length){
			out = var1->num[i] + var2->num[i];
		} else if (i < var1->length && i >= var2->length){
			out = var1->num[i];
		} else{
			out = var2->num[i];
		}
        
		/* check for a carryover */
        if (carryover != 0){
            out += carryover;
            carryover = 0;
        }
		/* carryover needs to be added to the next place so take the first 
			   digit of 2 digit number*/
        if (out >= 10){
            out -=10;
            carryover = 1;
        }
		/* add the computed result to the result array*/
        output.num[i] = out;
		output.length = i + 1;
		
    }
	// check addition does not yeild a number that is too big
	if((var1->length == INTSIZE || var2->length == INTSIZE) && carryover != 0){
		error_and_exit("integer overflow, program terminated");
	}
	if(carryover != 0){
		output.num[i] = carryover;
    	output.length = i + 1;
	}

	do_assign(var1,&output);
}

/****************************************************************/

/* multiply the two longint_t values together using the long
	multiplication method.
*/
void
do_multiplication(longint_t *var1, longint_t *var2) {
	/* find the number of digits in the larger number and shorter number*/
    int max = var1->length > var2->length ? var1->length : var2->length;
    int min = var1->length < var2->length ? var1->length : var2->length;
    int i, j, out, carryover = 0;
    longint_t output[min];
	zero_vars(output, min);

	/* check whether var1 or var2 is 0 */
	if((var1->length == 1 && var1->num[0] == 0) || 
		(var2->length == 1 && var2->num[0] == 0)){
		var1->length = 1;
		var1->num[0] = 0;
		return;
	}
	/* The algorithm will generate an array of longint_t values based on the 
       number of digits in the shorter number (int min). The first digit 
       multiplied in the first array has no offset. Then each following digit 
       has an increasing offset to be the equivalent to multiplying by ten. 
	*/

	/* primatively check the operation will not make number > intsize */
	if (max + (min - 1) > INTSIZE){
		error_and_exit("integer overflow, program terminated");
	}

    for(i = 0; i < min; i++){
        for(j = 0; j < max; j++){
			/* Choose order to multiply in with longer number "ontop" */
			if (var1->length < var2->length){
				out = var1->num[i] * var2->num[j];	
			} else {
				out = var2->num[i] * var1->num[j];
			}

			/* add the carryover value to the next place and then reset it */
			if (carryover != 0){
				out += carryover;
				carryover = 0;
			}
			/* carryover needs to be added to the next place so take the first 
			   digit of 2 digit number*/
			if (out >= 10){
				carryover += (out - (out % 10)) / 10;
				out = out % 10;
			}

			//printf("out: %d\n", out);
			output[i].num[j+i] = out;	
			output[i].length = j + i + 1;
			if (j == max - 1 && carryover != 0){
				/* because the carryover is not 0 the final number will be 1 
                   bigger than INTSIZE so  there will an overflow 
				*/
				if ((i == min - 1) && (max + min - 1 == INTSIZE)){
					print_error("integer overflow, program terminated");
					exit(EXIT_FAILURE);
				}
				/* add the final carryover to the output as it won't overflow */
				output[i].num[j+i+1] = carryover;
				output[i].length += 1;
				carryover = 0;
			}	
		}
    }    
	/* add together each subarray generated as part of the multiplication steps 
    */
    for(i=1; i<min; i++){
        do_plus(&output[0], &output[i]);
    }

	do_assign(var1, &output[0]);
}

/****************************************************************/

/* perform exponentiation of the longint_t value using repeated
	multiplication method.
*/
void
do_exponentiation(longint_t *var1, longint_t *var2) {
	int num = 0;
	/* if the var1 is 0, set var1 to 0*/
	if((var1->num[0] == 0 && var1->length == 1)){
		var1->length = 1;
		var1->num[0] = 0;
		return;
	   }
	/* if the var1 is 1 or var 2 is 0, set var 1 to 1*/
	if((var1->num[0] == 1 && var1->length == 1) ||
	   (var2->num[0] == 0 && var2->length == 1)){
		var1->length = 1;
		var1->num[0] = 1;
		return;
	}

	/* largest possible exponent for 500 digits is 4 digits long */
	if (var2->length <= 4){
		//convert var2 to int as it is small enough
		for (int i = var2->length - 1; i >= 0; i--){
			num = num * 10 + var2->num[i];
		}
		if (num > MAX_EXPONENT){
			error_and_exit("integer overflow, program terminated");
		}
	} else {
		error_and_exit("integer overflow, program terminated");
	}
	/* copy var1 to var2 as var1 will be changed during multiplication*/
	do_assign(var2,var1);

	//printf("num: %d\n",num);
	for (int i = 1; i < num; i++){
		do_multiplication(var1,var2);
	}
}

/****************************************************************/

/* Return VAR1GREATER,VAR2GREATER or VARSEQUAL, depending on 
   the size of var1 relative to var2.
*/
int
is_greater_than(longint_t *var1, longint_t *var2) {
	/* quick check for differing lengths as it is faster */
	if (var1->length > var2->length){
		return VAR1GREATER;  
	} else if (var1->length < var2->length) {
		return VAR2GREATER;
	} else {
		/* check numbers at individual indexes until one is smaller 
	  	   and use that to determine which is greater */
		for (int i = var1->length - 1 ; i >= 0; i--){
			if (var1->num[i] == var2->num[i]){
				continue;
			} else if (var1->num[i] > var2->num[i]) {
				return VAR1GREATER;
			} else {
				return VAR2GREATER;
			}
		}
		/* every digit checked was equal, so vars are equal */
		return VARSEQUAL;
	}
}

/* performs subtraction on two longint_t values. Used for division.
*/
void
do_subtraction(longint_t *var1, longint_t *var2) {
	longint_t result = LONG_ZERO;
	int i, out, status = is_greater_than(var1,var2);

	/* error checking */
	if(status == VAR2GREATER){
		error_and_exit("operation leads to negative integer");
	} else if (status == VARSEQUAL){
		do_assign(var1,&result);
		return;
	}

    /* perform long subtraction*/
    for(i = 0; i < var1->length ; i++){
		/* perform borrowing if the number "ontop" is smaller than the 
		   number below 
		*/
        if (var1->num[i] < var2->num[i] && i < var2->length){
			var1->num[i] += 10;
			var1->num[i+1] -= 1;
		}
		/* only perform subtraction on the places where the
		   magnitude is the same
		*/
		if (i < var1->length && i < var2->length){
			out = var1->num[i] - var2->num[i];
		} else {
			out = var1->num[i];
		}
        //printf("i: %d\n",i);
        result.num[i] = out;
		result.length = i + 1;
    }
	remove_leading_zeros(&result);
	do_assign(var1,&result);
}

/* shifts each number in longint_t num array up by one index 
   leaving the number in index 0 untouched as when it is called
   this index is overwritten
*/ 
void shiftup(longint_t *var1) {
	for (int i = var1->length; i > 0 ; i--){
		var1->num[i]  = var1->num[i-1];
	}
	var1->length += 1;
}

/* used to remove the end element of longint_t num array 
*/
void pop(longint_t *var1) {
	if (var1->length >1){
		var1->length -=1;
	}
}

/* perform long division on two longint_t values, disregarding the remainder
*/
void
do_division(longint_t *dividend, longint_t *divisor) {
	/* check divisor is non-zero */
	if (divisor->length == 1 && divisor->num[0] == 0){
		error_and_exit("divide by zero error");
	}

	/* for long division the dividend must be greater than divisor
	*/
	int status = is_greater_than(dividend,divisor);

	if (status == VARSEQUAL){
		/* avoid unnecessary computation, answer is 1 */ 
		dividend->length = 1;
		dividend->num[0] = 1;
		return;
	} else if (status == VAR2GREATER){
		/* avoid unnecessary computation, answer is 0 */ 
		dividend->length = 1;
		dividend->num[0] = 0;
		return;
	}

	/* perform long division as dividend > divisor */
	longint_t quotient = LONG_ZERO;
	longint_t current = LONG_ZERO;
	quotient.length = 0;
	int count = 0, quotient_index = 0;

	for (int i = dividend->length - 1; i >= 0; i--){
		/* set the current value as the ith digit
			in the dividend, when current has been shifted up by shiftup()
			the new value is the old value times 10 plus new value*/
		current.num[0] = dividend->num[i];
		count = 0;
		status = is_greater_than(&current,divisor);

		#if DEBUG
		debug_print("quotient",quotient);
		debug_print("current",current);
		debug_print("divisor",divisor);
		#endif 

		if (status == VARSEQUAL){
			/* any number divided by itself is 1*/
			quotient.num[quotient_index] = 1;
			quotient.length++;
			quotient_index++;
			pop(&current);
		} else if (status == VAR1GREATER){
			/*as the current value must be divided by the divisor
				implemement division by subtraction for intermediary
				operations 
			*/
			status = is_greater_than(&current,divisor);
			while(status != VAR2GREATER){
				do_subtraction(&current,divisor);
				count += 1;
				status = is_greater_than(&current,divisor);
			}
			/* set the quotient value to the quotient of sub operation*/
			quotient.num[quotient_index] = count;
			quotient_index++;
			quotient.length++;
			/* reset the count variable for future iteration */
			count = 0;
			if(current.num[0] != 0 || current.length > 1){
				/*value of current is non zero so it needs to carryover*/
				shiftup(&current);
			}
		} else {
			/* if divisor is larger than current value then set the 
				quotient in the current index to 0 as it will not divide*/
			quotient.num[quotient_index] = 0;
			quotient.length++;
			quotient_index++;
			if(current.num[current.length -1] != 0){
				/*value of current is non zero so it needs to carryover 
					but avoid shifting up a zero value*/
				shiftup(&current);
			}
		}			
	}	
	/* quotient was stored with start at index 0 of array which means 
	   it needs to be reversed to match the way the datatype is stored
	*/
	reverse_longint(&quotient);
	remove_leading_zeros(&quotient);
	do_assign(dividend,&quotient);
}
// Algorithms are fun!!!