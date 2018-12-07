#include	<stdio.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	<string.h>

#include	"symbol.h"
#include	"scan.h"


enum tokentype	scanword(char c, int *tabindex, FILE *fp);
enum tokentype	scannum(char c, int *tabindex, FILE *fp);
enum tokentype	scanop(char c, int *tabindex);
int firstchar(FILE *ifp);
void ungettc(int c, FILE *fp);
int	gettc(FILE *fp);

int	linenum = 1;

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

void	ungettc(int c, FILE *fp)
{
	if (c == '\n')
		--linenum;
        ungetc(c, fp);
}

int	gettc(FILE *fp)
{
	int	c;

	if ((c = getc(fp)) == '\n')
		linenum++;
        return(tolower(c));
}

enum tokentype	gettoken(FILE *ifp, int *tabindex)
{
	int	c;

	if ((c = firstchar(ifp)) == EOF)
        	return(tokeof);

	if (isalpha(c))
		return(scanword(c, tabindex, ifp));
	else if (isdigit(c))	
		return(scannum(c, tabindex, ifp));
	else		
		return(scanop(c, tabindex));

}

int	firstchar(FILE *ifp)
{
	int	c, goodchar = NO;	


        while	(!goodchar)	{
		while ((c = gettc(ifp)) != EOF && isspace(c));

		if  (c != '{')
			goodchar = YES;
		else
			while ((c = gettc(ifp)) != EOF && c != '}');
	}

	if (c == EOF)
		return(EOF);
	else
		return(c);
}

enum tokentype	scanword(char c, int *tabindex, FILE *fp)
{
	char	tokenstring[TOKENSTRINGLENGTH];
	int	i = 0;

	for (tokenstring[i++] = c;(c = gettc(fp)) != EOF && (isalpha(c) || isdigit(c));)
		tokenstring[i++] = c;
    tokenstring[i] ='\0';
	ungettc(c, fp);
	if (installname(tokenstring, tabindex))
		return(tokenclass(*tabindex));
	else	{
		setattrib(stunknown, tokidentifier, *tabindex);
		return(tokidentifier);
	}
}

enum tokentype	scannum(char c, int *tabindex, FILE *fp)
{
	int	i = 0, ival, isitreal = NO;
        float	rval;
        char	tokenstring[TOKENSTRINGLENGTH];
    
	for (tokenstring[i++] = c;  (c = gettc(fp)) != EOF && isdigit(c); )
		tokenstring[i++] = c;

	if (c == '.')	{
		isitreal = YES;
		for  (tokenstring[i++] = c; (c = gettc(fp)) != EOF && isdigit(c); )
			tokenstring[i++] = c;
	}
	tokenstring[i] = '\0';

	ungettc(c, fp);

	if (installname(tokenstring, tabindex))
		return(tokenclass(*tabindex));
	else if (isitreal)	{
		setattrib(stunknown, tokconstant, *tabindex);
		installdatatype(*tabindex, stliteral, dtreal);
		rval = atof(tokenstring);
        setrvalue(*tabindex, rval);
		return(tokconstant);
	}

	else	{
		setattrib(stunknown, tokconstant, *tabindex);
		installdatatype(*tabindex, stliteral, dtinteger);
		ival = atoi(tokenstring);
		setivalue(*tabindex, ival);
		return(tokconstant);
	}
}

enum tokentype	scanop(char c, int *tabindex)
{
	int	i;
	char	tokenstring[TOKENSTRINGLENGTH];
	tokenstring[0] = c;
	tokenstring[1] = '\0';
	if (!installname(tokenstring, tabindex))	{
		fprintf(stderr, "%s is an illegal operator on line #%d\n",
				tokenstring, linenum);
                exit(3);

        }

	return(tokenclass(*tabindex));

}
