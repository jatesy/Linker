#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<string>
#include<iostream>

using namespace std;

#define MAX 1000

FILE *rstream;                          //Input file
char line_token[MAX];					//token for each line while using fgets
int tok_index=0;
int token_pointer;
int module_num=0;
int t_index = 0;
int curr_i_index=0;
int curr_u_index=0;
int output_index = 0;
bool flag = true;
string carrier;
int carrier1;
int curr_i_num = 0;

struct
{
	string name;
	int line;
	int c_offset;
}token[MAX];

struct
{
	string name;
	bool used;
	int m_num;
	int ofst;
	int value;
	int error;
}symbol_table[MAX];

struct
{
	struct
	{
		string name;
		bool used;
	}uselist[MAX];
	int u_top_index;
	int dl_index;
	struct
	{
		string type;
		char opcode;
		string operand;
		int error;
	}inslist[MAX];
	int base_addr;
	int length;
}module[MAX];


void buildtoken();
void passone();
void build_symboltable();
void build_uselist();
void build_inslist();
int strtoint(string str);
void passtwo();
void print_error(int x);
void parseerror(int errcode, int linenum, int lineoffset);
void warning();
bool last_empty_line(char a[MAX]);


int main(int argc, char *argv[])
{
	char *filename;

		
	//cout<<"Please enter the filename of input:"<<endl;
	//scanf("%s", filename);						//User can enter the input file name

	filename = argv[1];
	if (fopen(filename, "r") == NULL)cout<<"\nFailed to open input file!"<<endl;
	else 
	{
		rstream = fopen(filename, "r");
		//cout << "\nInput file openned!\n\nOutput:\n" << endl;
		buildtoken();
		passone();
		if (flag == true)
		{
			passtwo();
			cout << endl;
			warning();
			cout << endl;
		}
	}
	//while (1);
	return (0);
}


void passone()
{
	token_pointer = 0;
	int curr_state = 1;		//1:def list  2:use list  3:ins list
	while (token_pointer < tok_index)
	{
		if (flag == false)return;
		if (isdigit(token[token_pointer].name[0]))
		{
			if (curr_state == 1)
			{
				if (strtoint(token[token_pointer].name) > 16)
				{
					parseerror(4, token[token_pointer].line, token[token_pointer].c_offset);
					break;
				}

				else
				{				
					build_symboltable();
					curr_state = 2;
				}

			}

			else if (curr_state == 2)
			{
				if (strtoint(token[token_pointer].name) > 16)
				{
					parseerror(5, token[token_pointer].line, token[token_pointer].c_offset);
					break;
				}

				else
				{					
					build_uselist();
					curr_state = 3;
				}

			}

			else if (curr_state == 3)
			{
				if (strtoint(token[token_pointer].name) > 512)
				{
					parseerror(6, token[token_pointer].line, token[token_pointer].c_offset);
					break;
				}

				else
				{					
					build_inslist();
					curr_state = 1;
				}
			}	
		}

		//else if (token[token_pointer].name[0] == '\0')
		//{
		//	parseerror(0, token[token_pointer-1].line, token[token_pointer-1].c_offset + strlen(&token[token_pointer-1].name[0]));
		//	return;
		//}
		else
		{
			parseerror(0, token[token_pointer].line, token[token_pointer].c_offset);
			break;
		}
	}
}

void build_symboltable()
{
	
	bool err=false; 
	int t = strtoint(token[token_pointer].name);
	module[module_num].dl_index = 0;
	token_pointer++;
	for (int i = 0; i < t; i++)
	{
		if (isupper(token[token_pointer].name[0]) || islower(token[token_pointer].name[0]))
		{
			if (isdigit(token[token_pointer + 1].name[0]))
			{
				for (int i = 0; i < t_index; i++)
				{
					if (symbol_table[i].name == token[token_pointer].name)
					{
						symbol_table[i].error = 1;
						err = true;
						token_pointer = token_pointer + 2;
						break;
					}
				}
				if (err == false)
				{				
					if (strlen(&token[token_pointer].name[0]) > 16)
					{
						parseerror(3, token[token_pointer].line, token[token_pointer].c_offset);
						return;
					}
					else
					{					
						symbol_table[t_index].name = token[token_pointer].name;
						symbol_table[t_index].ofst = strtoint(token[token_pointer + 1].name);
						symbol_table[t_index].m_num = module_num;
						module[module_num].dl_index++;
						t_index++;
						if (module[module_num].dl_index>16)
						{
							parseerror(4, token[token_pointer].line, token[token_pointer].c_offset);
							return;
						}
						token_pointer = token_pointer + 2;
					}
				}

			}
			
			//else if (token[token_pointer+1].name[0] == '\0')
			//{
			//	parseerror(0, token[token_pointer].line, token[token_pointer].c_offset + strlen(&token[token_pointer].name[0]));
			//	return;
			//}
			//
			else
			{
				parseerror(0, token[token_pointer + 1].line, token[token_pointer + 1].c_offset);
				return;
			}
		}
		
		//else if (token[token_pointer].name[0] == '\0')
		//{
		//	parseerror(1, token[token_pointer-1].line, token[token_pointer-1].c_offset + strlen(&token[token_pointer-1].name[0]));
		//	return;
		//}
		else
		{
			parseerror(1, token[token_pointer].line, token[token_pointer].c_offset);
			return;
		}
	}
}

