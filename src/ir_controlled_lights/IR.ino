

char translateIR(int inCMD){
  switch(inCMD){
    case CMD_YELLOW:
        return 'y';
    case CMD_BLUE:
        return 'b';
    case CMD_RED:
        return 'r';
    case CMD_GREEN:
        return 'g';
        
    case CMD_1:
        return '1';
    case CMD_2:
        return '2';
    case CMD_3:
        return '3';
    case CMD_4:
        return '4';
    case CMD_5:
        return '5';
    case CMD_6:
        return '6';
    case CMD_7:
        return '7';
    case CMD_8:
        return '8';
    case CMD_9:
        return '9';
    case CMD__:
        return '-';
    case CMD_0:
        return '0';
    case CMD_ENTER:
        return 'e';
        
    case CMD_UP:
        return 'U';
    case CMD_RIGHT:
        return 'R';
    case CMD_LEFT:
        return 'L';
    case CMD_DOWN:
        return 'D';
    case CMD_CUBE:
        return '#';
    case CMD_REC:
        return 'g';        
    default:
        return ' ';
  }
}
