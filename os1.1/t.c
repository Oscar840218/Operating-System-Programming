/*********  t.c file *********************/

int prints(char *s)
{
  int index = 0;
  while (*(s+index) != 0 ) 
  {
    putc(*(s+index));

    index++;
  }
}

int gets(char *s) 
{
  int index = 0;
  while (1) 
  {
    *(s+index) = getc();

    if(*(s+index) == 0x0D)
    {
      *(s+index) = 0; // 0 = '\0'
      break;
    }

    putc(*(s+index));
    index++;
  }

}

char ans[64];

main()
{
  while(1){
    prints("What's your name? ");
    gets(ans);  prints("\n\r");

    if (ans[0]==0){
      prints("return to assembly and hang\n\r");
      return;
    }
    prints("Welcome "); prints(ans); prints("\n\r");
  }
}
  