void build_uselist()
{
	int t = strtoint(token[token_pointer].name);
	token_pointer++;
	for (int i = 0; i < t; i++)
	{
		if (isupper(token[token_pointer].name[0]) || islower(token[token_pointer].name[0]))
		{
			module[module_num].uselist[curr_u_index].name = token[token_pointer].name;
			curr_u_index++;
			if (curr_u_index>16)
			{
				parseerror(5, token[token_pointer].line, token[token_pointer].c_offset);
				return;
			}
			token_pointer++;
		}
		//else if (token[token_pointer].name[0] == '\0')
		//{
		//	parseerror(1, token[token_pointer-1].line, token[token_pointer-1].c_offset + strlen(&token[token_pointer-1].name[0]));
		//	return;
		//}
		else
		{
			parseerror(1, token[token_pointer].line, token[token_pointer].c_offset);
			return;
		}
	}
}

void build_inslist()
{
	int t = strtoint(token[token_pointer].name);
	if ((curr_i_num + t + 1) > 512)
	{
		parseerror(6, token[token_pointer].line, token[token_pointer].c_offset);
		return;
	}
	token_pointer++;
	string a;
	for (int i = 0; i < t; i++)
	{
		if (token[token_pointer].name == "I" || token[token_pointer].name == "R" || 
			token[token_pointer].name == "A" || token[token_pointer].name == "E")
		{
			token_pointer++;
			if (isdigit(token[token_pointer].name[0]))
			{
				if (strlen(&token[token_pointer].name[0]) < 4)
				{
					switch (strlen(&token[token_pointer].name[0]))
					{
					case 1:
						a = "00";
						a.operator+=(token[token_pointer].name);
						break;
					case 2:
						a = "0";
						a.operator+=(token[token_pointer].name);
						break;
					case 3:
						a = token[token_pointer].name;
						break;
					default:
						break;
					}
					module[module_num].inslist[curr_i_index].type = token[token_pointer - 1].name;
					module[module_num].inslist[curr_i_index].opcode = '0';
					module[module_num].inslist[curr_i_index].operand = a;
				}

				else
				{
					module[module_num].inslist[curr_i_index].type = token[token_pointer - 1].name;
					module[module_num].inslist[curr_i_index].opcode = token[token_pointer].name[0];
					module[module_num].inslist[curr_i_index].operand = &token[token_pointer].name[1];
				}
				curr_i_num += curr_i_index;
				curr_i_index++;
				if (curr_i_index > 512)
				{
					parseerror(6, token[token_pointer - 1].line, token[token_pointer - 1].c_offset);
					return;
				}
				token_pointer++;
			}

			//else if (token[token_pointer].name[0] == '\0')
			//{
			//	parseerror(0, token[token_pointer - 1].line, token[token_pointer - 1].c_offset + strlen(&token[token_pointer - 1].name[0]));
			//	return;
			//}
			else
			{
				parseerror(0, token[token_pointer].line, token[token_pointer].c_offset);
				return;
			}
		}

		//else if (token[token_pointer].name[0] == '\0')
		//{
		//	parseerror(2, token[token_pointer-1].line, token[token_pointer-1].c_offset + strlen(&token[token_pointer-1].name[0]));
		//	return;
		//}

		else
		{
			parseerror(2, token[token_pointer].line, token[token_pointer].c_offset);
			return;
		}
	}
	/*Set base address and length for each module*/
	if (module_num == 0)
	{
		module[module_num].base_addr = 0;
		module[module_num].length = curr_i_index;
	}
	else
	{
		module[module_num].base_addr = module[module_num-1].base_addr + module[module_num-1].length;
		module[module_num].length = curr_i_index;
	}
	/*********************************************/

	/*End of a module*/
	module[module_num].u_top_index = curr_u_index - 1;
	module_num++;
	curr_i_index = 0;
	curr_u_index = 0;
	/*****************/
}

