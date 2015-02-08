#include <windows.h>
#include <stdio.h>
#include <winuser.h>
#include <windowsx.h>
#include <time.h>

void setTyping(long numChars);

unsigned long leftArrow;
unsigned long rightArrow;
unsigned long upArrow;
unsigned long downArrow;
unsigned long pageUp;
unsigned long pageDown;
unsigned long leftClicks;
unsigned long rightClicks;
unsigned long tabs;
unsigned long enters;
long typing;
    
int main(){
    
    FILE* ifp = fopen("BIN.LOG", "r");
    FILE* ofp = fopen("OUT.LOG", "w");
    
    char c;
    int temp;
    
    setTyping(0);
    
    
    unsigned char bracketCount = 0;
    
    while(!feof(ifp)){
        
        fscanf(ifp,"%c", &c);
        //ffprintf(ofp,ofp,"\n1%c2-%d, %d\n", c, downArrow, bracketCount);
        //getchar();
        if(c=='[') bracketCount = 1;
        
        else if(bracketCount){
            if(typing){
                temp = typing;
                typing = 0;
                if(c=='<') { fprintf(ofp,"\n"); leftArrow++; }
                else if (c=='>') { fprintf(ofp,"\n");  rightArrow++; }
                else if (c=='^'){
                    fprintf(ofp,"\n"); 
                    fscanf(ifp,"%c",&c);
                    if(c=='^') pageUp++;
                    else {   upArrow++; }
                }
                else if (c=='v'){
                    fprintf(ofp,"\n"); 
                    fscanf(ifp,"%c",&c);
                    if(c=='v') pageDown++;
                    else {   downArrow++; }
                }
                else if (c=='R') { fprintf(ofp,"\n");  rightClicks++; }
                else if (c=='L') { fprintf(ofp,"\n");  leftClicks++; }
                else if(c=='B'){
                    fscanf(ifp, "%c", &c);
                    fseek ( ofp , -1 , SEEK_CUR );
                    setTyping(temp-1);
                    //fprintf(ofp,"|%d|", typing);
                    //getchar();
                    bracketCount = 0;
                }
                else if(c=='E'){
                    fscanf(ifp, "%c", &c);
                    fprintf(ofp,"\n");
                    setTyping(temp+1);
                    bracketCount = 0;
                }
                else {
                    fprintf(ofp,"[%c", c);
                    setTyping(-1);
                    bracketCount = 0;
                }
            }
            else if(leftArrow){
                if(c=='<') leftArrow++;
                else{
                    fprintf(ofp,"%d×[<-], ", leftArrow);
                    leftArrow = 0;
                    if (c=='>') rightArrow++;
                    else if (c=='^'){
                        fscanf(ifp,"%c",&c);
                        if(c=='^') pageUp++;
                        else {   upArrow++; }
                    }
                    else if (c=='v'){
                        fscanf(ifp,"%c",&c);
                        if(c=='v') pageDown++;
                        else {   downArrow++; }
                    }
                    else if (c=='R') rightClicks++;
                    else if (c=='L') leftClicks++;
                    else {
                        fprintf(ofp,"[%c", c);
                        setTyping(typing+2);
                        bracketCount = 0;
                    }
                }
            }
            else if(rightArrow){
                if(c=='>') rightArrow++;
                else{
                    fprintf(ofp,"%d×[->], ", rightArrow);
                    rightArrow = 0;
                    if (c=='<') leftArrow++;
                    else if (c=='^'){
                        fscanf(ifp,"%c",&c);
                        if(c=='^') pageUp++;
                        else {   upArrow++; }
                    }
                    else if (c=='v'){
                        fscanf(ifp,"%c",&c);
                        if(c=='v') pageDown++;
                        else {   downArrow++; }
                    }
                    else if (c=='R') rightClicks++;
                    else if (c=='L') leftClicks++;
                    else {
                        fprintf(ofp,"[%c", c);
                        setTyping(typing+2);
                        bracketCount = 0;
                    }
                }
            }
            else if(upArrow){
                if(c=='^'){
                    fscanf(ifp,"%c",&c);
                    if(c=='^'){
                        pageUp++;
                        fprintf(ofp,"%d×[Up], ", upArrow);
                        upArrow = 0;
                    }
                    else {   upArrow++; }
                }
                else{
                    fprintf(ofp,"%d×[Up], ", upArrow);
                    upArrow = 0;
                    if (c=='<') leftArrow++;
                    else if (c=='>') rightArrow++;
                    else if (c=='v'){
                        fscanf(ifp,"%c",&c);
                        if(c=='v') pageDown++;
                        else {   downArrow++; }
                    }
                    else if (c=='R') rightClicks++;
                    else if (c=='L') leftClicks++;
                    else {
                        fprintf(ofp,"[%c", c);
                        setTyping(typing+2);
                        bracketCount = 0;
                    }
                }
            }
            else if(downArrow){
                if(c=='v'){
                    fscanf(ifp,"%c",&c);
                    if(c=='v'){
                        pageDown++;
                        fprintf(ofp,"%d×[Down], ", downArrow);
                        downArrow = 0;
                    }
                    else {   downArrow++;}
                }
                else{
                    fprintf(ofp,"%d×[Down], ", downArrow);
                    downArrow = 0;
                    if (c=='<') leftArrow++;
                    else if (c=='>') rightArrow++;
                    else if (c=='^'){
                        fscanf(ifp,"%c",&c);
                        if(c=='^') pageUp++;
                        else {   upArrow++; }
                    }
                    else if (c=='R') rightClicks++;
                    else if (c=='L') leftClicks++;
                    else {
                        fprintf(ofp,"[%c", c);
                        setTyping(typing+2);
                        bracketCount = 0;
                    }
                }
            }
            else if(pageUp){
                if(c=='^'){
                    fscanf(ifp,"%c",&c);
                    if(c=='^') pageUp++;
                    else { 
                        fprintf(ofp,"%d×[PageUp], ", pageUp);
                        pageUp = 0;
                          
                        upArrow++; 
                    }
                }
                else{
                    fprintf(ofp,"%d×[PageUp], ", pageUp);
                    pageUp = 0;
                    if (c=='<') leftArrow++;
                    else if (c=='>') rightArrow++;
                    else if (c=='v'){
                        fscanf(ifp,"%c",&c);
                        if(c=='v') pageDown++;
                        else {   downArrow++; }
                    }
                    else if (c=='R') rightClicks++;
                    else if (c=='L') leftClicks++;
                    else {
                        fprintf(ofp,"[%c", c);
                        setTyping(typing+2);
                        bracketCount = 0;
                    }
                }
            }
            else if(pageDown){
                if(c=='v'){
                    fscanf(ifp,"%c",&c);
                    if(c=='v') pageDown++;
                    else { 
                        fprintf(ofp,"%d×[PageDown], ", pageDown);
                        pageDown = 0;
                          
                        downArrow++; 
                    }
                }
                else{
                    fprintf(ofp,"%d×[PageDown], ", pageDown);
                    pageDown = 0;
                    if (c=='<') leftArrow++;
                    else if (c=='>') rightArrow++;
                    else if (c=='^'){
                        fscanf(ifp,"%c",&c);
                        if(c=='^') pageUp++;
                        else {   upArrow++; }
                    }
                    else if (c=='R') rightClicks++;
                    else if (c=='L') leftClicks++;
                    else {
                        fprintf(ofp,"[%c", c);
                        setTyping(typing+2);
                        bracketCount = 0;
                    }
                }
            }
            else if(leftClicks){
                if(c=='L') leftClicks++;
                else{
                    fprintf(ofp,"%d×[LeftMouse], ", leftClicks);
                    leftClicks = 0;
                    if (c=='<') leftArrow++;
                    else if (c=='>') rightArrow++;
                    else if (c=='^'){
                        fscanf(ifp,"%c",&c);
                        if(c=='^') pageUp++;
                        else {   upArrow++; }
                    }
                    else if (c=='v'){
                        fscanf(ifp,"%c",&c);
                        if(c=='v') pageDown++;
                        else {   downArrow++; }
                    }
                    else if (c=='R') rightClicks++;
                    else {
                        fprintf(ofp,"[%c", c);
                        setTyping(typing+2);
                        bracketCount = 0;
                    }
                }
            }
            else if(rightClicks){
                if(c=='R') rightClicks++;
                else{
                    fprintf(ofp,"%d×[RightMouse], ", rightClicks);
                    rightClicks = 0;
                    if (c=='<') leftArrow++;
                    else if (c=='>') rightArrow++;
                    else if (c=='^'){
                        fscanf(ifp,"%c",&c);
                        if(c=='^') pageUp++;
                        else {   upArrow++; }
                    }
                    else if (c=='v'){
                        fscanf(ifp,"%c",&c);
                        if(c=='v') pageDown++;
                        else {   downArrow++; }
                    }
                    else if (c=='L') leftClicks++;
                    else {
                        fprintf(ofp,"[%c", c);
                        setTyping(typing+2);
                        bracketCount = 0;
                    }
                }
            }
            if(bracketCount && !upArrow && !downArrow) fscanf(ifp,"%c", &c);
            bracketCount = 0;
        } // close else if bracketCount
        else if (c == '\n');
        else {
            if(leftArrow){
                fprintf(ofp,"%d×[<-]\n", leftArrow);
                leftArrow = 0;
            }
            else if(rightArrow){
                fprintf(ofp,"%d×[->]\n", rightArrow);
                rightArrow = 0;
            }
            else if(upArrow){
                fprintf(ofp,"%d×[Up]\n", upArrow);
                upArrow = 0;
            }
            else if(downArrow){
                fprintf(ofp,"%d×[Down]\n", downArrow);
                downArrow = 0;
            }
            else if(pageUp){
                fprintf(ofp,"%d×[PageUp]\n", pageUp);
                pageUp = 0;
            }
            else if(pageDown){
                fprintf(ofp,"%d×[PageDown]\n", pageDown);
                pageDown = 0;
            }
            else if(leftClicks){
                fprintf(ofp,"%d×[LeftMouse]\n", leftClicks);
                leftClicks = 0;
            }
            else if(rightClicks){
                fprintf(ofp,"%d×[RightMouse]\n", rightClicks);
                rightClicks = 0;
            }
            fprintf(ofp,"%c", c);
            setTyping(typing+1);
            bracketCount = 0;
        }
    } // Close while loop reading file
    fclose(ifp);
    fclose(ofp);
    
}

void setTyping(long numChars){
    leftArrow = 0;
    rightArrow = 0;
    upArrow = 0;
    downArrow = 0;
    pageUp = 0;
    pageDown = 0;
    leftClicks = 0;
    rightClicks = 0;
    tabs = 0;
    enters = 0;
    typing = numChars;
}
