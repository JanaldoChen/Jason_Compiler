#include  <ctype.h>
#include  "symbol.h"
#define min(x, y) ((x)<(y)?(x):(y))

char *tokclstring[] = {    "begin     ", "call      ",
    "declare   ", "do        ", "else      ", "end       ",
    "endif     ", "enduntil  ", "endwhile  ", "if        ",
    "integer   ", "parameters", "procedure ", "program   ",
    "read      ", "real      ", "set       ", "then      ",
    "until     ", "while     ", "write     ", "star      ",
    "plus      ", "minus     ", "slash     ", "equals    ",
    "semicolon ", "comma     ", "period    ", "greater   ",
    "less      ", "notequal  ", "openparen ", "closeparen",
    "float     ", "identifier", "constant  ", "error     ",
    "eof       "
};

char *symtypestring[] =  {"unknown  ", "keyword  ", "program  ",
    "parameter", "variable ", "temp. var",
    "constant ", "enum     ", "struct   ",
    "union    ", "procedure", "function ",
    "label    ", "literal  ", "operator "
};

char *datatypestring[] = {"unknown","none   ", "program",
    "proced.", "integer", "real   "
};

char *keystring[/*NUMTOKENS*/] = {"begin", "call", "declare",
    "do", "else", "end", "endif", "enduntil", "endwhile",
    "if", "integer", "parameters", "procedure", "program",
    "read", "real", "set", "then", "until", "while",
    "write", "*", "+", "-", "/", "=", ";",
    ",", ".", ">", "<", "!", "(", ")", "_float"
};

char stringtable[STRINGTABLESIZE];
struct nametabtype nametable[TABLESIZE];
struct symtabtype symtab[SYMTABLESIZE];

int  hashtab[HASHTABLESIZE];
int  strtablen, namtablen, symtablen, auxtablen;
procstackitem  thisproc;

enum boolean   ispresent(char string[], int length, int *code,
                         int *nameindex);
void      LexemeInCaps(int tabindex);
int       hashcode(char string[], int length);
void	makelabel(int tabindex, char *label);

void initializesymtab(void)
{
    int  i, nameindex;
    
    initprocstack();
    thisproc = initprocentry(-1);
    
    for  (i = 0; i < HASHTABLESIZE;  i++)
        hashtab[i] = -1;
    
    for  (i = 0;  i < TABLESIZE;  i++)
        nametable[i].nextname = -1;
    
    for  (i = 0;  i < SYMTABLESIZE;  i++)   {
        symtab[i].symtype = -1;
        symtab[i].tok_class = -1;
        symtab[i].thisname = -1;
        symtab[i].value.tag = tint;
        symtab[i].value.val.ival = 0;
        symtab[i].owningprocedure = -1;
        symtab[i].outerscope = -1;
        symtab[i].scopenext = -1;
        symtab[i].label[0] = '\0';
    }
    
    /* Install the keywords in the name table and
     set their attributes to keyword */
    for  (i = 0;  i < NUMKEYWORDS;  i++)    {
        installname(keystring[i], &nameindex);
        setattrib(stkeyword, i, nameindex);
    }
    
    /* Install the operators in the name table and
     set their attributes to operator */
    for  (i = NUMKEYWORDS; i < NUMTOKENS;  i++)  {
        installname(keystring[i],&nameindex);
        setattrib(stoperator, i, nameindex);
    }
    
    installname(keystring[i], &nameindex);
    setattrib(stfunction, i, nameindex);
    installdatatype(nameindex, stfunction, dtreal);
    
    printf("all initiallized\n");
}

/*
 * DumpSymbolTable() -   Prints out the basic symbol table
 *             information, including the name and token class
 */
