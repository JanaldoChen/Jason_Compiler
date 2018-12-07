#include	<stdio.h>

#include	"scan.h"
#include	"quad.h"



#define	MAXPARSESTACK	1000
#define	MAXACTIONSTACK	1000
#define	FILENAMELEN	100
#define Null 0
enum nontermtype {NTProgram, NTHeader, NTDeclSec, NTVarDecls,
			NTMoreVarDecls, NTVarDecl, NTDataType, NTIdList,
			NTMoreIdList, NTProcDecls, NTProcDecl, NTProcHeader,
			NTProcDeclSec, NTParamDeclSec, NTParamDecls,
			NTMoreParamDecls, NTParamDecl, NTBlock, NTStatements, 
			NTMoreStatements, NTStatement, NTExpression,
			NTMoreExpression, NTTerm, NTMoreTerm, NTFactor,
			NTCondition, NTAddOp, NTMultOp, NTRelOp, NTArglist,
			NTArguments, NTMoreArguments, NTElseClause
};

char	*nontermstrings[] = {"NTProgram", "NTHeader", "NTDeclSec",
			"NTVarDecls", "NTMoreVarDecls", "NTVarDecl",
			"NTDataType", "NTIdList", "NTMoreIdList",
			"NTProcDecls", "NTProcDecl", "NTProcHeader",
			"NTProcDeclSec", "NTParamDeclSec", "NTParamDecls",
			"NTMoreParamDecls", "NTParamDecl", "NTBlock",
			"NTStatements", "NTMoreStatements", "NTStatement",
			"NTExpression", "NTMoreExpression", "NTTerm",
			"NTMoreTerm", "NTFactor", "NTCondition", "NTAddOp",
			"NTMultOp", "NTRelOp", "NTArglist", "NTArguments",
			"NTMoreArguments", "NTElseClause"};

extern char *tokclstring[];

