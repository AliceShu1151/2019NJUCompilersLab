#include <stdio.h>
void yyrestart(FILE* fp);
int yyparse(void);
FILE *fout;

int main(int argc, char **argv) {
    if (argc <= 1) 
        return 1;
    FILE *fp = fopen(argv[1], "r");
    if (!fp) 
    {
        perror(argv[1]);
        return 1;
    }  

    fout = stdout;
    if (argc > 2)
    {
        fout = fopen(argv[2], "w");
        if (!fout)
        {
            perror(argv[2]);
            return 1;
        }
    }
    
  	yyrestart(fp);
  	yyparse();
	return 0;
}
