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

/* Placeholder typedef for skeleton code
*/
typedef struct{
	int length;
	int num[INTSIZE];
} longint_t;	

#define LONG_ZERO 0
#define LONG_ONE  1


/****************************************************************/

/* A "magic" additional function needing explicit declaration */
int fileno(FILE *);

// Function prototypes 
void print_prompt(void);
void print_tadaa();
void print_error(char *message);
int  read_line(char *line, int maxlen);
void process_line(longint_t vars[], char *line);
int  get_second_value(longint_t vars[], char *rhsarg, longint_t *second_value);
int  to_varnum(char ident);
void do_print(int varnum, longint_t var);
void do_assign(longint_t *var1, longint_t *var2);
void do_plus(longint_t *var1, longint_t var2);
void zero_vars(longint_t vars[], int numvars);
void reverse_array(longint_t *var);
int parse_num(char rhs);
void do_multiplication(longint_t *var1, longint_t var2);
void do_exponentiation(longint_t *var1, longint_t var2);



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

void
print_tadaa() {
	/* all done, so pack up bat and ball and head home,
	   getting the exact final lines right is a bit tedious,
	   because input might be coming from a file and output
	   might be going to a file */
	if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
		printf("\n");
	}
	printf("ta daa!!!\n");
	if (isatty(fileno(stdin)) && !isatty(fileno(stdout))) {
		fprintf(stderr, "\n");
	}
}

void
print_error(char *message) {
	/* need to write an error message to the right place(s)
	*/
	if (isatty(fileno(stdin)) || isatty(fileno(stdout))) {
		fprintf(stderr, "%s\n", message);
		fflush(stderr);
	}
	if (!isatty(fileno(stdout))) {
		printf("%s\n", message);
	}
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
/****************************************************************/

/* Reverse an array
*/
void 
reverse_array(longint_t *var){
	longint_t tmp;
	tmp = *var;
	int i = 0;
	for (int j = var->length - 1; j >= 0; j--){
		var->num[i] = tmp.num[j];
		i++;
	}
	return;
}

/****************************************************************/

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
		do_print(varnum, vars[varnum]);
	} else if (optype == ASSIGN) {
		do_assign(&vars[varnum], &second_value);
	} else if (optype == PLUS) {
		do_plus(&vars[varnum], second_value);
	} else if (optype == MULT){
		do_multiplication(&vars[varnum], second_value);
	} else if (optype == POWR){
		do_exponentiation(&vars[varnum], second_value);
	} else {
		print_error("operation not available yet");
		return;
	}
	return;
}

/****************************************************************/

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

/****************************************************************/

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

		int i = 0, j = 0;
		int flag = 0;
		for (p=rhsarg; *p; p++) {
			if (!isdigit(*p)) {
				/* nope, found an illegal character */
				return ERROR;
			} 
			second_value->num[j] = parse_num(*(rhsarg+i));
			i++;
			// remove leading zeros from the parsed integer
			if (flag == 1 || second_value->num[j] != 0){
				j++; 
				flag = 1;
			}
		}

		second_value->length = j;
		reverse_array(second_value);
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

/* Set the vars array to all zero values
*/
void
zero_vars(longint_t vars[], int numvars) {
	for (int i=0; i< numvars; i++) {
		for (int j=0; j<INTSIZE; j++){
			vars[i].num[j] = 0;
		}
	}
}


/* converter the character representation of num to an int
*/
int
parse_num(char rhs) {
	return (rhs - CH_ZERO);
}

/****************************************************************/

/* Print out a longint value
*/
void
do_print(int varnum, longint_t var) {
	printf("register %c: ", varnum+CH_A);
	int ref = var.length % 3;
    ref = ref == 0 ? 3 : 0;
    for (int i = var.length - 1; i >= 0; i--){
		printf("%d", var.num[i]);
        if (i != 0 && (ref + i) % 3 == 0){
            printf(",");
        }
	}
	printf("\n");
}

/****************************************************************/

/* Assign a longint value, could do this with just an assignment
   statement, because structs can be assigned, but this is more
   elegant, and only copies over the array elements (digits) that
   are currently in use: var1 = var2

*/
void
do_assign(longint_t *var1, longint_t *var2) {
	*var1 = *var2;
}

/****************************************************************/

/* Update the indicated variable var1 by doing an addition
   using var2 to compute var1 = var1 + var2
*/
void
do_plus(longint_t *var1, longint_t var2) {
    longint_t result;
    int out;
    int carryover = 0;
	int max = var1->length > var2.length ? var1->length : var2.length;
	int i;

    for(i = 0; i < max; i++){
		if (i < var1->length && i < var2.length){
			out = var1->num[i] + var2.num[i];
		} else if (i < var1->length && i >= var2.length){
			out = var1->num[i];
		} else{
			out = var2.num[i];
		}
        
        
        if (carryover != 0){
            out += carryover;
            carryover = 0;
        }
        if (out >= 10){
            out -=10;
            carryover = 1;
        }

        result.num[i] = out;
		result.length = i + 1;
    }
	// check addition does not yeild a number that is too big
	if(var1->length == INTSIZE && var2.length == INTSIZE && carryover != 0){
		print_error("calculated integer exceeds max intsize");
		return;
	}
	if(carryover != 0){
		result.num[i] = carryover;
    	result.length = i + 1;
	}
	*var1 = result;
}

/* multiply the two integer arrays together using the long
	multiplication method
*/
void
do_multiplication(longint_t *var1, longint_t var2){

    int out;
    int carryover = 0;
    int max = var1->length > var2.length ? var1->length : var2.length;
    int min = var1->length < var2.length ? var1->length : var2.length;
    int i, j;
    longint_t output[min];
	zero_vars(output, min);

    for(i=0; i<min; i++){
        for(j=0; j<max;j++){
			if (var1->length < var2.length){
				out = var1->num[i] * var2.num[j];	
			} else {
				out = var1->num[j] * var2.num[i];
			}
			
			if (carryover != 0){
				out += carryover;
				carryover = 0;
			}
			if (out >= INT_TEN){
				carryover += (out - (out % INT_TEN)) / INT_TEN;
				out = out % INT_TEN;
			}
			//printf("out: %d\n", out);
			output[i].num[j+i] = out;	
        }
        output[i].length = max + i;    
    }
	//printf("carry: %d\n",carryover);
	if(carryover > 0){
		output[i-1].num[j+i-1] = carryover;
		output[i-1].length += 1;	
	}

	//do_print(0,output[0]);
    for(i=1; i<min; i++){
		//do_print(0,output[i]);
        do_plus(&output[0], output[i]);
    }

    *var1 = output[0];
}


void
do_exponentiation(longint_t *var1, longint_t var2){
	int num = 0;
	if (var2.length <= 4){
		//convert var2 to int as it is small enough
		for (int i = var2.length - 1; i >= 0; i--){
			num = num * 10 + var2.num[i];
		}
		if (num > 1660){
			print_error("Exponent too big");
			return;
		}
	}
	longint_t var1copy = *var1;
	printf("num: %d\n",num);
	for (int i = 1; i < num; i++){
		do_multiplication(var1,var1copy);
	}
}

void
do_division(longint_t *var1, longint_t var2){



}
// Algorithms are fun!!!