const int	prodtable[][NUMTOKENS+3] = {
/*Program*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*Header*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*DeclSect*/	{ 4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*VarDeclSect*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*MoreVarDecls*/{ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 7, 0, 0, 6, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*VarDecl*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 8, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*DataType*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 9, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*IdList*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,11,
			0},
/*MoreIdList*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0,13,12, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*ProcDecls*/	{15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,14, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*ProcDecl*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,16, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*ProcHeader*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,17, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*ProcDeclSect*/{18, 0,18, 0, 0, 0, 0, 0, 0, 0, 0,18, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*ParamDeclSec*/{20, 0,20, 0, 0, 0, 0, 0, 0, 0, 0,19, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*ParamDecls*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,21, 0, 0, 0, 0,21, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*MoreParmDcls*/{23, 0,23, 0, 0, 0, 0, 0, 0, 0,22, 0, 0, 0, 0,22, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*ParamDecl*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,24, 0, 0, 0, 0,24, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*Block*/	{25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*Statements*/	{ 0,26, 0, 0, 0, 0, 0, 0, 0,26, 0, 0, 0, 0,26, 0,26, 0,26,
		       26,26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		        0},
/*MoreStatmnts*/{ 0, 0, 0, 0,28,28,28,28,28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0,27, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*Statement*/	{ 0,35, 0, 0,36,36,36,36,36,32, 0, 0, 0, 0,29, 0,30, 0,34,
		       33,31, 0, 0, 0, 0, 0,36, 0, 0, 0, 0, 0, 0, 0, 0, 0,

			0},
/*Expression*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,37,
		       37},
/*MoreExpr.*/	{ 0, 0, 0,39,39,39,39,39,39, 0, 0, 0, 0, 0, 0, 0, 0,39, 0,
			0, 0, 0,38,38, 0,39,39, 0, 0,39,39,39, 0, 0, 0, 0,
			0},
/*Term*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,40,
		       40},
/*MoreTerm*/	{ 0, 0, 0,42,42,42,42,42,42, 0, 0, 0, 0, 0, 0, 0, 0,42, 0,
			0, 0,41,42,42,41,42,42, 0, 0,42,42,42, 0, 0, 0, 0,
			0},
/*Factor*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,43,
		       44},
/*Condition*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,45,
		       45},
/*AddOp*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0,46,47, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*MultOp*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0,48, 0, 0,49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0},
/*RelOp*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0,50, 0, 0, 0,52,53,51, 0, 0, 0, 0,
			0},
/*ArgList*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0,55, 0, 0, 0, 0, 0,54, 0, 0, 0,
			0},
/*Arguments*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,56,
			0},
/*MoreArgs.*/	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,57, 0, 0, 0, 0, 0,58, 0, 0,
			0},
/*ElseClause*/	{ 0, 0, 0, 0,59, 0,60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0}
};

enum termtagtype	{Nonterm, Term, Action};

enum actiontype		{AcAddProgName, AcPushReal, AcPushInt, AcDeclVar,
			AcPopType, AcAddProcName, AcStartBlock, AcEndBlock,
			AcCloseProc, AcSetParamLink, AcDeclParam,
			AcGenRead, AcGenWrite, AcPushId, AcPushConst,
			AcPushAddOp, AcPushMultOp, AcPushRelOp, AcCalcTerm,
			AcCalcExpression, AcGenAssn, AcEvalCondition,
			AcStartIf, AcStartElse, AcFinishIf, AcPrepareLoop,
			AcStartWhile, AcStartUntil, AcFinishLoop,
			AcAddArgument, AcStartCall, AcEndCall, AcEndArgList
};

const char  *actionstring[] = {"AddProgName", "PushReal", "PushInt",
			"DeclVar", "PopType", "AddProcName", "StartBlock",
			"EndBlock", "CloseProc", "SetParamLink",
			"DeclParam", "GenRead", "GenWrite", "PushId",
			"PushConst", "PushAddOp", "PushMultOp",
			"PushRelOp", "CalcTerm", "CalcExpression",
			"GenAssn", "EvalCondition", "StartIf", "StartElse",
			"FinishIf", "PrepareLoop", "StartWhile",
			"StartUntil", "FinishLoop", "AddArgument",
			"StartCall", "EndCall", "EndArgList"
};

struct prodarraytype	{
	enum termtagtype	PrTermOrNonterm;
        int			PrParseItem;
};

const struct prodarraytype	prodarray[] = 	{
/* Program */	{Nonterm, NTHeader},		{Nonterm, NTDeclSec},
		{Nonterm, NTBlock},             {Term, tokperiod},

/* Header */	{Term, tokprogram},             {Term, tokidentifier},
		{Action, AcAddProgName},	{Term, toksemicolon},

/* DeclSec */	{Term, tokdeclare},		{Nonterm, NTVarDecls},
		{Nonterm, NTProcDecls},
/* DeclSec */	/* <Nil> */

/* VarDeclSec*/	{Nonterm, NTVarDecl}, 		{Nonterm, NTMoreVarDecls},

/*MoreVarDcls*/	{Nonterm, NTVarDecl}, 		{Nonterm, NTMoreVarDecls},
/*MoreVarDcls*/	/* <Nil> */

/* VarDecl */	{Nonterm, NTDataType}, 		{Nonterm, NTIdList},
		{Term, toksemicolon},

/* DataType */	{Term, tokreal},		{Action, AcPushReal},
/* DataType */	{Term, tokinteger},		{Action, AcPushInt},

/* IdList */	{Term, tokidentifier},		{Action, AcDeclVar},
		{Nonterm, NTMoreIdList},

/* MoreIdLst*/	{Term, tokcomma},		{Term, tokidentifier},
		{Action, AcDeclVar},		{Nonterm, NTMoreIdList},
/* MoreIdList*/	{Action, AcPopType},

/* ProcDecls*/	{Nonterm, NTProcDecl}, 		{Nonterm, NTProcDecls},
/* ProcDecls*/	/* <Nil> */

/* ProcDecl*/	{Nonterm, NTProcHeader}, 	{Nonterm, NTProcDeclSec},
		{Nonterm, NTBlock}, 		{Action, AcCloseProc},
		{Term, toksemicolon},

/* ProcHeader*/	{Term, tokprocedure}, 		{Term, tokidentifier},
		{Action, AcAddProcName},	{Term, toksemicolon},

/* ProcDeclSec*/{Nonterm, NTParamDeclSec},	{Nonterm, NTDeclSec},

/* ParamDclSec*/{Term, tokparameters},		{Action, AcSetParamLink},
		{Nonterm, NTParamDecls},
/* ParamDclSec*//* <Nil> */       

/* ParamDecls*/	{Nonterm, NTParamDecl},		{Nonterm, NTMoreParamDecls},

/*MorePrmDcls*/	{Nonterm, NTParamDecl},		{Nonterm, NTMoreParamDecls},
/*MorePrmDcls*/	/* <Nil> */

/* ParamDecl */	{Nonterm, NTDataType},		{Term, tokidentifier},
		{Action, AcDeclParam},		{Term, toksemicolon},

/* Block */	{Term, tokbegin},		{Action, AcStartBlock},
		{Nonterm, NTStatements},	{Term, tokend},
		{Action, AcEndBlock},

/* Statements*/	{Nonterm, NTStatement},		{Nonterm, NTMoreStatements},

/*MoreStatmnts*/{Term, toksemicolon},		{Nonterm, NTStatement},
		{Nonterm, NTMoreStatements},
/*MoreStatmnts*//* <Nil> */

/* Statement */	{Term, tokread}, 		{Term, tokidentifier},
		{Action, AcGenRead},
/* Statement */	{Term, tokset},			{Term, tokidentifier},
		{Action, AcPushId},		{Term, tokequals},
		{Nonterm, NTExpression},	{Action, AcGenAssn},
/* Statement */	{Term, tokwrite}, 		{Term, tokidentifier},
		{Action, AcGenWrite},
/* Statement */	{Term, tokif},			{Nonterm, NTCondition},
		{Term, tokthen},		{Action, AcStartIf},
		{Nonterm, NTStatements},	{Nonterm, NTElseClause},
/* Statement */	{Term, tokwhile},		{Action, AcPrepareLoop},
		{Nonterm, NTCondition},		{Action, AcStartWhile},
		{Term, tokdo},			{Nonterm, NTStatements},
		{Term, tokendwhile},		{Action, AcFinishLoop},
/* Statement */	{Term, tokuntil},		{Action, AcPrepareLoop},
		{Nonterm, NTCondition},		{Action, AcStartUntil},
		{Term, tokdo},			{Nonterm, NTStatements},
		{Term, tokenduntil},		{Action, AcFinishLoop},
/* Statement */	{Term, tokcall},		{Term, tokidentifier},
		{Action, AcStartCall},		{Nonterm, NTArglist},
		{Action, AcEndCall},
/* Statement */	/* <Nil> */

/* Expression */{Nonterm, NTTerm},		{Nonterm, NTMoreExpression},

/* MoreExpr */	{Nonterm, NTAddOp},		{Nonterm, NTTerm},
		{Action, AcCalcExpression},	{Nonterm, NTMoreExpression},
/* MoreExpr */	/* <Nil> */

/* Term */      {Nonterm, NTFactor},		{Nonterm, NTMoreTerm},

/* MoreTerm */	{Nonterm, NTMultOp},		{Nonterm, NTFactor},
		{Action, AcCalcTerm},		{Nonterm, NTMoreTerm},
/* MoreTerm */	/* <Nil> */

/* Factor */	{Term, tokidentifier},		{Action, AcPushId},
/* Factor */	{Term, tokconstant},		{Action, AcPushConst},

/* Condition */	{Nonterm, NTExpression}, 	{Nonterm, NTRelOp},
		{Nonterm, NTExpression},	{Action, AcEvalCondition},

/* AddOp */	{Term, tokplus},		{Action, AcPushAddOp},
/* AddOp */	{Term, tokminus},		{Action, AcPushAddOp},

/* MultOp */	{Term, tokstar},		{Action, AcPushMultOp},
/* MultOp */	{Term, tokslash},		{Action, AcPushMultOp},

/* RelOp */	{Term, tokequals},		{Action, AcPushRelOp},
/* RelOp */	{Term, toknotequal},		{Action, AcPushRelOp},
/* RelOp */	{Term, tokgreater},		{Action, AcPushRelOp},
/* RelOp */	{Term, tokless},		{Action, AcPushRelOp},

/* ArgList */	{Term, tokopenparen},		{Nonterm, NTArguments},
		{Term, tokcloseparen},
/* ArgList */	/* <Nil> */

/* Arguments */	{Term, tokidentifier},		{Action, AcAddArgument},
		{Nonterm, NTMoreArguments},

/* MoreArgs */	{Term, tokcomma},		{Term, tokidentifier},
		{Action, AcAddArgument},	{Nonterm, NTMoreArguments},
/* MoreArgs */	{Action, AcEndArgList},

/* ElseClause */{Term, tokelse},		{Action, AcStartElse},
		{Nonterm, NTStatements},	{Term, tokendif},
                {Action, AcFinishIf},

/* ElseClause */{Term, tokendif},		{Action, AcFinishIf}
};

struct prodindexrec	{
	int	prstart, prlength;
};

/*
 *	The index to the productions array, showing the
 *	starting position of the production and the number of items
 *	on its right-hand side. The first item is a dummy to get past the
 *	zeroth element of the array and any other second-entries of 0
 *	indicate an epsilon production.
 */
const struct prodindexrec	prodindex[]	=	{
		  {0, 0},   {0, 4},   {4, 4},   {8, 3},  {11, 0},  {11, 2},
		 {13, 2},  {15, 0},  {15, 3},  {18, 2},  {20, 2},  {22, 3},
		 {25, 4},  {29, 1},  {30, 2},  {32, 0},  {32, 5},  {37, 4},
		 {41, 2},  {43, 3},  {46, 0},  {46, 2},  {48, 2},  {50, 0},
		 {50, 4},  {54, 5},  {59, 2},  {61, 3},  {64, 0},  {64, 3},
		 {67, 6},  {73, 3},  {76, 6},  {82, 8},  {90, 8},  {98, 5},
		{103, 0}, {103, 2}, {105, 4}, {109, 0}, {109, 2}, {111, 4},
		{115, 0}, {115, 2}, {117, 2}, {119, 4}, {123, 2}, {125, 2},
		{127, 2}, {129, 2}, {131, 2}, {133, 2}, {135, 2}, {137, 2},
		{139, 3}, {142, 0}, {142, 3}, {145, 4}, {149, 1}, {150, 5},
		{155, 2}

};

struct parsenoderec	{
	int	level;
	enum termtagtype	TermOrNonterm;
	int	ParseItem, symtabentry;
};


struct parsenoderec	*getparsenode(enum termtagtype termtag, int info);
void parse(void);
void error(char message[], int linenum);

typedef	struct	{
	int				parsetop;
	struct	parsenoderec		*parsptr[MAXPARSESTACK];
}	parsestack;

parsestack	pa;

struct parsenoderec	*parsepop(void);
void			parsepush(struct parsenoderec *x);
enum logical    	parseempty(void);
void			initparsestack(void);
procstackitem		initparseentry(int tabindex);

void    processnonterm(struct parsenoderec *thisnode);
void	printnonterm(int i);
void	printaction(enum actiontype i);
int	matchtoken(int thistoken, int thattoken);

int		actionpop(void);
void		actionpush(int x);
enum logical    actionempty(void);
int		actiontopitem(void);
void		initactionstack(void);

typedef	struct	{
	int	      	actiontop;
	int   		actionitem[MAXACTIONSTACK];
}	actionstack;

actionstack	ac;

const struct addressrec	no_op = {opnnull, Null};
const struct addressrec	func = {opnaddr, tokfloat};

struct addressrec	getaddress(enum tokentype tokopnd);
struct addressrec	GenArithQuad(struct addressrec op2,
enum optype optor, struct addressrec op3);

int		foldconstants(enum optype intopcode, struct addressrec a,
				struct addressrec b, struct addressrec c);
int		foldrconstants(enum optype intopcode, struct addressrec a,
				struct addressrec b, struct addressrec c);
int		foldiconstants(enum optype intopcode, struct addressrec a,
				struct addressrec b, struct addressrec c);
int	genfuncall(struct addressrec *op1, struct addressrec op2);

void	processaction(int actiontype);
void	AddProgName(void); 
void	PushReal(void);
void	PushInt(void);
void	DeclareVar(void);
void	PopType(void);
void	AddProcName(void);
void	StartBlock(void);
void	EndBlock(void);
void	CloseProc(void);
void	SetParamLink(void);
void 	DeclParam(void);
void	GenRead(void);
void	GenWrite(void);
void	PushId(void);
void	PushConst(void);
void	PushAddOp(void);
void	PushMultOp(void);
void	PushRelOp(void);
void	CalcTerm(void);
void	CalcExpression(void);
void	GenAssn(void);
void	EvalCondition(void);
void	StartIf(void);
void	StartElse(void);
void	FinishIf(void);
void	PrepareLoop(void);
void	StartWhile(void);
void	StartUntil(void);
void	FinishLoop(void);
void	AddArgument(void);
void	StartCall(void);
void	EndCall(void);
void	EndArgList(void);

FILE *ifp;
char tokenstring[TOKENSTRINGLENGTH];
enum tokentype	thistoken, lasttoken;
extern procstackitem thisproc;
struct parsenoderec	*parsetree, *thisnode;
int	currentlevel = 1, tabindex, oldtabindex, numops,
			paramlink, nextop = 0, nextarg,
			didcalculation = NO;

int	main(int argc, char *argv[])
{
	char	filename[FILENAMELEN];
	int	numcodes;

	initializesymtab();
	ifp = openfile(argc, argv, filename);
    printf("lexeme    token_class   linenum\n");
    printf("_______________________________\n");
	thistoken = gettoken(ifp, &tabindex);
    printlexeme(tabindex);
    printf("     %s ", tokclstring[thistoken]);
    printf("%d\n", linenum);
	parse();
	dumpsymboltable();
	intpeephole();
	numcodes = intglobal();
	printintcode();
    printintcode2();
	return(0);
}

void	parse(void)
{
	initparsestack();
	parsetree = getparsenode(Nonterm, NTProgram);
	parsepush(parsetree);
	do	{

		thisnode = parsepop();

		switch (thisnode -> TermOrNonterm)	{
		  case Term:
			if (matchtoken(thistoken, thisnode -> ParseItem))
				thisnode -> symtabentry = tabindex;
			else 	{
                putchar('\"');
				printlexeme(thisnode -> ParseItem);
                printf("\" expected instead of \"");
				printlexeme(tabindex);
				error("\"", linenum);
			}
			lasttoken = thistoken;
            oldtabindex = tabindex;
			thistoken = gettoken(ifp, &tabindex);
            printlexeme(tabindex);
            printf("     %s ", tokclstring[thistoken]);
            printf("%d\n", linenum);
			break;

		  case Nonterm:
			processnonterm(thisnode);
			break;

		  case Action:
			processaction(thisnode -> ParseItem);
			break;

		  default:
			error("Encountered invalid production item", linenum);
                }
        }  while(!parseempty());
}

struct parsenoderec	*getparsenode(enum termtagtype termtag, int info)
{
	struct parsenoderec	*p;

	p = (struct parsenoderec *)malloc(sizeof(struct parsenoderec));
	p -> level = currentlevel;
	p -> TermOrNonterm = termtag;
	p -> ParseItem = info;
        return(p);
}

void processnonterm(struct parsenoderec *thisnode)
{
	struct parsenoderec	*p;
	int			prodnum, i;

	currentlevel = thisnode -> level +1;
	if ((prodnum = prodtable[thisnode -> ParseItem][thistoken]) == 0){
        	putchar('\"');
		printlexeme(thistoken);
		error("\" cannot be used this way", linenum);
        }

	if (prodindex[prodnum].prlength != 0)	{
		i = prodindex[prodnum].prstart
			+ prodindex[prodnum].prlength - 1;
		p = getparsenode(prodarray[i].PrTermOrNonterm,
				prodarray[i].PrParseItem);
		parsepush(p);
        	
		for (i = prodindex[prodnum].prstart
					+ prodindex[prodnum].prlength - 2;
				i >= prodindex[prodnum].prstart;  --i)	{
			p = getparsenode(prodarray[i].PrTermOrNonterm,
				prodarray[i].PrParseItem);
                	parsepush(p);
		}
	}

}

void	processaction(int actiontype)
{
	switch(actiontype)	{
	  case AcAddProgName:	AddProgName(); break;
	  case AcPushReal:	PushReal(); break;
	  case AcPushInt:	PushInt(); break;
	  case AcDeclVar:	DeclareVar(); break;
	  case AcPopType:	PopType(); break;
	  case AcAddProcName:	AddProcName(); break;
	  case AcStartBlock:	StartBlock(); break;
	  case AcEndBlock:	EndBlock(); break;
	  case AcCloseProc:	CloseProc(); break;
	  case AcSetParamLink:	SetParamLink(); break;
	  case AcDeclParam:	DeclParam(); break;
	  case AcGenRead:	GenRead(); break;
	  case AcGenWrite:	GenWrite(); break;
	  case AcPushId:	PushId(); break;
	  case AcPushConst:	PushConst(); break;
	  case AcPushAddOp:     PushAddOp(); break;
	  case AcPushMultOp:	PushMultOp(); break;
	  case AcPushRelOp:	PushRelOp(); break;
	  case AcCalcTerm:	CalcTerm(); break;
	  case AcCalcExpression:CalcExpression(); break;
	  case AcGenAssn:	GenAssn(); break;
	  case AcEvalCondition:	EvalCondition(); break;
	  case AcStartIf:	StartIf(); break;
	  case AcStartElse:	StartElse(); break;
	  case AcFinishIf:	FinishIf(); break;
	  case AcPrepareLoop:	PrepareLoop(); break;
	  case AcStartWhile:	StartWhile(); break;
	  case AcStartUntil:	StartUntil(); break;
	  case AcFinishLoop:	FinishLoop(); break;
	  case AcAddArgument:	AddArgument(); break;
	  case AcStartCall:	StartCall(); break;
	  case AcEndCall:	EndCall(); break;
	  case AcEndArgList:	EndArgList(); break;
	  default:		error("Invalid action", linenum);
        }
}

void	AddProgName(void)
{
	installdatatype(oldtabindex, stprogram, dtprogram);
    thisproc = initprocentry(oldtabindex);
}

void	PushReal(void)
{
	actionpush(dtreal);
}

void	PushInt(void)
{
	actionpush(dtinteger);
}

void	DeclareVar(void)
{
	int	vartype;
    vartype = actiontopitem();
    if (data_class(oldtabindex) != dtunknown) {
		if (getproc(oldtabindex) != thisproc.proc)
			oldtabindex = openscope(oldtabindex);
        else
			error("Redeclared variable", linenum);
    }

	installdatatype(oldtabindex, stvariable, vartype);
        setproc(thisproc.proc, oldtabindex);
}

void	PopType(void)
{
	actionpop();
}

void	AddProcName(void)
{
    if (data_class(oldtabindex) != dtunknown) {
		if (getproc(oldtabindex) != thisproc.proc)
			oldtabindex = openscope(oldtabindex);
        else
			error("Variable redeclared as procedure", linenum);
    }
	installdatatype(oldtabindex, stprocedure, dtprocedure);
        setproc(thisproc.proc, oldtabindex);
	procpush(thisproc);
        thisproc = initprocentry(oldtabindex);
}

void	StartBlock(void)
{
	struct addressrec	proc;
	proc = setaddress(opnaddr, thisproc.proc);
        genquad(oplabel,proc, no_op, no_op); 
}

void	EndBlock(void)
{
        genquad(opreturn, no_op, no_op, no_op);
}

void	CloseProc(void)
{
	closescope();
        thisproc = procpop();
}

void	SetParamLink(void)
{
	paramlink = thisproc.proc;
}

void 	DeclParam(void)
{
    enum datatype	paramtype;

	if ((paramtype = actionpop()) != dtinteger
				&& paramtype != dtreal)	{
         	printf("typetoken is %d\n", paramtype/*token*/);
		error("Expected \"Integer\" or \" Real\"", linenum);
        }
    if (data_class(oldtabindex) != dtunknown) {
		if (getproc(oldtabindex) != thisproc.proc)
			oldtabindex = openscope(oldtabindex);
        else
			error("Redeclared variable", linenum);
    }

	installdatatype(oldtabindex, stparameter, paramtype);
	setivalue(paramlink, oldtabindex);
        setproc(thisproc.proc, oldtabindex);
        paramlink = oldtabindex;

	
}

void	GenRead(void)
{
	struct addressrec x;

	if (!isvalidtype(oldtabindex))	{
		printlexeme(oldtabindex); 
                error(" is an invalid type", linenum);
	}

	x = setaddress(opnvar, oldtabindex);
	nextop = genquad(opread, x, no_op, no_op); 
}

void	GenWrite(void)
{
	struct addressrec x;
	if (!isvalidtype(oldtabindex))	{
		printlexeme(oldtabindex); 
                error(" is an invalid type", linenum);
	}

	x = setaddress(opnvar, oldtabindex);
	nextop = genquad(opwrite, x, no_op, no_op);
}

void	PushId(void)
{
	if (!isvalidtype(oldtabindex))	{
		printlexeme(oldtabindex); 
                error(" is an invalid type", linenum);
	}
	actionpush(oldtabindex);
}

void	PushConst(void)
{
	actionpush(oldtabindex);
}

void	PushAddOp(void)
{
	actionpush(oldtabindex);
}

void	PushMultOp(void)
{
	actionpush(oldtabindex);
}

void	PushRelOp(void)
{
	actionpush(oldtabindex);
}

 void	CalcTerm(void)
{
	enum tokentype		tokoptor, tokopnd1, tokopnd2;
	enum optype		optor = opnull;
        struct addressrec	op1, op2, op3;

	tokopnd2 = actionpop();
	tokoptor = actionpop();
	tokopnd1 = actionpop();

	switch(tokoptor)	{
	  case tokstar:	optor = opmult; break;
	  case tokslash: optor = opdiv;	break;
	  default: printlexeme(tokoptor);
	  			error(" is an invalid operator", linenum); 
	}
	op2 = getaddress(tokopnd1);
        op3 = getaddress(tokopnd2);

	op1 = GenArithQuad(op2, optor, op3);

	didcalculation = YES;
	actionpush(op1.opnd);                  
}

void	CalcExpression(void)
{
	enum tokentype		tokoptor, tokopnd1, tokopnd2;
	enum optype		optor = opnull;
        struct addressrec	op1, op2, op3;

	tokopnd2 = actionpop();
	tokoptor = actionpop();
	tokopnd1 = actionpop();

	switch(tokoptor)	{
	  case tokplus:		optor = opadd; break;
	  case tokminus:	optor = opsub;	break;
	  default:	printlexeme(tokoptor);
	  			error(" is an invalid operator", linenum); 
	}

	op2 = getaddress(tokopnd1);
        op3 = getaddress(tokopnd2);

	op1 = GenArithQuad(op2, optor, op3);	
	actionpush(op1.opnd);

    didcalculation = YES;
}

void	GenAssn(void)
{
	struct addressrec	op1, op2, op3;
	op2 = getaddress(actionpop());
	op1 = getaddress(actionpop());

	if (data_class(op1.opnd) == data_class(op2.opnd))	
		nextop = genquad(opassn, op1, op2, no_op);
	else if (data_class(op1.opnd) == dtreal)	{
		nextop = genfuncall(&op3, op2);

        didcalculation = YES;
		nextop = genquad(opassn, op1, op3, no_op);
	}
	else	{
		printf("%d\t%d", op1.opndtype, op2.opndtype);
		error("Assigning a value to an incompatible type",
			linenum);
	}        	
 
	if (didcalculation)
		permtarget(op1);
  	didcalculation = NO;
}

int		foldconstants(enum optype intopcode, struct addressrec a,
				struct addressrec b, struct addressrec c)
{
	int	nextop;

	if (data_class(b.opnd) == dtreal)	
		nextop = foldrconstants(intopcode, a, b, c);
	else	
		nextop = foldiconstants(intopcode, a, b, c);
        didcalculation = YES;
        return(nextop);
}

int		foldrconstants(enum optype intopcode, struct addressrec a,
				struct addressrec b, struct addressrec c)
{
	char	valuestring[10];
	float	x = 0, y = 0, z = 0;
	int	tabindex;
	y = getrvalue(b.opnd);
	z = getrvalue(c.opnd);

	switch(intopcode)	{
	  case opadd:	x = y + z;	break;
	  case opsub:	x = y - z;	break;	
	  case opmult:	x = y * z;	break;
	  case opdiv:	x = y / z;	break;
      default: break;
	}

	gcvt(x, 5, valuestring);
	if (!installname(valuestring, &tabindex))	{
		setattrib(stunknown, tokconstant, tabindex);
		installdatatype(tabindex, stliteral, dtreal);
		setrvalue(tabindex, x);
}
	b.opnd = tabindex;
        b.opndtype = opnconst;
	return(genquad(opassn,a, b, no_op));


}

int		foldiconstants(enum optype intopcode, struct addressrec a,
				struct addressrec b, struct addressrec c)
{
	char	valuestring[10];
	int	x = 0, y, z;
        int	tabindex;
	y = getivalue(b.opnd);
	z = getivalue(c.opnd);

	switch(intopcode)	{
	  case opadd:	x = y + z;	break;
	  case opsub:	x = y - z;	break;	
	  case opmult:	x = y * z;	break;
	  case opdiv:	x = y / z;	break;
      default: break;
	}
    sprintf(valuestring, "%d", x);
	if (!installname(valuestring, &tabindex))	{
		setattrib(stunknown, tokconstant, tabindex);
		installdatatype(tabindex, stliteral, dtinteger);
	}

	b.opnd = tabindex;
        b.opndtype = opnconst;
	return(genquad(opassn,a, b, no_op));
}

int genfuncall(struct addressrec *op1, struct addressrec op2)
{
	struct	addressrec	op3;
	char valuestring[10];
    int	tabindex;

	*op1 = gettempvar(dtreal);
        setproc(thisproc.proc, op1->opnd); 
	if  (symclass(op2.opnd) == stliteral)	{
        sprintf(valuestring, "%d", getivalue(op2.opnd));
		strcat(valuestring,".0");
		if (!installname(valuestring, &tabindex))	{
			setattrib(stunknown, tokconstant, tabindex);
			installdatatype(tabindex, stliteral, dtreal);
		}

		op3.opnd = tabindex;
        	op3.opndtype = opnconst;
		nextop = genquad(opassn,*op1, op3, no_op);

	}
	else
		nextop = genquad(opfunc, *op1, func, op2);
        return(nextop);
} 

void	EvalCondition(void)
{
	int	tokoptor;
	struct addressrec	op1, op2, op3;

	op3 = getaddress(actionpop());
	tokoptor = actionpop();
	op2 = getaddress(actionpop());
	op1 = GenArithQuad(op2, opsub, op3);
	actionpush(op1.opnd);
        actionpush(tokoptor);
}

void	StartIf(void)
{
	enum tokentype		relop;
	enum optype	otype = opnull;
	struct addressrec	operand, addr1;
	relop = actionpop();
	operand = getaddress(actionpop());
	switch(relop)	{
	  case tokgreater:	otype = opifnegzjump; break;
	  case tokless:	otype = opifposzjump; break;
      case tokequals: otype = opifnzjump; break;
      case toknotequal:	otype = opifzjump; break;
      default: break;
	}
	addr1 = gettemplabel();
	nextop = genquad(otype, operand, addr1, no_op);
        actionpush(addr1.opnd);


}

void	StartElse(void)
{
    struct addressrec	addr1, addr2;
	addr1 = setaddress((enum operandtype)oplabel, actionpop());
	addr2 = gettemplabel();

	genquad(opjump, addr2, no_op, no_op);
	nextop = genquad(oplabel, addr1, no_op, no_op);
	setivalue(addr1.opnd, nextop);
        actionpush(addr2.opnd);

}

void	FinishIf(void)
{
	struct addressrec	addr1;

        addr1 = setaddress(opnaddr,  actionpop());
	nextop = genquad(oplabel, addr1, no_op, no_op);
        setivalue(addr1.opnd, nextop);

}

void	PrepareLoop(void)
{
	struct addressrec	addr1;
	addr1 = gettemplabel();
	nextop = genquad(oplabel, addr1, no_op, no_op);
	setivalue(addr1.opnd, nextop);
        actionpush(addr1.opnd);
}

void	StartWhile(void)
{
	enum tokentype		relop;
	enum operandtype	otype = opnnull;
        struct addressrec	addr2, op;

	relop = actionpop();
        op = getaddress(actionpop());
	switch(relop)	{
        case tokgreater:	otype = (enum operandtype)opifnegzjump; break;
        case tokless:		otype = (enum operandtype)opifposzjump; break;
        case tokequals:		otype = (enum operandtype)opifnzjump; break;
        case toknotequal:	otype = (enum operandtype)opifzjump; break;
        default: break;
	}
	addr2 = gettemplabel();
	nextop = genquad((enum optype)otype, op, addr2, no_op);
        actionpush(addr2.opnd);

}

void	StartUntil(void)
{
	enum tokentype		relop;
	enum operandtype	otype = opnnull;
        struct addressrec	addr2, op;
	relop = actionpop();
        op = getaddress(actionpop());
	switch(relop)	{
	case tokgreater:	otype = (enum operandtype)opifposjump; break;
	case tokless:		otype = (enum operandtype)opifnegjump; break;
	case tokequals:		otype = (enum operandtype)opifzjump; break;
    case toknotequal:	otype = (enum operandtype)opifnzjump; break;
    default: break;
	}

	addr2 = gettemplabel();
	nextop = genquad((enum optype)otype, op, addr2, no_op);
        actionpush(addr2.opnd);

}

void	FinishLoop(void)
{
	struct addressrec	addr1, addr2;

	addr2 = setaddress(opnaddr, actionpop());
	addr1 = setaddress(opnaddr, actionpop());
		
	genquad(opjump, addr1, no_op, no_op);
	nextop = genquad(oplabel, addr2, no_op, no_op);
	setivalue(addr2.opnd, nextop);

}

void	AddArgument(void)
{
    struct addressrec	addr1;
		
	if(!isvalidtype(oldtabindex))	{
        	printlexeme(oldtabindex);
		error(" is an invalid type", linenum);
        }

	if ((nextarg = getivalue(nextarg)) == 0)
		error("Too many arguments in call statement", linenum);
	if (data_class(nextarg) != data_class(oldtabindex))	
		error("Argument type does not match parameter type",
			linenum); 

	addr1 = setaddress(opnvar, oldtabindex);
        genquad(oparg, addr1, no_op, no_op);
}

void	StartCall(void)
{
	if (data_class(oldtabindex) != dtprocedure)
		error("Cannot call - this is not a procedure", linenum);
	actionpush(oldtabindex);
	nextarg = oldtabindex;

}

void	EndCall(void)
{
	struct addressrec	procname;

    procname = setaddress(opnaddr, actionpop());
	genquad(opproc, procname, no_op, no_op);

}

void	EndArgList(void)
{
	if ((nextarg = getivalue(nextarg)) != 0)
        	error("Missing arguments", linenum);

}

struct addressrec	getaddress(enum tokentype tokopnd)
{
	switch(tokenclass(tokopnd))	{
	  case tokidentifier:	return(setaddress(opnvar, tokopnd));
	  case tokconstant:	return(setaddress(opnconst, tokopnd));

	  default:		printlexeme(tokopnd);
				error(" is an invalid operator", linenum);
        }
    return(setaddress(opnconst, tokopnd));
}


struct addressrec	GenArithQuad(struct addressrec op2,
			enum optype optor, struct addressrec op3)
{
	struct addressrec	op1, op4;

	if (data_class(op2.opnd) == dtinteger)
		if (data_class(op3.opnd) == dtinteger)	{	
			op1 = gettempvar(dtinteger);
                        setproc(thisproc.proc, op1.opnd);
		}
		else	{
			op4 = op2;
                        nextop = genfuncall(&op2, op4);
			op1 = gettempvar(dtreal);
			setproc(thisproc.proc, op1.opnd);
                        didcalculation = 1;
                }
	else if (data_class(op3.opnd) == dtinteger)	{
        op4 = op3;
                nextop = genfuncall(&op3, op4);
		op1 = gettempvar(dtreal);
		setproc(thisproc.proc, op1.opnd);
         }
		else	{
			op1 = gettempvar(dtreal);
			setproc(thisproc.proc, op1.opnd);
		}
	if (symclass(op2.opnd) == stliteral
			&& symclass(op3.opnd) == stliteral)	
		nextop = foldconstants(optor, op1, op2, op3);
	else
        	nextop = genquad(optor, op1, op2, op3);
        return(op1);
}


int		actionpop(void)
{
	if (actionempty())	{
		printf("Parse stack underflow\n");
                exit(10);
	}
	     
	return(ac.actionitem[--ac.actiontop]);
}

void		actionpush(int x)
{
	if (ac.actiontop == MAXACTIONSTACK)	{
		printf("Actions stack overflow\n");
                exit(10);
	}
	ac.actionitem[ac.actiontop++] = x;
}

enum logical    actionempty(void)
{
	return(ac.actiontop == 0);
}

void		initactionstack(void)
{
	ac.actiontop = 0;

}

int	actiontopitem(void)
{
	return(ac.actionitem[ac.actiontop-1]);
}

struct parsenoderec	*parsepop(void)
{
	if (parseempty())	{
		printf("Parse stack underflow\n");
                exit(10);
	}
	     
	return(pa.parsptr[--pa.parsetop]);
}

void	parsepush(struct parsenoderec *x)
{
	if (pa.parsetop == MAXPARSESTACK)	{
		printf("Parse stack overflow\n");
                exit(10);
	}
	pa.parsptr[pa.parsetop++] = x;
}

enum logical    parseempty()
{
	return(pa.parsetop == 0);
}

void	initparsestack(void)
{
	pa.parsetop = 0;
}

int	matchtoken(int thistoken, int thattoken)
{
	return(thistoken == thattoken);
}

void	printnonterm(int i)
{
	printf("%s", nontermstrings[i]);
}

void	printaction(enum actiontype i)
{
	printf("%s", actionstring[i]);
}

void	error(char message[], int linenum)
{
	printf("%s on line %d\n", message, linenum);
        exit(4);
}
