#include	"object.h"
#include	"quad.h"

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>

#define		FILENAMELENGTH		100

enum	opcodetype	{opcmov, opcadd, opcsub, opcimul, opcidiv, opcinc,
			opcdec, opcneg, opcsahf, opcjmp, opcjno, opcjne,
			opcje, opcjz, opcjp, opcjc,
			opcjl, opcjle, opcjg, opcjge, opccmp, opccall,
			opcret, opcpush, opcpop, opcfinit, opcfadd,
			opcfsub, opcfmul, opcfdiv, opcfld, opcfild,
			opcfst, opcfist, opcftst, opcfxam, opcfstsw,
			opcfwait
};

const char	*opcodestr[] = 	{"mov", "add", "sub", "imul", "idiv", "inc",
			"dec", "neg", "sahf", "jmp", "jno", "jne", "je",
			"jz", "jp", "jc","jl",
			"jle", "jg", "jge", "cmp", "call", "ret", "push",
			"pop", "finit", "fadd", "fsub", "fmul", "fdiv",
			"fld", "fild", "fst", "fist", "ftst", "fxam",
			"fstsw", "fwait"
};

FILE	*ofp;
int	ioverflow = NO, divby0 = NO;

void	writestack(void);
void	writedata(void);
void	writecode(int numcodes);
void	writeprocs(void);
void	writeproc(int i, int totalbytes);

void	emitone(enum opcodetype opcode, int tabindex);
void	emitoffset(enum opcodetype opcode, char *target, int tabindex);
void	emitlabel(enum opcodetype opcode, char *label);
void	emittoone(enum opcodetype opcode, int tabindex, char *label);
void	emitfromone(enum opcodetype opcode, char *label, int tabindex);

void	translate(struct quadtype nextquad);
void	transadd(struct quadtype nextquad);
void	transsub(struct quadtype nextquad);
void	transneg(struct quadtype nextquad);
void	transmult(struct quadtype nextquad);
void	transdiv(struct quadtype nextquad);
void	transassn(struct quadtype nextquad);
void	transjump(struct quadtype nextquad);
void	transifjump(struct quadtype nextquad);
void	transifposz(struct quadtype nextquad);
void	transifpos(struct quadtype nextquad);
void	transifnegz(struct quadtype nextquad);
void	transifneg(struct quadtype nextquad);
void	transifz(struct quadtype nextquad);
void	transifnz(struct quadtype nextquad);
void	translabel(struct quadtype nextquad);
void	transread(struct quadtype nextquad);
void	transwrite(struct quadtype nextquad);
void	transarg(struct quadtype nextquad);
void	transproc(struct quadtype nextquad);

void	writeaddovr(void);
void	writedivby0(void);
void	getjumplabel(char *jumplabel);

void	writeobject(char name[], int numcodes)
{
	char	filename[FILENAMELENGTH];

	strcpy(filename, name);
	strcat(filename, ".asm");

	if ((ofp = fopen(filename, "w")) == NULL)	{
		printf("Cannot open %s\n", filename);
                exit(1);
	}

	writestack();
	writedata();
	writecode(numcodes);
	fclose(ofp);        
}

void	writestack(void)
{
	fprintf(ofp, "%s\n\n%s\n%s\n%s\n\n",
		"DOSSEG",
		"_STACK SEGMENT para stack \'stack\'",
		"              dw      1000 dup(?)",
                "_STACK ENDS");
}

void	writedata(void)
{
	int	i, datasize;
        float	litvalue;
        char	label[LABELSIZE];

	fprintf(ofp, "_DATA SEGMENT word public \'data\'\n");
        fprintf(ofp, "TestResult dw  ?\n"); 

	for (i = NUMTOKENS+2; i < tablesize();  i++)	{
		if ((symclass(i) == sttempvar || symclass(i) == stvariable)
				&& getproc(i) == NUMTOKENS+1)	{
                	printlexeme(i);getchar();
			getlabel(i, label);
			if (data_class(i) == dtinteger)
				fprintf(ofp, "%-10s     dw     ?\n", label);
			else
				fprintf(ofp, "%-10s     dd     ?\n", label);
		}
		else if (symclass(i) == stliteral
				&& data_class(i) == dtreal)	{
			printlexeme(i);getchar();
				getlabel(i, label);
                                litvalue = getrvalue(i);
				fprintf(ofp, "%-10s     dd     %f\n",
						label, litvalue);     
                }
	} 

	fprintf(ofp, "_DATA ENDS\n\n");
}

