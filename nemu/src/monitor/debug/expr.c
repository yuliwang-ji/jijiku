#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, UEQ, logical_AND, logical_OR, logical_NOT,Register, Variable, Number, Hex, Eip

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"==", EQ},						// equal
        {"\\(", '('},                                   //left parenthesis
        {"\\)", ')'},                                   //right parenthesis
        {"\\*", '*'},                                   //multiplication
        {"/", '/'},                                     //division
        {"-", '-'},                                     //subtraction
        {"!=", UEQ},                                    //unequal
        {"&&", logical_AND},                            //logical AND
        {"\\|\\|", logical_OR},                         //logical OR
        {"!", logical_NOT},                             //logical NOT
        {"\\$[a-dA-D][hlHL]|\\$[eE]?(ax|dx|cx|bx|bp|si|di|sp)", Register}, //register
        {"[a_zA_Z_][a-zA-Z0-9_]*", Variable},           //variable
        {"[0-9]{1,10}", Number},                        //nmumber
        {"0[xX][A-Fa-f0-9]{1,8}", Hex},                 //hex











};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;

	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {

                                  case 257:
		                	tokens[nr_token].type=257;
			                strcpy(tokens[nr_token].str,"==");
		                 	break;
	        case 40:
		      	tokens[nr_token].type=40;
		        break;
	        case 41:
		        tokens[nr_token].type=41;
			break;
		case 42:
			tokens[nr_token].type=42;
			break;
		case 47:
			tokens[nr_token].type=47;
			break;
		case 43:
			tokens[nr_token].type=43;
			break;
		case 45:
			tokens[nr_token].type=45;
			break;
		case 258:
			tokens[nr_token].type=258;
			strcpy(tokens[nr_token].str,"!=");
			break;
		case 259:
			tokens[nr_token].type=259;
			strcpy(tokens[nr_token].str,"&&");
			break;
		case 260:
			tokens[nr_token].type=260;
			strcpy(tokens[nr_token].str,"||");
			break;
		case 261:
			tokens[nr_token].type=261;
			break;
		case 262:
			tokens[nr_token].type=262;
			strncpy(tokens[nr_token].str,&e[position-substr_len],substr_len);
			break;
		case 263:
			tokens[nr_token].type=263;
			strncpy(tokens[nr_token].str,&e[position-substr_len],substr_len);
			break;
		case 264:
			tokens[nr_token].type=264;
			strncpy(tokens[nr_token].str,&e[position-substr_len],substr_len);
			break;
		case 265:
			tokens[nr_token].type=265;
			strncpy(tokens[nr_token].str,&e[position-substr_len],substr_len);
			break;
		case 266:
			tokens[nr_token].type=266;
			break;      
                               
					default: panic("please implement me");
                                                 nr_token--;
                                                 break;
				}
                                nr_token++;
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	panic("please implement me");
	return 0;
}

