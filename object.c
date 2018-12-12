#include    "object.h"
#include    "quad.h"
#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>

#define        FILENAMELENGTH        100

enum    opcodetype    {opcmov, opcadd, opcsub,
    opcdec, opcneg, opcjmp, opcjno, opcjne,
    opcje, opcjz, opcjp, opcjc,
    opcjl, opcjle, opcjg, opcjge, opccmp, opccall,
    opcret, opcpush, opcpop
};

const char    *opcodestr[] =    {"mov", "add", "sub",
    "dec", "neg", "jmp", "jno", "jne", "je", "jz", "jp",
    "jc","jl", "jle", "jg", "jge", "cmp", "call", "ret",
    "push", "pop",
};

FILE    *ofp;
int    ioverflow = NO, divby0 = NO;

void    writestack(void);
void    writedata(void);
void    writecode(int numcodes);
void    writeprocs(void);
void    writeproc(int i, int totalbytes);

void    emitone(enum opcodetype opcode, int tabindex);
void    emitoffset(enum opcodetype opcode, char *target, int tabindex);
void    emitlabel(enum opcodetype opcode, char *label);
void    emittoone(enum opcodetype opcode, int tabindex, char *label);
void    emitfromone(enum opcodetype opcode, char *label, int tabindex);

void    translate(struct quadtype nextquad);
void    transadd(struct quadtype nextquad);
void    transsub(struct quadtype nextquad);
void    transneg(struct quadtype nextquad);
void    transmult(struct quadtype nextquad);
void    transdiv(struct quadtype nextquad);
void    transassn(struct quadtype nextquad);
void    transjump(struct quadtype nextquad);
void    transifjump(struct quadtype nextquad);
void    transifposz(struct quadtype nextquad);
void    transifpos(struct quadtype nextquad);
void    transifnegz(struct quadtype nextquad);
void    transifneg(struct quadtype nextquad);
void    transifz(struct quadtype nextquad);
void    transifnz(struct quadtype nextquad);
void    translabel(struct quadtype nextquad);
void    transread(struct quadtype nextquad);
void    transwrite(struct quadtype nextquad);
void    transarg(struct quadtype nextquad);
void    transproc(struct quadtype nextquad);

void    writeaddovr(void);
void    getjumplabel(char *jumplabel);
void    readInt(void);
void    printInt(void);

void    writeobject(char name[], int numcodes)
{
    char    filename[FILENAMELENGTH];
    
    strcpy(filename, name);
    strcat(filename, ".asm");
    
    if ((ofp = fopen(filename, "w")) == NULL)    {
        printf("Cannot open %s\n", filename);
        exit(1);
    }
    
    writestack();
    writedata();
    writecode(numcodes);
    
    fclose(ofp);
}

void    writestack(void)
{
    fprintf(ofp, "%s\n\n%s\n%s\n%s\n\n",
            "DOSSEG",
            "_STACK SEGMENT para stack \'stack\'",
            "              dw      1000 dup(?)",
            "_STACK ENDS");
}

void    writedata(void)
{    
    fprintf(ofp, "_DATA SEGMENT word public \'data\'\n");
    fprintf(ofp, "TestResult     dw     ?\n");
    
    int    i;
    float    litvalue;
    char    label[LABELSIZE];
    
    for (i = NUMTOKENS+2; i < tablesize();  i++)    {
        if ((symclass(i) == sttempvar || symclass(i) == stvariable)
            && getproc(i) == NUMTOKENS+1)    {
            //printlexeme(i);
            //getchar();
            getlabel(i, label);
            if (data_class(i) == dtinteger)
                fprintf(ofp, "%-10s     dw     ?\n", label);
            else
                fprintf(ofp, "%-10s     dd     ?\n", label);
        }
        else if (symclass(i) == stliteral
                 && data_class(i) == dtreal)    {
            //printlexeme(i);
            //getchar();
            getlabel(i, label);
            litvalue = getrvalue(i);
            fprintf(ofp, "%-10s     dd     %f\n",
                    label, litvalue);
        }
    }
    
    fprintf(ofp, "_DATA ENDS\n\n");
}

