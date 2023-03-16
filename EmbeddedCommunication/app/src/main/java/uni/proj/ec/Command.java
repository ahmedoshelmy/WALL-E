package uni.proj.ec;

import android.os.Build;

import java.util.ArrayList;
import java.util.FormatFlagsConversionMismatchException;

public class Command {
    public static class CommandArgument{
        public final String name;
        public final Object value;
        public CommandArgument(String n , Object v){
            name = n;
            value = v;
        }
    }

    public final String cmd;
    public final CommandArgument[] arguments;
    public Command(String cmd , CommandArgument... args){
        this.cmd = cmd.trim();
        this.arguments = args;
    }

    private static String src;
    private static int    pos;
    private static boolean eat(char target){
        if (pos >= src.length()) return false;

        while (src.charAt(pos) == ' ')
            pos++;
        if (src.charAt(pos) == target) {
            pos++;
            return true;
        }
        return false;
    }

    private static boolean next(char target){
        if (pos >= src.length()) return false;

        while (src.charAt(pos) == ' ')
            pos++;
        if (src.charAt(pos) == target) {
            return true;
        }
        return false;
    }
    private static String readName(){
        StringBuilder n = new StringBuilder();
        while (src.charAt(pos) != '{'){
            n.append(src.charAt(pos));
            pos++;
        }
        return n.toString().trim();
    }

    private static String readArgName(){
        if (next('}')) return null;
        StringBuilder n = new StringBuilder();
        while (src.charAt(pos) != '='){
            n.append(src.charAt(pos));
            pos++;
        }
        return n.toString().trim();
    }

    private static Object readArgValue(){
        StringBuilder n = new StringBuilder();
        boolean quotation = false;
        while (quotation || (src.charAt(pos) != ',' && src.charAt(pos) != '}')) {
            if (quotation && src.charAt(pos) == '\"'){ //end quotation
                n.append('\"');
                pos++;
                break;
            }
            if (src.charAt(pos) == '\"') {
                quotation = true;
            }
            n.append(src.charAt(pos));
            pos++;
        }
        String val = n.toString().trim();
        if (val.endsWith("\"") && val.startsWith("\"")){
            return val.substring(1 , val.length() - 2); //remove the quotation marks
        }else if (val.contains(".")){
            return Float.parseFloat(val);
        }else {
            return Integer.parseInt(val);
        }
    }


    public static Command fromString(String str) throws Exception {
        src = str;
        pos = 0;
        String name = readName();
        ArrayList<CommandArgument> args = new ArrayList<>();

        if (eat('{')){ //begin reading the string
            while (true){
                String a_name = readArgName();
                if (a_name == null){
                    eat('}');
                    break;
                }
                if (!eat('=')){
                    throw new Exception("illegal format at: " + pos);
                }
                Object a_val  = readArgValue();
                args.add(new CommandArgument(a_name , a_val));
                if (eat('}'))
                    break;
                eat(',');
            }
        }

        CommandArgument arr[] = new CommandArgument[args.size()];
        for (int i= 0;i < arr.length;i++)
            arr[i] = args.get(i);

        return new Command(name , arr);

    }

}