void buildtoken()
{
	int line_num = 0;		//current line number
	int l_offset;		//curren character offset of current line
	int line_len;
	char *curr_tok;
	char line_buf[MAX];

	while (feof(rstream) == 0)
	{
		fgets(line_token, MAX, rstream);
		strcpy(&line_buf[0], &line_token[0]);
		if (line_token == NULL)
			break;
		
		else if (last_empty_line(&line_buf[0]) == true && feof(rstream) != 0)
			break;

		else
		{
			line_len = strlen(&line_token[0]);
			line_num++;
			l_offset = 0;
			for (int i = 0; line_token[i] == ' ' || line_token[i] == '\t';i++)		//ignore the spaces in front of this line
			{
				l_offset++;
			}
			curr_tok = strtok(line_token, " \t\n");
			if (curr_tok != NULL && curr_tok[0] != '\n')
			{
				//cout<<"%s\n", curr_tok);
				if (curr_tok[strlen(curr_tok)-1] == '\n')curr_tok[strlen(curr_tok) - 1] = '\0';		//delete the '\n'
				token[tok_index].name = curr_tok;
				token[tok_index].line = line_num;
				l_offset++;
				token[tok_index].c_offset = l_offset;
				l_offset = l_offset + strlen(curr_tok);
				tok_index++;
			}
			while (curr_tok != NULL && curr_tok[0] != '\n')
			{
				curr_tok = strtok(NULL, " \t\n");
				if (curr_tok == NULL|| curr_tok[0] == '\n')
					break;
				//for (int i = 0; curr_tok[i] == '\t' || curr_tok[i] == ' '; i++)		//2 for loops to delete spaces in front of words
				//{
				//	for (int j = i; curr_tok[j] != '\0'; j++)
				//	{
				//		curr_tok[j] = curr_tok[j + 1];
				//	}
				//}
				if (curr_tok != NULL)
				{
					//cout<<curr_tok<<endl;
					if (curr_tok[strlen(curr_tok)-1] == '\n')curr_tok[strlen(curr_tok) - 1] = '\0';		//delete the '\n'
					token[tok_index].name = curr_tok;
					token[tok_index].line = line_num;
					l_offset = line_len - (strlen(strstr(&line_buf[l_offset], curr_tok))) + 1;
					token[tok_index].c_offset = l_offset;
					l_offset = l_offset + strlen(curr_tok);
					tok_index++;
				}
			}

		}
		for (int i = 0; line_token[i] != '\0'; i++)line_token[i] = ' ';
	}
	
	token[tok_index].line = line_num;
	token[tok_index].c_offset = line_len;
	
	//for (int i = 0; i < tok_index; i++)cout << token[i].name <<"    line:"<<token[i].line<<"  offset:"<<token[i].c_offset<< endl;

	//cout<<"\nFile read!"<<endl;
	
	//if(feof(rstream) != 0) cout<<"\nEOF!\n"<<endl;
	
	//while(1);
}

