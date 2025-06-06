/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the C-Minus compiler                         */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken(TokenType token, const char* tokenString)
{
  switch (token)
  {
    case IF:
    case ELSE:
    case INT:
    case RETURN:
    case VOID:
    case WHILE:
      fprintf(listing, "reserved word: %s\n", tokenString);
      break;
    case ASSIGN: fprintf(listing, "=\n"); break;
    case EQ: fprintf(listing, "==\n"); break;
    case LT: fprintf(listing, "<\n"); break;
    case PLUS: fprintf(listing, "+\n"); break;
    case MINUS: fprintf(listing, "-\n"); break;
    case TIMES: fprintf(listing, "*\n"); break;
    case OVER: fprintf(listing, "/\n"); break;
    case LPAREN: fprintf(listing, "(\n"); break;
    case RPAREN: fprintf(listing, ")\n"); break;
    case LBRACE: fprintf(listing, "{\n"); break;
    case RBRACE: fprintf(listing, "}\n"); break;
    case SEMI: fprintf(listing, ";\n"); break;
    case COMMA: fprintf(listing, ",\n"); break;
    case ENDFILE: fprintf(listing, "EOF\n"); break;
    case NUM:
      fprintf(listing, "NUM, val= %s\n", tokenString);
      break;
    case ID:
      fprintf(listing, "ID, name= %s\n", tokenString);
      break;
    case ERROR:
      fprintf(listing, "ERROR: %s\n", tokenString);
      break;
    default:
      fprintf(listing, "Unknown token: %d\n", token);
  }
}

/* Function newStmtNode creates a new statement node */
TreeNode * newStmtNode(StmtKind kind)
{
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t == NULL)
    fprintf(listing, "Out of memory error at line %d\n", lineno);
  else {
    for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newExpNode creates a new expression node */
TreeNode * newExpNode(ExpKind kind)
{
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t == NULL)
    fprintf(listing, "Out of memory error at line %d\n", lineno);
  else {
    for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = Void;
  }
  return t;
}

/* Function copyString allocates and makes a new copy of a string */
char * copyString(char * s)
{
  int n;
  char * t;
  if (s == NULL) return NULL;
  n = strlen(s) + 1;
  t = (char *) malloc(n);
  if (t == NULL)
    fprintf(listing, "Out of memory error at line %d\n", lineno);
  else strcpy(t, s);
  return t;
}

/* indentno is used by printTree to store indentation level */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno += 2
#define UNINDENT indentno -= 2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{
  int i;
  for (i = 0; i < indentno; i++)
    fprintf(listing, " ");
}

/* procedure printTree prints a syntax tree */
void printTree(TreeNode * tree)
{
  int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind == StmtK)
    {
      switch (tree->kind.stmt)
      {
        case IfK:
          fprintf(listing, "If\n");
          break;
        case WhileK:
          fprintf(listing, "While\n");
          break;
        case AssignK:
          fprintf(listing, "Assign to: %s\n", tree->attr.name);
          break;
        case ReturnK:
          fprintf(listing, "Return\n");
          break;
        case CompoundK:
          fprintf(listing, "Compound\n");
          break;
        case FunctionK:
          fprintf(listing, "Function: %s\n", tree->attr.name);
          break;
        case VarDeclK:
          fprintf(listing, "Var Declaration: %s\n", tree->attr.name);
          break;
        case ParamK:
          fprintf(listing, "Parameter: %s\n", tree->attr.name);
          break;
        default:
          fprintf(listing, "Unknown StmtNode kind\n");
          break;
      }
    }
    else if (tree->nodekind == ExpK)
    {
      switch (tree->kind.exp)
      {
        case OpK:
          fprintf(listing, "Op: ");
          printToken(tree->attr.op, "\0");
          break;
        case ConstK:
          fprintf(listing, "Const: %d\n", tree->attr.val);
          break;
        case IdK:
          fprintf(listing, "Id: %s\n", tree->attr.name);
          break;
        default:
          fprintf(listing, "Unknown ExpNode kind\n");
          break;
      }
    }
    else fprintf(listing, "Unknown node kind\n");
    
    for (i = 0; i < MAXCHILDREN; i++)
      printTree(tree->child[i]);
    
    tree = tree->sibling;
  }
  UNINDENT;
}