void    writeprocs(void)
{
    int    i = 0, localvarbytes;
    char    label[LABELSIZE];
    struct quadtype        nextquad;
    
    for (i = 0; i < getnumcodes(); i++)    {
        nextquad = getquad(i);
        
        while ((nextquad.opcode != oplabel
                || symclass(nextquad.op1.opnd) != stprocedure)
               && symclass(nextquad.op1.opnd) != stprogram)
            nextquad = getquad(++i);
        
        if (symclass(nextquad.op1.opnd) == stprogram)
            break;
        
        localvarbytes = labelscope(nextquad.op1.opnd);
        //  printf("Used %d bytes\n", localvarbytes);
        getlabel(nextquad.op1.opnd, label);
        writeproc(i, localvarbytes);
    }
}

void    writeproc(int i, int totalbytes)
{
    struct    quadtype    nextquad;
    nextquad = getquad(i);
    fprintf(ofp, "%s\n",    "_TEXT SEGMENT");
    fprintlexeme(ofp, nextquad.op1.opnd);
    fprintf(ofp, ":\n\n");
    fprintf(ofp, "%s\n%s\n%s\n\n",
            "         push      bp",
            "         mov       bp, sp",
            "         add       bp, 2");
    
    if (totalbytes != 0)    {
        fprintf(ofp, "; Allocate space for local variables\n");
        fprintf(ofp, "         sub       sp, %d\n", totalbytes);
    }
    nextquad = getquad(++i);
    while (nextquad.opcode != opreturn)    {
        translate(nextquad);
        nextquad = getquad(++i);
    }
    
    if (totalbytes != 0)    {
        fprintf(ofp, "; Return space used by local variables\n");
        fprintf(ofp, "         add       sp, %d\n", totalbytes);
    }
    fprintf(ofp, "%s\n%s\n\n",
            "         pop       bp",
            "         ret       2");
    fprintf(ofp, "_TEXT ENDS\n\n\n");
}

void    writecode(int numcodes)
{
    int            i, argnum=0;
    char            label[LABELSIZE];
    struct quadtype        nextquad;
    
    fprintf(ofp, "%s\n%s\n\n",
            "_TEXT SEGMENT word public \'code\'",
            "         assume    cs: _TEXT");
    
    //printf("Let\'s get started\n");
    getlabel(NUMTOKENS+1, label);
    fprintf(ofp, "%s%c\n\n%s\n%s\n%s\n%s\n\n",
            label,':',
            "; set up the ds register so that it points to _DATA",
            "         mov       ax, seg _DATA",
            "         mov       ds, ax",
            "         assume    ds: _DATA");
    
    i = getprogstart();
    
    while (i <= numcodes)    {
        nextquad = getquad(i++);
        if (nextquad.opcode == oparg) {
            argnum++;
        }
        
        translate(nextquad);
        if (nextquad.opcode == opccall) {
            
            fprintf(ofp, "; pop these args\n");
            while (argnum != 0) {
                fprintf(ofp, "         pop       ax\n");
                argnum --;
            }
            fprintf(ofp, "\n");
        }
    }
    
    fprintf(ofp, "%s\n%s\n%s\n\n",
            "; exit to DOS",
            "         mov       ax, 4C00h",
            "         int       21h");
    
    //writeaddovr();
    readInt();
    printInt();
    
    fprintf(ofp, "_TEXT ENDS\n\n");
    
    writeprocs();
    fprintf(ofp, "END %s\n\n", label);
}

void    translate(struct quadtype nextquad)
{
    switch(nextquad.opcode)    {
        case opadd:        transadd(nextquad);  break;
        case opsub:        transsub(nextquad);  break;
        case opneg:        transneg(nextquad);  break;
        case opmult:        transmult(nextquad); break;
        case opdiv:        transdiv(nextquad); break;
        case opassn:      transassn(nextquad);  break;
        case opjump:        transjump(nextquad); break;
        case opifposzjump:
        case opifposjump:
        case opifnegzjump:
        case opifnegjump:
        case opifzjump:
        case opifnzjump:    transifjump(nextquad); break;
            
        case oplabel:        translabel(nextquad); break;
        case opread:        transread(nextquad); break;
        case opwrite:        transwrite(nextquad); break;
        case oparg:        transarg(nextquad); break;
        case opproc:        transproc(nextquad); break;
        default: break;
    }
}

