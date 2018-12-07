#define		FILENAMELENGTH		40
#define		TOKENSTRINGLENGTH	40

FILE 		*openfile(int argc, char *argv[], char name[]);
enum tokentype	gettoken(FILE *ifp, int *tabindex);

extern int	linenum;
