#include "calc.h"

int calc(char * user_input) {
	
	// Create an array of pointers
	char ** infix_expression; 
	char ** postfix_expression;
	int max_expression_length = 100;						// ONLY 10
	infix_expression =  malloc(max_expression_length*sizeof(char*));
	postfix_expression =  malloc(max_expression_length*sizeof(char*));
	int infix_length, postfix_length;
	
	allocate2D(infix_expression, max_expression_length);
	allocate2D(postfix_expression, max_expression_length);
	
	// Create an infix expression and return the no of terms
	infix_length = createInfix(user_input, infix_expression);
	print2D(infix_expression, infix_length);
	
	
	//printf("infix ength: %d\n", infix_length);
	
	// Turn the infix expression into a postfix expression
	postfix_length = infixToPostfix(infix_expression, infix_length, postfix_expression);
	//strcpy(*postfix_expression, "1");
	print2D(postfix_expression, postfix_length);	
	
	// Free dat memory
	free2D(infix_expression, max_expression_length);
	free(infix_expression);
	//free2D(postfix_expression, max_expression_length);
	//free(postfix_expression);
	//free(postfix_expression);
	
	return 0;
}

int createInfix(char * user_input, char ** infix_expression) {
	
	//char ** p_infix = infix_expression;
	// Variable to count the number of terms in expression
	int expression_length = 0;
	
	// Create a temporary string that we put onto the infix_expression
	int element_length = 16;
	char * string;
	string = malloc(element_length*sizeof(char));
	char * p_string = string;
	//printf("pointer to string: %p\n");
	//printf("%s\n", user_input);
	
	
	
	// Go through the input until we reach a null char
	while (*user_input) {
		
		// Here we put the character onto a string and either put it on to the expression
		// Or we keep adding to the string, depending on wether it forms part of a number
		int should_push = 0;
		
		// Put the character onto the string
		*string = *user_input;
		//printf("user iput: %c\n", *user_input);
				
		// If the first element and negative  // POTENTIAL CAUSE OF ERROR
		if ( (*user_input == '-') && (isCharInString("0123456789.", *(user_input+1)) == 1) && (*(user_input-1) == '\0') ) {
			should_push = 0;
		}
		// if format (5)-6 minus after bracket
		else if (*(user_input-1) == ')' && *(user_input) == '-') {
			should_push = 1;
		}
		// if format 1-(1+2)  minus before brackets
		else if (*(user_input) == '-' && isCharInString("(", *(user_input+1)) == 1) {
			//printf("fire 2\n");
			should_push = 1;
		}
		// If the number is negative (as opposed to minus sign)
		else if (*(user_input) == '-' && isCharInString("+-/*^()", *(user_input-1)) == 1) {
			//printf("1\n");
			should_push = 0;
		}
		// if format 1+(3*5) two operators in a row
		else if (isCharInString("()", *(user_input)) && isCharInString("+-/*^()", *(user_input-1)) == 1) {
			//printf("3\n");
			should_push = 1;
		}
		// if format 1e-5
		else if (*(user_input) == '-' && *(user_input-1) == 'e') {
			//printf("4\n");
			should_push = 0;
		}
		// If a valid double character and the next character isnt an operator
		else if (isCharInString("0123456789.e", *user_input) == 1 && isCharInString("+-/*()", *(user_input+1)) == 0 ) {
			//printf("5\n");
			should_push = 0;
		}
		// If a vlaid double cahracter and the next character is an operator
		else if (isCharInString("0123456789.", *user_input) == 1 && isCharInString("+-/*()", *(user_input+1)) == 1 ) {
			//printf("6\n");
			should_push = 1;
		}
		// If an operator
		else if (isCharInString("+-/*^()", *user_input) == 1) {
			//printf("7\n");
			should_push = 1;
		}	
		// If the last character
		if (*(user_input + 1) == '\0') {
			should_push = 1;
			//printf("last character\n");
		}
		
		// If we want to push the string
		if (should_push == 1) {
			
			//printf("pushing string %s\n", p_string);
			
			// Copy the string to the infix_expression
			strcpy(*infix_expression, p_string);
			infix_expression++;


			// Reallocate memory for the string
			//p_string = realloc(p_string, element_length*sizeof(char));
			p_string = malloc(element_length*sizeof(char));
			
			// Point back to the first element of the string
			string = p_string;
			
			// Count the number of elements
			//printf("exp length %d\n", expression_length);
			expression_length++;
			//printf("exp length %d\n", expression_length);
		}
		else {
			
			// Go to next element of the string
			string++;
		}
		
		// Go to the next input character
		user_input++;
		
	}
	//print2D(p_infix);
	free(p_string);
	
	return expression_length;
}

