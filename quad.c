#include	"quad.h"

#define		INTCODESIZE		200
#define		TRUE			1
#define		FALSE			0

char	*opchar[] = {"<null>", "+", "-", "-", "*", "/", "=", "arg", "goto",
			 "ifposz", "ifpos", "ifnegz", "ifneg", "ifz",
			 "ifnz", "label", "call", "call", "read",
			 "write", "return"
};

const struct addressrec	noop = {opnnull, NULL};
struct quadtype	intcode[INTCODESIZE];
int		numcodes = 0, numtemps = 0;

void	clearidentities(int i);
void	reducestrength(int i);
int	addidentity1(int i);
int	addidentity2(int i);
int	multidentity1(int i);
int	multidentity2(int i);
int	zeromult1(int i);
int	zeromult2(int i);
int	twotimes(int i);
int	times2(int i);
float	getvalue(int tabindex);
int	insamebb(int j);
int	copyprop(int i, int j);
int	codeappears(int i, int j);
void	packcode(void);

struct addressrec	setaddress(enum operandtype otype, int tabindex)
{
	struct addressrec	a;

	a.opndtype = otype;
	a.opnd = tabindex;
	return(a);
}

struct addressrec gettempvar(enum datatype otype)
{
	struct addressrec	result;
	char		tempname[10], numtempchar[10];
	int		tabindex;

	strcpy(tempname, "temp_");
	//itoa(numtemps++, numtempchar, 10);
    sprintf(numtempchar, "%d", numtemps++);
    strcat(tempname, numtempchar);

	installname(tempname, &tabindex);
	setattrib(stunknown, tokidentifier, tabindex);
	installdatatype(tabindex, sttempvar, otype);

	result.opnd = tabindex;
	result.opndtype = opntempvar;
	return(result);
}

struct addressrec gettemplabel(void)
{
	struct addressrec	result;
	char		tempname[10], numtempchar[10];
	int		tabindex;
        static int	numtemps = 0;

	strcpy(tempname, "label_");
	//itoa(numtemps++, numtempchar, 10);
    sprintf(numtempchar, "%d", numtemps++);
    strcat(tempname, numtempchar);

	installname(tempname, &tabindex);
	setattrib(stunknown, tokidentifier, tabindex);
	installdatatype(tabindex, stlabel, dtnone);

	result.opnd = tabindex;
	result.opndtype = opnaddr;
        return(result);
}


int	genquad(enum optype intopcode, struct addressrec a,
				struct addressrec b, struct addressrec c)
{
	intcode[numcodes].opcode = intopcode;
	intcode[numcodes].op1.opndtype = a.opndtype;
        intcode[numcodes].op1.opnd = a.opnd;
	intcode[numcodes].op2 = b;
        intcode[numcodes++].op3 = c;
	return(numcodes-1);
}


void permtarget(struct addressrec a)	{
	intcode[--numcodes-1].op1 = a;
	--numtemps;
}

 struct quadtype	getquad(int i)
 {
	const struct quadtype	nullquad = {opnull, {opnnull, NULL},
					{opnnull, NULL}, { opnnull, NULL}};
 	if (i < numcodes)
 		return(intcode[i]);
 }

int	getnumcodes(void)
{
	return(numcodes);
}

void	intpeephole(void)
{
	int	i;

	for	(i = 0;  i < numcodes;  i++)	{
		clearidentities(i);
                reducestrength(i);
	}
}


void	clearidentities(int i)
{

	if (addidentity1(i))	{
		intcode[i].opcode = opassn;
		intcode[i].op2 = intcode[i].op3;
		intcode[i].op3 = noop;
	}
	else if (addidentity2(i))	{
		intcode[i].opcode = opassn;
		intcode[i].op3 = noop;
	}
	else if (multidentity1(i))	{
		intcode[i].opcode = opassn;
		intcode[i].op2 = intcode[i].op3;
		intcode[i].op3 = noop;
	}
	else if (multidentity2(i))	{
		intcode[i].opcode = opassn;
		intcode[i].op3 = noop;
	}
	else if (zeromult1(i))	{
		intcode[i].opcode = opassn;
       	        intcode[i].op3 = noop;
	}
	else if (zeromult2(i))	{
		intcode[i].opcode = opassn;
		intcode[i].op2 = intcode[i].op3;
                intcode[i].op3 = noop;
        }
}


void	reducestrength(int i)
{

	if (twotimes(i))	{
		intcode[i].opcode = opadd;
                intcode[i].op2 = intcode[i].op3;
        }
	else if (times2(i))	{
		intcode[i].opcode = opadd;
                intcode[i].op3 = intcode[i].op2;
        }
}