void dumpsymboltable(void)
{
    int  i, j;
    
    printf("SYMBOL TABLE DUMP\n-----------------\n\n");
    printf("                   Token       Symbol     Data");
    printf("              Owning\n");
    printf("Index   Name       Class       Type       Type");
    printf("    Value   Procedure    Label\n");
    printf("-----   ----       -----       ------     ----");
    printf("    -----   ---------\n");
    
    for  (i = 0;  i < symtablen;  i++) {
        printf("%5d\t",i);
        printlexeme(i);
        if (nametable[symtab[i].thisname].strlength < 11)
            for (j = 0;
                 j < 11
                 - nametable[symtab[i].thisname].strlength;
                 j++)
                putchar(' ');
        else
            printf("\n          ");
        
        printf("%s  ",tokclstring[symtab[i].tok_class]);
        
        printf("%s  ", symtypestring[symtab[i].symtype]);
        
        printf("%s",  datatypestring[symtab[i].dataclass]);
        
        if (symtab[i].value.tag == tint)
            printf(" %5d", symtab[i].value.val.ival);
        else
            printf(" %8.3f", symtab[i].value.val.rval);
        
        if (symtab[i].owningprocedure == -1)
            printf("   global");
        else {
            printf("   ");
            LexemeInCaps(symtab[i].owningprocedure);
        }
        printf("   %s", symtab[i].label);
        putchar('\n');
    }
    
}

void dumpsymboltable2(void)
{
    int  i;
    
    for (i = 0;  i < namtablen;  i++)  {
        if (i%10 == 9) getchar();
        printf("%d\t%d\t%d\t%d\t%d\n", i,
               nametable[i].strstart,
               nametable[i].strlength, nametable[i].symtabptr,
               nametable[i].nextname);
    }
    
    for (i = 0;  i < symtablen;  i++)  {
        if (i%10 == 9) getchar();
        printf("%d  %d  %d  %d  %d  %d  %d  %d  %d\t", i,
               symtab[i].symtype,
               symtab[i].tok_class, symtab[i].dataclass,
               symtab[i].owningprocedure, symtab[i].thisname,
               symtab[i].outerscope, symtab[i].scopenext,
               symtab[i].value.tag);
        if (symtab[i].value.tag == treal)
            printf("%f\t", symtab[i].value.val.rval);
        else
            printf("%d\t", symtab[i].value.val.ival);
        printf("%s\n", symtab[i].label);
    }
    
    for (i = 0;  i< strtablen; i++)    {
        if (i%60 == 59) getchar();
        putchar(stringtable[i]);
    }
}

enum boolean installname(char string[], int *tabindex)
{
    int  i, code, length, nameindex;
    length = (int)strlen(string);
    if (ispresent(string, length, &code, &nameindex)) {
        if (nametable[nameindex].symtabptr == -1)    {
            *tabindex = installattrib(nameindex);
            return(NO);
        }
        else {
            *tabindex = nametable[nameindex].symtabptr;
            return(YES);
        }
    }
    nametable[nameindex = namtablen++].strstart = strtablen;
    nametable[nameindex].strlength = length;
    
    for  (i = 0;  i < length;  i++)
        stringtable[strtablen++] = string[i];
        
        nametable[nameindex].nextname = hashtab[code];
        hashtab[code] = nameindex;
        *tabindex = installattrib(nameindex);
        return(NO);
}


int  installattrib(int nameindex)
{
    int  tabindex;
    
    tabindex = nametable[nameindex].symtabptr = symtablen++;
    symtab[tabindex].thisname = nameindex;
    symtab[tabindex].symtype = stunknown;
    symtab[tabindex].dataclass = dtunknown;
    
    
    return(tabindex);
}

enum boolean   ispresent(char string[], int length, int *code,
                         int *nameindex)
{
    int  found = NO, oldnameindex, j, k;
    oldnameindex = -1;
    
    *code = hashcode(string, length);
    
    for  (*nameindex = hashtab[*code];
          !found && *nameindex != -1;
          oldnameindex = *nameindex,
          *nameindex = nametable[*nameindex].nextname) {
        for  (j = 0, k = nametable[*nameindex].strstart;
              j < length &&
              string[j] == stringtable[k];  j++, k++)
            ;
        if  (j == length)
            found = YES;
        
    }
    
    if (found)
        *nameindex = oldnameindex;
        
        return(found);
}

int  hashcode(char string[], int length)
{
    int       i, numshifts, startchar;
    unsigned  code;
    
    numshifts = (int) min(length, (8*sizeof(int)-8));
    startchar = ((length-numshifts) % 2);
    code = 0;
    
    for (i = startchar;  i <= startchar + numshifts - 1;  i++)
        code = (code << 1) + string[i];
    
    return(code % HASHTABLESIZE);
}