int infixToPostfix (char ** infix_expression, int infix_length, char ** postfix_expression) {
	
	// Counter for no of elements of our postfix stack
	int postfix_length = 0;
	
	// This is a temporary string that we will use to pop strings to due to memory faults
	char string[] = "\0";
	
	// Define our stack
	int stack_length = 100;
	char * stack;
	stack = malloc(sizeof(char)*stack_length);
	
	// Go through our infix_expression
	int i = 0;
	while (i < infix_length) {
		
		printf("address we are writing: %p\n", postfix_expression);
		
		// If the element is an operator
		if (isOperator(*infix_expression) == 1) {
			
			
			// If the stack is empty
			if (peek(stack) == '\0') {
				printf("case 1\n");
				
				// Push the character onto the stack
				push(*(infix_expression), stack);
				
			}
			// If the operator has higher precedence than the top of the stack
			else if (precedence(*infix_expression[0]) > precedence(peek(stack))) {
				printf("case 2\n");
				
				// Put it on the stack
				push(*(infix_expression), stack);
				
			}
			// Otherwise the precedence must be less than or equal to the top of the stack
			else {
				printf("case 3\n");
				
				while (precedence(*infix_expression[0]) <= precedence(peek(stack))) {
					
					// THIS WILL PROBABLY BREAK CHECK THE MEMORY ADDRESSES
					// Make a temporary string to put on the postfix expression
					char str[] = "\0";
					
					// Pop the value of the stack
					str[0] = pop(stack);
					printf("temp str: %s\n", str);
					
					// Put the top of the stack onto the expression
					*postfix_expression = str;
					printf("this potifx %s at %p\n", *postfix_expression, postfix_expression);
					
					// Increment the expression pointer and length
					postfix_expression++;
					postfix_length++;

					printf("stack: %s", stack);
				
				}
				
				// Put the infix operator onto the stack
				push(*(infix_expression), stack);

			}
			
		}
		// If the element is a number put it straight onto the postfix expression
		else {
			printf("case 4\n");	
			//printf("tesst1\n");
			//*postfix_expression = "test";
			*postfix_expression = *infix_expression;
			//strcpy(*postfix_expression, "1");
			
			//printf("test2\n");
			
			// Increment the expression pointer and length
			postfix_expression++;
			postfix_length++;
			//printf("test3\n");	
		}
		
		//infix_expression++;
		i++;
		//printf("infix element: %s\n", *infix_expression);
		//printf("test4\n");
		infix_expression++;
		
	}
	
	printf("postfix_expression addr: %p\n", postfix_expression);
	
	// Make a temporary string to put on the postfix expression
		
	/*
		char str[] = "\0";
		
		// Pop the value off the stack		// The fuck is with this code? it changes memory that should be allocated allready
		str[0] = pop(stack);
	*/
		
		string[0] = pop(stack);
	
		printf("popped value: %s", string);	
	
	printf("postfix_expression addr: %p\n", postfix_expression);
		// Put the top of the stack onto the expression
		*postfix_expression = string;
		string[0] = '\0';
	
	/*
	// Empty the remainder of the stack onto the postfix
	while (peek(stack)) {
		
		// Make a temporary string to put on the postfix expression
		char str[] = "\0";
		
		// Pop the value off the stack
		str[0] = pop(stack);
		
		printf("popped value: %s", str);	

		// Put the top of the stack onto the expression
		*postfix_expression = "a";	
		
		printf("postfix_expression count: %p", postfix_expression);
		// Increment the expression pointer and length
		postfix_expression++;
		postfix_length++;
	}
	*/
	
	//push("a", stack);
	printf("stack: %s\n", stack);
	
	// Free de memz
	free(stack);
	
	return postfix_length;
}