void	writeprocs(void)
{
	int	i = 0, localvarbytes;
        char	label[LABELSIZE];
        struct quadtype		nextquad;


	/*  Scan the intermediate code looking for each procedure
        	contained within the program */
	for (i = 0; i < getnumcodes(); i++)	{   
		nextquad = getquad(i);

		/*
		 *	Get the next intermediate code instruction
		 *	containing a procedure's initial label
		 *	Stop if there are no more procedures
		 */
		while ((nextquad.opcode != oplabel
			  || symclass(nextquad.op1.opnd) != stprocedure)
				&& symclass(nextquad.op1.opnd) != stprogram)	
			nextquad = getquad(++i);


		/*  If we reached the main program's label,
			there are no more procedures */ 
		if (symclass(nextquad.op1.opnd) == stprogram)
			break;

		/*
		 *	As long as we haven't reached the beginning of
		 *	the main program, label the parameters and the
		 *	local variables, and generate code for the
		 *	procedure
		 */
		localvarbytes = labelscope(nextquad.op1.opnd);
                printf("Used %d bytes\n", localvarbytes);
		getlabel(nextquad.op1.opnd, label);
                writeproc(i, localvarbytes);
	}
}

void	writeproc(int i, int totalbytes)
{
	struct	quadtype	nextquad;

	nextquad = getquad(i);
	fprintf(ofp, "%s\n",	"_TEXT SEGMENT");
        fprintlexeme(ofp, nextquad.op1.opnd);
	fprintf(ofp, ":\n\n");
	fprintf(ofp, "%s\n%s\n\n",
		     "         push      bp",
		     "         mov       bp, sp");

        if (totalbytes != 0)	{
		fprintf(ofp, "; Allocate space for local variables\n");
		fprintf(ofp, "         sub       sp, %d\n", totalbytes);
        }
	nextquad = getquad(++i);
	while (nextquad.opcode != opreturn)	{
		translate(nextquad);
		nextquad = getquad(++i);
	}

        if (totalbytes != 0)	{
		fprintf(ofp, "; Return space used by local variables\n");
		fprintf(ofp, "         add       sp, %d\n", totalbytes);
        }

	fprintf(ofp, "%s\n%s\n\n",
		     "         pop       bp",
		     "         ret       2");
	fprintf(ofp, "_TEXT ENDS\n\n\n");
}

void	writecode(int numcodes)
{
	int			i;
        char			label[LABELSIZE];
        struct quadtype		nextquad;
	
	fprintf(ofp, "%s\n%s\n\n",
		"_TEXT SEGMENT word public \'code\'",
		"         assume    cs: _TEXT");

        printf("Let\'s get started\n");
        getlabel(NUMTOKENS+1, label);
	fprintf(ofp, "%s%c\n\n%s\n%s\n%s\n%s\n\n",
		label,':',
		"; set up the ds register so that it points to _DATA",
		"         mov       ax, seg _DATA",
		"         mov       ds, ax",
                "         assume    ds: _DATA");

	i = getprogstart();
		 
	while (i <= numcodes)	{
		nextquad = getquad(i++);
		translate(nextquad);
	}        	

	if (ioverflow)
		writeaddovr();	

	if (divby0)
		writedivby0();
	fprintf(ofp, "%s\n%s\n%s\n\n%s\n\n",
                "; exit to DOS",
		"         mov       ax, 4C00h",
                "         int       21h",
		"_TEXT ENDS");

	writeprocs();
        fprintf(ofp, "END %s\n", label);
}