void    emitone(enum opcodetype opcode, int tabindex)
{
    char    label[LABELSIZE];
    
    getlabel(tabindex, label);
    if (opcode == opccall) {
        fprintf(ofp, "         %-10s  %s\n", opcodestr[opcode], label);
    } else {
        fprintf(ofp, "         %-10s word ptr %s\n", opcodestr[opcode], label);
    }
}

void    emitoffset(enum opcodetype opcode, char *target, int tabindex)
{
    char    label[LABELSIZE];
    
    getlabel(tabindex, label);
    //fprintf(ofp, "         %-10s%s, offset ", opcodestr[opcode], target);
    fprintf(ofp, "         %-10s%s, ", opcodestr[opcode], target);
    
    if (strncmp(label, "[bp", 3) == 0)    {
        fprintf(ofp, "word ptr ");
    }
    fprintf(ofp, "%s\n", label);
}

void    emitlabel(enum opcodetype opcode, char *label)
{
    fprintf(ofp, "         %-10s%s\n", opcodestr[opcode], label);
}

void    emittoone(enum opcodetype opcode, int tabindex, char *label)
{
    char    target[LABELSIZE];
    
    getlabel(tabindex, target);
    
    fprintf(ofp, "         %-10s", opcodestr[opcode]);
    if (strncmp(target, "[bp", 3) == 0)    {
        if (data_class(tabindex) == dtreal)
            putc('d', ofp);
        fprintf(ofp, "word ptr ");
    }
    
    fprintf(ofp, "%s, %s\n", target, label);
}

void    emitfromone(enum opcodetype opcode, char *label, int tabindex)
{
    char    source[LABELSIZE];
    
    getlabel(tabindex, source);
    
    fprintf(ofp, "         %-10s%s, ", opcodestr[opcode], label);
    if (strncmp(source, "[bp", 3) == 0)    {
        if (data_class(tabindex) == dtreal)
            putc('d', ofp);
        fprintf(ofp, "word ptr ");
    }
    
    fprintf(ofp, "%s\n", source);
}

void    transadd(struct quadtype nextquad)
{
    char    label[LABELSIZE];
    
    fprintf(ofp, "; ");
    fprintlexeme(ofp, nextquad.op1.opnd);
    fprintf(ofp, " := ");
    fprintlexeme(ofp, nextquad.op2.opnd);
    fprintf(ofp, " + ");
    fprintlexeme(ofp, nextquad.op3.opnd);
    fprintf(ofp, "\n");
    
    getjumplabel(label);
    emitfromone(opcmov, "ax", nextquad.op2.opnd);
    emitfromone(opcmov, "bx", nextquad.op3.opnd);
    fprintf(ofp, "         add       ax, bx\n");
    // fprintf(ofp, "         jc        iovrflo\n");
    
    emittoone(opcmov, nextquad.op1.opnd, "ax");
    
    fprintf(ofp, "%s:\n\n", label);
}

void    transsub(struct quadtype nextquad)
{
    char    label[LABELSIZE];
    
    fprintf(ofp, "; ");
    fprintlexeme(ofp, nextquad.op1.opnd);
    fprintf(ofp, " := ");
    fprintlexeme(ofp, nextquad.op2.opnd);
    fprintf(ofp, " - ");
    fprintlexeme(ofp, nextquad.op3.opnd);
    fprintf(ofp, "\n");
    
    getjumplabel(label);
    emitfromone(opcmov, "ax", nextquad.op2.opnd);
    emitfromone(opcmov, "bx", nextquad.op3.opnd);
    fprintf(ofp, "         sub       ax, bx\n");
    //  fprintf(ofp, "         jc        iovrflo\n");
    emittoone(opcmov, nextquad.op1.opnd, "ax");
    
    fprintf(ofp, "%s:\n\n", label);
}

