/*
Author- Kaevin Barta
File- Stacks.snl
version- 1.0

 The program takes an infix expression as input, validates it, converts 
 it to postfix notation, evaluates the postfix expression, and outputs the 
 result.:)
*/
#include <iostream>
#include <string>
#include <stack>
#include <limits.h>


// Function to check if a character is an operator @param c- checks character to see if it is an operator
bool isOperator(char c);

// Function to get the precedence of an operator @param c- checks character then orders them with Pemdas 
int getPrecedence(char c);

// Function to convert an infix expression to postfix @param infix- the orginal equation @param postfix- the solution to the last equation @return true, if succsefully converted into postfix @return The postfix, solved
bool convertToPostfix(std::string infix, std::string& postfix);

// Function to evaluate a postfix expression @param postfix- the solution to the last equation, which then get the postfix equation evaluated @return the postfix equation solved
float evaluatePostfix(std::string postfix);

// Function to validate an infix expression @param infix- The orginal equation to be solved @return true, if its valid expression
bool validateInfix(std::string infix);

int main() {
    std::cout << "Author: Kaevin Barta" << std::endl << std::endl;

    std::string infixExpression = "7*3"; // does not accept double didget numbers, EXAMPLE: "(9+2)*7"
    std::string postfixExpression;
    float result;

    // validate infix expression
    if (!validateInfix(infixExpression)) {
        std::cout << "Invalid infix expression" << std::endl;
        return 1;
    }

    // convert infix to postfix
    if (!convertToPostfix(infixExpression, postfixExpression)) {
        std::cout << "Failed to convert infix to postfix" << std::endl;
        return 1;
    }

    // evaluate postfix expression
    result = evaluatePostfix(postfixExpression);

    if (result == INT_MAX) {//the only way INT_MAX would hit is because of an divide by 0 error on line 157
        std::cout << "Failed to divide by 0, Answer is undifined" << std::endl;
        return 1;
    }

    // output result
    std::cout << "Infix expression: " << infixExpression << std::endl;
    std::cout << "Postfix expression: " << postfixExpression << std::endl;
    std::cout << "Result: " << result << std::endl;
    return 0;
}

bool isOperator(char c) {
    switch (c) {//If c matches any of the operators, the function returns true. If c is not an operator, the function returns false.
    case '+':
        return true;
    case '-':
        return true;
    case '*':
        return true;
    case '/':
        return true;
    default:
        return false;
    }
}

int getPrecedence(char c) {//PEMDAS!
    if (c == '*' || c == '/')
        return 2;// The higher the value, the higher the precedence.
    else if (c == '+' || c == '-')
        return 1;
    else
        return 0;
}

bool convertToPostfix(std::string infix, std::string& postfix) {//we need the data so we pass by refrence for postfix
    std::stack<char> stack;// create an empty stack to hold operators
    postfix = ""; 
    bool success = true;//for the return 
    for (int i = 0; i < infix.length(); i++) {
        if (isdigit(infix[i])) { // if the character is a digit, add it to the postfix string
            postfix += infix[i];
        }
        else if (infix[i] == '(') {// if the character is an opening parenthesis, push it onto the stack
            stack.push(infix[i]);
        }
        else if (infix[i] == ')') { // if the character is a closing parenthesis, pop the operators from the stack and add them to the postfix string until the matching opening parenthesis is found
            while (!stack.empty() && stack.top() != '(') {
                postfix += stack.top();
                stack.pop();
            }
            if (stack.empty()) {// if there is no matching opening parenthesis, set success to false and break out of the loop
                success = false;
                break;
            }
            stack.pop(); // Pop the '('
        }
        else if (isOperator(infix[i])) {// if the character is an operator, pop operators from the stack and add them to the postfix string until an operator with lower precedence is found, then push the new operator onto the stack
            while (!stack.empty() && stack.top() != '(' && getPrecedence(stack.top()) >= getPrecedence(infix[i])) {
                postfix += stack.top();//add the number with equal or higher precedence to the postfix solution
                stack.pop();
            }
            stack.push(infix[i]);
        }
        else { // if the character is invalid, set success to false and break out of the loop
            success = false;
            break;
        }
    } // pop any remaining operators from the stack and add them to the postfix string
    while (!stack.empty()) {
        if (stack.top() == '(') {// if there are any remaining opening parentheses on the stack, the expression is invalid
            success = false;
            break;
        }
        postfix += stack.top();
        stack.pop();
    }
    return success; //return whatevers in success(true/false)
}

float evaluatePostfix(std::string postfix) {
    std::stack<float> stack;
    for (int i = 0; i < postfix.length(); i++) {
        if (isdigit(postfix[i])) {// if the current character is a digit, push it onto the stack
            stack.push(postfix[i] - '0');//All ASCII values are like '0'=48, '1'=49....'3'=51 So when we minus them it just equals the real number it should have been. could also use static cast probably
        }
        else if (isOperator(postfix[i])) {// if the current character is an operator, pop the top two operands from the stack and apply the operator to them
            float operand2 = stack.top();
            stack.pop();//get the next number
            float operand1 = stack.top();
            stack.pop();
            float result;
            switch (postfix[i]) { // perform the operation on the operand, specified by the operator and push the result onto the stack using a switch statment.   
            case '+':
                result = operand1 + operand2;//addition for +
                break;
            case '-':
                result = operand1 - operand2;//subtraction for -
                break;
            case '*':
                result = operand1 * operand2;//multiplication for *
                break;
            case '/':
                if (operand2 == 0) {// if the divisor is zero, return INT_MAX to indicate an error
                    return INT_MAX;
                }
                result = operand1 / operand2;
                break;
            }
            stack.push(result);//this is so when it pulls the first number off the stack again, it will be the result of the last expression
        }
    }
    return stack.top(); // return the final result that is left on the top of the stack
}

bool validateInfix(std::string infix) {
    for (int i = 0; i < infix.length(); i++) { // valid operators (+,-,*,/), left and right parentheses and any other ints
        if (!isdigit(infix[i]) && !isOperator(infix[i]) && infix[i] != '(' && infix[i] != ')') {//checking if its not a operator, because it is, if !isOperator = true or number.
            return false;
        }
    }
    return true;
}