void	translate(struct quadtype nextquad)
{
	switch(nextquad.opcode)	{
	  case opadd:		transadd(nextquad);  break;
	  case opsub:		transsub(nextquad);  break;
	  case opneg:		transneg(nextquad);  break;
	  case opmult:		transmult(nextquad); break;
          case opdiv:		transdiv(nextquad); break;
	  case opassn:  	transassn(nextquad);  break;
 	  case opjump:		transjump(nextquad); break;
	  case opifposzjump:	
	  case opifposjump:	
	  case opifnegzjump:	
	  case opifnegjump:	
	  case opifzjump:	
	  case opifnzjump:	transifjump(nextquad); break;

	  case oplabel:		translabel(nextquad); break;
	  case opread:		transread(nextquad); break;
	  case opwrite:		transwrite(nextquad); break;
	  case oparg:		transarg(nextquad); break;
	  case opproc:		transproc(nextquad); break;
        }
}

void	emitone(enum opcodetype opcode, int tabindex)
{
	char	label[LABELSIZE];

	getlabel(tabindex, label);
        fprintf(ofp, "         %-10s%s\n", opcodestr[opcode], label);
}

void	emitoffset(enum opcodetype opcode, char *target, int tabindex)
{
	char	label[LABELSIZE];

	getlabel(tabindex, label);
	fprintf(ofp, "         %-10s%s, offset ", opcodestr[opcode],
					target);

	if (strncmp(label, "[bp", 3 == 0))	{
		if  (data_class(tabindex) == dtreal)
			putc('d', ofp);
		fprintf(ofp, "word ptr ");
	}
        fprintf(ofp, "%s\n", label);
}

void	emitlabel(enum opcodetype opcode, char *label)
{
	fprintf(ofp, "         %-10s%s\n", opcodestr[opcode], label);    
}

void	emittoone(enum opcodetype opcode, int tabindex, char *label)
{
	char	target[LABELSIZE];

	getlabel(tabindex, target);
	if (strncmp(target, "[bp+", 4) == 0)	{
		fprintf(ofp, "         mov       bx, ");
		if (data_class(tabindex) == dtreal)
			putc('d', ofp);
		fprintf(ofp, "word ptr %s\n", target);
		strcpy(target, "[bx]");		 
        }
	fprintf(ofp, "         %-10s", opcodestr[opcode]);
	if (strncmp(target, "[bp", 3) == 0)	{
		if (data_class(tabindex) == dtreal)
			putc('d', ofp);
                fprintf(ofp, "word ptr ");
	}

	fprintf(ofp, "%s, %s\n", target, label);
}

void	emitfromone(enum opcodetype opcode, char *label, int tabindex)
{
	char	source[LABELSIZE];

	getlabel(tabindex, source);
	if (strncmp(source, "[bp+", 4) == 0)	{
		fprintf(ofp, "         mov       bx, ");
		if (data_class(tabindex) == dtreal)
			putc('d', ofp);
		fprintf(ofp, "word ptr %s\n", source);
		strcpy(source, "[bx]");		 
        }
	fprintf(ofp, "         %-10s%s, ", opcodestr[opcode], label);
	if (strncmp(source, "[bp", 3) == 0)	{
		if (data_class(tabindex) == dtreal)
			putc('d', ofp);
                fprintf(ofp, "word ptr ");
	}

	fprintf(ofp, "%s\n", source);
}

