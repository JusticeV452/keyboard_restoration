int logb2(int x) {
    if (x <= 0) return -1;
    int result = 0;
    while (x > 1) {
        x /= 2;
        result++;
    }
    return result;
}

int char_lookup(int idx)
{
    int result = -1;
    // 0 - 9
    if (idx == 69) {
        result = 48;
    } else if (idx == 112) {
        result = 49;
    } else if (idx == 51) {
        result = 50;
    } else if (idx == 87) {
        result = 51;
    } else if (idx == 117) {
        result = 52;
    } else if (idx == 113) {
        result = 53;
    } else if (idx == 137) {
        result = 54;
    } else if (idx == 66) {
        result = 55;
    } else if (idx == 31) {
        result = 56;
    } else if (idx == 46) {
        result = 57;
    // A - Z
    } else if (idx == 62) {
        result = 65;
    } else if (idx == 138) {
        result = 66;
    } else if (idx == 52) {
        result = 67;
    } else if (idx == 1) {
        result = 68;
    } else if (idx == 104) {
        result = 69;
    } else if (idx == 26) {
        result = 70;
    } else if (idx == 115) {
        result = 71;
    } else if (idx == 76) {
        result = 72;
    } else if (idx == 116) {
        result = 73;
    } else if (idx == 110) {
        result = 74;
    } else if (idx == 3) {
        result = 75;
    } else if (idx == 60) {
        result = 76;
    } else if (idx == 25) {
        result = 77;
    } else if (idx == 100) {
        result = 78;
    } else if (idx == 65) {
        result = 79;
    } else if (idx == 4) {
        result = 80;
    } else if (idx == 75) {
        result = 81;
    } else if (idx == 136) {
        result = 82;
    } else if (idx == 36) {
        result = 83;
    } else if (idx == 129) {
        result = 84;
    } else if (idx == 42) {
        result = 85;
    } else if (idx == 139) {
        result = 86;
    } else if (idx == 20) {
        result = 87;
    } else if (idx == 123) {
        result = 88;
    } else if (idx == 127) {
        result = 89;
    } else if (idx == 125) {
        result = 90;
    // Special
    } else if (idx == 19) {
        result = 13;    // '\r'
    } else if (idx == 19) {
        result = 10;    // '\n'
    } else if (idx == 130) {
        result = 32;    // ' '
    } else if (idx == 17) {
        result = 8;    // '<-'
    } else if (idx == 142) {
        result = 27;    // 'ESC'
    } else if (idx == 140) {
        result = 38;    // 'UP'
    } else if (idx == 109) {
        result = 40;    // 'DOWN'
    } else if (idx == 6) {
        result = 37;    // 'LEFT'
    } else if (idx == 29) {
        result = 39;    // 'RIGHT'
    } else if (idx == 105) {
        result = 91;    // 'WIN'
    } else if (idx == 70) {
        result = 46;    // 'DEL'
    } else if (idx == 89) {
        result = 17;    // 'CTRL'
    } else if (idx == 38) {
        result = 96;    // 'numpad0'
    } else if (idx == 92) {
        result = 97;    // 'numpad1'
    } else if (idx == 21) {
        result = 98;    // 'numpad2'
    } else if (idx == 34) {
        result = 99;    // 'numpad3'
    } else if (idx == 39) {
        result = 100;    // 'numpad4'
    } else if (idx == 12) {
        result = 101;    // 'numpad5'
    } else if (idx == 80) {
        result = 102;    // 'numpad6'
    } else if (idx == 55) {
        result = 103;    // 'numpad7'
    } else if (idx == 67) {
        result = 104;    // 'numpad8'
    } else if (idx == 121) {
        result = 105;    // 'numpad9'
    } else if (idx == 24) {
        result = 144;    // 'numlock'
    } else if (idx == 114) {
        result = 20;    // 'capslock'
    }
    return result;
}