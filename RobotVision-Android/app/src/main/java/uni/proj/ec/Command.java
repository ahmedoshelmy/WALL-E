package uni.proj.ec;

import androidx.annotation.NonNull;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Objects;

public class Command {

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Command)) return false;

        Command command = (Command) o;

        if (!Objects.equals(cmd, command.cmd)) return false;
        // Probably incorrect - comparing Object[] arrays with Arrays.equals
        return Arrays.equals(arguments, command.arguments);
    }

    @Override
    public int hashCode() {
        int result = cmd != null ? cmd.hashCode() : 0;
        result = 31 * result + Arrays.hashCode(arguments);
        return result;
    }

    @NonNull
    @Override
    public String toString() {
        return "Command{" +
                "cmd='" + cmd + '\'' +
                ", arguments=" + Arrays.toString(arguments) +
                '}';
    }

    public static class CommandArgument{
        public final String name;
        public final Object value;
        public CommandArgument(String n , Object v){
            name = n;
            value = v;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (!(o instanceof CommandArgument)) return false;

            CommandArgument that = (CommandArgument) o;

            if (!Objects.equals(name, that.name)) return false;
            return Objects.equals(value, that.value);
        }

        @Override
        public int hashCode() {
            int result = name != null ? name.hashCode() : 0;
            result = 31 * result + (value != null ? value.hashCode() : 0);
            return result;
        }

        @NonNull
        @Override
        public String toString() {
            return "CommandArgument{" +
                    "name='" + name + '\'' +
                    ", value=" + value +
                    '}';
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

        return src.charAt(pos) == target;
    }

    private static String readName(){
        StringBuilder n = new StringBuilder();
        while (pos < src.length() && src.charAt(pos) != '{'){
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

    private static final Object lock = new Object();

    /**
     * format: "command_name{parameter1 = value1 , parameter2 = value2 , string = "string value here"}"
     * ex: hello_world{}
     *     say_hi{to = "Abdo"}
     *     set_value{val = 15 , val2 = 17}
     *     ...
     *
     * @param str the input command as a string format
     * @return the command
     * */
    public static Command fromString(String str) throws Exception {

        synchronized (lock) {
            src = str;
            pos = 0;
            String name = readName();
            ArrayList<CommandArgument> args = new ArrayList<>();

            if (eat('{')) { //begin reading the string
                while (true) {
                    String a_name = readArgName();
                    if (a_name == null) {
                        eat('}');
                        break;
                    }

                    if (!eat('=')) {
                        throw new Exception("illegal format at: " + pos);
                    }

                    Object a_val = readArgValue();
                    args.add(new CommandArgument(a_name, a_val));
                    if (eat('}'))
                        break;
                    eat(',');
                }
            }

            CommandArgument[] arr = new CommandArgument[args.size()];
            for (int i = 0; i < arr.length; i++)
                arr[i] = args.get(i);

            return new Command(name, arr);
        }
    }

    /**
     * same as fromString
     * but doesn't throw an Exception
     * return null if input is invalid
     * */
    public static Command s_fromString(String str){
        try {
            return fromString(str);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    /**
     * formats a string and returns the result of converting the formatted string
     * as a command
     * @param f the format string
     * @param args the objects that replaces the format parameters
     *
     * @return command
     * */
    public static Command format(String f , Object... args) throws Exception {
        return fromString(String.format(f , args));
    }

    /**
     * same as format , but doesn't throw an Exception
     * return null if input is invalid
     * */
    public static Command s_format(String f , Object... args){
        try {
            return fromString(String.format(f , args));
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

}
