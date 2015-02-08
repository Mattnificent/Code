//Search for elements
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define     TOTAL_KEYWORDS        12
#define     DELIMETER_LEN         25
#define     SEND_NO_MORE_THAN     15
#define     KEY_LENGTH            20
#define     MAX_LISTINGS          40
#define     MAX_CHARS_PER_LINE   500


/**************************************************************************************
                 This function searches for a keyword in a line, if the keyword
                 is found, it returns the position in the line array that the
                 first letter of the match was found, otherwise, it returns -1.
                 The "wild" character acts just like a blank tile in scrabble
                 (for the keyword)
*************************************************************************************/
int line_search(char line[], char keyword[], char wild);


/**************************************************************************************
                 scans up to the first '>' in the delimeter and puts the file
                 pointer(fp) up to the '|'.
                 
                 ...<delimeter>|
                 
                 NOTE: The char passed through must be an address(&delimeter), 
                 because *delim is a pointer, which is why in the function, the 
                 delim must always be written with a * in front of it(*delim).
*************************************************************************************/
void fp_to_delimeter(char *delim, FILE *pointer);


/**************************************************************************************
                 This function will scan all the way up to a reference web
                 address(item.link), then through up through until the next text 
                 outside from the tags(getting pointed at by the inequalities)
                 
                 ...<a href="web address(item.link)"...><...>|unscanned outer
                 
*************************************************************************************/
void a_href(FILE *pointer);


/**************************************************************************************
                 This function will scan everything outside the <tagged> regions,
                 put it into the outer_text list, and return the number of tagged
                 items.
                 
*************************************************************************************/
int get_outer_text(char *delim, FILE *pointer);


/**************************************************************************************
                 This function searches a string for keywords. If any keyword is
                 found in the string, the string will go to the sent function.
                 Also, the character '~' can be used as a wild card character in
                 the keywords.
*************************************************************************************/
int search_for(char string_of_interest[MAX_CHARS_PER_LINE]);



/**************************************************************************************
                 This function creates the file "Send.txt", and puts the
                 what_to_send string into it (starting where the actual
                 important text is)
*************************************************************************************/
void send(char what_to_send[MAX_CHARS_PER_LINE]);

//Only 1 universal structure to be gangbanged by all the listings
struct listing {
       char main_details[MAX_CHARS_PER_LINE];
       int price;
       char link[MAX_CHARS_PER_LINE];
};

char listing_delimeter[DELIMETER_LEN];
char a_href_link[MAX_CHARS_PER_LINE];
char keywords[TOTAL_KEYWORDS][KEY_LENGTH];
char sent[SEND_NO_MORE_THAN][MAX_CHARS_PER_LINE];
char outer_text[10][MAX_CHARS_PER_LINE];
int lines_sent=0, quit=0;