void	transadd(struct quadtype nextquad)
{
	char	label[LABELSIZE];

	fprintf(ofp, "; ");
	fprintlexeme(ofp, nextquad.op1.opnd);
	fprintf(ofp, " := ");
	fprintlexeme(ofp, nextquad.op2.opnd);
	fprintf(ofp, " + ");
	fprintlexeme(ofp, nextquad.op3.opnd);
	fprintf(ofp, "\n");

	if (data_class(nextquad.op1.opnd) == dtinteger)	{
        	getjumplabel(label);
		if (getivalue(nextquad.op2.opnd) == 1
			&& nextquad.op1.opnd == nextquad.op3.opnd
			|| getivalue(nextquad.op3.opnd) == 1
			&& nextquad.op1.opnd == nextquad.op2.opnd)	{
				emitone(opcinc, nextquad.op1.opnd);
				emitlabel(opcjno, label);
		}
                else	{
			emitfromone(opcmov, "ax", nextquad.op2.opnd);
			emitfromone(opcadd, "ax", nextquad.op3.opnd);
			emitlabel(opcjno, label);
			emittoone(opcmov, nextquad.op1.opnd, "ax");
		}
		emitlabel(opcjmp, "iovrflo");
		fprintf(ofp, "%s:\n\n", label);         
	}
	else	{
		getjumplabel(label);
		fprintf(ofp, "         finit\n");
		emitone(opcfld, nextquad.op2.opnd);
		emitone(opcfadd, nextquad.op3.opnd);
		fprintf(ofp, "         fxam\n");
		emitlabel(opcfstsw, "TestResult");
		emitone(opcfst, nextquad.op1.opnd);
		fprintf(ofp,"%s\n%s\n%s\n\n", 
			"         fwait",
			"         mov       ax, TestResult",
                        "         sahf");
		emitlabel(opcjc, label);
		emitlabel(opcjmp, "iovrflo");
                putc('\n', ofp);
        }
	ioverflow = YES;
}

void	transsub(struct quadtype nextquad)
{
	char	label[LABELSIZE];

	fprintf(ofp, "; ");
	fprintlexeme(ofp, nextquad.op1.opnd);
	fprintf(ofp, " := ");
	fprintlexeme(ofp, nextquad.op2.opnd);
	fprintf(ofp, " - ");
	fprintlexeme(ofp, nextquad.op3.opnd);
	fprintf(ofp, "\n");

	if (data_class(nextquad.op1.opnd) == dtinteger)	{
        	getjumplabel(label);
		if (getivalue(nextquad.op2.opnd) == 1
			&& nextquad.op1.opnd == nextquad.op3.opnd
			|| getivalue(nextquad.op3.opnd) == 1
			&& nextquad.op1.opnd == nextquad.op2.opnd)	{
				emitone(opcdec, nextquad.op1.opnd);
				emitlabel(opcjno, label);
		}
                else	{
        		emitfromone(opcmov, "ax", nextquad.op2.opnd);
			emitfromone(opcsub, "ax", nextquad.op3.opnd);
			emitlabel(opcjno, label);
			emittoone(opcmov, nextquad.op1.opnd, "ax");
		}
		emitlabel(opcjmp, "iovrflo");			
		fprintf(ofp, "%s:\n\n", label);         
	}
	else	{
		getjumplabel(label);
		fprintf(ofp, "         finit\n");
		emitone(opcfld, nextquad.op2.opnd);
		emitone(opcfsub, nextquad.op3.opnd);
		fprintf(ofp, "         fxam\n");
		emitlabel(opcfstsw, "TestResult");
		emitone(opcfst, nextquad.op1.opnd);
		fprintf(ofp,"%s\n%s\n%s\n\n", 
			"         fwait",
			"         mov       ax, TestResult",
                        "         sahf");
		emitlabel(opcjc, label);
		emitlabel(opcjmp, "iovrflo");
                putc('\n', ofp);
        }
	ioverflow = YES;
}

void	transneg(struct quadtype nextquad)
{
	char	label[LABELSIZE];

	if (data_class(nextquad.op1.opnd) == dtinteger)	{
		getjumplabel(label);
		fprintf(ofp, "; ");
		fprintlexeme(ofp, nextquad.op1.opnd);
		fprintf(ofp, " := - ");
		fprintlexeme(ofp, nextquad.op1.opnd);
		fprintf(ofp, "\n");

		emitone(opcneg, nextquad.op1.opnd);
		emitlabel(opcjno, label);
		emitlabel(opcjmp, "iovrflo");
                fprintf(ofp, "%s:\n\n", label);
		ioverflow = YES;
        }
}

