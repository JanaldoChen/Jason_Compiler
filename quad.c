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

/*
 * setaddress() -	Given the operand type and the index to the
 *			attribute table, this function sets the addressrec
 *			structure for this entry.
 */
struct addressrec	setaddress(enum operandtype otype, int tabindex)
{
	struct addressrec	a;

	a.opndtype = otype;
	a.opnd = tabindex;
	return(a);
}


/*
 * gettempvar() -	Creates a temporary variable name beginning with
 *			"$_" in numeric sequence.  The variable type depends
 *			on otype, passed as an argument.
 */
struct addressrec gettempvar(enum datatype otype)
{
	struct addressrec	result;
	char		tempname[10], numtempchar[10];
	int		tabindex;


	/*
	 *	Create the name of the temporary variable beginning
	 *	with "$_" and then containing a number in sequence
	 *	as a character string.
	 */ 
	strcpy(tempname, "temp_");
	//itoa(numtemps++, numtempchar, 10);
    sprintf(numtempchar, "%d", numtemps++);
        strcat(tempname, numtempchar);

	/*	Install the name in the symbol table and give the
	 *	proper attribute as a variable of the type passed
	 *	to the function.
         */
	installname(tempname, &tabindex);
	setattrib(stunknown, tokidentifier, tabindex);
	installdatatype(tabindex, sttempvar, otype);

	/*
	 *	Return the address of the temporary variable,
	 *	including its index and that it is a variable.
         */
	result.opnd = tabindex;
	result.opndtype = opntempvar;
	return(result);
}

/*
 * gettemplabel() -	Creates a temporary label with its name beginning
 *			with "!_" in numeric sequence.
 */
struct addressrec gettemplabel(void)
{
	struct addressrec	result;
	char		tempname[10], numtempchar[10];
	int		tabindex;
        static int	numtemps = 0;

	/*
	 *	Create the name of the temporary label beginning
	 *	with "!_" and then containing a number in sequence
	 *	as a character string.
	 */ 
	strcpy(tempname, "label_");
	//itoa(numtemps++, numtempchar, 10);
    sprintf(numtempchar, "%d", numtemps++);
    strcat(tempname, numtempchar);

	/*	Install the name in the symbol table and give the
	 *	proper attribute as a label without a data type. 
	 */
	installname(tempname, &tabindex);
	setattrib(stunknown, tokidentifier, tabindex);
	installdatatype(tabindex, stlabel, dtnone);

	/*
	 *	Return the address of the temporary label, 
	 *	including its index and that it is an address.
         */
	result.opnd = tabindex;
	result.opndtype = opnaddr;
        return(result);
}


/*
 * genquad() -	generates a quadruple using the opcode passed to it and
 *		and three addressreces passed to it, any of which may be
 *		null.		     
 */
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


/*
 * permtarget() -	A "patch" function which replaces the temporary
 *			variable which is the destination of the last
 *			instruction with a permanent user-assigned
 *			variable.
 */
void permtarget(struct addressrec a)	{
	intcode[--numcodes-1].op1 = a;
	--numtemps;
}

/*
 * getquad() - 	Returns a single instruction in quadruple form
 */
 struct quadtype	getquad(int i)
 {
	const struct quadtype	nullquad = {opnull, {opnnull, NULL},
					{opnnull, NULL}, { opnnull, NULL}};
 	if (i < numcodes)
 		return(intcode[i]);
 }

/*
 * getnumcodes() -	Returns the number of intermediate code instructions
 */
int	getnumcodes(void)
{
	return(numcodes);
}

/*
 * intpeephole() -	Performs "peephole" optimization on the
 *			intermediate code generated, replacing identity
 *			arithmetic operations with assignments and
 *			performing reduction in strength optimizations.
 */
void	intpeephole(void)
{
	int	i;

	for	(i = 0;  i < numcodes;  i++)	{
		clearidentities(i);
                reducestrength(i);
	}
}


/*
 * clearidentities() -	Optimizes the programs by replacing addition
 *			by zero and multiplication by one with assignment
 *			statements and multiplication by zero with
 *			assignment of zero.
 */
