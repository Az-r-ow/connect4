/**
* getstr_to_int will prompt the user for input
* and then return an integer value of the first char inputted
* if a letter was prompted, its ascii code will be returned
*/
int getstr_to_int(void){

  char string[50];

  scanf("%s", string);
  int int_value = string[0] - '0';
  return int_value;

}
