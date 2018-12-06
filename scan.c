#include	<stdio.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	<string.h>

#include	"symbol.h"
#include	"scan.h"


enum tokentype	scanword(char c, int *tabindex, FILE *fp);
enum tokentype	scannum(char c, int *tabindex, FILE *fp);
enum tokentype	scanop(char c, int *tabindex);
int		firstchar(FILE *ifp);
void	ungettc(int c, FILE *fp);
int		gettc(FILE *fp);

int	linenum = 1;


/*
 * openfile() - Checks the command-line parameter count in
 *		search of a second parameter that will be the
 * 		filename.  If there is only one argument, it's the
 *		name of the program's executable file.  If there are
 *		more than two, it is an unrecoverable error - Jason
 *		does not compile multi-file programs.
 */
FILE *openfile(int argc, char *argv[], char name[])
{
	char	filename[FILENAMELENGTH];
	FILE	*ifp;

        /* Do we need to get the file name? */
	switch (argc)	{
		case 1:	printf("File name?\t");
			gets(name);
                        break;

		case 2:	strcpy(name, argv[1]);
			break;

		default:printf("Usage: Jason \n");
                	exit(1);
	}

        /*  Add the extension .jsn to the input file's name */
	strcpy(filename, name);
	strcat(filename, ".jsn");

	/* Open the file and quit with an error message
		if the file cannot be opened */
	if ((ifp = fopen(filename, "r")) == NULL)	{
		printf("Cannot open %s\n", filename);
                exit(2);
	}
	return(ifp);
}

/*
 * ungettc() -	Returns a character to the file.  Uses ungetc and will
 *		adjust line number count.
 */
void	ungettc(int c, FILE *fp)
{
	if (c == '\n')
		--linenum;
        ungetc(c, fp);
}


/*
 * gettc() -	Fetches a character from a file.  It uses getc and adjusts
 *		the line number count when necessary.	
 */
int	gettc(FILE *fp)
{
	int	c;

	if ((c = getc(fp)) == '\n')
		linenum++;
        return(tolower(c));
}

/*
 * gettoken() -	Scan out the token strings of the language and return
 *		the cooresponding token class to the parser. 
 */
enum tokentype	gettoken(FILE *ifp, int *tabindex)
{
	int	c;

        
	/*  If this is the end of the file, send the
				token that indicates this*/
	if ((c = firstchar(ifp)) == EOF)
        	return(tokeof);

	/*
	 *	If it begins with a letter, it is a word.  If
	 *	begins with a digit, it is a number.  Otherwise,
	 *	it is an error.
	 */
	if (isalpha(c))
		return(scanword(c, tabindex, ifp));
	else if (isdigit(c))	
		return(scannum(c, tabindex, ifp));
	else		
		return(scanop(c, tabindex));

}

/*
 * firstchar() -	Skips past both white space and comments until
 *			it finds the first non-white space character
 *			outside a comment.
 */			
int	firstchar(FILE *ifp)
{
	int	c, goodchar = NO;	


        while	(!goodchar)	{
		/* Skip the white space in the program */
		while ((c = gettc(ifp)) != EOF && isspace(c))
			;

		/* Is it a comment or a real first character? */
		if  (c != '{')
			goodchar = YES;
		else
                	/* Skip the comment */
			while ((c = gettc(ifp)) != EOF && c != '}')
                		;
	}

	if (c == EOF)
		return(EOF);
	else
		return(c);
}

/*
 * scanword() -	Scan until you encounter something other than a letter.
 */
enum tokentype	scanword(char c, int *tabindex, FILE *fp)
{
	char	tokenstring[TOKENSTRINGLENGTH];
	int	i = 0;


	/*
	 *	Build the string one character at a time.  It keeps
	 *	scanning until either the end of file or until it
	 *	encounters a non-letter
         */
	for (tokenstring[i++] = c;
		(c = gettc(fp)) != EOF && (isalpha(c) || isdigit(c));
				)   
		tokenstring[i++] = c;
	tokenstring[i] ='\0';

        /*  Push back the last character */
	ungettc(c, fp);

	/*
	 *	If the lexeme is already in the symbol table,
	 *	return its tokenclass.  If it isn't,            it must
	 *	be an identifier whose type we do not know yet.
	 */
	if (installname(tokenstring, tabindex))
		return(tokenclass(*tabindex));
	else	{
		setattrib(stunknown, tokidentifier, *tabindex);
		return(tokidentifier);
	}
}


/*
 * scannum() -	Scan for a number.
 */ 
enum tokentype	scannum(char c, int *tabindex, FILE *fp)
{
	int	i = 0, ival, isitreal = NO;
        float	rval;
        char	tokenstring[TOKENSTRINGLENGTH];

	/*Scan until you encounter something that cannot be
		part of a number or the end of file */
	for (tokenstring[i++] = c;  (c = gettc(fp)) != EOF && isdigit(c); )
		tokenstring[i++] = c;

        /* Is there a fractional part? */
	if (c == '.')	{
		isitreal = YES;
		for  (tokenstring[i++] = c;
				(c = gettc(fp)) != EOF && isdigit(c);
				)
			tokenstring[i++] = c;
	}
        /*  Put the null byte at the end to terminate the string */
	tokenstring[i] = '\0';

	/* Push back the last character */
	ungettc(c, fp);

	/* If there is no fractional part, it is an integer literal
          constant.  Otherwise, it is a real literal constant. */
	if (installname(tokenstring, tabindex))
		return(tokenclass(*tabindex));
	else if (isitreal)	{
		setattrib(stunknown, tokconstant, *tabindex);
		installdatatype(*tabindex, stliteral, dtreal);
		rval = atof(tokenstring);
		//printf("*****rval is %f\n*******", rval);
                setrvalue(*tabindex, rval);
		return(tokconstant);
	}

	else	{
		setattrib(stunknown, tokconstant, *tabindex);
		installdatatype(*tabindex, stliteral, dtinteger);
		ival = atoi(tokenstring);
		//printf("*****ival is %d\n*******", ival);
		setivalue(*tabindex, ival);
		return(tokconstant);
	}
}

/*
 * scanop() -	Scan for an operator, which is a single character
 *		other than a letter or number.
 */
enum tokentype	scanop(char c, int *tabindex)
{
	int	i;
	char	tokenstring[TOKENSTRINGLENGTH];

	/*  If it's not already in the symbol table, it cannot
        	be a legal operator.  */
	tokenstring[0] = c;
	tokenstring[1] = '\0';
	if (!installname(tokenstring, tabindex))	{
		fprintf(stderr, "%s is an illegal operator on line #%d\n",
				tokenstring, linenum);
                exit(3);

        }

	return(tokenclass(*tabindex));

}