void setattrib(int symbol, int token, int tabindex)
{
    symtab[tabindex].symtype = symbol;
    symtab[tabindex].tok_class = token;
    
    if (symtab[tabindex].symtype == stkeyword
        || symtab[tabindex].symtype == stoperator)
        symtab[tabindex].dataclass = dtnone;
    else
        symtab[tabindex].dataclass = dtunknown;
    
    if (tokenclass(tabindex) == tokidentifier && thisproc.proc != -1) {
        if (thisproc.sstart == -1)    {
            thisproc.sstart = tabindex;
            thisproc.snext = tabindex;
        }
        else {
            symtab[thisproc.snext].scopenext = tabindex;
            thisproc.snext = tabindex;
        }
    }
    
}


int  openscope(int tabindex)
{
    int  newtabindex, nameindex;
    
    nameindex = symtab[tabindex].thisname;
    newtabindex = installattrib(nameindex);
    setattrib(stunknown, tokidentifier, newtabindex);
    symtab[newtabindex].outerscope = tabindex;
    return(newtabindex);
}

void closescope(void)
{
    int  nmptr, symptr;
    
    for (symptr = thisproc.sstart;  symptr != -1 && symptr != symtab[symptr].scopenext;
         symptr = symtab[symptr].scopenext) {
        nmptr = symtab[symptr].thisname;
        nametable[nmptr].symtabptr = symtab[symptr].outerscope;
    }
    
}

int	labelscope(int	procindex)
{
    int	oldsymptr, symptr, numbytes = 0, totalbytes;
    char	label[LABELSIZE];
    
    for  (symptr = getivalue(procindex); symptr != 0;
          symptr = getivalue(symptr))
        numbytes += (data_class(symptr) == dtinteger)? 2 : 4;
    
    totalbytes = numbytes;
    for  (oldsymptr = symptr, symptr = getivalue(procindex);
          symptr != 0;
          oldsymptr = symptr, symptr = getivalue(symptr))
        paramlabel(symptr, label, &numbytes);
    
    
    numbytes -=2;
    for (symptr = (getivalue(procindex) == 0)?
         procindex+1: symtab[oldsymptr].scopenext;
         symptr != -1 && symclass(symptr) != stprocedure;
         symptr = symtab[symptr].scopenext)
        paramlabel(symptr, label, &numbytes);
    return(-numbytes-2);
}

void installdatatype(int tabindex, enum symboltype stype,
                     enum datatype dclass)
{
    symtab[tabindex].symtype = stype;
    symtab[tabindex].dataclass = dclass;
}

enum tokentype tokenclass(int tabindex)
{
    return(symtab[tabindex].tok_class);
}

enum datatype  data_class(int tabindex)
{
    return(symtab[tabindex].dataclass);
}

enum boolean   isvalidtype(int tabindex)
{
    return(symtab[tabindex].dataclass == dtinteger
           ||symtab[tabindex].dataclass == dtreal);
}

enum symboltype     symclass(int tabindex)
{
    return(symtab[tabindex].symtype);
}

void setivalue(int tabindex, int val)
{
    symtab[tabindex].value.tag = tint;
    symtab[tabindex].value.val.ival = val;
}

int  getivalue(int tabindex)
{
    return(symtab[tabindex].value.val.ival);
}

void setrvalue(int tabindex, float val)
{
    symtab[tabindex].value.tag = treal;
    symtab[tabindex].value.val.rval = val;
}

float	getrvalue(int tabindex)
{
    return(symtab[tabindex].value.val.rval);
}


void setproc(int thisproc, int tabindex)
{
    symtab[tabindex].owningprocedure = thisproc;
}

int  getproc(int tabindex)
{
    return(symtab[tabindex].owningprocedure);
}

void printtoken(int i)
{
    printf("%s", tokclstring[i]);
}

void printlexeme(int tabindex)
{
    int  i, j;
    
    i = symtab[tabindex].thisname;
    
    for  (j = nametable[i].strstart;
          j < nametable[i].strstart+nametable[i].strlength;
          j++)
        putchar(stringtable[j]);
}

