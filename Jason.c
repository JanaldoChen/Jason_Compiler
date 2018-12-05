#include	<stdio.h>

#include	"scan.h"
#include	"quad.h"
#include	"object.h"



#define		MAXPARSESTACK	1000
#define		MAXACTIONSTACK	1000
#define		FILENAMELEN	100
/* The list of nonterminals in the Jason LL(1) grammar */
enum nontermtype	{NTProgram, NTHeader, NTDeclSec, NTVarDecls,
			NTMoreVarDecls, NTVarDecl, NTDataType, NTIdList,
			NTMoreIdList, NTProcDecls, NTProcDecl, NTProcHeader,
			NTProcDeclSec, NTParamDeclSec, NTParamDecls,
			NTMoreParamDecls, NTParamDecl, NTBlock, NTStatements, 
			NTMoreStatements, NTStatement, NTExpression,
			NTMoreExpression, NTTerm, NTMoreTerm, NTFactor,
			NTCondition, NTAddOp, NTMultOp, NTRelOp, NTArglist,
			NTArguments, NTMoreArguments, NTElseClause
};

/*
 *	The nonterminals as character strings, for
 *	when it is necessary or helpful to print them
 *	for debugging or verification purposes
 */
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

/*
 *	The production table for predictive parsing.
 *	The nonzero entries are the production numbers for a
 *	particular nonterminal matched with a lookahead token
 *	Zero entries means that there is no such production
 *	and it is a parsing error.
 */ 
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

/*  Three type of items appear in a production:
	a nonterminal, a terminal or a semantic action */
enum termtagtype	{Nonterm, Term, Action};

/*  The semantic actions of the Jason programming language */
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

/*	The semantic actions as character strings, for
 *	when it is necessary or helpful to print them
 *	for debugging or verification purposes
 */
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

/*
 *	The productions of the Jason language complete with
 *	their semantic actions.  Only the right-hand sides
 *	of these productions appear.  The epsilon-productions
 *	are not listed here.
 */
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

/*  The structure for the nodes of the parse tree */
struct parsenoderec	{
	int			level;
	enum termtagtype	TermOrNonterm;
	int			ParseItem, symtabentry;
};


/* The basic parse functions */
struct parsenoderec	*getparsenode(enum termtagtype termtag, int info);
void			parse(void);
void			error(char message[], int linenum);

/*  The declarations and functions for handling the parse stack */
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

/* The functions which are subsidiary to the function parse */
void    processnonterm(struct parsenoderec *thisnode);
void	printnonterm(int i);
void	printaction(enum actiontype i);
int	matchtoken(int thistoken, int thattoken);

/*  The declarations and functions for handling
			the semantic actions stack */
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

/* Constants used in intermediate code generation */
const struct addressrec	no_op = {opnnull, NULL};
const struct addressrec	func = {opnaddr, tokfloat};


/* Functions used for intermediate code generation
 		which are not a part of the file quad.c */ 
struct addressrec	getaddress(enum tokentype tokopnd);
struct addressrec	GenArithQuad(struct addressrec op2,
			enum optype optor, struct addressrec op3);

/*  Functions used for optimizing intermediate code */
int		foldconstants(enum optype intopcode, struct addressrec a,
				struct addressrec b, struct addressrec c);
int		foldrconstants(enum optype intopcode, struct addressrec a,
				struct addressrec b, struct addressrec c);
int		foldiconstants(enum optype intopcode, struct addressrec a,
				struct addressrec b, struct addressrec c);
int	genfuncall(struct addressrec *op1, struct addressrec op2);
/*
 *	The functions that process the various semantic actions.
 *	processaction is the driver for processing semantic actions,
 *	passing control to the functions which handle specific actions.
 */
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


/* Enternal variables global to the parser */
FILE			*ifp;
char			tokenstring[TOKENSTRINGLENGTH];
enum tokentype		thistoken, lasttoken;
extern procstackitem	thisproc;
struct parsenoderec	*parsetree, *thisnode;
int			currentlevel = 1, tabindex, oldtabindex, numops,
			paramlink, nextop = 0, nextarg,
			didcalculation = NO;

/*
 * main() -	This is a temporary main function to get the parser
 *		up and running.  So far, it opens the source code file,
 * 		parses, pauses then dumps the symbol table and the
 *		optimized intermediate code.  
 */