void	clearidentities(int i)
{
/*	int	tabindex;*/

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


/*
 * reducestrength() -	Performs the reduction by strength optimization
 *			replacing y = 2 * x with y = x + x
 */
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

/*
 * addidentity1() -	Returns true if the first operand of addition
 *			is 0,  false if it is any other value.
 */
int	addidentity1(int i)
{
	return((intcode[i].opcode == opadd
		&& symclass(intcode[i].op2.opnd) == stliteral 
			&& getvalue(intcode[i].op2.opnd) == 0)? TRUE
							: FALSE);
} 

/*
 * addidentity2() -	Returns true if the second operand of addition
 *			is 0, false if it is any other value.
 */
int	addidentity2(int i)
{
	return(((intcode[i].opcode == opadd || intcode[i].opcode == opsub)
		&& symclass(intcode[i].op3.opnd) == stliteral
		&& getvalue(intcode[i].op3.opnd) == 0)? TRUE : FALSE);
}

/*
 * multidentity1() -	Returns true if the first operand of
 *			multiplication is 1,
 *			false if it is any other value.
 */
int	multidentity1(int i)
{
	return((intcode[i].opcode == opmult
		&& symclass(intcode[i].op2.opnd) == stliteral
			&& getvalue(intcode[i].op2.opnd) == 1)? TRUE
							: FALSE);
}

/*
 * multidentity2() -	Returns true if the second operand of
 *			multiplication is 1,
 *			false if it is any other value.
 */
int	multidentity2(int i)
{
	return(((intcode[i].opcode == opmult || intcode[i].opcode == opdiv)
		&& symclass(intcode[i].op3.opnd) == stliteral
			&& getvalue(intcode[i].op3.opnd) == 1)? TRUE
							: FALSE);
}

/*
 * zeromult1() -	Returns true if the first operand of
 *			multiplication is 0,
 *			false if it is any other value.
 */
int	zeromult1(int i)
{
	return(((intcode[i].opcode == opmult || intcode[i].opcode == opdiv)
		&& symclass(intcode[i].op2.opnd) == stliteral
			&& getvalue(intcode[i].op2.opnd) == 0) ? TRUE
							: FALSE);
}

/*
 * zeromult2() -	Returns true if the second operand of
 *			multiplication is 0,
 *			false if it is any other value.
 */
int	zeromult2(int i)
{
        return((intcode[i].opcode == opmult
		&& symclass(intcode[i].op3.opnd) == stliteral
			&& getvalue(intcode[i].op3.opnd) == 0)? TRUE
							 : FALSE);
}

/*
 * twotimes() -	Returns true if the first operand of
 *			multiplication is 2,
 *			false if it is any other value.
 */
int	twotimes(int i)
{
	return((intcode[i].opcode == opmult
		&& symclass(intcode[i].op2.opnd) == stliteral
			&& getvalue(intcode[i].op2.opnd) == 2)? TRUE
							 : FALSE);
}

/*
 * times2() -	Returns true if the second operand of
 *			multiplication is 2,
 *			false if it is any other value.
 */
int	times2(int i)
{
	return((intcode[i].opcode == opmult
		&& symclass(intcode[i].op3.opnd) == stliteral
			&& getvalue(intcode[i].op3.opnd) == 2)? TRUE
							 : FALSE);
}

/*
 * getvalue() -	Gets the values stored at a particular entry in the
 *		attribute table, converting it to float if it is integer.
 */
float	getvalue(int	tabindex)
{
	if (data_class(tabindex) == dtinteger)
        	return((float)getivalue(tabindex));
	else
		return(getrvalue(tabindex));       
}

/*
 * intglobal() -	Performs "global" optmizations.
 *			First eliminates copy propagation of temporary
 *			variables in all the basic blocks.
 *			Then eliminates temporary variables that are
 *			assigned values but are never used.
 *			Lastly, it packs the intermediate code,
 *			eliminating the null instructions created in
 *			these first two operations.
 */
int	intglobal(void)
{
	int	i, j, tempused, tempcopied;

	for (i = 0;  i < numcodes;  i++)
		/*  If a temporary variable is assigned a value,
			eliminate copy propagation with the
                        basic block.  */
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
		/*  If a temporary variable is assigned a value that
                	is never used, eliminate the instruction */
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

/*
 * insamebb() -	Returns true if the intermediate instruction is not
 *		one that indicates the beginning of the
 *		next basic block, returns false if it is.    
 */
int	insamebb(int j)
{
	return(intcode[j].opcode < opjump);
}

/*
 * copyprop() - If intermediate instructions i and j are an example of
 *		copy propagation, it returns 1 for the second operand
 *		and 2 for the third operand so the global optmizing
 *		function knows which operand to replace;
 *		otherwise it returns 0.
 */
int	copyprop(int i, int j)
{
	if (intcode[i].op1.opnd == intcode[j].op2.opnd)
		return(1);
	else if (intcode[i].op1.opnd == intcode[j].op3.opnd)
		return(2);
	else
        	return(0);
}

/*
 * codeappears() -	Returns true if the temporary variable set in
 *			intermediate instruction i is used in intermediate
 *			instruction j. 
 */
int	codeappears(int i, int j)
{
	return(intcode[j].op1.opnd == intcode[i].op1.opnd
		|| intcode[j].op2.opnd == intcode[i].op1.opnd
		|| intcode[j].op3.opnd == intcode[i].op1.opnd);
}

/*
 * packcode() - "Packs" the intermediate code, eliminating the
 *		null instructions created when eliminating copy
 *		propagation and unused temporary variables.
 */
void	packcode(void)
{
	int	i, j;

	for  (i = 0, j = 0;  i < numcodes;  i++)
		if (intcode[i].opcode != opnull)
			intcode[j++] = intcode[i];
        numcodes = j;
}

/*
 * printintcode() -	Prints the intermediate code program in a form
 *			that users can understand more easily than
 *			intermediate representation.  
 */
void	printintcode(void)
{
	int	i;



	for  (i = 0; i < numcodes; i++)	{
        	printf("%d\t", i);
		switch (intcode[i].opcode)	{
		case opadd: case opsub: case opmult: case opdiv:
		/* Print these operations in the form of
				an assignment statement */
			putchar('\t');
			printlexeme(intcode[i].op1.opnd);
			printf(" := ");
			printlexeme(intcode[i].op2.opnd);
			printf(" %s ", opchar[intcode[i].opcode]);
			printlexeme(intcode[i].op3.opnd);
                        putchar('\n');
			break;

		case opassn:
                 	/* Print this in the form a := b */
                	putchar('\t');
			printlexeme(intcode[i].op1.opnd);
			printf(" := ");
			printlexeme(intcode[i].op2.opnd);
                        putchar('\n');
			break;

		case opifposzjump: case opifnegzjump: case opifzjump:
		case opifnzjump:
			/* Print these in the form
                        	if condition  then goto address */
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
			/* Print this in the form
                        	Label:			*/
			printlexeme(intcode[i].op1.opnd);
			printf(":\n");
			break;

		case opfunc:
			/* Print this in the form
                        	x := f(y)	*/
                	putchar('\t');
			printlexeme(intcode[i].op1.opnd);
			printf(" := ");
			printlexeme(intcode[i].op2.opnd);
			putchar('(');
			printlexeme(intcode[i].op3.opnd);
                        printf(")\n");
			break;

		default:
			/* Print these in the form
                        	opcode op1 op2 op3 */
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

                /* Pause every tenth line */
		if (i%10 == 9)
			getchar();
        }
}

/*
 * printintcode2() -	Prints the intermediate code program in a form
 *			that facilitates debugging.
 */
void	printintcode2(void)
{
	int	i;
	for (i = 0;  i < numcodes;  i++)
		printf("%d\t%d  %d\t%d  %d\t%d  %d\n",intcode[i].opcode,
		intcode[i].op1.opnd, intcode[i].op1.opndtype,
		intcode[i].op2.opnd, intcode[i].op2.opndtype,
		intcode[i].op3.opnd, intcode[i].op3.opndtype);

}

/*
 * printquad() -	Prints the quadruple instruction indicated by
 *			number.  Used for debugging purposes.
 */
void	printquad(int i)
{

       	printf("%d\t", i);
	switch (intcode[i].opcode)	{
	case opadd: case opsub: case opmult: case opdiv:
	/* Print these operations in the form of
			an assignment statement */
		putchar('\t');
		printlexeme(intcode[i].op1.opnd);
		printf(" := ");
		printlexeme(intcode[i].op2.opnd);
		printf(" %s ", opchar[intcode[i].opcode]);
		printlexeme(intcode[i].op3.opnd);
                putchar('\n');
		break;

	case opassn:
               	/* Print this in the form a := b */
               	putchar('\t');
		printlexeme(intcode[i].op1.opnd);
		printf(" := ");
		printlexeme(intcode[i].op2.opnd);
		putchar('\n');
		break;

	case opifposzjump: case opifnegzjump: case opifzjump:
	case opifnzjump:
		/* Print these in the form
                       	if condition  then goto address */
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
		/* Print this in the form
                       	Label:			*/
		printlexeme(intcode[i].op1.opnd);
		printf(":\n");
		break;

	case opfunc:
		/* Print this in the form
                       	x := f(y)	*/
               	putchar('\t');
		printlexeme(intcode[i].op1.opnd);
		printf(" := ");
		printlexeme(intcode[i].op2.opnd);
		putchar('(');
		printlexeme(intcode[i].op3.opnd);
		printf(")\n");
		break;

	default:
		/* Print these in the form
                       	opcode op1 op2 op3 */
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