int main(){
   
   struct listing item;
   int hours, i, j, k, t, num, found;
   char dummy, send_this[MAX_CHARS_PER_LINE];
   
   FILE *ifp, *ofp;
   
   printf("For how many hours do you want to run the program?");
   scanf("%d", &hours);
   getchar();
   
   //inialize the keywords
   for(i=0; i<TOTAL_KEYWORDS; i++)
   memset(keywords[i], '\0', KEY_LENGTH);
      
   strcpy(keywords[0], "E. ORLANDO");
   strcpy(keywords[1], "E ORLANDO");
   strcpy(keywords[2], "EAST ORLANDO");
   strcpy(keywords[3], "UCF");
   strcpy(keywords[4], "E COLONIAL");
   strcpy(keywords[5], "E. COLONIAL");
   strcpy(keywords[6], "EAST COLONIAL");
   strcpy(keywords[7], "ALAFAYA");
   strcpy(keywords[8], "ROUSE");
   strcpy(keywords[9], "UNIVERSITY");
   strcpy(keywords[10],"AVALON");
   strcpy(keywords[11],"PEGASUS");
   
   //Ebay Delimeter
   //strcpy(listing_delimeter,"td class=\"details\"");
   //Craigslist Delimeter
   strcpy(listing_delimeter,"p class=\"row\"");
   
   
   
   for(t=0;t<(360*hours);t++){//Timed loop - Specified by user input
   ////////////////////////////////////////////////////////////////////////////
      
     //system("pause");
      
      ifp = fopen("myfile.txt", "r+");
      //fscanf(ifp, "%[^'>']", &item.main_details);
      //fscanf(ifp, "%c", &dummy);
      //printf("%s|%c\n", item.main_details, dummy);
      fp_to_delimeter(listing_delimeter, ifp);
     
     
      for(i=0; i<MAX_LISTINGS; i++){
         
         a_href(ifp);
         
         //system("pause");
         
         num = get_outer_text(listing_delimeter, ifp);
      
         //printf("%da_href =|%s|\n", i, a_href_link);
         for(j=0; j<num; j++){
      
            //printf("%d - |%s|\n", j, outer_text[j]); 
            
            if(search_for(outer_text[j])!=0){
                    
                for(j=0; j<num; j++)
                    strcat(send_this,outer_text[j]);
                
                send(send_this);
                break;
            } 
         }
       
       memset(send_this, '\0', MAX_CHARS_PER_LINE );
         //getchar();
         //printf("\n");
         
         //system("pause");
         
      }
      //Time Loop is the only loop still in right here
      
      fclose(ifp);
      sleep(5000);
      
      //system("pause");
      //scanf("%s", &item.link);
      //scanf("%s", &item.main_details);
      //scanf("%d", &item.price);
     
      //printf("Block info: %d %c %s\n", first.number, first.letter, first.color);
   
   }
  
  return 0;
    
}



/**************************************************************************************
                 This function searches for a keyword in a line, if the keyword
                 is found, it returns the position in the line array that the
                 first letter of the match was found, otherwise, it returns -1.
                 The "wild" character acts just like a blank tile in scrabble
                 (for the keyword)
*************************************************************************************/

int line_search(char line[], char keyword[], char wild) {
   
    int i, j, key_length, line_length, k=0;
    key_length = strlen(keyword);
    line_length = strlen(line);
   
    for(i=0; i<=(line_length-key_length); i++){
         for(j=0; j<key_length; j++){
                if (line[i+j]==keyword[j]||wild==keyword[j]){
                    k++;
                }
                else{
                    k=0;
                    break;
                }
         }
         if(k==key_length)
         return i;
    }
    return -1;
}



/**************************************************************************************
                 scans up to the delimeter at the first '>' and puts the file
                 pointer(fp) up to the '|'.
                 
                 ...<delimeter>|
                 
                 NOTE: The char passed through must be an address(&delimeter), 
                 because *delim is a pointer, which is why in the function, the 
                 delim must always be written with a * in front of it(*delim).
*************************************************************************************/
void fp_to_delimeter(char *delim, FILE *pointer){
     
     char string[MAX_CHARS_PER_LINE], dummy;
     
     while(1==1){
                  
           
                   
           fscanf(pointer, "%[^'.MAX_CHARS_PER_LINE>']", string);
           fscanf(pointer, "%c", &dummy);
           //printf("%s|%c\n",string, dummy);
           if(line_search(string, delim, '~')!=-1)
           break;
     }
     return;
}


/**************************************************************************************
                 This function will scan all the way up to a reference web
                 address(item.link), that address will be saved in the a_href_link
                 then through up through until the next text outside from the tags
                 (getting pointed at by the inequalities)
                 
                 ...<a href="web address(item.link)"|...><...>|unscanned outer
                 
*************************************************************************************/
void a_href(FILE *pointer){
      
     char string[MAX_CHARS_PER_LINE], dummy, tagstart;
     
     
     
     while(1==1){
           
           //Scan up to quotation marks  
           fscanf(pointer, "%[^'\"']", string);
           fscanf(pointer, "%c", &dummy);
           //printf("%s|%c\n",string, dummy);
           
           //Check if the a href is there
           if(line_search(string, "a href=", '~')!=-1)
           break;//If it is...
     }
     
     //Scan the web address into a_href_link
     fscanf(pointer, "%[^'\"']", a_href_link);
     //printf("string =|%s|\na_href_link = |%s|\n", string, a_href_link);
     //system("pause");
     
     
     return;
}