int	main(int argc, char *argv[])
{
	char	filename[FILENAMELEN];
	int	numcodes;

	initializesymtab();
	ifp = openfile(argc, argv, filename);
	thistoken = gettoken(ifp, &tabindex);
	parse();
	dumpsymboltable();
	intpeephole();
	numcodes = intglobal();
	printintcode();
	writeobject(filename, numcodes);
    dumpsymboltable();
	return(0);
}

/*
 * Parse() -	This procedure checks the production table to
 *		make certain that there is a production for    
 *		which this nonterminal can be expanded that    
 *		begins with this token.  If there isn't, this  
 *		is a fatal syntactic error; the compiler will  
 *		terminate execution.                           
 *
 *		Then it pushes its right sentential form on    
 *		the stack after linking them to their next     
 *		node.                                          
 */
void	parse(void)
{
	int	i, lines = 0;

	initparsestack();
	parsetree = getparsenode(Nonterm, NTProgram);
	parsepush(parsetree);
	do	{
		/*
		 *  Look up the production in the production table.  
		 *  If not there, terminate with an error message. 
		 */
		thisnode = parsepop();

		switch (thisnode -> TermOrNonterm)	{
		  case Term:
			/* If its's a terminal, match it to the 
				lookahead and get a new lookahead token */
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
			break;

		  case Nonterm:
			/* Expand the nonterminal and push the items on
				the right hand side in reverse order */
			processnonterm(thisnode);
/*			if (thisnode -> ParseItem == NTHeader)
                        	AddProgName();*/
			break;

		  case Action:
			/*  Perform the appropriate semantic action */
			processaction(thisnode -> ParseItem);
			break;

		  default:
			error("Encountered invalid production item",
					linenum);
                }
        }  while(!parseempty());
}

/*
 * GetParseNode() -	Get a parse node that will be pushed
 *			on the stack and then fill it with the
 *			appropriate information.
 */
struct parsenoderec	*getparsenode(enum termtagtype termtag, int info)
{
	struct parsenoderec	*p;

	p = (struct parsenoderec *)malloc(sizeof(struct parsenoderec));
	p -> level = currentlevel;
	p -> TermOrNonterm = termtag;
	p -> ParseItem = info;
        return(p);
}

/*
 * ProcessNonterm() -	The details of looking up the nonterminal in the
 *			production table and pushing items on the stack
 */