void    transneg(struct quadtype nextquad)
{
    char    label[LABELSIZE];
    
    if (data_class(nextquad.op1.opnd) == dtinteger)    {
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

void    transmult(struct quadtype nextquad)
{
    char    label[LABELSIZE];
    
    fprintf(ofp, "; ");
    fprintlexeme(ofp, nextquad.op1.opnd);
    fprintf(ofp, " := ");
    fprintlexeme(ofp, nextquad.op2.opnd);
    fprintf(ofp, " * ");
    fprintlexeme(ofp, nextquad.op3.opnd);
    fprintf(ofp, "\n");
    
    getjumplabel(label);
    
    emitfromone(opcmov, "ax", nextquad.op2.opnd);
    emitfromone(opcmov, "bx", nextquad.op3.opnd);
    fprintf(ofp, "         mul       bx\n");
    emittoone(opcmov, nextquad.op1.opnd, "ax");
    
    fprintf(ofp, "%s:\n\n", label);
    putc('\n', ofp);
    
}

void    transdiv(struct quadtype nextquad)
{
    char    label[LABELSIZE];
    
    fprintf(ofp, "; ");
    fprintlexeme(ofp, nextquad.op1.opnd);
    fprintf(ofp, " := ");
    fprintlexeme(ofp, nextquad.op2.opnd);
    fprintf(ofp, " / ");
    fprintlexeme(ofp, nextquad.op3.opnd);
    fprintf(ofp, "\n");
    
    getjumplabel(label);
    fprintf(ofp, "         xor       dx, dx\n");
    emitfromone(opcmov, "ax", nextquad.op2.opnd);
    emitfromone(opcmov, "bx", nextquad.op3.opnd);
    fprintf(ofp, "         div       bx\n");
    emittoone(opcmov, nextquad.op1.opnd, "ax");
    
    fprintf(ofp, "%s:\n\n", label);
    putc('\n', ofp);
}


void    transassn(struct quadtype nextquad)
{
    char    target[LABELSIZE], source[LABELSIZE];
    getlabel(nextquad.op1.opnd, target);
    getlabel(nextquad.op2.opnd, source);
    fprintf(ofp, "; ");
    fprintlexeme(ofp, nextquad.op1.opnd);
    fprintf(ofp, " := ");
    fprintlexeme(ofp, nextquad.op2.opnd);
    fprintf(ofp, "\n");
    
    emitfromone(opcmov, "ax", nextquad.op2.opnd);
    emittoone(opcmov, nextquad.op1.opnd, "ax");
    
    
}

void    transjump(struct quadtype nextquad)
{
    char    label[LABELSIZE];
    
    getlabel(nextquad.op1.opnd, label);
    fprintf(ofp, "; goto %s\n", label);
    fprintf(ofp, "         jmp        %s\n\n", label);
}

void    transifjump(struct quadtype nextquad)
{
    char    label1[LABELSIZE], label2[LABELSIZE];
    
    getjumplabel(label2);
    getlabel(nextquad.op2.opnd, label1);
    
    fprintf(ofp, "; if ");
    fprintlexeme(ofp, nextquad.op1.opnd);
    switch(nextquad.opcode)    {
        case opifposzjump:    fprintf(ofp, " >=");  break;
        case opifposjump:    fprintf(ofp, " >");  break;
        case opifnegzjump:    fprintf(ofp, " <=");  break;
        case opifnegjump:    fprintf(ofp, " <");  break;
        case opifzjump:    fprintf(ofp, " =");  break;
        case opifnzjump:    fprintf(ofp, " !=");  break;
        default: break;
    }
    fprintf(ofp, " 0 goto %s\n", label1);
    
    emittoone(opccmp, nextquad.op1.opnd, "0");
    switch(nextquad.opcode)    {
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
        default: break;
    }
    
    emitlabel(opcjmp, label1);
    fprintf(ofp, "%s:\n\n", label2);
}

void    translabel(struct quadtype nextquad)
{
    char    target[LABELSIZE];
    
    getlabel(nextquad.op1.opnd, target);
    fprintf(ofp, "%s:\n\n", target);
}

void    transread(struct quadtype nextquad)
{
    fprintf(ofp, "; read ");
    fprintlexeme(ofp, nextquad.op1.opnd);
    fprintf(ofp, "\n");
    
    fprintf(ofp, "         call      Readint\n");
    emittoone(opcmov, nextquad.op1.opnd, "cx");
    fprintf(ofp, "\n");
}

void    transwrite(struct quadtype nextquad)
{
    fprintf(ofp, "; write ");
    fprintlexeme(ofp, nextquad.op1.opnd);
    fprintf(ofp, "\n");
    
    emitfromone(opcmov, "ax", nextquad.op1.opnd);
    fprintf(ofp, "         call      Writeint\n");
    fprintf(ofp, "\n");
}

void    transarg(struct quadtype nextquad)
{
    char    target[LABELSIZE];
    
    getlabel(nextquad.op1.opnd, target);
    fprintf(ofp, "; arg ");
    fprintlexeme(ofp, nextquad.op1.opnd);
    fprintf(ofp, "\n");
    emitoffset(opcmov, "ax", nextquad.op1.opnd);
    emitlabel(opcpush, "ax");
    fprintf(ofp, "\n");
}

void    transproc(struct quadtype nextquad)
{
    char    target[LABELSIZE];
    
    getlabel(nextquad.op1.opnd, target);
    fprintf(ofp, "; call ");
    fprintlexeme(ofp, nextquad.op1.opnd);
    fprintf(ofp, "\n");
    emitone(opccall, nextquad.op1.opnd);
    fprintf(ofp, "\n");
}

void    writeaddovr(void)
{
    fprintf(ofp, "%s\n%s\n%s\n%s\n\n",
            "; if there is integer overflow, abend to DOS",
            "iovrflo:",
            "         mov       ax, 4C01h",
            "         int       21h");
}

void    getjumplabel(char *jumplabel)
{
    static int    jlnumber = 0;
    char        jlnumstring[5];
    
    strcpy(jumplabel, "Jump");
    sprintf(jlnumstring, "%d", jlnumber++);
    strcat(jumplabel, jlnumstring);
}

void     readInt()
{
    fprintf(ofp, "Readint:\n");
    fprintf(ofp, "         xor       cx, cx\n");
    fprintf(ofp, "         mov       bx, 10\n");
    fprintf(ofp, "Re1:\n");
    fprintf(ofp, "         mov       ah, 01h\n");
    fprintf(ofp, "         int       21h\n");
    fprintf(ofp, "         cmp       al, 13\n");
    fprintf(ofp, "         jz        Re2\n");
    
    fprintf(ofp, "         and       ax, 0fh\n");
    fprintf(ofp, "         xchg      ax, cx\n");
    fprintf(ofp, "         mul       bx\n");
    fprintf(ofp, "         add       ax, cx\n");
    fprintf(ofp, "         xchg      ax, cx\n");
    fprintf(ofp, "         jmp       Re1\n");
    
    fprintf(ofp, "Re2:\n");
    fprintf(ofp, "         ret\n");
}

void     printInt()
{
    fprintf(ofp, "Writeint:\n");
    
    fprintf(ofp, "         xor       cx, cx\n");
    fprintf(ofp, "Wr1:\n");
    fprintf(ofp, "         mov       dx, 0\n");
    fprintf(ofp, "         mov       bx, 10\n");
    fprintf(ofp, "         div       bx\n");
    fprintf(ofp, "         push      dx\n");
    fprintf(ofp, "         inc       cx\n");
    fprintf(ofp, "         cmp       ax, 0\n");
    fprintf(ofp, "         je        Wr2\n");
    fprintf(ofp, "         jmp       Wr1\n");
    
    fprintf(ofp, "Wr2:\n");
    fprintf(ofp, "         pop       dx\n");
    fprintf(ofp, "         add       dl, 30h\n");
    fprintf(ofp, "         mov       ah, 02h\n");
    fprintf(ofp, "         int       21h\n");
    fprintf(ofp, "         loop      Wr2\n");
    
    fprintf(ofp, "         mov       dl, 0dh\n");
    fprintf(ofp, "         mov       ah, 02h\n");
    fprintf(ofp, "         int       21h\n");
    fprintf(ofp, "         mov       dl, 0ah\n");
    fprintf(ofp, "         int       21h\n");
    fprintf(ofp, "         ret\n");
}