void	transmult(struct quadtype nextquad)
{
	char	label[LABELSIZE];

	fprintf(ofp, "; ");
	fprintlexeme(ofp, nextquad.op1.opnd);
        fprintf(ofp, " := ");
	fprintlexeme(ofp, nextquad.op2.opnd);
	fprintf(ofp, " * ");
	fprintlexeme(ofp, nextquad.op3.opnd);
	fprintf(ofp, "\n");

	if (data_class(nextquad.op1.opnd) == dtinteger)	{
		getjumplabel(label);

		emitfromone(opcmov, "ax", nextquad.op2.opnd);
		emitone(opcimul, nextquad.op3.opnd);
		emitlabel(opcjno, label);
		emitlabel(opcjmp, "iovrflo");

                fprintf(ofp, "%s:\n", label); 
		emittoone(opcmov, nextquad.op1.opnd, "ax");
                fprintf(ofp, "\n");
                ioverflow = YES;
	}
	else	{
		getjumplabel(label);
	
		fprintf(ofp, "         finit\n");
		emitone(opcfld, nextquad.op2.opnd);
		emitone(opcfmul, nextquad.op3.opnd);
		fprintf(ofp, "         fxam\n");
		emitlabel(opcfstsw, "TestResult");
		emitone(opcfst, nextquad.op1.opnd);
		fprintf(ofp,"%s\n%s\n%s\n\n", 
			"         fwait",
			"         mov       ax, TestResult",
                        "         sahf");
		emitlabel(opcjc, label);
		emitlabel(opcjmp, "iovrflo");
                putc('\n', ofp);
        }

}

void	transdiv(struct quadtype nextquad)
{
	char	label[LABELSIZE];

	fprintf(ofp, "; ");
	fprintlexeme(ofp, nextquad.op1.opnd);
        fprintf(ofp, " := ");
	fprintlexeme(ofp, nextquad.op2.opnd);
        fprintf(ofp, " / ");
	fprintlexeme(ofp, nextquad.op3.opnd);
	fprintf(ofp, "\n");
	if (data_class(nextquad.op1.opnd) == dtinteger)	{
        	getjumplabel(label);

		emittoone(opccmp, nextquad.op3.opnd, "0");
		emitlabel(opcjne, label);
		emitlabel(opcjmp, "divby0");
                divby0 = YES;
		fprintf(ofp, "%s:\n", label);

                getjumplabel(label);
		emitfromone(opcmov, "ax", nextquad.op2.opnd);
		emitlabel(opcmov, "dx, 0");
		emitone(opcidiv, nextquad.op3.opnd);
		emitlabel(opcjno, label);
		emitlabel(opcjmp, "iovrflo");
		fprintf(ofp, "%s:\n", label);
		emittoone(opcmov, nextquad.op1.opnd, "ax");
                fprintf(ofp, "\n");
                ioverflow = YES;
        }
}


void	transassn(struct quadtype nextquad)
{
	char	target[LABELSIZE], source[LABELSIZE];
	getlabel(nextquad.op1.opnd, target);
	getlabel(nextquad.op2.opnd, source);
	fprintf(ofp, "; ");
	fprintlexeme(ofp, nextquad.op1.opnd);
	fprintf(ofp, " := ");
	fprintlexeme(ofp, nextquad.op2.opnd);
	fprintf(ofp, "\n");

	if (data_class(nextquad.op1.opnd) == dtinteger)	{
        	emitfromone(opcmov, "ax", nextquad.op2.opnd);
		emittoone(opcmov, nextquad.op1.opnd, "ax");
	}
	else	{
		fprintf(ofp, "         finit\n");
		emitone(opcfld, nextquad.op2.opnd);
		emitone(opcfst, nextquad.op3.opnd);
		fprintf(ofp, "         fwait\n");
        }
}

void	transjump(struct quadtype nextquad)
{
	char	label[LABELSIZE];

	getlabel(nextquad.op1.opnd, label);
	fprintf(ofp, "; goto %s\n", label);
	fprintf(ofp, "         jmp        %s\n\n", label);
}