void fprintlexeme(FILE *ptr, int tabindex)
{
    int  i, j;
    
    i = symtab[tabindex].thisname;
    
    
    for  (j = nametable[i].strstart;
          j < nametable[i].strstart+nametable[i].strlength;
          j++)
        
        putc(stringtable[j], ptr);
}

void LexemeInCaps(int tabindex)
{
    int  i, j;
    
    i = symtab[tabindex].thisname;
    
    for  (j = nametable[i].strstart;
          j < nametable[i].strstart+nametable[i].strlength;
          j++)
        putchar(toupper(stringtable[j]));
}

void	getlabel(int tabindex, char *varlabel)
{
    if (symtab[tabindex].label[0] != '\0' && symtab[tabindex].symtype != stlabel)
        strcpy(varlabel, symtab[tabindex].label);
    else
        makelabel(tabindex, varlabel);
}

void	makelabel(int tabindex, char *label)
{
    int		i, j, k;
    char		indexstr[5];
    
    label[0] = '\0';
    switch (symclass(tabindex))	{
        case stliteral:
            if (data_class(tabindex) == dtinteger)	{
                i = symtab[tabindex].thisname;
                
                for  (k = 0, j = nametable[i].strstart;
                      j < nametable[i].strstart+
                      nametable[i].strlength;
                      j++, k++)
                    label[k] = stringtable[j];
                label[k] = '\0';
                break;
            }
            
            
        case sttempvar:
            label[0] = '_';
            label[1] = 't';
            label[2] = '\0';
            //itoa(tabindex, indexstr, 10);
            sprintf(indexstr, "%d", tabindex);
            strcat(label, indexstr);
            break;
            
        case stlabel:
            strcpy(label, "_loop");
            //itoa(tabindex, indexstr, 10);
            sprintf(indexstr, "%d", tabindex);
            strcat(label, indexstr);
            break;
        case stprogram:
        case stvariable:
        case stparameter:
        case stprocedure:
            i = symtab[tabindex].thisname;
            
            for (k = 0, j = nametable[i].strstart; j < nametable[i].strstart+nametable[i].strlength && j < nametable[i].strstart+5; j++, k++)
                label[k] = stringtable[j];
            label[k] = '\0';
            if (strlen(label) >= 5)	{
                //itoa(tabindex, indexstr, 10);
                sprintf(indexstr, "%d", tabindex);
                strcat(label, indexstr);
            }
            
        default: break;
    }
    strcpy(symtab[tabindex].label, label);
    //printf("Tabindex is %d\n", tabindex);
}


void	paramlabel(int tabindex, char *label, int *bytecount)
{
    char indexstr[5];
    if (*bytecount < 0)	{
        if (data_class(tabindex) == dtinteger)
            strcpy(label, "[bp");
        else
            strcpy(label, "[bp");
    }
    else
        strcpy(label, "[bp");
    if (*bytecount > 0)
        strcat(label, "+");
    //itoa(*bytecount, indexstr, 10);
    sprintf(indexstr, "%d", *bytecount);
    strcat(label, indexstr);
    *bytecount -= data_class(tabindex) == dtinteger? 2: 4;
    strcat(label, "]");
    strcpy(symtab[tabindex].label, label);
}

int	tablesize(void)
{
    return(symtablen);
}

procstack      ps;

procstackitem  procpop(void)
{
    if (procempty())    {
        printf("Procedure stack underflow\n");
        exit(10);
    }
    
    return(ps.item[--ps.top]);
}

void procpush(procstackitem x)
{
    if (ps.top == MAXSTACK)  {
        printf("Procedure stack overflow\n");
        exit(10);
    }
    ps.item[ps.top++] = x;
}

enum logical    procempty()
{
    return(ps.top == 0);
}

void initprocstack(void)
{
    ps.top = 0;
}

procstackitem  initprocentry(int tabindex)
{
    procstackitem thisproc;
    
    thisproc.proc = tabindex;
    thisproc.sstart = -1;
    thisproc.snext = -1;
    return(thisproc);
}
