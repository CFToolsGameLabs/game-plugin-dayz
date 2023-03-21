class GameLabsUtil {
    static int hextable_resolve_char(string character) {
        switch(character) {
            case "0": return 0;
            case "1": return 1;
            case "2": return 2;
            case "3": return 3;
            case "4": return 4;
            case "5": return 5;
            case "6": return 6;
            case "7": return 7;
            case "8": return 8;
            case "9": return 9;
            case "A": return 10;
            case "B": return 11;
            case "C": return 12;
            case "D": return 13;
            case "E": return 14;
            case "F": return 15;
        }
        return 0;
    }

    static int hex_to_rgb(string hex) {
        if(hex.Length() != 6) return 0;
        int r = hex_to_int(hex.Substring(0, 2));
        int g = hex_to_int(hex.Substring(2, 2));
        int b = hex_to_int(hex.Substring(4, 2));
        return ARGB(255, r, g, b);
    }

    static int hex_to_int(string hex) {
        int dec = 0, c;
        hex.ToUpper();
        for(int i = 0; i < hex.Length(); i++) {
            dec += hextable_resolve_char(hex.Get(i)) * Math.Pow(16, ((hex.Length() - i) - 1));
        }
        return dec;
    }
};