int	addidentity1(int i)
{
	return((intcode[i].opcode == opadd
		&& symclass(intcode[i].op2.opnd) == stliteral 
			&& getvalue(intcode[i].op2.opnd) == 0)? TRUE
							: FALSE);
} 

int	addidentity2(int i)
{
	return(((intcode[i].opcode == opadd || intcode[i].opcode == opsub)
		&& symclass(intcode[i].op3.opnd) == stliteral
		&& getvalue(intcode[i].op3.opnd) == 0)? TRUE : FALSE);
}

int	multidentity1(int i)
{
	return((intcode[i].opcode == opmult
		&& symclass(intcode[i].op2.opnd) == stliteral
			&& getvalue(intcode[i].op2.opnd) == 1)? TRUE
							: FALSE);
}

int	multidentity2(int i)
{
	return(((intcode[i].opcode == opmult || intcode[i].opcode == opdiv)
		&& symclass(intcode[i].op3.opnd) == stliteral
			&& getvalue(intcode[i].op3.opnd) == 1)? TRUE
							: FALSE);
}

int	zeromult1(int i)
{
	return(((intcode[i].opcode == opmult || intcode[i].opcode == opdiv)
		&& symclass(intcode[i].op2.opnd) == stliteral
			&& getvalue(intcode[i].op2.opnd) == 0) ? TRUE
							: FALSE);
}

int	zeromult2(int i)
{
        return((intcode[i].opcode == opmult
		&& symclass(intcode[i].op3.opnd) == stliteral
			&& getvalue(intcode[i].op3.opnd) == 0)? TRUE
							 : FALSE);
}

int	twotimes(int i)
{
	return((intcode[i].opcode == opmult
		&& symclass(intcode[i].op2.opnd) == stliteral
			&& getvalue(intcode[i].op2.opnd) == 2)? TRUE
							 : FALSE);
}

int	times2(int i)
{
	return((intcode[i].opcode == opmult
		&& symclass(intcode[i].op3.opnd) == stliteral
			&& getvalue(intcode[i].op3.opnd) == 2)? TRUE
							 : FALSE);
}

float	getvalue(int	tabindex)
{
	if (data_class(tabindex) == dtinteger)
        	return((float)getivalue(tabindex));
	else
		return(getrvalue(tabindex));       
}

int	intglobal(void)
{
	int	i, j, tempused, tempcopied;

	for (i = 0;  i < numcodes;  i++)
		if (intcode[i].opcode == opassn
			&& symclass(intcode[i].op1.opnd) == sttempvar)	{
			tempcopied = 0;
			for (j = i + 1;  j < numcodes && insamebb(j)
						&& !tempcopied;  j++)	
				tempcopied = copyprop(i, j);
			if (tempcopied == 1)	{
				intcode[j-1].op2 = intcode[i].op2;
				intcode[i].opcode = opnull;
			}
                        else if (tempcopied == 2)	{
				intcode[j-1].op3 = intcode[i].op2;
				intcode[i].opcode = opnull;
			}
                }

	for (i = 0;  i < numcodes;  i++)
		if (intcode[i].opcode == opassn
			&& symclass(intcode[i].op1.opnd) == sttempvar)	{
        		tempused = 0;
			for (j = i + 1;  j < numcodes && !tempused;  j++)
				tempused = codeappears(i, j);
			if (!tempused)	
				intcode[i].opcode = opnull;
		}
	packcode();

	return(numcodes);
}

int	insamebb(int j)
{
	return(intcode[j].opcode < opjump);
}

int	copyprop(int i, int j)
{
	if (intcode[i].op1.opnd == intcode[j].op2.opnd)
		return(1);
	else if (intcode[i].op1.opnd == intcode[j].op3.opnd)
		return(2);
	else
        	return(0);
}

int	codeappears(int i, int j)
{
	return(intcode[j].op1.opnd == intcode[i].op1.opnd
		|| intcode[j].op2.opnd == intcode[i].op1.opnd
		|| intcode[j].op3.opnd == intcode[i].op1.opnd);
}

void	packcode(void)
{
	int	i, j;

	for  (i = 0, j = 0;  i < numcodes;  i++)
		if (intcode[i].opcode != opnull)
			intcode[j++] = intcode[i];
        numcodes = j;
}