void	transifjump(struct quadtype nextquad)
{
	char	label1[LABELSIZE], label2[LABELSIZE];

	getjumplabel(label2);
	getlabel(nextquad.op2.opnd, label1);

	fprintf(ofp, "; if ");
	fprintlexeme(ofp, nextquad.op1.opnd);
	switch(nextquad.opcode)	{
	  case opifposzjump:	fprintf(ofp, " >=");  break;
	  case opifposjump:	fprintf(ofp, " >");  break;
	  case opifnegzjump:	fprintf(ofp, " <=");  break;
	  case opifnegjump:	fprintf(ofp, " <");  break;
	  case opifzjump:	fprintf(ofp, " =");  break;
	  case opifnzjump:	fprintf(ofp, " !=");  break;
        }
	fprintf(ofp, " 0 goto %s\n", label1);

	emittoone(opccmp, nextquad.op1.opnd, "0");
	switch(nextquad.opcode)	{
	  case opifposzjump:
	  	emitlabel(opcjl, label2); break;
	  case opifposjump:
	  	emitlabel(opcjle, label2); break;
	  case opifnegzjump:
		emitlabel(opcjg, label2); break;
	  case opifnegjump:
		emitlabel(opcjge, label2); break;
	  case opifzjump:
		emitlabel(opcjne, label2);  break;
	  case opifnzjump:
		emitlabel(opcje, label2); break;
        }

	emitlabel(opcjmp, label1);
	fprintf(ofp, "%s:\n\n", label2);
}

void	translabel(struct quadtype nextquad)
{
	char	target[LABELSIZE];

	getlabel(nextquad.op1.opnd, target);
	fprintf(ofp, "%s:\n\n", target);
}

void	transread(struct quadtype nextquad)
{
	if (data_class(nextquad.op1.opnd) == dtinteger)	{
		fprintf(ofp, "; read ");
		fprintlexeme(ofp, nextquad.op1.opnd);
                fprintf(ofp, "\n");

		emitlabel(opccall, "readint");
		emittoone(opcmov, nextquad.op1.opnd, "ax");
                fprintf(ofp, "\n");
        }
}

void	transwrite(struct quadtype nextquad)
{
	if (data_class(nextquad.op1.opnd) == dtinteger)	{
		fprintf(ofp, "; write ");
		fprintlexeme(ofp, nextquad.op1.opnd);
                fprintf(ofp, "\n");

		emitfromone(opcmov, "ax", nextquad.op1.opnd);
		emitlabel(opccall, "writeint");
		fprintf(ofp, "\n");
        }
}

void	transarg(struct quadtype nextquad)
{
	char	target[LABELSIZE];

	getlabel(nextquad.op1.opnd, target);
	fprintf(ofp, "; arg ");
	fprintlexeme(ofp, nextquad.op1.opnd);
	fprintf(ofp, "\n");
        emitoffset(opcmov, "ax", nextquad.op1.opnd);
	emitlabel(opcpush, "ax");
        fprintf(ofp, "\n");
}

void	transproc(struct quadtype nextquad)
{
	char	target[LABELSIZE];

	getlabel(nextquad.op1.opnd, target);
	fprintf(ofp, "; call ");
	fprintlexeme(ofp, nextquad.op1.opnd);
	fprintf(ofp, "\n");
	emitone(opccall, nextquad.op1.opnd);
	fprintf(ofp, "\n");
}

void	writeaddovr(void)
{
	fprintf(ofp, "%s\n%s\n%s\n%s\n\n",
		"; if there is integer overflow, abend to DOS",
                "iovrflo:",
		"         mov       ax, 4C01h",
                "         int       21h");
}

void	writedivby0(void)
{
	fprintf(ofp, "%s\n%s\n%s\n%s\n\n",
		"; if there is division by zero, abend to DOS",
                "divby0:",
		"         mov       ax, 4C02h",
                "         int       21h");
}

void	getjumplabel(char *jumplabel)
{
	static int	jlnumber = 0;
	char		jlnumstring[5];

	strcpy(jumplabel, "Jump");
    //itoa(jlnumber++, jlnumstring, 10);
    sprintf(jlnumstring, "%d", jlnumber++);
	strcat(jumplabel, jlnumstring);
}