void passtwo()
{
	int temp;
	int u_num;
	int t_num;
	string s_name;
	
	for (int i = 0; i < t_index; i++)
	{
		if (symbol_table[i].ofst > module[symbol_table[i].m_num].length)
		{
			printf("Warning: Module %d: %s too big %d (max=%d) assume zero relative\n", symbol_table[i].m_num + 1, &symbol_table[i].name[0], symbol_table[i].ofst, module[symbol_table[i].m_num].length - 1);
			symbol_table[i].ofst = 0;
		}
	}
	
	cout << "Symbol Table" << endl;
	for (int i = 0; i < t_index; i++)
	{
		symbol_table[i].value = module[symbol_table[i].m_num].base_addr + symbol_table[i].ofst;
		printf("%s=%d",&symbol_table[i].name[0],symbol_table[i].value); 
		print_error(symbol_table[i].error);
		printf("\n");
	}
	cout << "\nMemory Map" << endl;
	for (int i = 0; i < module_num; i++)
	{
		for (int j = 0; j < module[i].length; j++)
		{
			printf("%03d:  ",output_index);
			output_index++;
			if (strlen(&module[i].inslist[j].operand[0]) > 3 && module[i].inslist[j].type != "I")
			{
				module[i].inslist[j].error = 7;
				printf("9999");
				print_error(module[i].inslist[j].error);
				printf("\n");
			}
			else if (module[i].inslist[j].type == "R")
			{
				if (strtoint(module[i].inslist[j].operand) > module[i].length)
				{
					module[i].inslist[j].error = 4;
					temp = 0 + module[i].base_addr;
					printf("%c%03d", module[i].inslist[j].opcode, temp);
					print_error(module[i].inslist[j].error);
					printf("\n");
				}
				else
				{
					temp = strtoint(module[i].inslist[j].operand) + module[i].base_addr;
					printf("%c%03d\n", module[i].inslist[j].opcode, temp);
				}
			}
			
			else if (module[i].inslist[j].type == "E")
			{
				u_num = atoi(&module[i].inslist[j].operand[2]);
				if (u_num > module[i].u_top_index)				//If an external address is too large to reference an entry in the use list, print an error message and treat the address as immediate.
				{
					module[i].inslist[j].error = 3;
					printf("%c%s", module[i].inslist[j].opcode, &module[i].inslist[j].operand[0]);
					print_error(module[i].inslist[j].error);
					printf("\n");
				}
				else
				{				
					s_name = module[i].uselist[u_num].name;
					module[i].uselist[u_num].used = true;
					for (t_num = 0; symbol_table[t_num].name != s_name; t_num++)//search symbol in symbol table
					{
						if (t_num == t_index)		//if symbol not defined, use 0
						{
							//symbol_table[t_num].name = s_name;
							symbol_table[t_num].value = 0 + module[i].base_addr;
							module[i].inslist[j].error = 2;
							carrier = s_name;
							break;
						}
					}
					temp = symbol_table[t_num].value;
					symbol_table[t_num].used = true;
					printf("%c%03d", module[i].inslist[j].opcode, temp);
					print_error(module[i].inslist[j].error);
					printf("\n");
				}

			}
			
			else if (module[i].inslist[j].type == "I")
			{
				if (strlen(&module[i].inslist[j].operand[0]) > 3)
				{
					module[i].inslist[j].error = 6;
					printf("9999");
					print_error(module[i].inslist[j].error);
					printf("\n");
				}
				else printf("%c%s\n", module[i].inslist[j].opcode, &module[i].inslist[j].operand[0]);
			}
			
			else if (module[i].inslist[j].type == "A")
			{
				if (strtoint(module[i].inslist[j].operand) >= 512)
				{
					module[i].inslist[j].error = 5;
					printf("%c000", module[i].inslist[j].opcode);
					print_error(module[i].inslist[j].error);
					printf("\n");
				}
				else printf("%c%s\n", module[i].inslist[j].opcode, &module[i].inslist[j].operand[0]);
			}

			//print warning of symbols that apear on uselist but not used


		}
		for (int j = 0; j <= module[i].u_top_index; j++)
		{
			if (module[i].uselist[j].used != true)
				printf("Warning: Module %d: %s appeared in the uselist but was not actually used\n", i + 1, &module[i].uselist[j].name[0]);
		}

	}


}

int strtoint(string str)
{
	char *a=&str[0];
	int result = atoi(a);
	return result;
}

void parseerror(int errcode, int linenum, int lineoffset) 
{
	static char* errstr[] = 
	{
		"NUM_EXPECTED", // Number expect
		"SYM_EXPECTED", // Symbol Expected
		"ADDR_EXPECTED", // Addressing Expected
		"SYM_TOLONG", // Symbol Name is to long
		"TO_MANY_DEF_IN_MODULE", // ..
		"TO_MANY_USE_IN_MODULE",
		"TO_MANY_INSTR"
	};
	printf("Parse Error line %d offset %d: %s\n\n", linenum, lineoffset, errstr[errcode]);
	flag = false;
}

void print_error(int x)
{
	switch (x)
	{
	case 1:
		printf(" Error: This variable is multiple times defined; first value used");
		break;

	case 2:
		printf(" Error: %s is not defined; zero used", &carrier[0]);
		break;

	case 3:
		printf(" Error: External address exceeds length of uselist; treated as immediate");
		break;

	case 4:
		printf(" Error: Relative address exceeds module size; zero used");
		break;

	case 5:
		printf(" Error: Absolute address exceeds machine size; zero used");
		break;

	case 6:
		printf(" Error: Illegal immediate value; treated as 9999");
		break;

	case 7:
		printf(" Error: Illegal opcode; treated as 9999");
		break;

	default:
		break;
	}
}

void warning()
{
	for (int i = 0; i < t_index; i++)
	{
		if (symbol_table[i].used != true)
			printf("Warning: Module %d: %s was defined but never used\n", symbol_table[i].m_num + 1, &symbol_table[i].name[0]);
	}
}

bool last_empty_line(char *a)
{
	bool result = true;
	for (int q = 0; q < strlen(a); q++)
	{
		if (a[q] != ' ' && a[q] != '\t' && a[q] != '\0' && a[q] != '\n')
		{
			result = false;
			break;
		}
	}
	return(result);
}