void processnonterm(struct parsenoderec *thisnode)
{
	struct parsenoderec	*p/*, *q*/;
	int			prodnum, i;

	/*
	 *	Look up the nonterminal in the production table.
	 *	If the production number is 0, there is no production
	 *	and it is a parse error.
         */
	currentlevel = thisnode -> level +1;
	if ((prodnum = prodtable[thisnode -> ParseItem][thistoken]) == 0){
        	putchar('\"');
		printlexeme(thistoken);
		error("\" cannot be used this way", linenum);
        }

	/*
	 *	If there is a production, push the items
	 *	on the right-hand onto the parse stack in
	 *	reverse order.
	 */
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

/*
 * ProcessAction() -	The driver for the semantic actions.  It
 *			calls the appropriate function for the
 *			necessary semantic action.
 */
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

/*
 * AddProgName() -     Install the program name's
 *			attributes in the symbol table
 */
void	AddProgName(void)
{
	/*  Set the attributes as program */
	installdatatype(oldtabindex, stprogram, dtprogram);

	/* Initialize the record thisproc to indicate
		the main program  */
        thisproc = initprocentry(oldtabindex);
}

/*
 * PushReal() -	Push the data type Real onto the
 *		semantic actions stack.
 */
void	PushReal(void)
{
	actionpush(dtreal);
}

/*
 * PushInt() -	Push the data type Integer onto the
 *		semantic actions stack.
 */
void	PushInt(void)
{
	actionpush(dtinteger);
}

/*
 * DeclareVar() -       Install the data type information about
 *			this variable in the attribute table.
 *			Make sure that it hasn't been declared
 *			before in this scope.
 */
void	DeclareVar(void)
{
	int	vartype;

	/*
	 *	This must be a new identifier, whose data type
	 *	must be unknown.  If it is not, it may be from a
	 *	different procedure scope and we can simply open a
	 *	new scope.  If it is not unknown and is in the current
         *	scope, it is an illegal attempt to redeclare the variable.
	 */

        vartype = actiontopitem(); 
	if (data_class(oldtabindex) != dtunknown)
		if (getproc(oldtabindex) != thisproc.proc)
			oldtabindex = openscope(oldtabindex);
                else
			error("Redeclared variable", linenum);

	/* Set the data type and owning procedure for this
			variable in the symbol table */
	installdatatype(oldtabindex, stvariable, vartype);
        setproc(thisproc.proc, oldtabindex);
}

/*
 *  PopType() -	Pop the data type off the semantic actions stack.
 */
void	PopType(void)
{
	actionpop();
}

/*
 * AddProcName() -       Install the data type information about
 *			this procedure in the attribute table.
 *			Make sure that it hasn't been declared
 *			before in this scope.
 */
void	AddProcName(void)
{
	/* Make sure that the identifier is not being redeclared,
		opening a new scope if necessary */
	if (data_class(oldtabindex) != dtunknown)
		if (getproc(oldtabindex) != thisproc.proc)
			oldtabindex = openscope(oldtabindex);
                else
			error("Variable redeclared as procedure", linenum);

	/*
	 *	Set the identifier's attributes as a procedure in
	 *	the symbol table.  Also, push the old procedure that
	 *	was being parsed on the procedure stack and set the scope
	 *	for this procedure.
         */
	installdatatype(oldtabindex, stprocedure, dtprocedure);
        setproc(thisproc.proc, oldtabindex);
	procpush(thisproc);
        thisproc = initprocentry(oldtabindex);
}

/*
 * StartBlock() -	Generate the necessary instructions
 *			for the label beginning the loop and mark the
 *			beginning of a new scope.
 */			
void	StartBlock(void)
{
	struct addressrec	proc;
	/* Generation a label instruction to mark the beginning
					of the block */
	proc = setaddress(opnaddr, thisproc.proc);
        genquad(oplabel,proc, no_op, no_op); 
}

/*
 * EndBlock() -	Generate return at the end of the
 *		procedure or program.
 */
void	EndBlock(void)
{
       /* Generate a Return instruction at the end */
        genquad(opreturn, no_op, no_op, no_op);
}

/*
 * CloseProc() -	Close the scope and pop the procedure stack
 *			to restore the state of affairs to that before
 *			you entered this procedure.
 */	
void	CloseProc(void)
{
	closescope();
        thisproc = procpop();
}

/*
 * SetParamLink() -	Initialize the linked list of parameters that
 *			is stored within the symbol table.	
 */
void	SetParamLink(void)
{
	paramlink = thisproc.proc;
}

/*
 * DeclParam() -	Install the data type information about
 *			this variable in the attribute table.
 *			Make sure that it hasn't been declared
 *			before in this scope.  Also, link to the
 *			parameter list.
 */
void 	DeclParam(void)
{
	enum datatype	paramtype;
        enum tokentype	typetoken;

	if ((paramtype = actionpop()) != dtinteger
				&& paramtype != dtreal)	{
         	printf("typetoken is %d\n", paramtype/*token*/);
		error("Expected \"Integer\" or \" Real\"", linenum);
        }

	/* Make sure that the identifier is not being redeclared.
        	If necessary, open a new scope for the name */
	if (data_class(oldtabindex) != dtunknown)
		if (getproc(oldtabindex) != thisproc.proc)
			oldtabindex = openscope(oldtabindex);
                else
			error("Redeclared variable", linenum);

	/* Set the identifier in the symbol table with the proper type
        	and link it the list of parameters. */
	installdatatype(oldtabindex, stparameter, paramtype);
	setivalue(paramlink, oldtabindex);
        setproc(thisproc.proc, oldtabindex);
        paramlink = oldtabindex;

	
}

/*
 * GenRead() -	Generate the intermediate code for Read statements 
 */
void	GenRead(void)
{
	struct addressrec x;

	/*
	 *	Generate two instructions: an arg instruction to
	 *	pass the identifier as the one argument and
	 *	a call of the Read procedure
	 */
	if (!isvalidtype(oldtabindex))	{
		printlexeme(oldtabindex); 
                error(" is an invalid type", linenum);
	}

	x = setaddress(opnvar, oldtabindex);
	nextop = genquad(opread, x, no_op, no_op); 
}

/*
 * GenWrite() -	Generate the intermediate code for Write statements 
 */
void	GenWrite(void)
{
	struct addressrec x;

	/*
	 *	Generate two instructions: an arg instruction to
	 *	pass the identifier as the one argument and
	 *	a call of the Write procedure
	 */
	if (!isvalidtype(oldtabindex))	{
		printlexeme(oldtabindex); 
                error(" is an invalid type", linenum);
	}

	x = setaddress(opnvar, oldtabindex);
	nextop = genquad(opwrite, x, no_op, no_op);
}

/*
 * PushId() -	Push an identifier onto the semantic actions stack
 *		but first make sure that it is a valid data type
 *		for a variable. 
 */
void	PushId(void)
{
	if (!isvalidtype(oldtabindex))	{
		printlexeme(oldtabindex); 
                error(" is an invalid type", linenum);
	}
	actionpush(oldtabindex);
}

/*
 * PushConst() -	Push a constant onto the semantic actions stack.
 */
void	PushConst(void)
{
	actionpush(oldtabindex);
}

/*
 * PushAddOp() -	Push an additive operator (+ or -) onto
 *			the semantic actions stack.
 */
void	PushAddOp(void)
{
	actionpush(oldtabindex);
}

/*
 * PushMultOp() -	Push a multiplicative operator (* or /) onto
 *			the semantic actions stack.
 */
void	PushMultOp(void)
{
	actionpush(oldtabindex);
}

/*
 * PushRelOp() -	Push a relational operator (=, !, > or <) onto
 *			the semantic actions stack.
 */
void	PushRelOp(void)
{
	actionpush(oldtabindex);
}

/*
 * CalcTerm() -	Calculate a term in a expression being parsed.  The
 *		factor and the rest of the term are on the semantic
 *		actions stack along with the operator, which is the
 *		middle item on the top of the stack.
 */
 void	CalcTerm(void)
{
	enum tokentype		tokoptor, tokopnd1, tokopnd2;
	enum optype		optor;
        struct addressrec	op1, op2, op3;

        /*  Pop the two operands and the operator */
	tokopnd2 = actionpop();
	tokoptor = actionpop();
	tokopnd1 = actionpop();

        /* Convert the operator's token into the matching operation */
	switch(tokoptor)	{
	  case tokstar:		optor = opmult; break;
	  case tokslash:	optor = opdiv;	break;
	  default:		printlexeme(tokoptor);
	  			error(" is an invalid operator", linenum); 
	}

	/* Convert the operand's symbol table entry
		into an address and generate the intermediate code */ 
	op2 = getaddress(tokopnd1);
        op3 = getaddress(tokopnd2);

	op1 = GenArithQuad(op2, optor, op3);

	/*
	 *	There was at least one term calculated;
	 *	we will replace a temporary variable with
	 *	a permanent variable when we generate the
	 *	assignment statement.
	 */ 
	didcalculation = YES;
	actionpush(op1.opnd);                  
}

/*
 * CalcExpression() -	Calculate the expression being a parsed or
 *			a part of it.  The term and rest of the
 *			expression are on the top of the semantic
 *			actions stack.
 */
void	CalcExpression(void)
{
	enum tokentype		tokoptor, tokopnd1, tokopnd2;
	enum optype		optor;
        struct addressrec	op1, op2, op3;

	/*  Pop the two operands and the operator */
	tokopnd2 = actionpop();
	tokoptor = actionpop();
	tokopnd1 = actionpop();

	/* Convert the operator's token into the matching operation */
	switch(tokoptor)	{
	  case tokplus:		optor = opadd; break;
	  case tokminus:	optor = opsub;	break;
	  default:		printlexeme(tokoptor);
	  			error(" is an invalid operator", linenum); 
	}

	/* Convert the operand's symbol table entry
		into an address and generate the intermediate code */ 
	op2 = getaddress(tokopnd1);
        op3 = getaddress(tokopnd2);

	op1 = GenArithQuad(op2, optor, op3);	
	actionpush(op1.opnd);

	/*
	 *	There was at least one term calculated;
	 *	we will replace a temporary variable with
	 *	a permanent variable when we generate the
	 *	assignment statement.
	 */ 
        didcalculation = YES;                  
}

/*
 * GenAssn() -	Generate an assignment instruction, and do the

 *		data type checking and necessary type conversion. 
 */
void	GenAssn(void)
{
	struct addressrec	op1, op2, op3;

	/*
	 *	Pop the two operands from the semantic actions stack -
	 *	the value being assigned and the variable to which
	 *       it is being assigned
	 */
	op2 = getaddress(actionpop());
	op1 = getaddress(actionpop());

	/*
	 *	Make sure that the destination variable is a compatible
	 *	type with the expression.  If they are not the same
	 *	type, this may require converting the source expression's
	 *	value from integer to real.
         */
	if (data_class(op1.opnd) == data_class(op2.opnd))	
		nextop = genquad(opassn, op1, op2, no_op);
	else if (data_class(op1.opnd) == dtreal)	{
		nextop = genfuncall(&op3, op2);

		/*
		 *	There was at least one term calculated;
		 *	we will replace a temporary variable with
		 *	a permanent variable when we generate the
		 *	assignment statement.
		 */ 
                didcalculation = YES;
		nextop = genquad(opassn, op1, op3, no_op);
	}
	else	{
		printf("%d\t%d", op1.opndtype, op2.opndtype);
		error("Assigning a value to an incompatible type",
			linenum);
	}        	
 
	/*
	 *	This is a small "patch" which replaces a temporary
	 *	variable in an assignment instruction with the
	 *	permanent variable to which it should be assigned.
         *	We must also reset the switch didcalculation.
	 */
	if (didcalculation)
		permtarget(op1);
  	didcalculation = NO;
}

/*
 * foldconstants() -	Folds two literal constants together, replacing
 *			an arithmetic operation with an assignment
 *
 */
int		foldconstants(enum optype intopcode, struct addressrec a,
				struct addressrec b, struct addressrec c)
{
	int	nextop;

	/*  We use different subsidiary functions to fold integer
        	and real constants */
	if (data_class(b.opnd) == dtreal)	
		nextop = foldrconstants(intopcode, a, b, c);
	else	
		nextop = foldiconstants(intopcode, a, b, c);
	/* Set the switch so that we can eliminate
		an unnecessary temporary constant*/
        didcalculation = YES;
        return(nextop);
}

/*
 * foldrconstants() -	Fold two real literal constants into one.
 */
int		foldrconstants(enum optype intopcode, struct addressrec a,
				struct addressrec b, struct addressrec c)
{
	char	valuestring[10];
	float	x, y, z;
	int	tabindex;

	/*  Get the values of the constants to be folded
		and perform the appropriate operations */
	y = getrvalue(b.opnd);
	z = getrvalue(c.opnd);

	switch(intopcode)	{
	  case opadd:	x = y + z;	break;
	  case opsub:	x = y - z;	break;	
	  case opmult:	x = y * z;	break;
	  case opdiv:	x = y / z;	break;
	}

	/*  Install the result (in string form) in the symbol table
		so we can reference it properly */
	gcvt(x, 5, valuestring);
	//printf("*****rval is %f\n*******", x);
	if (!installname(valuestring, &tabindex))	{
		setattrib(stunknown, tokconstant, tabindex);
		installdatatype(tabindex, stliteral, dtreal);
		//printf("*****rval is %f\n*******", x);
		setrvalue(tabindex, x);
}
	b.opnd = tabindex;
        b.opndtype = opnconst;
	return(genquad(opassn,a, b, no_op));


}

/*
 * foldiconstants() -	Fold two integer literal constants into one.
 */
int		foldiconstants(enum optype intopcode, struct addressrec a,
				struct addressrec b, struct addressrec c)
{
	char	valuestring[10];
	int	x, y, z;
        int	tabindex;

	/*  Get the values of the constants to be folded
		and perform the appropriate operations */
	y = getivalue(b.opnd);
	z = getivalue(c.opnd);

	switch(intopcode)	{
	  case opadd:	x = y + z;	break;
	  case opsub:	x = y - z;	break;	
	  case opmult:	x = y * z;	break;
	  case opdiv:	x = y / z;	break;
	}

	/*  Install the result (in string form) in the symbol table
		so we can reference it properly */
	//itoa(x, valuestring, 10);
    sprintf(valuestring, "%d", x);
	if (!installname(valuestring, &tabindex))	{
		setattrib(stunknown, tokconstant, tabindex);
		installdatatype(tabindex, stliteral, dtinteger);
	}

	b.opnd = tabindex;
        b.opndtype = opnconst;
	return(genquad(opassn,a, b, no_op));
}


/*
 * genfuncall() -	Generate a call to the function float to
 *			convert an integer value to real.  If it is a
 *			literal, do the conversion and assign that to a
 *			temporary variable.
 */
int genfuncall(struct addressrec *op1, struct addressrec op2)
{
	struct	addressrec	op3;
        float			x;
	char			valuestring[10];
        int			tabindex;

	*op1 = gettempvar(dtreal);
        setproc(thisproc.proc, op1->opnd); 
	if  (symclass(op2.opnd) == stliteral)	{
		//itoa(getivalue(op2.opnd), valuestring,10);
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

/*
 * EvalCondition() -	Generate the necessary instruction to
 *			evaluation a condition
 */
void	EvalCondition(void)
{
	int	tokoptor;
	struct addressrec	op1, op2, op3;

	/* Pop the operands and the relational operator
		from the semantic actions stack */ 
	op3 = getaddress(actionpop());
	tokoptor = actionpop();
	op2 = getaddress(actionpop());

	/*
	 *	Subtract the two expressions and push the result
	 *	and the relational operator back onto the stack
	 *	so the appropriate functions can generate the correct
	 *       condition jumps.
         */
	op1 = GenArithQuad(op2, opsub, op3);
	actionpush(op1.opnd);
        actionpush(tokoptor);
}

/*
 * StartIf() -	Generate the necessary instruction for the beginning
 *		of an IF-THEN-ELSE-ENDIF or IF-THEN-ENDIF
 */
void	StartIf(void)
{
	enum tokentype		relop;
	enum optype		otype;
	struct addressrec	operand, addr1;

        /*  Pop the relational operator and the condition */
	relop = actionpop();
	operand = getaddress(actionpop());

	/* Depending on which relational operator appears, a different
				conditional jump must be used.*/
	switch(relop)	{
	case tokgreater:	otype = opifnegzjump; break;
	case tokless:		otype = opifposzjump; break;
	case tokequals:		otype = opifnzjump; break;
        case toknotequal:	otype = opifzjump;
	}
	/* Get a temporary label and generate a conditional jump
					to that label */
	addr1 = gettemplabel();
	nextop = genquad(otype, operand, addr1, no_op);
        actionpush(addr1.opnd);


}

/*
 * StartElse() -	Generate the instructions to finish the THEN
 */
void	StartElse(void)
{       struct addressrec	addr1, addr2;

	addr1 = setaddress(oplabel, actionpop());
	addr2 = gettemplabel();

	genquad(opjump, addr2, no_op, no_op);
	nextop = genquad(oplabel, addr1, no_op, no_op);
	setivalue(addr1.opnd, nextop);
        actionpush(addr2.opnd);

}

/*
 * FinishIf() -	Generate the instructions to finish the
 *		IF-THEN-ELSE-ENDIF
 */
void	FinishIf(void)
{
	struct addressrec	addr1;

        addr1 = setaddress(opnaddr,  actionpop());
	nextop = genquad(oplabel, addr1, no_op, no_op);
        setivalue(addr1.opnd, nextop);

}

/*
 * PrepareLoop() - Generate the instructions for the start of a loop
 */
void	PrepareLoop(void)
{
	struct addressrec	addr1;
	/*
	 *	Generate the temporary label to which the program
	 *	returns at the beginning of each iteration of the
	 *	loop
         */
	addr1 = gettemplabel();
	nextop = genquad(oplabel, addr1, no_op, no_op);
	setivalue(addr1.opnd, nextop);
        actionpush(addr1.opnd);
}

/*
 * StartWhile() -	Generate the instructions for the beginning of
 *			a loop after the condition			   
 */
void	StartWhile(void)
{
	enum tokentype		relop;
	enum operandtype	otype;
        struct addressrec	addr2, op;

	/* Depending on which relational operator appears, a different
				conditional jump must be used.*/
	relop = actionpop();
        op = getaddress(actionpop());
	switch(relop)	{
	case tokgreater:	otype = opifnegzjump; break;
	case tokless:		otype = opifposzjump; break;
	case tokequals:		otype = opifnzjump; break;
        case toknotequal:	otype = opifzjump;
	}

	/*
	 *	Generate the temporary label to which the program
	 *	jumps to terminate the loop.
	 */
	addr2 = gettemplabel();
	nextop = genquad(otype, op, addr2, no_op);
        actionpush(addr2.opnd);

}

/*
 * StartUntil() -	Generate the instructions for the beginning of
 *			a loop after the condition			   
 */
void	StartUntil(void)
{
	enum tokentype		relop;
	enum operandtype	otype;
        struct addressrec	addr2, op;

	/* Depending on which relational operator appears, a different
				conditional jump must be used.*/
	relop = actionpop();
        op = getaddress(actionpop());
	switch(relop)	{
	case tokgreater:	otype = opifposjump; break;
	case tokless:		otype = opifnegjump; break;
	case tokequals:		otype = opifzjump; break;
        case toknotequal:	otype = opifnzjump;
	}

	/*
	 *	Generate the temporary label to which the program
	 *	jumps to terminate the loop.
	 */
	addr2 = gettemplabel();
	nextop = genquad(otype, op, addr2, no_op);
        actionpush(addr2.opnd);

}

/*
 * FinishLoop() -	Generate the instruction at the end of a loop
 *			to take you back to the beginning where there is
 *			a conditional jump which ends the loop when
 *			appropriate.
 */
void	FinishLoop(void)
{
	struct addressrec	addr1, addr2;

	addr2 = setaddress(opnaddr, actionpop());
	addr1 = setaddress(opnaddr, actionpop());
		
	genquad(opjump, addr1, no_op, no_op);
	nextop = genquad(oplabel, addr2, no_op, no_op);
	setivalue(addr2.opnd, nextop);

}

/*
 * AddArgument() -	Add another argument to the actual parameter
 *			list.
 */
void	AddArgument(void)
{
        struct addressrec	addr1;
		
	/*
	 *	Make sure that the argument's type is valid and that it
	 *	matches the number arguments in the parameter list of
	 *	the procedure and matches the type of the corresponding
	 *	formal parameter
	 */
	if(!isvalidtype(oldtabindex))	{
        	printlexeme(oldtabindex);
		error(" is an invalid type", linenum);
        }

	if ((nextarg = getivalue(nextarg)) == 0)
		error("Too many arguments in call statement", linenum);
	if (data_class(nextarg) != data_class(oldtabindex))	
		error("Argument type does not match parameter type",
			linenum); 

	/* Set of the address and generate an argument instruction */
	addr1 = setaddress(opnvar, oldtabindex);
        genquad(oparg, addr1, no_op, no_op);
}

/*
 * StartCall() -	Check the type of the identifier to ensure that
 *			it is a procedure, and push it on the semantic
 *			actions stack.
 */
void	StartCall(void)
{
	/* Make sure that the identifier is a procedure name and
       		set the address to which you are transferring control */
	if (data_class(oldtabindex) != dtprocedure)
		error("Cannot call - this is not a procedure", linenum);
	actionpush(oldtabindex);
	nextarg = oldtabindex;

}
/*
 * EndCall() -	Pop the procedure and generate the procedure call.
 */ 
void	EndCall(void)
{
	struct addressrec	procname;

        procname = setaddress(opnaddr, actionpop());
	genquad(opproc, procname, no_op, no_op);

}

/*
 * EndArgList() -	Make sure that there ae enough actual parameters.
 */
void	EndArgList(void)
{
	if ((nextarg = getivalue(nextarg)) != 0)
        	error("Missing arguments", linenum);

}

/*
 * GetAddress() - Given the symbol table index for an identifier
 *		  or constant, create a valid address record for it.
 */
struct addressrec	getaddress(enum tokentype tokopnd)
{
	switch(tokenclass(tokopnd))	{
	  case tokidentifier:	return(setaddress(opnvar, tokopnd));
	  case tokconstant:	return(setaddress(opnconst, tokopnd));

	  default:		printlexeme(tokopnd);
				error(" is an invalid operator", linenum);
        }
}


/*
 * GenArithQuad() -	Generate an arithmetic quadruple instruction
 */
struct addressrec	GenArithQuad(struct addressrec op2,
			enum optype optor, struct addressrec op3)
{
	struct addressrec	op1, op4;
	enum optype		otype;

	/*
	 *	If the two operands match, get a temporary variable
	 *	of matching type.  If not, convert the integer
	 *	variable to real and get a real temporary variable.
	 */ 
	if (data_class(op2.opnd) == dtinteger)
		if (data_class(op3.opnd) == dtinteger)	{	
			op1 = gettempvar(dtinteger);
                        setproc(thisproc.proc, op1.opnd);
		}
		else	{
                	/*  Convert the integer operand to real */
			op4 = op2;
                        nextop = genfuncall(&op2, op4);
			op1 = gettempvar(dtreal);
			setproc(thisproc.proc, op1.opnd);
                        didcalculation = 1;
                }
	else if (data_class(op3.opnd) == dtinteger)	{
		op4 = op3;
		/*  Convert the integer operand to real */
                nextop = genfuncall(&op3, op4);
		op1 = gettempvar(dtreal);
		setproc(thisproc.proc, op1.opnd);
         }
		else	{
			op1 = gettempvar(dtreal);
			setproc(thisproc.proc, op1.opnd);
		}

	/* Generate the appropriate instruction.  Fold constants
		together into an assignment instruction if possible */
	if (symclass(op2.opnd) == stliteral
			&& symclass(op3.opnd) == stliteral)	
		nextop = foldconstants(optor, op1, op2, op3);
	else
        	nextop = genquad(optor, op1, op2, op3);
        return(op1);
}


/*
 *  ActionPop() -	Remove the top element from the Semantic Actions
 *			Stack and return it.
 *			Precondition: the stack is not empty.
 */
int		actionpop(void)
{
	if (actionempty())	{
		printf("Parse stack underflow\n");
                exit(10);
	}
	     
	return(ac.actionitem[--ac.actiontop]);
}

/*
 * ActionPush() -	Place the item given as an argument onto the
 *			top of the Semantic Actions Stack.
 *			Precondition: the stack is not full
 */
void		actionpush(int x)
{
	if (ac.actiontop == MAXACTIONSTACK)	{
		printf("Actions stack overflow\n");
                exit(10);
	}
	ac.actionitem[ac.actiontop++] = x;
}

/*
 * ActionEmpty() -	Returns True if the Semantic Actions stack is
 *			empty, False if it is not empty.
 */
enum logical    actionempty(void)
{
	return(ac.actiontop == 0);
}

/*
 * InitActionStack() -	Initialize the Semantic Actions stack by setting
 *			top to zero.
 */
void		initactionstack(void)
{
	ac.actiontop = 0;

}

int	actiontopitem(void)
{
	return(ac.actionitem[ac.actiontop-1]);
}

/*
 *  ParsePop() -	Remove the top element from the Procedure Stack
 *			and return it.
 *			Precondition: the stack is not empty.
 */
struct parsenoderec	*parsepop(void)
{
	if (parseempty())	{
		printf("Parse stack underflow\n");
                exit(10);
	}
	     
	return(pa.parsptr[--pa.parsetop]);
}

/*
 * ParsePush() -	Place the item given as an argument onto the
 *			top of the Parse Stack.
 *			Precondition: the stack is not full
 */
void	parsepush(struct parsenoderec *x)
{
	if (pa.parsetop == MAXPARSESTACK)	{
		printf("Parse stack overflow\n");
                exit(10);
	}
	pa.parsptr[pa.parsetop++] = x;
}

/*
 * ParseEmpty() -	Returns True if the parse stack is empty,
 *			False if it is not empty.
 */
enum logical    parseempty()
{
	return(pa.parsetop == 0);
}

/*
 * InitParsestack() -	Initialize the Parse stack by setting top to zero.
 */
void	initparsestack(void)
{
	pa.parsetop = 0;
}

/*
 * MatchToken() -	Returns TRUE of the lookahead token matches
 *			the expected terminal, FALSE if not.
 */
int	matchtoken(int thistoken, int thattoken)
{
	return(thistoken == thattoken);
}

/*
 * PrintNonterm() -	Given its enumeration, print the corresponding
 *			nonterminal.
 */
void	printnonterm(int i)
{
	printf("%s", nontermstrings[i]);
}

/*
 * PrintAction() -	Given its enumeration, print the corresponding
 *			semantic action.
 */
void	printaction(enum actiontype i)
{
	printf("%s", actionstring[i]);
}

/*
 * error() -	A catch-all routine for compiling errors.  It prints an
 *		error message including line number and then terminates
 *		execution.
 */
void	error(char message[], int linenum)
{
	printf("%s on line %d\n", message, linenum);
        exit(4);
}