/**************************************************************************************
                 This function will scan everything outside the <tagged> regions,
                 put it into the outer_text list, and return the number of tagged
                 items.
                 
*************************************************************************************/
int get_outer_text(char *delim, FILE *pointer){
     
     char string[MAX_CHARS_PER_LINE], dummy, is_tag;
     int count=0, i, j;
     
     
     for(i=0; i<10; i++)
     memset(outer_text[i], '\0', MAX_CHARS_PER_LINE);
     
     while(1==1){
                          
           fscanf(pointer, "%[^'>']", string);
           fscanf(pointer, "%c", &dummy);
           
           //This loop has to be here in case there are any spaces or \n's
           is_tag = ' ';
           j=0;
           while(is_tag==' '||is_tag=='\n'||is_tag=='\t'||is_tag=='\r'||is_tag=='-'){
           
               fscanf(pointer, "%c", &is_tag);

           }
           
           //printf("%s|%c|%c\n",string, dummy, is_tag);
           
           if(line_search(string, delim, '~')!=-1)
           break;
           
           if(is_tag!='<'){
                
                outer_text[count][0] = is_tag;
                fscanf(pointer, "%[^'<']", string);
                strcat(outer_text[count], string);
                
                count++;  
           }
                
     }
     return count;
}


/**************************************************************************************
                 This function searches a string for keywords. If any keyword is
                 found in the string, the string will go to the sent function.
                 Also, the character '~' can be used as a wild card character in
                 the keywords.
*************************************************************************************/

int search_for(char string_of_interest[MAX_CHARS_PER_LINE]){
    
    int i, j, send_it=0;
    
    //printf("search_for loop\n");
    
    //printf("%d, %s", was_it_sent, string_of_interest);

    //Make upper case
    for(i=0; i<MAX_CHARS_PER_LINE; i++)
             string_of_interest[i]= toupper(string_of_interest[i]);
    //printf("\nstring_of_interest = |%s|", string_of_interest);
    //system("pause");
    //For each keyword
    for(i=0;i<TOTAL_KEYWORDS;i++){         
         
          //Search the string for
          
          
          
          if(line_search(string_of_interest, keywords[i], '~')!=-1){
                
                //printf("\n\nfound a keyword!!!\n");
                send_it++;
                
                //If it hasn't been sent, send it
                //if(was_it_sent==0)
                //     send(string_of_interest);
                
          }//Out of the if the keyword was found statement
    
    }//Out of the keyword loop
    //printf("%d, %s", was_it_sent, string_of_interest);
    
    
    return send_it;
}


/**************************************************************************************
                 This function creates the file "Send.txt", and puts the
                 what_to_send string into it (starting where the actual
                 important text is)
*************************************************************************************/

void send(char what_to_send[MAX_CHARS_PER_LINE]){
    
     FILE *ofp;
     
     int i, j;
     
     //Check every other line that has been sent
     for(j=0;j<lines_sent;j++){
         //If it has been sent already,
           if(strcmp(what_to_send, sent[j])==0){
                  //printf("it has already been sent");
                  //system("pause");
                  //It doesn't need to be sent again;
                  return;
           }            
     }
     
     strcpy(sent[lines_sent], what_to_send);
     lines_sent++;
         
     ofp = fopen("Send.txt", "w");
     fprintf(ofp, "%s", what_to_send);
     
     //printf("send:%s", what_to_send);
     //printf("\n|%s|sent!!!!\n", sent[lines_sent-1]);
    
     fclose(ofp);
     
     printf("this was sent:|%s|\n\n", what_to_send);
     //system("pause");
}
