#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

#define		TABSTOP		8

#define		TABLESIZE	200
#define		HASHTABLESIZE	100
#define		STRINGTABLESIZE 1200
#define		SYMTABLESIZE  	200

#define		MAXLINE        	121
#define   	NUMKEYWORDS    	21
#define		NUMTOKENS	34

#define		LABELSIZE	10

enum boolean   {NO, YES};

enum tokentype {tokbegin, tokcall, tokdeclare, tokdo, tokelse,
tokend,
          tokendif, tokenduntil,  tokendwhile, tokif, tokinteger,
          tokparameters, tokprocedure, tokprogram, tokread,
          tokreal, tokset, tokthen, tokuntil, tokwhile, tokwrite,
          tokstar, tokplus, tokminus, tokslash, tokequals,
          toksemicolon, tokcomma, tokperiod, tokgreater, tokless,
          toknotequal, tokopenparen, tokcloseparen, tokfloat,
          tokidentifier, tokconstant, tokerror, tokeof
};




enum symboltype     {stunknown, stkeyword, stprogram,
	  stparameter, stvariable, sttempvar,
          stconstant, stenum, ststruct, stunion, stprocedure,
          stfunction, stlabel, stliteral, stoperator
};

enum datatype  {dtunknown, dtnone, dtprogram, dtprocedure,
          dtinteger, dtreal
};

struct nametabtype  {
     int       strstart;
     int       strlength;
     int       symtabptr;
     int       nextname;
}; 


enum tagtype   {tint, treal};
union valtype  {
     int  ival;
     float     rval;
};

struct valrec  {
     enum tagtype   tag;
     union valtype  val;
};

struct symtabtype   {
     enum symboltype    symtype;
     enum tokentype     tok_class;
     enum datatype      dataclass;
     int                owningprocedure;
     int                thisname;
     int                outerscope, scopenext;
     struct valrec	value;
     char		label[LABELSIZE];
};

void      initializesymtab(void);
void      dumpsymboltable(void);
void      dumpsymboltable2(void);

int            installattrib(int nameindex);
enum boolean   installname(char string[], int *tabindex);
void           setattrib(int symbol, int token, int tabindex);
void           installdatatype(int tabindex,
                    enum symboltype stype, enum datatype dclass);

int	openscope(int tabindex);
void	closescope(void);
int	labelscope(int	procindex);

enum tokentype tokenclass(int tabindex);
enum datatype  data_class(int tabindex);
enum symboltype     symclass(int tabindex);
enum boolean   isvalidtype(int tabindex);

void    setivalue(int tabindex, int val);
int     getivalue(int tabindex);
void	setrvalue(int tabindex, float val);
float	getrvalue(int tabindex);
void    printtoken(int i);
void    printlexeme(int i);
void    fprintlexeme(FILE *ptr, int i);
void    setproc(int thisproc, int tabindex);
int     getproc(int tabindex);
void	getlabel(int tabindex, char *label);
void	paramlabel(int tabindex, char *label, int *bytecount);
int	tablesize(void);

/* The declarations necessary for the procedure stack */
enum logical        {false, true};

#define        MAXSTACK  100

typedef struct {
     int  proc;
     int  sstart, snext;
} procstackitem;

typedef   struct    {
     int            top;
     procstackitem  item[MAXSTACK];
}    procstack;

procstackitem  procpop(void);
void           procpush(procstackitem x);
enum logical   procempty(void);
void           initprocstack(void);


procstackitem  initprocentry(int tabindex);