void	printintcode(void)
{
	int	i;



	for  (i = 0; i < numcodes; i++)	{
        	printf("%d\t", i);
		switch (intcode[i].opcode)	{
		case opadd: case opsub: case opmult: case opdiv:
			putchar('\t');
			printlexeme(intcode[i].op1.opnd);
			printf(" := ");
			printlexeme(intcode[i].op2.opnd);
			printf(" %s ", opchar[intcode[i].opcode]);
			printlexeme(intcode[i].op3.opnd);
                        putchar('\n');
			break;

		case opassn:
                	putchar('\t');
			printlexeme(intcode[i].op1.opnd);
			printf(" := ");
			printlexeme(intcode[i].op2.opnd);
                        putchar('\n');
			break;

		case opifposzjump: case opifnegzjump: case opifzjump:
		case opifnzjump:
                	putchar('\t');
			printf("if ");
			printlexeme(intcode[i].op1.opnd);
			switch(intcode[i].opcode)	{
			case opifposzjump:	printf(" >="); break;
			case opifnegzjump:	printf(" <="); break;
			case opifnzjump:	printf(" <>"); break;
			case opifzjump:		printf(" ="); break;
			}
			printf(" 0 goto ");
			printlexeme(intcode[i].op2.opnd);
                        putchar('\n');
			break;

		case oplabel:
			printlexeme(intcode[i].op1.opnd);
			printf(":\n");
			break;

		case opfunc:
                	putchar('\t');
			printlexeme(intcode[i].op1.opnd);
			printf(" := ");
			printlexeme(intcode[i].op2.opnd);
			putchar('(');
			printlexeme(intcode[i].op3.opnd);
                        printf(")\n");
			break;

		default:
			putchar('\t');			                		
			printf("%s ", opchar[intcode[i].opcode]);
			if (intcode[i].op1.opndtype != opnnull)	{
				printlexeme(intcode[i].op1.opnd);
				putchar(' ');
                        }

			if (intcode[i].op2.opndtype != opnnull)	{
				printlexeme(intcode[i].op2.opnd);
				putchar(' ');
                        }

			if (intcode[i].op3.opndtype != opnnull)
				printlexeme(intcode[i].op3.opnd);
			putchar('\n');
		}

		if (i%10 == 9)
			getchar();
        }
}

void	printintcode2(void)
{
	int	i;
	for (i = 0;  i < numcodes;  i++)
		printf("%d\t%d  %d\t%d  %d\t%d  %d\n",intcode[i].opcode,
		intcode[i].op1.opnd, intcode[i].op1.opndtype,
		intcode[i].op2.opnd, intcode[i].op2.opndtype,
		intcode[i].op3.opnd, intcode[i].op3.opndtype);

}

void	printquad(int i)
{

       	printf("%d\t", i);
	switch (intcode[i].opcode)	{
	case opadd: case opsub: case opmult: case opdiv:
		putchar('\t');
		printlexeme(intcode[i].op1.opnd);
		printf(" := ");
		printlexeme(intcode[i].op2.opnd);
		printf(" %s ", opchar[intcode[i].opcode]);
		printlexeme(intcode[i].op3.opnd);
                putchar('\n');
		break;

	case opassn:
        putchar('\t');
		printlexeme(intcode[i].op1.opnd);
		printf(" := ");
		printlexeme(intcode[i].op2.opnd);
		putchar('\n');
		break;

	case opifposzjump: case opifnegzjump: case opifzjump:
	case opifnzjump:
        putchar('\t');
		printf("if ");
		printlexeme(intcode[i].op1.opnd);
		switch(intcode[i].opcode)	{
		case opifposzjump:	printf(" >="); break;
		case opifnegzjump:	printf(" <="); break;
		case opifnzjump:	printf(" <>"); break;
		case opifzjump:		printf(" ="); break;
		}
		printf(" 0 goto ");
		printlexeme(intcode[i].op2.opnd);
		putchar('\n');
		break;

	case oplabel:
		printlexeme(intcode[i].op1.opnd);
		printf(":\n");
		break;

	case opfunc:
        putchar('\t');
		printlexeme(intcode[i].op1.opnd);
		printf(" := ");
		printlexeme(intcode[i].op2.opnd);
		putchar('(');
		printlexeme(intcode[i].op3.opnd);
		printf(")\n");
		break;

	default:
		putchar('\t');			                		
		printf("%s ", opchar[intcode[i].opcode]);
		if (intcode[i].op1.opndtype != opnnull)	{
			printlexeme(intcode[i].op1.opnd);
			putchar(' ');
		}

		if (intcode[i].op2.opndtype != opnnull)	{
			printlexeme(intcode[i].op2.opnd);
			putchar(' ');
		}

		if (intcode[i].op3.opndtype != opnnull)
			printlexeme(intcode[i].op3.opnd);
			putchar('\n');
		}

}

int	getprogstart(void)
{
	int	i;

	for  (i = 0;  intcode[i].opcode != oplabel
			|| intcode[i].op1.opnd != NUMTOKENS+1; i++)
		;
	return(i+1);
}
