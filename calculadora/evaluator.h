#ifndef EVALUATOR_H

#define EVALUATOR_H

int evaluate(char *expression, int length);

int main_op(char *expression, int length);

int strtonum(char *string, int length);

int is_operand(char c);

#endif